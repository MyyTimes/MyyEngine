#define _CRT_SECURE_NO_WARNINGS

#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// static file stream
std::ofstream Logger::m_fileStream;

void Logger::Init()
{
	// std::ios::app (Append) 
	// std::ios::out (Out) : only write
	m_fileStream.open("EngineLog.txt", std::ios::out | std::ios::app);

	if (m_fileStream.is_open())
	{
		LOG_INFO("STARTED");
	}
	else
	{
		std::cerr << "[ERROR] EngineLog.txt file could not be created!" << std::endl;
	}
}

void Logger::Shutdown()
{
	if (m_fileStream.is_open())
	{
		LOG_INFO("ENDED");
		m_fileStream.close();
	}
}

// Timestamp
std::string Logger::CurrentDateTime()
{
	auto now = std::chrono::system_clock::now();
	std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	// Format: YYYY-MM-DD HH:MM:SS
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss.str();
}

std::string Logger::GetFileName(const char* path)
{
	std::string fullPath(path);
	size_t pos = fullPath.find_last_of("/\\");
	return (pos == std::string::npos) ? fullPath : fullPath.substr(pos + 1);
}

void Logger::Log(LogType type, const char* file, int line, const std::string& message)
{
	std::string typeStr;
	switch (type)
	{
		case LogType::INFO:    typeStr = "INFO "; break;
		case LogType::WARNING: typeStr = "WARN "; break;
		case LogType::ERROR:   typeStr = "ERROR"; break;
	}

	std::string fileName = GetFileName(file);
	std::string timeStr = CurrentDateTime();

	std::stringstream ss;
	ss << "[" << timeStr << "] [" << typeStr << "] [" << fileName << ":" << line << "] " << message;
	std::string finalMessage = ss.str();

	// Console
	if (type == LogType::ERROR)
	{
		std::cerr << finalMessage << std::endl;
	}
	else
	{
		std::cout << finalMessage << std::endl; 
	}

	// File
	if (m_fileStream.is_open())
	{
		m_fileStream << finalMessage << "\n";
		// to protect the logs: flush() -> It writes directly to the hard drive. 
		m_fileStream.flush();
	}
}
