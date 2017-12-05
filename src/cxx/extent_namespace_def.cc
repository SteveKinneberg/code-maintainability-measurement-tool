/**
 * @file ""
 *
 * C/C++ namespace definition extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_namespace_def.h"

#include "extent_statement.h"
#include "utility.h"

namespace cxx {

extent::check_return extent_namespace_def::check_token(language_processor& processor, std::string_view line)
{
    auto& cxx_processor = to_cxx_source_processor(processor);

    if (auto& e = cxx_processor.get_current_extent();
        typeid(e) == typeid(extent_statement)) {

        auto token = match_keyword(line, { "namespace" });

        if (!token.empty()) {
            auto& es = static_cast<extent_statement&>(e);
            es.set_end_on_block_end();
            cxx_processor.push_extent(extent_namespace_def(cxx_processor));
            return { true, token.length() };
        }
    }

    return { false, 0 };
}

extent::process_return extent_namespace_def::process(std::string_view line)
{
    std::size_t processed = 0;

    if (line[0] == '{') {
        if (_name.empty()) {
            _name = "<anonymous>";
        }

        auto full_name = _cxx_processor.current_namespace() + _name + "::";
        _cxx_processor.push_namespace(std::move(full_name));

    } else if ((line.length() >= 2) && (line[0] == ':') && (line[1] == ':')) {
        _name += "::";
        processed = 2;

    } else if (_has_body) {
        complete();
        _cxx_processor.pop_namespace();

        return { 0, [&processor = _cxx_processor,
                     bi = body_info()]() {
                processor.pop_extent(true);

                auto& e = processor.get_current_extent();
                assert(typeid(e) == typeid(extent_statement));
                e.import_body_info(bi);
            }};

    } else {
        auto n = get_name(line);
        _name += n;
        processed = n.length();
    }

    return { processed, nullptr };
}

}
