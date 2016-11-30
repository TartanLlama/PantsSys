#pragma once

namespace pants {
    namespace utils {
struct EnumHash
{
    template <typename T>
    inline
    typename std::enable_if<std::is_enum<T>::value, std::size_t>::type
    operator ()(T const value) const
    {
        return static_cast<std::size_t>(value);
    }
};
    }
}
