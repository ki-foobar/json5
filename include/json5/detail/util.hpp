#pragma once

#include <limits>
#include "../types.hpp"



namespace json5
{
namespace detail
{

inline constexpr number_type infinity() noexcept
{
    return std::numeric_limits<number_type>::infinity();
}



inline constexpr number_type nan() noexcept
{
    return std::numeric_limits<number_type>::quiet_NaN();
}



template <typename T>
void destroy(T* ptr)
{
    if (ptr)
    {
        ptr->~T();
    }
}



inline constexpr bool is_digit(char c) noexcept
{
    return '0' <= c && c <= '9';
}



inline constexpr bool is_hex_digit(char c) noexcept
{
    return is_digit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}



inline constexpr bool is_identifier_start(char c) noexcept
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_' ||
        c == '$';
}



inline constexpr bool is_identifier_continue(char c) noexcept
{
    return is_identifier_start(c) || is_digit(c);
}

} // namespace detail
} // namespace json5
