#pragma once

#include "./token.hpp"
#include "./util.hpp"



namespace json5
{
namespace detail
{

class lexer
{
public:
    lexer(const std::string& source)
        : _sourse(source)
        , _pos(0)
    {
    }



    token scan()
    {
        skip_whitespaces_and_comments();

        if (eof())
            return token{token_type::eof};

        return scan_internal();
    }



private:
    std::string _sourse;
    size_t _pos;



    void skip_whitespaces_and_comments()
    {
        if (eof())
            return;

        while (true)
        {
            const auto c = peek();
            switch (c)
            {
            case '/':
            {
                get();
                if (eof())
                {
                    throw lex_error{"<EOF>", "'//' or '/*'"};
                }

                const auto k = get();
                if (k == '/')
                {
                    while (!eof())
                    {
                        const auto c = get();
                        if (c == '\r')
                        {
                            if (!eof() && peek() == '\n')
                            {
                                get();
                            }
                            break;
                        }
                        else if (c == '\n')
                        {
                            break;
                        }
                    }
                }
                else if (k == '*')
                {
                    bool previos_char_is_asterisk = false;
                    while (true)
                    {
                        if (eof())
                        {
                            throw lex_error{"<EOF>", "'*/'"};
                        }

                        const auto c = get();
                        if (c == '*')
                        {
                            previos_char_is_asterisk = true;
                        }
                        else if (c == '/')
                        {
                            if (previos_char_is_asterisk)
                            {
                                break;
                            }
                        }
                        else
                        {
                            previos_char_is_asterisk = false;
                        }
                    }
                }
                else
                {
                    throw lex_error{format_char(k), "'//' or '/*'"};
                }
            }
            break;
            case ' ':
            case '\t':
            case '\r':
            case '\n': get(); break;
            default: return;
            }
        }
    }



    token scan_internal()
    {
        const auto c = peek();
        switch (c)
        {
        case '[': get(); return token{token_type::bracket_left};
        case ']': get(); return token{token_type::bracket_right};
        case '{': get(); return token{token_type::brace_left};
        case '}': get(); return token{token_type::brace_right};
        case ':': get(); return token{token_type::colon};
        case ',': get(); return token{token_type::comma};
        case '\'':
        case '"': return scan_string();
        default: return scan_numeric_or_identifier();
        }
    }



    token scan_string()
    {
        std::string s;
        const auto q = get(); // ' or "
        while (true)
        {
            if (eof())
            {
                throw lex_error{"<EOF>", q == '"' ? "'\"'" : "'"};
            }

            const auto c = get();
            if (c == q)
            {
                break;
            }
            else if (c == '\r' || c == '\n')
            {
                throw lex_error{"line break", c == '\r' ? "\\\\r" : "\\\\n"};
            }
            else if (c == '\\')
            {
                s += scan_escape_sequence();
            }
            else
            {
                s += c;
            }
        }
        return token{token_type::string, s};
    }



    std::string scan_escape_sequence()
    {
        if (eof())
        {
            throw lex_error{"<EOF>", "escape sequence"};
        }

        const auto c = get();
        switch (c)
        {
        case '\'': return "'";
        case '\\': return "\\";
        case '"': return "\"";
        case 'a': return "\a";
        case 'b': return "\b";
        case 'f': return "\f";
        case 't': return "\t";
        case 'r': return "\r";
        case 'n': return "\n";
        case '0': return "\0";
        default:
            // todo
            return "";
        }
    }



