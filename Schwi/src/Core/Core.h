#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#define SW_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#else
	#error "Only Windows-x64 is supported!"
#endif// _WIN32

#ifdef _DEBUG
	#define SW_ASSERT(x, ...) { if(!(x)) { SW_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }  
#else
	#define SW_ASSERT(...)
#endif //_DEBUG

#ifdef SW_PLATFORM_WINDOWS
	#ifdef SW_BUILD_DLL
		#define SCHWI_API __declspec(dllexport)
	#else
		#define SCHWI_API __declspec(dllimport)
	#endif
#else
	#error Hazel only supports Windows!
#endif

#define SW_FMT_FOMATTER(T) template<>                              \
struct fmt::formatter<T> : fmt::formatter<std::string>             \
{                                                                  \
    auto format(T e, format_context& ctx) -> decltype(ctx.out())   \
    {                                                              \
        return format_to(ctx.out(), "{}", e.ToString());           \
    }                                                              \
};                                                                                                   

//#define SW_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
#define SW_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include <memory>
namespace schwi
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ... Args>
	constexpr Ref<T> CastRef(Args&& ... args)
	{
		return std::dynamic_pointer_cast<T>(std::forward<Args>(args)...);
	}
}