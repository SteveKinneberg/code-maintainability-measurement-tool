/**
 * @file ""
 *
 * C/C++ comment extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_using_statement.h"

#include "extent_statement.h"
#include "utility.h"

namespace cxx {

extent::check_return extent_using_statement::check_token(language_processor& processor, std::string_view line)
{
    auto& cxx_processor = to_cxx_source_processor(processor);

    if (auto& e = cxx_processor.get_current_extent();
        typeid(e) == typeid(extent_statement)) {

        auto token = match_keyword(line, { "using" });

        if (!token.empty()) {
            auto& es = static_cast<extent_statement&>(e);
            es.set_end_on_semicolon();
            cxx_processor.push_extent(extent_using_statement(cxx_processor));
            return { true, token.length() };
        }
    }
    return { false, 0 };
}

extent::process_return extent_using_statement::process(std::string_view line)
{
    std::size_t pos = std::min(line.length(), line.find_first_of(";/"));


    if ((pos < line.length()) && (line[pos] == ';')) {
        complete();
        return { pos, [&processor = _cxx_processor]() { processor.pop_extent(true); }};
    }

    return { pos, nullptr };
    }

}
