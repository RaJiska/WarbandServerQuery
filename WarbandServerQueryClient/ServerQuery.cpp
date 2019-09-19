#include "Globals.hpp"
#include "ServerQuery.hpp"

#include <boost/bind.hpp>
#include <iostream> // Debug

ServerQuery::ServerQuery()
	: sock(this->ioService)
{
}

void ServerQuery::connect()
{
	// DEBUG
	this->connect(this->address, this->port, this->password);
}

void ServerQuery::connect(const std::string &address, const std::string &port, const std::string &password)
{
	this->msgMap[ServerQuery::CTRL_LOGIN] = &ServerQuery::msgLogin;
	this->msgMap[ServerQuery::GAME_PLAYER_JOINED] = &ServerQuery::msgPlayerJoined;
	this->msgMap[ServerQuery::GAME_PLAYER_LEFT] = &ServerQuery::msgPlayerLeft;
	boost::asio::ip::tcp::resolver resolver(this->ioService);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
	boost::system::error_code error = boost::asio::error::host_not_found;
	boost::asio::connect(sock, it, error);
	if (error)
		std::cout << "Could not connect" << std::endl;
	this->setupRead();
	this->sendLoginMsg(password);
	this->ioService.run();
	std::cout << "Connection Over" << std::endl;
	/* Handle Error */
}

void ServerQuery::setAddress(const std::string &address)
{
	this->address = address;
}

void ServerQuery::setPort(const std::string &port)
{
	this->port = port;
}

void ServerQuery::setPassword(const std::string &password)
{
	this->password = password;
}

void ServerQuery::setupRead(void)
{
	this->sock.async_read_some(
			boost::asio::buffer(
				&this->readBuffer[this->readSz],
				ServerQuery::MSG_MAX_SZ - this->readSz),
			boost::bind(
				&ServerQuery::handleRead,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
}

void ServerQuery::handleRead(
	const boost::system::error_code& err,
	size_t nBytes)
{
	MessageHeader *header = reinterpret_cast<MessageHeader *>(&this->readBuffer[0]);

	std::cout << "Reading Msg: " << nBytes << std::endl;
	if (err)
		return;
	this->readSz += nBytes;
	while (this->readSz >= sizeof(MessageHeader)) {
		header = reinterpret_cast<MessageHeader*>(&this->readBuffer[0]);
		if (header->sz > ServerQuery::MSG_MAX_SZ) {
			this->readSz = 0;
			return;
		}
		long overflow = this->readSz - (sizeof(MessageHeader) + header->sz);
		if (overflow < 0)
			break;
		(this->*msgMap[static_cast<ServerQuery::MessageType>(header->type)])(
			reinterpret_cast<const BYTE *>(header + 1),
			header->sz
		);
		/* Handle Message + Check if type exists */
		this->readSz = overflow;
		if (this->readSz > 0)
			std::memmove(&this->readBuffer[0], &this->readBuffer[overflow], overflow); /* Move next message to beginning */
	}
	this->setupRead();
}

void ServerQuery::sendMessage(MessageType type, const BYTE *msg, unsigned int sz)
{
	BYTE *netMsg = new BYTE[sizeof(MessageHeader) + sz];
	MessageHeader *header = reinterpret_cast<MessageHeader*>(netMsg);
	boost::system::error_code err;

	header->type = type;
	header->sz = sz;
	std::memcpy(header + 1, msg, sz);
	boost::asio::write(
		this->sock,
		boost::asio::buffer(netMsg, sizeof(MessageHeader) + sz),
		boost::asio::transfer_all(),
		err
	);
	std::cout << "Writing: " << sz << std::endl;
	if (err)
		std::cout << "Err sending msg: " << err.message() << std::endl;
	delete [] netMsg;
}

void ServerQuery::msgLogin(const BYTE *data, unsigned)
{
	/* Handle failed login */
	this->loggedIn = true;
	std::cout << "Client received" << std::endl;
}

void ServerQuery::msgPlayerJoined(const BYTE *data, unsigned)
{
	Player *player = new Player;
	const MsgPlayerJoined *netPlayer = (const MsgPlayerJoined *) data;

	std::cout << "Player Joined" << std::endl;

	player->id = netPlayer->id;
	player->uid = netPlayer->uid;
	std::memcpy(&player->ipAddress[0], &netPlayer->ipAddress[0], 4);
	player->port = netPlayer->port;
	player->name = std::string((const char *) netPlayer->name);
	player->role = netPlayer->role;
	gWarbandServer->addPlayer(player);
}

void ServerQuery::msgPlayerLeft(const BYTE *data, unsigned)
{
	const MsgPlayerLeft *player = (const MsgPlayerLeft *) data;

	gWarbandServer->removePlayer(player->uid);
}

void ServerQuery::sendLoginMsg(const std::string &password)
{
	/* Add password block */
	this->sendMessage(MessageType::CTRL_LOGIN, 0, 0);
}
