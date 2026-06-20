#pragma once
#include <string>
#include <fstream>
#include <iostream>

enum class LogType
{
	INFO,
	WARNING,
	ERROR
};

class Logger
{
public:
	static void Init();
	static void Shutdown();

	// It is not called directly, it is called via macros
	// type, file, line, message
	static void Log(LogType, const char*, int, const std::string&); 

private:
	static std::ofstream m_fileStream;

	static std::string CurrentDateTime();
	static std::string GetFileName(const char*);
};

// CONDITIONAL COMPILATION
#if defined(DEBUG) || defined(_DEBUG) // DEBUG

	// __FILE__: It automatically retrieves the NAME OF THE FILE where that code was written at that moment.
	// __LINE__: It automatically retrieves the LINE NUMBER where that code was written at that moment.
	#define LOG_INFO(msg)  Logger::Log(LogType::INFO, __FILE__, __LINE__, msg)
	#define LOG_WARN(msg)  Logger::Log(LogType::WARNING, __FILE__, __LINE__, msg)
	#define LOG_ERROR(msg) Logger::Log(LogType::ERROR, __FILE__, __LINE__, msg)

#else // RELEASE

	// RELEASE mode -> skip logger lines
	#define LOG_INFO(msg)
	#define LOG_WARN(msg) 
	#define LOG_ERROR(msg)

#endif
