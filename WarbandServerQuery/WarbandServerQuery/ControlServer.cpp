#include "WarbandServerQueryAdmin.hpp"
#include "ControlServer.hpp"

#include <boost/bind.hpp>
#include <iostream>

ControlServer::ControlServer(UINT16 port) :
	acceptor(this->ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
	this->msgMap[ControlServer::CTRL_LOGIN] = std::bind(
		&ControlServer::loginClient,
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
	);
	this->msgMap[ControlServer::CTRL_LOGOUT] = std::bind(
		&ControlServer::logoutClient,
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
	);
	this->setupAccept();
}

ControlServer::~ControlServer()
{
}

void ControlServer::run()
{
	this->ioService.run();
}

void ControlServer::sendMessage(unsigned long long id, MessageType type, const BYTE *msg, unsigned sz)
{
	std::shared_ptr<Client> client;
	BYTE *netMsg = new BYTE[sizeof(MessageHeader) + sz];
	MessageHeader *header = reinterpret_cast<MessageHeader *>(netMsg);

	header->type = type;
	header->sz = sz;
	std::memcpy(header + 1, msg, sz);
	for (auto& it : this->clients) {
		if (it->id == id) {
			it->send(netMsg, sizeof(MessageHeader) + sz);
			break;
		}
	}
	delete [] netMsg;
}

void ControlServer::broadcastMessage(MessageType type, const BYTE *msg, unsigned sz)
{
	BYTE *netMsg = new BYTE[sizeof(MessageHeader) + sz];
	MessageHeader *header = reinterpret_cast<MessageHeader *>(netMsg);

	header->type = type;
	header->sz = sz;
	std::memcpy(header + 1, msg, sz);
	for (auto &it : this->clients) {
		if (it->isLoggedIn()) {
			it->send(netMsg, sz + sizeof(MessageHeader));
		}
	}
	delete [] netMsg;
}

void ControlServer::disconnectClient(std::shared_ptr<Client> &client) noexcept
{
	std::cout << "Client disconnected (" << client->id << "): " <<
		client->getSocket().remote_endpoint().address().to_string() << std::endl;
	client->getSocket().close();
	for (auto it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (*(*it) == *client) {
			this->clients.erase(it);
			break;
		}
	}
}

void ControlServer::setupAccept()
{
	std::shared_ptr<Client> client(new Client(clientIdCounter++, this->ioService));

	this->acceptor.async_accept(
		client->getSocket(),
		boost::bind(&ControlServer::handleAccept, this, client, boost::asio::placeholders::error)
	);
}

void ControlServer::handleAccept(std::shared_ptr<Client> &client, const boost::system::error_code &error)
{
	if (!error) {
		/* TODO: Handle Exception */
		std::cout << "Client connected (" << client->id << "): " <<
			client->getSocket().remote_endpoint().address().to_string() << std::endl;
		this->clients.push_back(client);
		this->setupRead(*std::next(this->clients.end(), -1));
	}
	this->setupAccept();
}

void ControlServer::setupRead(std::shared_ptr<Client> &client)
{
	client->getSocket().async_read_some(
		boost::asio::buffer(
			&client->getData()[client->getReadDataSz()],
			Client::MSG_MAX_SZ - client->getReadDataSz()),
		boost::bind(
			&ControlServer::handleRead,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			client
		)
	);
}

void ControlServer::handleRead(
	const boost::system::error_code& err,
	size_t nBytes,
	std::shared_ptr<Client> &client)
{
	MessageHeader *header = reinterpret_cast<MessageHeader *>(client->getData());

	if (!nBytes || err) {
		this->disconnectClient(client);
		return;
	}
	client->getReadDataSz() += nBytes;
	while (client->getReadDataSz() >= sizeof(MessageHeader)) {
		header = reinterpret_cast<MessageHeader*>(&client->getData()[0]);
		if (header->sz > Client::MSG_MAX_SZ) {
			this->disconnectClient(client);
			return;
		}
		long overflow = client->getReadDataSz() - (sizeof(MessageHeader) + header->sz);
		if (overflow < 0)
			break;
		this->msgMap[static_cast<ControlServer::MessageType>(header->type)](
			client->id,
			reinterpret_cast<BYTE *>(header + 1),
			header->sz
		);
		/* Handle Message + Check if type exists */
		client->getReadDataSz() = overflow;
		if (client->getReadDataSz() > 0)
			std::memmove(&client->getData()[0], &client->getData()[overflow], overflow); /* Move next message to beginning */
	}
	this->setupRead(client);
}

void ControlServer::loginClient(unsigned long long id, const BYTE *msg, unsigned sz)
{
	std::shared_ptr<Client> client;

	for (auto &it : this->clients) {
		if (it->id == id) {
			client = it;
			break;
		}
	}
	/* TODO: Auth system */
	this->sendMessage(id, ControlServer::CTRL_LOGIN, 0, 0);
	client->isLoggedIn() = true;
	gWarbandServer->synchronizeNewClient(client->id);
}

void ControlServer::logoutClient(unsigned long long id, const BYTE *, unsigned)
{
	for (auto &it : this->clients) {
		if (it->id == id) {
			this->disconnectClient(it);
			break;
		}
	}
}