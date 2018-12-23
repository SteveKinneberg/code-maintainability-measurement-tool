/**
 * @file ""
 *
 * C/C++ parenthetical extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_paren.h"

#include <cassert>


// #include "extent_closure_def.h"
#include "extent_element.h"
#include "extent_function_def.h"
#include "extent_operator_func_def.h"
#include "extent_statement.h"
#include "extent_template_def.h"
#include "utility.h"

#include "../debug_util.h"

namespace cxx {

std::uint32_t extent_paren::_level = 0;

extent::check_return extent_paren::check_token(language_processor& processor,
                                               std::string_view line)
{
    auto& cxx_processor = to_cxx_source_processor(processor);

    auto op = get_operator(line);

    if (op == "(" ) {
        ++_level;

        if (auto& e = cxx_processor.get_current_extent();
            typeid(e) == typeid(extent_element)) {

            auto name = cxx_processor.current_namespace() + e.name();

            cxx_processor.pop_extent();

            if (auto& p = cxx_processor.get_current_extent();
                typeid(p) == typeid(extent_statement)) {

                auto& es = static_cast<extent_statement&>(p);
                es.set_end_on_block_end();
                es.set_end_on_semicolon();

                cxx_processor.push_extent(extent_function_def(cxx_processor, std::move(name)));
            }

        } else if (typeid(e) == typeid(extent_paren))  {
            auto& ep = static_cast<extent_paren&>(e);
            ep.count_param();

        } else if (typeid(e) == typeid(extent_template_def))  {
            auto& ep = static_cast<extent_paren&>(e);
            ep.count_param();
        }

        cxx_processor.push_extent(extent_paren(cxx_processor));
        return { true, 1 };
    }

    return { false, 0 };
}

extent::process_return extent_paren::process(std::string_view line)
{
    auto op = get_operator(line);
    DEBUG_ONLY(LOG << "op = '" << op << "'" << std::endl);

    if (op == ")") {
        --_level;
        complete();

        if (_start < _end) {
            _cxx_processor.add_line_scores(_cxx_processor.rules().indent_level(), _start + 1, _end);
        }

        return { 1, [&processor = _cxx_processor,
                     param_count = _param_count,
                     start = _start, end = _end] () {
                        processor.pop_extent();

                        if (auto& e = processor.get_current_extent();
                            typeid(e) == typeid(extent_function_def) ||
                            typeid(e) == typeid(extent_operator_func_def)) {

                            auto& efd = static_cast<extent_function_def&>(e);
                            efd.set_param_count(param_count);
                            efd.add_score(processor.rules().statement_line_count(end - start + 1));
                        }
                    }};
    }

    // if (struct { int a; } x = { 1 }; x.a == 2) {}

    return { 0, nullptr };
}

}
