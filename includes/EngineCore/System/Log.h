#pragma once

//#define DEBUG_CONSOLE_OBJ_LOAD

#define DEBUG_ENGINE

#ifdef DEBUG_ENGINE
#define DEBUG_CONSOLE_ON 
#define DEBUG_CONSOLE_LOG
#endif // DEBUG_CONSOLE

#include "EngineCore/System/LogSystem.h"

#define LOG_INFO(...)	LogSystem::info(__VA_ARGS__)
#define LOG_WARN(...)	LogSystem::warn(__VA_ARGS__)
#define LOG_ERROR(...)	LogSystem::error(__VA_ARGS__)
#define LOG_CRIT(...)	LogSystem::critical(__VA_ARGS__)
#define HEX_FMT(__number__) HexFormatLogging(__number__)

#ifndef DEBUG_CONSOLE_ON 
#ifdef WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // WIN32
#endif // DEBUG_CONSOLE