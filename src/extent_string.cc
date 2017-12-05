/**
 * @file ""
 *
 * String extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_string.h"


extent::check_return extent_string::check_token(language_processor& processor, std::string_view line)
{
    std::size_t processed = 0;

    if (line[0] == '\'') {
        processor.push_extent(extent_string(processor, style::single_quote));
        processed = 1;
    } else if (line[0] == '"') {
        processor.push_extent(extent_string(processor, style::double_quote));
        processed = 1;
    }

    return { processed > 0, processed };
}


extent::process_return extent_string::process(std::string_view line)
{
    auto sc = (_style == style::single_quote) ? '\'' : '"';

    auto end = line.find(sc);
    auto found = end != line.npos;
    end = std::min(line.length(), end);

    if (end > 0) {
        while ((end < line.length()) && (line[end - 1] == '\\')) {
            end = line.find(sc, end + 1);
            found = end != line.npos;
            end = std::min(line.length(), end);
        }

    }

    if (found) {
        complete();
        return { end, [&processor = _processor]() { processor.pop_extent(true); }};
    }

    return { end, nullptr };
}

