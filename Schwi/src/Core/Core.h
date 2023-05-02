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