#include "Addresses.hpp"

const unsigned int Addresses::playerJoined_EntryPoint = 0x00428CBF;
const unsigned int Addresses::playerJoined_ExitPoint = 0x00428CC6;
unsigned int Addresses::playerJoined_playerBase;
const unsigned int Addresses::playerJoined_playerUID_offset = 0x787C;
const unsigned int Addresses::playerJoined_playerID_offset = 0x0;
const unsigned int Addresses::playerJoined_playerRole_offset = 0x0F070;
const unsigned int Addresses::playerJoined_playerName_offset = 0x7886;


const unsigned int Addresses::chatMessageSent_EntryPoint = 0x004398DE;
const unsigned int Addresses::chatMessageSent_ExitPoint = 0x004398E4;