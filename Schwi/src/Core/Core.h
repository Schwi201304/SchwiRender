#pragma once

#define SW_FMT_FOMATTER(T) template<>                              \
struct fmt::formatter<T> : fmt::formatter<std::string>             \
{                                                                  \
    auto format(T e, format_context& ctx) -> decltype(ctx.out())   \
    {                                                              \
        return format_to(ctx.out(), "{}", e.ToString());           \
    }                                                              \
};                                                                                                   

#define SW_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define SW_ASSERT(x, ...) { if(!(x)) { SW_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }  

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