    /*
    NumericOrIdentifier
        Numeric
        Identifier

    Numeric
        Sign? 'Infinity'
        Sign? 'NaN'
        Sign? HexadecimalInteger
        Sign? '0' Digit* ('.' Digit*)? Exponent?
        Sign? NonZeroDigit Digit* ('.' Digit*)? Exponent?
        Sign? '.' Digit+ Exponent?

    Sign
        '+'
        '-'

    Exponent
        ExponentPrefix Sign? Digit+

    ExponentPrefix
        'e'
        'E'

    HexadecimalInteger
        HexadecimalPrefix HexDigit+

    HexadecimalPrefix
        '0x'
        '0X'

    Digit
        '0'
        NonZeroDigit

    NonZeroDigit
        '1' | '2' | ... | '9'

    HexDigit
        Digit
        'a' | 'b' | ... | 'f'
        'A' | 'B' | ... | 'F'

    Identifier
        IdentifierStart IdentifierContinue*

    IdentifierStart
        '$'
        '_'
        'a' | 'b' | ... | 'z'
        'A' | 'B' | ... | 'Z'

    IdentifierContinue
        IdentifierStart
        Digit
    */
    token scan_numeric_or_identifier()
    {
        int8_t sign = 0;
        bool starts_with_decimal_point = false;
        size_t start = _pos;

        if (peek() == '+')
        {
            get();
            ++start;
            sign = 1;
        }
        else if (peek() == '-')
        {
            get();
            sign = -1;
        }

        const auto c = peek();
        if (c == '0')
        {
            get();
            if (eof())
            {
                return token{integer_type{0}};
            }

            if (peek() == 'x' || peek() == 'X')
            {
                get();
                consume_hexadecial_integer();
                return std::stoll(_sourse.substr(start), 0, 16);
            }

            while (!eof())
            {
                if (is_digit(peek()))
                {
                    get();
                }
                else
                {
                    break;
                }
            }
        }
        else if (is_digit(c))
        {
            while (!eof())
            {
                if (is_digit(peek()))
                {
                    get();
                }
                else
                {
                    break;
                }
            }
        }
        else if (c == '.')
        {
            starts_with_decimal_point = true;
        }
        else if (is_identifier_start(c))
        {
            return scan_identifier(sign);
        }
        else
        {
            throw lex_error{format_char(c), "invalid character"};
        }

        bool has_decimal_point = false;
        if (peek() == '.')
        {
            get();
            bool has_any_digit = false;
            while (!eof())
            {
                if (is_digit(peek()))
                {
                    get();
                    has_any_digit = true;
                }
                else
                {
                    break;
                }
            }
            if (starts_with_decimal_point && !has_any_digit)
            {
                throw lex_error{format_char(peek()), "digit"};
            }
            has_decimal_point = true;
        }

        bool has_exponent = consume_exponent();
        if (has_decimal_point || has_exponent)
        {
            return token{
                static_cast<number_type>(std::stold(_sourse.substr(start), 0))};
        }
        else
        {
            return token{static_cast<integer_type>(
                std::stoll(_sourse.substr(start), 0))};
        }
    }



    void literal(const char* s)
    {
        // The first character of s has already been checked by the caller.
        get();
        for (const char* s_ = s + 1; *s_; ++s_)
        {
            const auto c = get();
            if (*s_ != c)
            {
                throw lex_error{format_char(c), s};
            }
        }
    }



    void consume_hexadecial_integer()
    {
        bool has_any_digit = false;
        while (!eof())
        {
            if (is_hex_digit(peek()))
            {
                get();
                has_any_digit = true;
            }
            else
            {
                break;
            }
        }
        if (!has_any_digit)
        {
            throw lex_error{format_char(peek()),
                            "hexadecimal digit (0-9, a-f or A-F)"};
        }
    }



    bool consume_exponent()
    {
        if (eof())
            return false;

        if (peek() != 'e' && peek() != 'E')
            return false;

        get();
        if (eof())
        {
            throw lex_error{"<EOF>", "digit"};
        }
        if (peek() == '+' || peek() == '-')
        {
            get();
        }
        bool has_any_digit = false;
        while (!eof())
        {
            if (is_hex_digit(peek()))
            {
                get();
                has_any_digit = true;
            }
            else
            {
                break;
            }
        }
        if (!has_any_digit)
        {
            throw lex_error{format_char(peek()), "digit"};
        }

        return true;
    }



    token scan_identifier(int8_t sign)
    {
        string_type name;
        while (!eof())
        {
            if (is_identifier_continue(peek()))
            {
                name += get();
            }
            else
            {
                break;
            }
        }

        // Check special literals.
        switch (name[0])
        {
        case 'I':
            if (name == "Infinity")
            {
                if (sign == 1)
                {
                    return token{infinity()};
                }
                else if (sign == -1)
                {
                    return token{-infinity()};
                }
                else
                {
                    return token{token_type::infinity};
                }
            }
            break;
        case 'N':
            if (name == "NaN")
            {
                if (sign)
                {
                    return token{nan()};
                }
                else
                {
                    return token{token_type::nan};
                }
            }
            break;
        case 'n':
            if (name == "null")
            {
                if (sign)
                {
                    throw lex_error{"null", "digit"};
                }
                else
                {
                    return token{token_type::null};
                }
            }
            break;
        case 't':
            if (name == "true")
            {
                if (sign)
                {
                    throw lex_error{"true", "digit"};
                }
                else
                {
                    return token{token_type::true_};
                }
            }
            break;
        case 'f':
            if (name == "false")
            {
                if (sign)
                {
                    throw lex_error{"false", "digit"};
                }
                else
                {
                    return token{token_type::false_};
                }
            }
            break;
        }
        return token{token_type::identifier, name};
    }



    char peek()
    {
        return _sourse[_pos];
    }



    char get()
    {
        const auto ret = _sourse[_pos];
        ++_pos;
        return ret;
    }



    bool eof()
    {
        return _sourse.size() <= _pos;
    }



    std::string format_char(char c)
    {
        // TODO
        return std::string(1, c);
    }
};



class token_stream
{
public:
    token_stream(const std::string& source)
        : _lexer(source)
    {
        get();
    }



    token peek()
    {
        return _lookahead;
    }



    token get()
    {
        const auto ret = _lookahead;
        _lookahead = _lexer.scan();
        return ret;
    }



private:
    lexer _lexer;
    token _lookahead;
};

} // namespace detail
} // namespace json5
