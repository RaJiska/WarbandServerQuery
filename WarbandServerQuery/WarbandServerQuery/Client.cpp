#include "Client.hpp"

#include <boost/bind.hpp>

Client::Client(unsigned long long int id, boost::asio::io_service& ios) : id(id), socket(ios)
{
}

boost::asio::ip::tcp::socket &Client::getSocket() noexcept
{
	return this->socket;
}

void Client::send(const BYTE *msg, unsigned sz)
{
	boost::system::error_code err;

	boost::asio::write(
		this->socket,
		boost::asio::buffer(msg, sz),
		boost::asio::transfer_all(),
		err
	);
}

BYTE *Client::getData() noexcept
{
	return &this->readData[0];
}

unsigned &Client::getReadDataSz(void) noexcept
{
	return this->readDataSz;
}

bool &Client::isLoggedIn(void) noexcept
{
	return this->loggedIn;
}

bool Client::operator==(const Client& client) const noexcept
{
	return (client.id == this->id);
}

bool Client::operator!=(const Client &client) const noexcept
{
	return !((*this) == client);
}

void Client::handleWrite(const boost::system::error_code &, size_t)
{
}