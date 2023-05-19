#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace schwi {
	class SCHWI_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

#define SW_CORE_TRACE(...) schwi::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SW_CORE_DEBUG(...) schwi::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define SW_CORE_INFO(...) schwi::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SW_CORE_WARN(...) schwi::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SW_CORE_ERROR(...) schwi::Log::GetCoreLogger()->error(__VA_ARGS__)

#define SW_TRACE(...) schwi::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SW_DEBUG(...) schwi::Log::GetClientLogger()->debug(__VA_ARGS__)
#define SW_INFO(...) schwi::Log::GetClientLogger()->info(__VA_ARGS__)
#define SW_WARN(...) schwi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SW_ERROR(...) schwi::Log::GetClientLogger()->error(__VA_ARGS__)