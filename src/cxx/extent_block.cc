/**
 * @file ""
 *
 * C/C++ block extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_block.h"

#include "extent_class_def.h"
#include "extent_function_def.h"
#include "extent_namespace_def.h"
#include "utility.h"

#include "../debug_util.h"


namespace cxx {

std::uint32_t extent_block::_level = 0;


extent::check_return extent_block::check_token(language_processor& processor,
                                               std::string_view line)
{
    auto& cxx_processor = to_cxx_source_processor(processor);

    if (line[0] == '{' ) {
        ++_level;
        cxx_processor.push_extent(extent_block(cxx_processor));
        return { true, 1 };
    }

    return { false, 0 };
}

extent::process_return extent_block::process(std::string_view line)
{
    if (line[0] == '}') {
        --_level;
        complete();

        // Need to add indentation level scores to lines in post processing so
        // that the indentation level scores don't get added to namespace
        // blocks.  We'll only know if we're in a namespace block after popping
        // this block extent off the processing stack.

        return { 1, [&processor = _cxx_processor,
                     start = _start,
                     end = _end] () {
                processor.pop_extent(true);

                auto& e = processor.get_current_extent();
                if ((typeid(e) != typeid(extent_namespace_def)) && (start < end)) {
                    processor.add_line_scores(processor.rules().indent_level(), start + 1, end);
                }

                e.set_body_info(processor.score_line_range(start, end + 1), start, end);
            }};
    }

    return { 0, nullptr };
}

}
