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
        , _as(null_type{})
    {
    }



    constexpr value(null_type v) noexcept
        : _type(value_type::null)
        , _as(v)
    {
    }



    constexpr value(boolean_type v) noexcept
        : _type(value_type::boolean)
        , _as(v)
    {
    }



    constexpr value(integer_type v) noexcept
        : _type(value_type::integer)
        , _as(v)
    {
    }



    constexpr value(number_type v) noexcept
        : _type(value_type::number)
        , _as(v)
    {
    }



    value(const string_type& v)
        : _type(value_type::string)
        , _as(new string_type(v))
    {
    }



    value(string_type&& v)
        : _type(value_type::string)
        , _as(new string_type(std::move(v)))
    {
    }



    value(const array_type& v)
        : _type(value_type::array)
        , _as(new array_type(v))
    {
    }



    value(array_type&& v)
        : _type(value_type::array)
        , _as(new array_type(std::move(v)))
    {
    }



    value(const object_type& v)
        : _type(value_type::object)
        , _as(new object_type(v))
    {
    }



    value(object_type&& v)
        : _type(value_type::object)
        , _as(new object_type(std::move(v)))
    {
    }



    value(const value& other)
        : _type(other._type)
        , _as(other._as)
    {
        switch (_type)
        {
        case value_type::string:
            _as.string = new string_type(*other._as.string);
            break;
        case value_type::array:
            _as.array = new array_type(*other._as.array);
            break;
        case value_type::object:
            _as.object = new object_type(*other._as.object);
            break;
        default: break;
        }
    }



    value(value&& other) noexcept
        : _type(other._type)
        , _as(other._as)
    {
        switch (_type)
        {
        case value_type::string: other._as.string = nullptr; break;
        case value_type::array: other._as.array = nullptr; break;
        case value_type::object: other._as.object = nullptr; break;
        default: break;
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
        case value_type::string: delete _as.string; break;
        case value_type::array: delete _as.array; break;
        case value_type::object: delete _as.object; break;
        }
    }



    void swap(value& other) noexcept
    {
        std::swap(_type, other._type);
        std::swap(_as, other._as);
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
        return _as.null;
    }



    template <>
    constexpr null_type& get<null_type>()
    {
        if (_type != value_type::null)
        {
            throw invalid_type_error{_type, value_type::null};
        }
        return _as.null;
    }



    template <>
    constexpr const boolean_type& get<boolean_type>() const
    {
        if (_type != value_type::boolean)
        {
            throw invalid_type_error{_type, value_type::boolean};
        }
        return _as.boolean;
    }



    template <>
    constexpr boolean_type& get<boolean_type>()
    {
        if (_type != value_type::boolean)
        {
            throw invalid_type_error{_type, value_type::boolean};
        }
        return _as.boolean;
    }



    template <>
    constexpr const integer_type& get<integer_type>() const
    {
        if (_type != value_type::integer)
        {
            throw invalid_type_error{_type, value_type::integer};
        }
        return _as.integer;
    }



    template <>
    constexpr integer_type& get<integer_type>()
    {
        if (_type != value_type::integer)
        {
            throw invalid_type_error{_type, value_type::integer};
        }
        return _as.integer;
    }



    template <>
    constexpr const number_type& get<number_type>() const
    {
        if (_type != value_type::number)
        {
            throw invalid_type_error{_type, value_type::number};
        }
        return _as.number;
    }



    template <>
    constexpr number_type& get<number_type>()
    {
        if (_type != value_type::number)
        {
            throw invalid_type_error{_type, value_type::number};
        }
        return _as.number;
    }



    template <>
    constexpr const string_type& get<string_type>() const
    {
        if (_type != value_type::string)
        {
            throw invalid_type_error{_type, value_type::string};
        }
        return *_as.string;
    }



    template <>
    constexpr string_type& get<string_type>()
    {
        if (_type != value_type::string)
        {
            throw invalid_type_error{_type, value_type::string};
        }
        return *_as.string;
    }



    template <>
    constexpr const array_type& get<array_type>() const
    {
        if (_type != value_type::array)
        {
            throw invalid_type_error{_type, value_type::array};
        }
        return *_as.array;
    }



    template <>
    constexpr array_type& get<array_type>()
    {
        if (_type != value_type::array)
        {
            throw invalid_type_error{_type, value_type::array};
        }
        return *_as.array;
    }



    template <>
    constexpr const object_type& get<object_type>() const
    {
        if (_type != value_type::object)
        {
            throw invalid_type_error{_type, value_type::object};
        }
        return *_as.object;
    }



    template <>
    constexpr object_type& get<object_type>()
    {
        if (_type != value_type::object)
        {
            throw invalid_type_error{_type, value_type::object};
        }
        return *_as.object;
    }



private:
    value_type _type;


    union _U
    {
        null_type null;
        boolean_type boolean;
        integer_type integer;
        number_type number;
        string_type* string;
        array_type* array;
        object_type* object;



        constexpr _U(null_type v)
            : null(v)
        {
        }


        constexpr _U(boolean_type v)
            : boolean(v)
        {
        }


        constexpr _U(integer_type v)
            : integer(v)
        {
        }


        constexpr _U(number_type v)
            : number(v)
        {
        }


        constexpr _U(string_type* v)
            : string(v)
        {
        }


        constexpr _U(array_type* v)
            : array(v)
        {
        }


        constexpr _U(object_type* v)
            : object(v)
        {
        }
    } _as;
};

} // namespace json5