/**
 * @file ""
 *
 * C/C++ class/struct definition extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_template_def.h"

#include "extent_statement.h"
#include "utility.h"

#include "../debug_util.h"

namespace cxx {

extent::check_return extent_template_def::check_token(language_processor& processor,
                                                      std::string_view line)
{
    auto& cxx_processor = to_cxx_source_processor(processor);

    if (auto& e = cxx_processor.get_current_extent();
        (typeid(e) == typeid(extent_statement)) ||
        (typeid(e) == typeid(extent_template_def))) {

        auto token = match_keyword(line, { "template" });

        if (!token.empty()) {
            // Don't yet know if the statement will end with after a block or a semicolon
            cxx_processor.push_extent(extent_template_def(cxx_processor));
            return { true, token.length() };
        }
    }

    return { false , 0 };
}


extent::process_return extent_template_def::process(std::string_view line)
{
    const auto& rules = _cxx_processor.rules();

    if (auto op = get_operator(line);
        !_have_open_angle_bracket && (op == "<")) {
        DEBUG_ONLY(LOG << "op = '" << op << "'" << std::endl);
        _have_open_angle_bracket = true;
        return { 1, nullptr };

    } else if (_have_open_angle_bracket && (op == ">")) {
        DEBUG_ONLY(LOG << "op = '" << op << "'" << std::endl);
        complete();

        add_score(rules.template_parameter_count(_param_count));
        SJK << "param count: " << _param_count << "     score = " << _score << std::endl;

        return { 1, [&processor = _cxx_processor] () {
                processor.pop_extent(true);
            }};
    }

    return { 0, nullptr };
}

}
