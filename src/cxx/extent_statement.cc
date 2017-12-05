/**
 * @file ""
 *
 * C/C++ statement extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_statement.h"

#include "../extent_file.h"
#include "extent_block.h"
#include "extent_template_def.h"
#include "utility.h"

namespace cxx {

extent::check_return extent_statement::check_token(language_processor& processor,
                                                   std::string_view line)
{
    auto match = false;
    auto& cxx_processor = to_cxx_source_processor(processor);
    auto op = get_operator(line);

    if (auto& e = cxx_processor.get_current_extent();
        ((typeid(e) == typeid(extent_file)) ||
         ((typeid(e) == typeid(extent_block)) && (op != "}"))
         /* || ((typeid(e) == typeid(extent_template_def)) && (op != ">"))*/
         )) {
        cxx_processor.push_extent(extent_statement(cxx_processor));
        match = true;
    }

    return { match, 0 };
}

extent::process_return extent_statement::process(std::string_view line)
{
    bool statement_done = false;
    std::size_t processed = 0;
    post_process fn = nullptr;

    if (_end_on_block_end) {
        if (_has_body) {
            statement_done = true;
        }
    }

    if (_end_on_semicolon) {
        auto op = get_operator(line);
        if (op == ";") {
            processed = 1;
            statement_done = true;
        }
    }

    if (statement_done) {
        complete();
        auto line_count_score = _cxx_processor.rules().statement_line_count(line_count());
        add_score(line_count_score);

        if (_record_class_score) {
            _cxx_processor.add_class_score(_inner_name, _start, _end, _score);
        } else if (_record_function_score) {
            _cxx_processor.add_function_score(_inner_name, _start, _end, _score);
        }

        fn = [&processor = _cxx_processor] () { processor.pop_extent(); };
    }

    return { processed, fn };
}

}
