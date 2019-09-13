#ifndef SERVERQUERY_HPP
#define SERVERQUERY_HPP

#include <boost/asio.hpp>
#include <string>
#include <unordered_map>

class ServerQuery
{
public:
	static const unsigned MSG_MAX_SZ = 512;

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
		BYTE ipAddress[4];
		UINT16 port;
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

	ServerQuery();
	virtual ~ServerQuery() = default;

	void connect();
	void connect(const std::string &address, const std::string &port, const std::string &password);
	void setupRead();
	void handleRead(const boost::system::error_code& err, size_t nBytes);
	void sendMessage(MessageType type, const BYTE *msg, unsigned sz);

private:
	BYTE readBuffer[MSG_MAX_SZ];
	unsigned readSz = 0;
	boost::asio::io_service ioService;
	boost::asio::ip::tcp::socket sock;
	bool loggedIn = false;
	std::unordered_map<MessageType, void (ServerQuery::*)(const BYTE *, unsigned)> msgMap;

	void msgLogin(const BYTE *data, unsigned sz);
	void msgPlayerJoined(const BYTE *data, unsigned sz);
	void sendLoginMsg(const std::string &password);
};

#endif // SERVERQUERY_HPP
