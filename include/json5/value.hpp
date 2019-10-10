#pragma once

#include "./exceptions.hpp"
#include "./types.hpp"



namespace json5
{

struct value
{
public:
    using null_type = json5::null_type;
    using boolean_type = json5::boolean_type;
    using integer_type = json5::integer_type;
    using number_type = json5::number_type;
    using string_type = json5::string_type;
    using array_type = array_container_type<value>;
    using object_type = object_container_type<string_type, value>;



    constexpr value() noexcept
        : _type(value_type::null)
        , _null()
    {
    }



    constexpr value(null_type) noexcept
        : _type(value_type::null)
        , _null()
    {
    }



    constexpr value(bool v) noexcept
        : _type(value_type::boolean)
        , _boolean(v)
    {
    }



    constexpr value(integer_type v) noexcept
        : _type(value_type::integer)
        , _integer(v)
    {
    }



    constexpr value(number_type v) noexcept
        : _type(value_type::number)
        , _number(v)
    {
    }



    value(const string_type& v)
        : _type(value_type::string)
        , _string(new string_type(v))
    {
    }



    value(string_type&& v)
        : _type(value_type::string)
        , _string(new string_type(std::move(v)))
    {
    }



    value(const array_type& v)
        : _type(value_type::array)
        , _array(new array_type(v))
    {
    }



    value(array_type&& v)
        : _type(value_type::array)
        , _array(new array_type(std::move(v)))
    {
    }



    value(const object_type& v)
        : _type(value_type::object)
        , _object(new object_type(v))
    {
    }



    value(object_type&& v)
        : _type(value_type::object)
        , _object(new object_type(std::move(v)))
    {
    }



    value(const value& other)
        : _type(other._type)
    {
        switch (_type)
        {
        case value_type::null: _null = other._null; break;
        case value_type::boolean: _boolean = other._boolean; break;
        case value_type::integer: _integer = other._integer; break;
        case value_type::number: _number = other._number; break;
        case value_type::string:
            _string = new string_type(*other._string);
            break;
        case value_type::array: _array = new array_type(*other._array); break;
        case value_type::object:
            _object = new object_type(*other._object);
            break;
        }
    }



    value(value&& other) noexcept
        : _type(other._type)
    {
        switch (_type)
        {
        case value_type::null: _null = other._null; break;
        case value_type::boolean: _boolean = other._boolean; break;
        case value_type::integer: _integer = other._integer; break;
        case value_type::number: _number = other._number; break;
        case value_type::string:
            _string = other._string;
            other._string = nullptr;
            break;
        case value_type::array:
            _array = other._array;
            other._array = nullptr;
            break;
        case value_type::object:
            _object = other._object;
            other._object = nullptr;
            break;
        }
    }



    value& operator=(const value& other)
    {
        value tmp = other;
        tmp.swap(*this);
        return *this;
    }



    value& operator=(value&& other) noexcept
    {
        value tmp = std::move(other);
        tmp.swap(*this);
        return *this;
    }



    ~value()
    {
        switch (_type)
        {
        case value_type::null: break;
        case value_type::boolean: break;
        case value_type::integer: break;
        case value_type::number: break;
        case value_type::string: delete _string; break;
        case value_type::array: delete _array; break;
        case value_type::object: delete _object; break;
        }
    }



    void swap(value& other) noexcept
    {
        std::swap(_type, other._type);
        std::swap(_string, other._string); // TODO
    }



    constexpr value_type type() const noexcept
    {
        return _type;
    }



    template <typename T>
    constexpr const T& get() const;


    template <typename T>
    constexpr T& get();



    template <>
    constexpr const null_type& get<null_type>() const
    {
        if (_type != value_type::null)
        {
            throw invalid_type_error{_type, value_type::null};
        }
        return _null;
    }



    template <>
    constexpr null_type& get<null_type>()
    {
        if (_type != value_type::null)
        {
            throw invalid_type_error{_type, value_type::null};
        }
        return _null;
    }



    template <>
    constexpr const boolean_type& get<boolean_type>() const
    {
        if (_type != value_type::boolean)
        {
            throw invalid_type_error{_type, value_type::boolean};
        }
        return _boolean;
    }



    template <>
    constexpr boolean_type& get<boolean_type>()
    {
        if (_type != value_type::boolean)
        {
            throw invalid_type_error{_type, value_type::boolean};
        }
        return _boolean;
    }



    template <>
    constexpr const integer_type& get<integer_type>() const
    {
        if (_type != value_type::integer)
        {
            throw invalid_type_error{_type, value_type::integer};
        }
        return _integer;
    }



    template <>
    constexpr integer_type& get<integer_type>()
    {
        if (_type != value_type::integer)
        {
            throw invalid_type_error{_type, value_type::integer};
        }
        return _integer;
    }



    template <>
    constexpr const number_type& get<number_type>() const
    {
        if (_type != value_type::number)
        {
            throw invalid_type_error{_type, value_type::number};
        }
        return _number;
    }



    template <>
    constexpr number_type& get<number_type>()
    {
        if (_type != value_type::number)
        {
            throw invalid_type_error{_type, value_type::number};
        }
        return _number;
    }



    template <>
    constexpr const string_type& get<string_type>() const
    {
        if (_type != value_type::string)
        {
            throw invalid_type_error{_type, value_type::string};
        }
        return *_string;
    }



    template <>
    constexpr string_type& get<string_type>()
    {
        if (_type != value_type::string)
        {
            throw invalid_type_error{_type, value_type::string};
        }
        return *_string;
    }



    template <>
    constexpr const array_type& get<array_type>() const
    {
        if (_type != value_type::array)
        {
            throw invalid_type_error{_type, value_type::array};
        }
        return *_array;
    }



    template <>
    constexpr array_type& get<array_type>()
    {
        if (_type != value_type::array)
        {
            throw invalid_type_error{_type, value_type::array};
        }
        return *_array;
    }



    template <>
    constexpr const object_type& get<object_type>() const
    {
        if (_type != value_type::object)
        {
            throw invalid_type_error{_type, value_type::object};
        }
        return *_object;
    }



    template <>
    constexpr object_type& get<object_type>()
    {
        if (_type != value_type::object)
        {
            throw invalid_type_error{_type, value_type::object};
        }
        return *_object;
    }



private:
    value_type _type;


    union
    {
        null_type _null;
        boolean_type _boolean;
        integer_type _integer;
        number_type _number;
        string_type* _string;
        array_type* _array;
        object_type* _object;
    };
};

} // namespace json5
