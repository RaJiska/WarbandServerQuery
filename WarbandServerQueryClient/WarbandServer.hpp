#ifndef WARBANDSERVER_HPP
#define WARBANDSERVER_HPP

#include "Player.hpp"

#include <vector>

class WarbandServer
{
public:
	WarbandServer();
	virtual ~WarbandServer() = default;

	virtual void addPlayer(const Player &player);
	virtual void kickPlayer(const Player &player) noexcept;

private:
	std::vector<Player> players;
};

#endif // WARBANDSERVER_HPP
