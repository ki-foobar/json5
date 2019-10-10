#pragma once

#include <cmath>
#include "../types.hpp"
#include "./util.hpp"



namespace json5
{
namespace detail
{

enum class token_type
{
    eof,

    bracket_left,
    bracket_right,
    brace_left,
    brace_right,
    colon,
    comma,

    null,
    true_,
    false_,
    infinity,
    nan,

    integer,
    number,
    string,
    identifier,
};



struct token
{
public:
    token()
        : _type(token_type::eof)
        , _integer(0)
    {
    }



    token(token_type type)
        : _type(type)
        , _integer(0)
    {
    }



    token(integer_type value)
        : _type(token_type::integer)
        , _integer(value)
    {
    }



    token(number_type value)
        : _type(token_type::number)
        , _number(value)
    {
    }



    token(token_type type, const string_type& value)
        : _type(type)
        , _string(new std::string(value))
    {
        assert(type == token_type::string || type == token_type::identifier);
    }



    token(token_type type, const string_type::value_type* value)
        : _type(type)
        , _string(new std::string(value))
    {
        assert(type == token_type::string || type == token_type::identifier);
    }



    token(const token& other)
        : _type(other._type)
    {
        switch (_type)
        {
        case token_type::integer: _integer = other._integer; break;
        case token_type::number: _number = other._number; break;
        case token_type::string:
        case token_type::identifier:
            _string = new std::string(*other._string);
            break;
        default:
            _integer = 0; // dummy
            break;
        }
    }



    token(token&& other) noexcept
        : _type(other._type)
    {
        switch (_type)
        {
        case token_type::integer: _integer = other._integer; break;
        case token_type::number: _number = other._number; break;
        case token_type::string:
        case token_type::identifier:
            _string = other._string;
            other._string = nullptr;
            break;
        default:
            _integer = 0; // dummy
            break;
        }
    }



    token& operator=(const token& other)
    {
        token tmp = other;
        tmp.swap(*this);
        return *this;
    }



    token& operator=(token&& other) noexcept
    {
        token tmp = std::move(other);
        tmp.swap(*this);
        return *this;
    }



    void swap(token& other)
    {
        std::swap(_type, other._type);
        std::swap(_integer, other._integer); // TODO
    }



    ~token()
    {
        if (_type == token_type::string || _type == token_type::identifier)
        {
            destroy(_string);
        }
    }



    constexpr token_type type() const noexcept
    {
        return _type;
    }



    constexpr integer_type get_integer() const noexcept
    {
        return _integer;
    }



    constexpr number_type get_number() const noexcept
    {
        return _number;
    }



    constexpr const string_type& get_string() const noexcept
    {
        return *_string;
    }



    std::string to_string() const
    {
        switch (_type)
        {
        case token_type::eof: return "<EOF>";
        case token_type::bracket_left: return "'['";
        case token_type::bracket_right: return "']'";
        case token_type::brace_left: return "'{'";
        case token_type::brace_right: return "'}'";
        case token_type::colon: return "':'";
        case token_type::comma: return "','";
        case token_type::null: return "null";
        case token_type::true_: return "true";
        case token_type::false_: return "false";
        case token_type::infinity: return "Infinity";
        case token_type::nan: return "NaN";
        case token_type::integer: return std::to_string(get_integer());
        case token_type::number:
        {
            const auto p = get_number();
            if (std::isnan(p))
            {
                return "NaN";
            }
            else if (std::isinf(p))
            {
                return p < 0 ? "-Infinity" : "Infinity";
            }
            else
            {
                return std::to_string(p);
            }
        }
        case token_type::string:
        case token_type::identifier: return get_string();
        }
    }



private:
    token_type _type;


    union
    {
        integer_type _integer;
        number_type _number;
        string_type* _string;
    };
};

} // namespace detail
} // namespace json5
