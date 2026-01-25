#include "EngineCore/System/Log.h"

#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/System/SysFunc.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <filesystem>

#if defined(_WIN32)
#include <Windows.h>
#define PATH_PIECE "\\"
#elif defined(__linux__) || defined(__APPLE__)
#define PATH_PIECE "/"
#define UNIX_WAY_OS
#endif

std::string LogSystem::m_path;

std::string currentDateTime() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	auto tim = std::chrono::high_resolution_clock::now();

	auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(tim.time_since_epoch()).count();

	std::string tmp = std::to_string(sec);

	char buffer[128];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now);
	return std::string(buffer) + "." + tmp.substr(tmp.size() - 3);
}

void LogSystem::init_log_system(std::string relPathFolder)
{ 	
	std::filesystem::create_directory(ResourceManager::getExeFilePath() + PATH_PIECE + relPathFolder);
	if (std::filesystem::exists(ResourceManager::getExeFilePath() + PATH_PIECE + relPathFolder + PATH_PIECE + "lastest.log"))
	{
		std::time_t t = std::time(nullptr);
		std::tm* now = std::localtime(&t);
		char buffer[128];
		strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", now);

		std::filesystem::copy_file(ResourceManager::getExeFilePath() + PATH_PIECE + relPathFolder + PATH_PIECE + "lastest.log", 
			ResourceManager::getExeFilePath() + PATH_PIECE + relPathFolder + PATH_PIECE + buffer + ".log");
	}
	m_path = ResourceManager::getExeFilePath() + PATH_PIECE + relPathFolder + PATH_PIECE + "lastest.log";
	std::ofstream stream(m_path, std::ios::out);
	if (stream.is_open())
	{
		stream << "";
		stream.close();
	}
} 

void LogSystem::uninit_log_system()
{

}

void LogSystem::log_info(std::string msg)
{
#if defined(DEBUG_CONSOLE_LOG) && defined(_WIN32)
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "[" << currentDateTime() << "] [";
	SetConsoleTextAttribute(h, (((0 << 4) | 2)));
	std::cout << "info";
	SetConsoleTextAttribute(h, (((0 << 4) | 7)));
	std::cout << "] " << msg << "\n";
#elif defined(DEBUG_CONSOLE_LOG) && defined(UNIX_WAY_OS)
	std::cout << "[\033[36m" << currentDateTime() << "\033[39m] [\033[32minfo\033[39m] " << msg << "\n";
#endif
	std::ofstream stream(m_path, std::ios::app);
	if (stream.is_open())
	{
		stream << "[" + currentDateTime() + "] [info] " + msg + "\n";
		stream.close();
	}
}

void LogSystem::log_warn(std::string msg)
{
#if defined(DEBUG_CONSOLE_LOG) && defined(_WIN32)
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "[" << currentDateTime() << "] [";
	SetConsoleTextAttribute(h, (((0 << 4) | 14)));
	std::cout << "warn";
	SetConsoleTextAttribute(h, (((0 << 4) | 7)));
	std::cout << "] " << msg << "\n";
#elif defined(DEBUG_CONSOLE_LOG) && defined(UNIX_WAY_OS)
	std::cout << "[\033[36m" << currentDateTime() << "\033[39m] [\033[33mwarn\033[39m] " << msg << "\n";
#endif
	std::ofstream stream(m_path, std::ios::app);
	if (stream.is_open())
	{
		stream << "[" + currentDateTime() + "] [warn] " + msg + "\n";
		stream.close();
	}
}

void LogSystem::log_error(std::string msg)
{
#if defined(DEBUG_CONSOLE_LOG) && defined(_WIN32)
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "[" << currentDateTime() << "] [";
	SetConsoleTextAttribute(h, (((0 << 4) | 4)));
	std::cout << "error";
	SetConsoleTextAttribute(h, (((0 << 4) | 7)));
	std::cout << "] " << msg << "\n";
#elif defined(DEBUG_CONSOLE_LOG) && defined(UNIX_WAY_OS)
	std::cout << "[\033[36m" << currentDateTime() << "\033[39m] [\033[31merror\033[39m] " << msg << "\n";
#endif
	std::ofstream stream(m_path, std::ios::app);
	if (stream.is_open())
	{
		stream << "[" + currentDateTime() + "] [error] " + msg + "\n";
		stream.close();
	}
}

void LogSystem::log_crit(std::string msg)
{
#if defined(DEBUG_CONSOLE_LOG) && defined(_WIN32)
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "[" << currentDateTime() << "] [";
	SetConsoleTextAttribute(h, (((4 << 4) | 15)));
	std::cout << "critical";
	SetConsoleTextAttribute(h, (((0 << 4) | 7)));
	std::cout << "] " << msg << "\n";
#elif defined(DEBUG_CONSOLE_LOG) && defined(UNIX_WAY_OS)
	std::cout << "[\033[36m" << currentDateTime() << "\033[39m] [\033[37m\033[41mcritial\033[39m\033[49m] " << msg << "\n";
#endif
	std::ofstream stream(m_path, std::ios::app);
	if (stream.is_open())
	{
		stream << "[" + currentDateTime() + "] [critical] " + msg + "\n";
		stream.close();
	}
}

namespace std {

string to_string(HexFormatLogging number) {
  stringstream ss;
  if (number.m_sugar) ss << "0x";
  ss << std::hex << std::uppercase << number.m_number;
  return ss.str();
}

string to_string(string str)
{
	return str;
}

string to_string(wstring str)
{
	return sysfunc::ctostr(str);
}

wstring to_wstring(string str)
{
	return sysfunc::ctowstr(str);
}

wstring to_wstring(wstring str)
{
	return str;
}

}
