// LogSystem.h

#pragma once

#define CALLER __FUNCTION__, __FILE__, std::to_string(__LINE__)

#define LOG_INFO(format, ...)		LogSystem::SetLog("INFO", CALLER, format, __VA_ARGS__)
#define LOG_WARNING(format, ...)	LogSystem::SetLog("WARNING", CALLER, format, __VA_ARGS__)
#define LOG_ERROR(format, ...)		LogSystem::SetLog("ERROR", CALLER, format, __VA_ARGS__)

#define LOG_TARGET_ALL()			LogSystem::SetTarget(LogSystem::TARGET::ALL)
#define LOG_TARGET_CONSOLE()		LogSystem::SetTarget(LogSystem::TARGET::CONSOLE)
#define LOG_TARGET_FILE()			LogSystem::SetTarget(LogSystem::TARGET::FILE)

namespace LogSystem
{
	// Log를 출력할 타겟 Enum
	enum class TARGET
	{
		ALL,
		CONSOLE,
		FILE
	};

	extern void SetTarget(const TARGET target);

	extern void SetLog
	(
		const std::string type, 
		const std::string function, 
		const std::string file,
		const std::string line, 
		const std::string format, 
		...
	);
}