/**
 * @file ""
 *
 * C/C++ class/struct definition extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_class_def.h"

#include "extent_statement.h"
#include "utility.h"

#include "../debug_util.h"

namespace cxx {

extent::check_return extent_class_def::check_token(language_processor& processor, std::string_view line)
{
    auto& cxx_processor = to_cxx_source_processor(processor);

    if (auto& e = cxx_processor.get_current_extent();
        typeid(e) == typeid(extent_statement)) {

        auto token = match_keyword(line, { "class", "struct" });

        if (!token.empty()) {
            auto& es = static_cast<extent_statement&>(e);
            es.set_end_on_semicolon();
            cxx_processor.push_extent(extent_class_def(cxx_processor));
            return { true, token.length() };
        }
    }

    return { false, 0 };
}


extent::process_return extent_class_def::process(std::string_view line)
{
    std::size_t processed = 0;

    if (auto op = get_operator(line);
        op == "{") {
        if (_name.length() <= std::string_view("class ").length()) {
            update_name("<anonymous>");
        }

    } else if (op == ":") {
        if (_name.length() <= std::string_view("class ").length()) {
            update_name("<anonymous>");
        }

        _base_classes = true;
        processed = 1;

    } else if (op == ";") {
        complete();
        _cxx_processor.pop_namespace();

        return { 0, [&processor = _cxx_processor,
                     name = _name,
                     line_count_adjust = _line_count_adjust,
                     bi = body_info()] () {

                processor.pop_extent(true);

                auto& e = processor.get_current_extent();
                assert(typeid(e) == typeid(extent_statement));
                e.import_body_info(bi);

                if (std::get<0>(bi)) {
                    auto& es = static_cast<extent_statement&>(e);
                    es.record_class_score(name);
                    es.set_line_count_adjust(line_count_adjust);
                }
            }};

    } else if (!_base_classes && !_has_body) {
        auto n = get_name(line);

        if (!n.empty()) {
            update_name(n);
            processed = std::min(line.length(), line.find_first_of(":;{", n.length()));
            if (processed == line.length()) {
                _line_count_adjust = -1;
            }
        }

    } else if (_base_classes) {
        processed = std::min(line.length(), line.find_first_of(";{"));
        if (processed == line.length()) {
            _line_count_adjust = -1;
        }

    } else {
        processed = std::min(line.length(), line.find(';'));
    }

    return { processed, nullptr };
}


void extent_class_def::update_name(std::string_view n)
{
    auto full_name = _cxx_processor.current_namespace() + std::string(n);
    _name += full_name;
    _cxx_processor.push_namespace(full_name + "::");
}


}
