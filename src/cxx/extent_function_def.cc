/**
 * @file ""
 *
 * C/C++ function definition extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_function_def.h"

#include "extent_statement.h"
#include "utility.h"

#include "../debug_util.h"

namespace cxx {

extent::check_return extent_function_def::check_token(language_processor& processor[[maybe_unused]],
                                                      std::string_view line[[maybe_unused]])
{
    return { false, 0 };
}

extent::process_return extent_function_def::process(std::string_view line)
{
    const auto& rules = _cxx_processor.rules();
    post_process fn = nullptr;

    if (auto op = get_operator(line); op == "{") {
        add_score(rules.function_parameter_count(_param_count));
        DEBUG_ONLY( LOG << "param count: " << _param_count << "     score = " << _score << std::endl);

    } else if ((op == ";") || (_has_body)) {
        complete();

        fn = [&processor = _cxx_processor,
              name = _name,
              line_count_adjust = _line_count_adjust,
              bi = body_info()] () {

            processor.pop_extent(true);

            auto& e = processor.get_current_extent();
            assert(typeid(e) == typeid(extent_statement));
            e.import_body_info(bi);

            if (std::get<0>(bi)) {
                auto& es = static_cast<extent_statement&>(e);
                es.record_function_score(name);
                es.set_line_count_adjust(line_count_adjust);
            }
        };

    } if (line.empty()) {
        _line_count_adjust = -1;
    }

    return { 0, fn };
}

}
