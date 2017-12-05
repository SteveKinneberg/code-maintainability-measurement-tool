/**
 * @file ""
 *
 * C/C++ function definition extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_operator_func_def.h"

#include "extent_element.h"
#include "extent_statement.h"
#include "utility.h"

namespace cxx {

extent::check_return extent_operator_func_def::check_token(language_processor& processor, std::string_view line)
{
    auto& cxx_processor = to_cxx_source_processor(processor);

    auto token = match_keyword(line, { "operator" });

    if (!token.empty()) {
        while (typeid(cxx_processor.get_current_extent()) == typeid(extent_element)) {
            cxx_processor.pop_extent();
        }

        if (auto& e = cxx_processor.get_current_extent();
            typeid(e) == typeid(extent_statement)) {

            auto& es = static_cast<extent_statement&>(cxx_processor.get_current_extent());
            es.set_end_on_block_end();
            es.set_end_on_semicolon();
            cxx_processor.push_extent(extent_operator_func_def(cxx_processor));
            return { true, token.length() };
        }
    }

    return { false, 0 };
}


extent::process_return extent_operator_func_def::process(std::string_view line)
{
    std::size_t processed = 0;

    if (!_have_operator) {
        auto op = get_operator(line);

        if (!_is_func_operator && (op == "(")) {
            _name += " ()";
            processed = 1;
            _is_func_operator = true;

        } else if (_is_func_operator && (op == ")")) {
            processed = 1;
            _have_operator = true;

        } else if (op == "[") {
            _name += " []";
            processed = 1;

        } else if (op == "]") {
            processed = 1;
            _have_operator = true;

        } else {
            auto op_end = line.find_first_of(" \t(");
            auto op_name = line.substr(0, std::min(op_end, line.length()));
            _name += " " + std::string(op_name);
            processed = op_name.length();
            _have_operator = true;
        }

    } else {
        return extent_function_def::process(line);
    }

    return { processed, nullptr };
}

}
