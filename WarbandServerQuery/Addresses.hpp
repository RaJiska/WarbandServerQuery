#pragma once

#include "WarbandServerQuery.hpp"

struct Addresses
{
	const static unsigned int playerJoined_EntryPoint;
	const static unsigned int playerJoined_ExitPoint;
	static unsigned int playerJoined_playerBase;
	const static unsigned int playerJoined_playerUID_offset;
	const static unsigned int playerJoined_playerID_offset;
	const static unsigned int playerJoined_playerRole_offset;
	const static unsigned int playerJoined_playerName_offset;

	const static unsigned int chatMessageSent_EntryPoint;
	const static unsigned int chatMessageSent_ExitPoint;
};