#pragma once

#define WIN32_LEAN_AND_MEAN

#include "ControlServer.hpp"
#include "WarbandServer.hpp"

static ControlServer gControlServer(8888);
static WarbandServer gWarbandServer;