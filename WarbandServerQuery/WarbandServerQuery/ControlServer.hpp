#pragma once

#include "Client.hpp"

#include <boost/asio.hpp>
#include <memory>
#include <list>
#include <unordered_map>

class ControlServer
{
public:
	typedef enum
	{
		CTRL_LOGIN = 0,
		CTRL_LOGOUT,
		GAME_PLAYER_JOINED,
		GAME_PLAYER_LEFT,
		GAME_KICK_PLAYER,
		GAME_BAN_PLAYER,
		GAME_LOG_ENTRY
	} MessageType;

	#pragma pack(push, 1)
	typedef struct
	{
		UINT8 type; /* MessageType */
		UINT32 sz;
	} MessageHeader;
	#pragma pack(pop)

	#pragma pack(push, 1)
	typedef struct
	{
		UINT32 id;
		UINT32 uid;
		BYTE name[30];
		UINT32 role;
	} MsgPlayerJoined;
	#pragma pack(pop)

	#pragma pack(push, 1)
	typedef struct
	{
		UINT32 id;
	} MsgPlayerLeft;
	#pragma pack(pop)

	ControlServer(UINT16 port);
	~ControlServer();

	virtual void run();
	virtual void sendMessage(unsigned long long id, MessageType type, const BYTE *msg, unsigned sz);
	virtual void broadcastMessage(MessageType type, const BYTE *msg, unsigned sz);

private:
	const char *PASSWORD = "hello";

	void disconnectClient(std::shared_ptr<Client> &client) noexcept;
	void setupAccept();
	void handleAccept(std::shared_ptr<Client> &client, const boost::system::error_code &error);
	void setupRead(std::shared_ptr<Client> &client);
	void handleRead(const boost::system::error_code &err, size_t nBytes, std::shared_ptr<Client> &client);
	void loginClient(unsigned long long id, const BYTE *msg, unsigned sz);
	void logoutClient(unsigned long long id, const BYTE *msg, unsigned sz);

	std::unordered_map<
		MessageType,
		std::function<void(unsigned long long, const BYTE *, unsigned)>
	> msgMap;
	std::list<std::shared_ptr<Client>> clients;
	boost::asio::io_service ioService;
	boost::asio::ip::tcp::acceptor acceptor;
	std::vector<boost::asio::ip::tcp::socket> sockets;
	unsigned long long int clientIdCounter = 0;
};