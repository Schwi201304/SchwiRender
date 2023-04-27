#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Schwi.h"

namespace schwi {
	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return logger; }

	private:
		static std::shared_ptr<spdlog::logger> logger;

	};
}

#define SW_LOG_TRACE(...) schwi::Log::GetLogger()->trace(__VA_ARGS__);
#define SW_LOG_DEBUG(...) schwi::Log::GetLogger()->debug(__VA_ARGS__);
#define SW_LOG_INFO(...) schwi::Log::GetLogger()->info(__VA_ARGS__);
#define SW_LOG_WARN(...) schwi::Log::GetLogger()->warn(__VA_ARGS__);
#define SW_LOG_ERROR(...) schwi::Log::GetLogger()->error(__VA_ARGS__);