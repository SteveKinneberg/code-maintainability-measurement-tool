/**
 * @file ""
 *
 * C/C++ element extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_element.h"

#include "extent_operator_func_def.h"
#include "extent_paren.h"
#include "extent_statement.h"
#include "extent_template_def.h"
#include "utility.h"

namespace cxx {

extent::check_return extent_element::check_token(language_processor& processor,
                                                 std::string_view line)
{
    auto& cxx_processor = to_cxx_source_processor(processor);

    auto name = get_name(line);

    if (name.empty() ) {
        return { false, 0 };
    }

    if (auto& e = cxx_processor.get_current_extent();
        typeid(e) == typeid(extent_element)) {
        cxx_processor.pop_extent();

    } else if (typeid(e) == typeid(extent_paren)) {
        auto& ep = static_cast<extent_paren&>(e);
        ep.count_param();

    } else if (typeid(e) == typeid(extent_template_def)) {
        auto& etd = static_cast<extent_template_def&>(e);
        etd.count_param();

    } else if (typeid(e) != typeid(extent_statement)) {
        return { false, 0 };
    }

    cxx_processor.push_extent(extent_element(cxx_processor, name));
    return { true, name.length() };
}

extent::process_return extent_element::process(std::string_view line)
{
    std::size_t processed = 0;
    post_process fn = nullptr;

    auto op = get_operator(line);
    DEBUG_ONLY(LOG << "op = '" << op << "'" << std::endl);

    if (op == "::") {
        _name += "::";
        processed += 2;
        _continue_name = true;

    } else if ((op == ")") ||
               (op == "}") ||
               (op == "]")) {
        complete();
        fn = [&processor = _cxx_processor] () {
                 processor.pop_extent();
             };

    } else if (!op.empty() &&
               (op != "(") &&
               (op != "{") &&
               (op != "[")) {
        complete();
        processed = 1;
        fn = [&processor = _cxx_processor] () { processor.pop_extent(); };

    } else if (_continue_name) {
        _continue_name = false;
        auto name = get_name(line);
        _name += name;
        processed += name.length();
    }

    return { processed, fn };
}

}
