#ifndef _debug_util_h
#define _debug_util_h

#include <cxxabi.h>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <typeinfo>

namespace std {

inline auto to_string(const std::type_info& info) noexcept
{
    std::string info_str;

    char *realname = abi::__cxa_demangle(info.name(), nullptr, nullptr, nullptr);
    if (realname) {
        info_str = realname;
        free(realname);
    }

    return info_str;
}
}

inline auto& operator<<(std::ostream& os, const std::type_info& typeInfo) noexcept
{
    return os << std::to_string(typeInfo);
}


inline auto _filter_file(std::string_view file)
{
    if (file.length() > 30) {
        file.remove_prefix(file.length() - 30);
    }
    return file;
}

inline auto _filter_func(std::string_view func)
{
    if (func.length() > 15) {
        func.remove_prefix(func.length() - 15);
    }
    return func;
}

#ifdef NDEBUG
#define DEBUG_ONLY(_x)
#else
#define DEBUG_ONLY(_x) _x
#endif

#define LOG std::cout << std::setw(15) << _filter_func(__func__) << "[" << std::setw(30) << _filter_file(__FILE__)  << ":" << std::left << std::setw(3) << __LINE__ << std::right << "]: "

#ifndef SJK
#define SJK std::cout << "SJK - " << std::setw(15) << _filter_func(__func__) << "[" << std::setw(30) << _filter_file(__FILE__)  << ":" << std::left << std::setw(3) << __LINE__ << std::right << "]: "
#endif


#endif
