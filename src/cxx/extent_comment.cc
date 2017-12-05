/**
 * @file ""
 *
 * C/C++ comment extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_comment.h"

#include "utility.h"

#include "../debug_util.h"


namespace cxx {

extent::check_return extent_comment::check_token(language_processor& processor,
                                                 std::string_view line)
{
    auto& cxx_processor = to_cxx_source_processor(processor);

    if ((line.length() >= 2) && (line[0] == '/')) {
        if (line[1] == '/') {
            return { true, line.length() };
        } else if (line[1] == '*') {
            cxx_processor.push_extent(extent_comment(cxx_processor));
            return { true, 2 };
        }
    }

    return { false, 0 };
}

extent::process_return extent_comment::process(std::string_view line)
{
    std::size_t processed = 0;

    auto end = line.find("*/");

    if (end == line.npos) {
        processed = line.length();
    } else {
        complete();
        processed = end + 2;
        return { processed, [&processor=_processor]() { processor.pop_extent(true); }};
    }

    return { processed, nullptr };
}

}
