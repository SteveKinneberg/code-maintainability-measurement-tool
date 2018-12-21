#ifndef _cxx_utility_h
#define _cxx_utility_h
/**
 * @file
 *
 * Define C/C++ processing utility functions.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <cassert>
#include <string>
#include <string_view>

#include "cxx_source_processor.h"

namespace cxx {
std::string_view match_keyword(std::string_view line,
                               std::initializer_list<std::string_view>&& keywords);

std::string get_name(std::string_view line);

std::string_view get_operator(std::string_view line);

inline cxx_source_processor& to_cxx_source_processor(language_processor& p)
{
    assert(typeid(p) == typeid(cxx_source_processor));
    return static_cast<cxx_source_processor&>(p);
}
}


#endif
