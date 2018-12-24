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
/**
 * Utility function to determine which, if any keyword matches the first word in
 * the given line.
 *
 * @param line          The line to check.
 * @param keywords      List of key words to compare against.
 *
 * @return  The matching keyword or empty string view if no keyword matches.
 */
std::string_view match_keyword(std::string_view line,
                               std::initializer_list<std::string_view>&& keywords);

/**
 * Gets a C/C++ name (function, variable, label, etc.) from the beginning of the
 * given line.
 *
 * @param line      Line of source code.
 *
 * @return  The C/C++ name.
 */
std::string get_name(std::string_view line);

/**
 * Gets a C/C++ operator from the beginning of the given line.
 *
 * @param line      Line of source code.
 *
 * @return  The C/C++ operator.
 */
std::string_view get_operator(std::string_view line);

/**
 * Cast's a language processor reference to a C/C++ source processor reference.
 *
 * @param p     Reference to a language processor.
 *
 * @return  A C/C++ source processor reference.
 */
inline cxx_source_processor& to_cxx_source_processor(language_processor& p)
{
    assert(typeid(p) == typeid(cxx_source_processor));
    return static_cast<cxx_source_processor&>(p);
}
}

#endif
