#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player
{
public:
	unsigned id;
	unsigned uid;
	std::string name;
	unsigned role;

	Player();
	virtual ~Player() = default;
};

#endif // PLAYER_HPP
