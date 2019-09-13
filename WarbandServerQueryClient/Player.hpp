#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player
{
public:
	unsigned id;
	unsigned uid;
	unsigned char ipAddress[4];
	unsigned short port;
	std::string name;
	unsigned role;

	Player();
	virtual ~Player() = default;
};

#endif // PLAYER_HPP
