#pragma once

/*
 * Use 
 * target_compile_definitions(EngineCore PUBLIC DEBUG_ENGINE)
 * in CMakeLists.txt for enable console debug logging
 */

//#define DEBUG_CONSOLE_OBJ_LOAD

#ifdef DEBUG_ENGINE
#define DEBUG_CONSOLE_ON
#define DEBUG_CONSOLE_LOG
#endif

#include "EngineCore/System/LogSystem.h"

#define LOG_INFO(...)	LogSystem::info(__VA_ARGS__)
#define LOG_WARN(...)	LogSystem::warn(__VA_ARGS__)
#define LOG_ERROR(...)	LogSystem::error(__VA_ARGS__)
#define LOG_CRIT(...)	LogSystem::critical(__VA_ARGS__)
#define HEX_FMT(__number__) HexFormatLogging(__number__)

#if !defined(DEBUG_CONSOLE_ON) && defined(WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
