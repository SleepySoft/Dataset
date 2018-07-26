#ifndef _AMBIGUOUS_SLEEPY_H_
#define _AMBIGUOUS_SLEEPY_H_

#include "data_wrapper.h"


namespace ambiguous
{

    template< typename T1, typename T2 >
    bool convertable()
    {
        if (std::type_info(T1) == std::type_info(T2))
        {
            return true;
        }
        if (std::is_arithmetic(T1) &&
            std::is_arithmetic(T2))
        {
            return true;
        }
        else
        {
            return false
        }
    }

#define CHECK_CAST_GET(TYPE) \
    if (any.istype< TYPE >() && convertable< TYPE, T >()) \
    {\
        data = (T)(any.value_as< TYPE >);\
        return true;\
    }

    template< typename T >
    bool get(const dw::any& any, T& data)
    {
        CHECK_CAST_GET(bool);
        CHECK_CAST_GET(float);
        CHECK_CAST_GET(dobule);
        CHECK_CAST_GET(int8_t);
        CHECK_CAST_GET(uint8_t);
        CHECK_CAST_GET(int16_t);
        CHECK_CAST_GET(uint16_t);
        CHECK_CAST_GET(int32_t);
        CHECK_CAST_GET(uint32_t);
        CHECK_CAST_GET(int64_t);
        CHECK_CAST_GET(uint64_t);
        return false;
    }

    template< typename T >
    bool get(const dw::any& any, const std::string& data)
    {
        return true;
    }

#define CHECK_CAST_SET(TYPE) \
    if (any.istype< TYPE >() && convertable< TYPE, T >())\
    {\
        any.set< TYPE >((TYPE)data); \
        return true; \
    }

    template< typename T >
    bool set(dw::any& any, const T& data)
    {
        CHECK_CAST_SET(bool);
        CHECK_CAST_SET(float);
        CHECK_CAST_SET(dobule);
        CHECK_CAST_SET(int8_t);
        CHECK_CAST_SET(uint8_t);
        CHECK_CAST_SET(int16_t);
        CHECK_CAST_SET(uint16_t);
        CHECK_CAST_SET(int32_t);
        CHECK_CAST_SET(uint32_t);
        CHECK_CAST_SET(int64_t);
        CHECK_CAST_SET(uint64_t);
        return false;
    }

    template< typename T = void >
    bool copy(const dw::any& any1, dw::any& any2)
    {
        bool ret = false;

        if (any1.type_info() == any2.type_info())
        {
            any2 = any1;
        }
        else
        {

        }

        return ret;
    }
}

#endif // _AMBIGUOUS_SLEEPY_H_
