#pragma once

#include <string>
#include <vector>

struct HexFormatLogging {
  HexFormatLogging(long long number, bool sugar = true) : m_number(number), m_sugar(sugar) {}

  HexFormatLogging() = delete;
  long long m_number;
  bool m_sugar;
};

namespace std {
string to_string(HexFormatLogging number);
string to_string(string str);
string to_string(wstring str);
wstring to_wstring(string str);
wstring to_wstring(wstring str);
}

class LogSystem
{	
public:
	~LogSystem() = delete;
	LogSystem() = delete;

	LogSystem(const LogSystem&) = delete;
	LogSystem(const LogSystem&&) = delete;
	LogSystem& operator=(const LogSystem&) = delete;
	LogSystem& operator=(const LogSystem&&) = delete;
	
	static void init_log_system(std::string relPathFolder);
	static void uninit_log_system();

	static void log_info(std::string msg);
	static void log_warn(std::string msg);
	static void log_error(std::string msg);
	static void log_crit(std::string msg);

	template<typename... _Args>
	static void info(std::string text, _Args &&...args)
	{
    std::vector<std::string> strs;

    (strs.push_back(std::to_string(args)), ...);

    std::string temp;
    size_t start = -1;
    size_t size = text.size();
    for (size_t i = 0; i < size; ++i) {
      if (text[i] == '\\' && (i + 1) < size && text[i + 1] == '{') {
        ++i;
      } else {
        if (text[i] == '{') {
          ++i;
          start = i;
          while (text[i] != '}' && i < text.size()) {
            ++i;
          }
          int j = std::stoi(text.substr(start, i));
          temp += strs[j];
          continue;
        }
      }
      temp += text[i];
    }
    
    log_info(temp);
	}

	template<typename... _Args>
	static void warn(std::string text, _Args &&...args)
	{
		std::vector<std::string> strs;

		(strs.push_back(std::to_string(args)), ...);

		std::string temp;
    size_t start = -1;
    size_t size = text.size();
    for (size_t i = 0; i < size; ++i) {
      if (text[i] == '\\' && (i + 1) < size && text[i + 1] == '{') {
        ++i;
      } else {
        if (text[i] == '{') {
          ++i;
          start = i;
          while (text[i] != '}' && i < text.size()) {
            ++i;
          }
          int j = std::stoi(text.substr(start, i));
          temp += strs[j];
          continue;
        }
      }
      temp += text[i];
    }

		log_warn(temp);
	}

	template<typename... _Args>
	static void error(std::string text, _Args &&...args)
	{
		std::vector<std::string> strs;

		(strs.push_back(std::to_string(args)), ...);

		std::string temp;
    size_t start = -1;
    size_t size = text.size();
    for (size_t i = 0; i < size; ++i) {
      if (text[i] == '\\' && (i + 1) < size && text[i + 1] == '{') {
        ++i;
      } else {
        if (text[i] == '{') {
          ++i;
          start = i;
          while (text[i] != '}' && i < text.size()) {
            ++i;
          }
          int j = std::stoi(text.substr(start, i));
          temp += strs[j];
          continue;
        }
      }
      temp += text[i];
    }

		log_error(temp);
	}

	template<typename... _Args>
	static void critical(std::string text, _Args &&...args)
	{
		std::vector<std::string> strs;

		(strs.push_back(std::to_string(args)), ...);

		std::string temp;
    size_t start = -1;
    size_t size = text.size();
    for (size_t i = 0; i < size; ++i) {
      if (text[i] == '\\' && (i + 1) < size && text[i + 1] == '{') {
        ++i;
      } else {
        if (text[i] == '{') {
          ++i;
          start = i;
          while (text[i] != '}' && i < text.size()) {
            ++i;
          }
          int j = std::stoi(text.substr(start, i));
          temp += strs[j];
          continue;
        }
      }
      temp += text[i];
    }

		log_crit(temp);
	}
private:
	static std::string m_path;
};