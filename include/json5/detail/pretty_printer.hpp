#pragma once

#include <cmath>
#include "../stringify_options.hpp"
#include "../value.hpp"



namespace json5
{
namespace detail
{

class pretty_printer
{
public:
    pretty_printer(const stringify_options& opts)
        : _opts(opts)
    {
    }



    std::string stringify(const value& v)
    {
        switch (v.type())
        {
        case value_type::null: return "null";
        case value_type::boolean:
            return v.get<value::boolean_type>() ? "true" : "false";
        case value_type::integer:
            return std::to_string(v.get<value::integer_type>());
        case value_type::number:
        {
            const auto p = v.get<value::number_type>();
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
        case value_type::string: return quote(v.get<value::string_type>());
        case value_type::array: return array(v);
        case value_type::object: return object(v);
        }
    }



private:
    stringify_options _opts;



    std::string array(const value& v)
    {
        std::string items;
        const auto& array = v.get<value::array_type>();
        size_t i = 0;
        const auto size = array.size();
        for (const auto& v : array)
        {
            items += stringify(v);
            ++i;
            if (i != size)
            {
                items += ",";
            }
        }
        return "[" + items + "]";
    }



    std::string object(const value& v)
    {
        std::string items;
        const auto& object = v.get<value::object_type>();
        size_t i = 0;
        const size_t size = object.size();
        for (const auto& kvp : object)
        {
            items += quote(kvp.first);
            items += ":";
            items += stringify(kvp.second);
            ++i;
            if (i != size)
            {
                items += ",";
            }
        }
        return "{" + items + "}";
    }



    std::string quote(const std::string& s)
    {
        std::string ret;
        ret.reserve(s.size() + 2);
        ret += '\"';
        for (const auto& c : s)
        {
            if (c == '"' || c == '\\')
            {
                ret += '\\';
            }
            ret += c;
        }
        ret += '\"';
        return ret;
    }
};

} // namespace detail
} // namespace json5
