#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <fmt/ostream.h>

#define SW_FMT_FOMATTER(T) template<>                              \
struct fmt::formatter<T> : fmt::formatter<std::string>             \
{                                                                  \
    auto format(T e, format_context& ctx) -> decltype(ctx.out())   \
    {                                                              \
        return format_to(ctx.out(), "{}", e.ToString());           \
    }                                                              \
};                                                                 \