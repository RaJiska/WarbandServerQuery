#pragma once

#include <boost/asio.hpp>
#include <memory>

class Client
{
public:
	const static unsigned MSG_MAX_SZ = 512;

	Client(unsigned long long int id, boost::asio::io_service& ios);
	virtual ~Client() = default;

	virtual boost::asio::ip::tcp::socket &getSocket() noexcept;
	virtual void send(const BYTE *msg, unsigned sz);
	virtual BYTE *getData(void) noexcept;
	virtual unsigned &getReadDataSz(void) noexcept;
	virtual bool &isLoggedIn(void) noexcept;
	virtual bool operator==(const Client &client) const noexcept;
	virtual bool operator!=(const Client &client) const noexcept;

	const unsigned long long int id;

private:
	void handleWrite(const boost::system::error_code &, size_t);

	boost::asio::ip::tcp::socket socket;
	BYTE readData[Client::MSG_MAX_SZ * 2];
	unsigned readDataSz;
	bool loggedIn = false;
};