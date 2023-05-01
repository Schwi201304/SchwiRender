#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

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

#define SW_TRACE(...) schwi::Log::GetLogger()->trace(__VA_ARGS__);
#define SW_DEBUG(...) schwi::Log::GetLogger()->debug(__VA_ARGS__);
#define SW_INFO(...) schwi::Log::GetLogger()->info(__VA_ARGS__);
#define SW_WARN(...) schwi::Log::GetLogger()->warn(__VA_ARGS__);
#define SW_ERROR(...) schwi::Log::GetLogger()->error(__VA_ARGS__);