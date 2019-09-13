#pragma once

#define WIN32_LEAN_AND_MEAN

#include "ControlServer.hpp"
#include "WarbandServer.hpp"

class ControlServer;
class WarbandServer;

extern ControlServer *gControlServer;
extern WarbandServer *gWarbandServer;