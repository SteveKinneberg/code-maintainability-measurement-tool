/**
 * @file ""
 *
 * C/C++ source file processor class method implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "cxx_source_processor.h"

// #include <algorithm>
// #include <cassert>
// #include <locale>
// #include <typeinfo>
// #include <typeindex>
// #include <unordered_map>

#include "../extent.h"
#include "../extent_string.h"
#include "../processing_exception.h"

#include "extent_block.h"
#include "extent_class_def.h"
#include "extent_comment.h"
#include "extent_element.h"
#include "extent_function_def.h"
#include "extent_namespace_def.h"
#include "extent_operator_func_def.h"
#include "extent_paren.h"
#include "extent_statement.h"
#include "extent_template_def.h"
#include "extent_using_statement.h"

#include "../debug_util.h"

namespace {

/// The list of file name extensions for C/C++ source file.
constexpr auto exts = {
    ".c", ".c++", ".cc", ".cpp", ".cxx", ".C",
    ".h", ".h++", ".hh", ".hpp", ".hxx", ".H"
};

/**
 * Helper function to skip whitespace.
 *
 * @param[in,out] s     Reference to a string view to trim the leading whitespace from.
 */
void skip_ws(std::string_view& s) {
    s.remove_prefix(std::min(s.length(), s.find_first_not_of(" \t")));
}


// std::size_t find_op_end(std::string_view s)
// {
//     return std::find_if_not(s.begin(), s.end(), [](char c) {
//             return (( c == '%') ||
//                     ( c == '&') ||
//                     ( c == '*') ||
//                     ( c == '+') ||
//                     ( c == '-') ||
//                     ( c == '/') ||
//                     ( c == '<') ||
//                     ( c == '=') ||
//                     ( c == '>') ||
//                     ( c == '^') ||
//                     ( c == '|'));
//         }) - s.begin();
// }

// auto check_op_token(std::string_view s, std::string_view t)
// {
//     return ((s.compare(0, t.length(), t) == 0) &&
//             (s.length() == t.length() || find_op_end(s) == t.length()));
// }


}


namespace cxx {

cxx_source_processor::~cxx_source_processor()
{
    _namespace.pop(); // global namespace
}



std::int32_t cxx_source_processor::file_check(std::string_view filename,
                                              std::string_view source_data[[maybe_unused]])
{
    std::int32_t confidence = 0;

    confidence += extension_match(filename, exts);

    return confidence;
}


std::size_t cxx_source_processor::process_line(std::string_view line)
{
    static constexpr auto check_token_list = {
        &extent_string::check_token,    // This must be one of the first 2 in the list!
        &extent_comment::check_token,   // This must be one of the first 2 in the list!

        &extent_block::check_token,
        &extent_class_def::check_token,
        &extent_function_def::check_token,
        &extent_operator_func_def::check_token,
        &extent_namespace_def::check_token,
        &extent_paren::check_token,
        &extent_statement::check_token,
        &extent_template_def::check_token,
        &extent_using_statement::check_token,
        // &extent_extern_def::check_token,
        // &extent_template_def::check_token,

        &extent_element::check_token    // This must be last in the list!!!
    };

    _line_data = line;

    // FIXME: Need to rework WS analyzer for indent expectations
    // line_score += cxx_analyze_ws(line);

    bool done = false;
    while (!done) {
        skip_ws(line);

        auto& e = get_current_extent();
        assert(e.name().size() < 100);

        DEBUG_ONLY(LOG << "Processing: \"" << line << "\" in context " << e << std::endl);

        auto [processed, post_process] = e.process(line);
        line.remove_prefix(processed);
        DEBUG_ONLY(LOG << "processed: " << processed << "    line: \"" << line << "\"" << std::endl);
        if (post_process) {
            post_process();
        }

        bool match = false;
        if (!line.empty() && (processed == 0)) {
            for (const auto& check_token: check_token_list) {
                std::tie(match, processed) = check_token(*this, line);
                line.remove_prefix(processed);
                if (match) { break; }
            }
        }

        done = !post_process && (processed == 0) && !match;
    }
    return line.length();
}



#if 0




void cxx_source_processor::cxx_analyze_angle_open(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    static constexpr auto less = std::string_view("<");
    static constexpr auto less_equal = std::string_view("<=");
    static constexpr auto shift = std::string_view("<<");
    static constexpr auto shift_equal = std::string_view("<<=");

    auto is_less = check_op_token(line, less);
    auto is_less_equal = check_op_token(line, less_equal);
    auto is_shift = check_op_token(line, shift);
    auto is_shift_equal = check_op_token(line, shift_equal);

    if (is_less) {
        if (auto& e = get_current_extent(); e.type() == ex_type::template_def) {
            push_extent(line, "template<", ex_type::template_param);
            ++_block_level;
        } else {
            push_extent(line, "<", ex_type::angle_open);
            // ++_block_level;
        }
        line.remove_prefix(less.length());

    } else if(is_less_equal) {
        line.remove_prefix(less_equal.length());

    } else if(is_shift) {
        line.remove_prefix(shift.length());

    } else if(is_shift_equal) {
        line.remove_prefix(shift_equal.length());
    }
}

void cxx_source_processor::cxx_analyze_angle_close(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    static constexpr auto greater = std::string_view(">");
    static constexpr auto greater_equal = std::string_view(">=");
    static constexpr auto shift = std::string_view(">>");
    static constexpr auto shift_equal = std::string_view(">>=");

    auto is_greater = check_op_token(line, greater);
    auto is_greater_equal = check_op_token(line, greater_equal);
    auto is_shift = check_op_token(line, shift);
    auto is_shift_equal = check_op_token(line, shift_equal);

    if (is_greater) {
        if (auto& e = get_current_extent(); e.type() == ex_type::class_def) {
            pop_extent(true);
            if (get_current_extent().type() != ex_type::template_param) {
                throw processing_exception("unexpected \">\"", _filename, get_line_number() + 1,
                                           _line_data, _line_data.length() - line.length());
            }

        } else if (e.type() == ex_type::element) {
            pop_extent(true);
        }

        if (auto& e = get_current_extent(); e.type() == ex_type::template_param) {
            e.add_param();
            auto param_count = e.param_count();
            pop_extent(true);
            auto& p = get_current_extent();
            assert(p.type() == ex_type::template_def);
            p.add_score(_scoring_rules.template_parameter_count(param_count));
            add_line_scores(p.score(), p.line_num(), p.line_num() + 1);
            pop_extent();
            --_block_level;

        } else if (e.type() == ex_type::angle_open) {
            pop_extent(true);
        }
        line.remove_prefix(greater.length());

    } else if (is_greater_equal) {
        line.remove_prefix(greater_equal.length());

    } else if(is_shift) {
        line.remove_prefix(shift.length());

    } else if(is_shift_equal) {
        line.remove_prefix(shift_equal.length());
    }
}


void cxx_source_processor::cxx_analyze_curly_open(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == '{') {
        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        if (auto& e = get_current_extent(); e.type() == ex_type::namespace_def) {
            _namespace.emplace(e.name());

        } else {
            ++_block_level;
            if (e.type() == ex_type::class_def) {
                _namespace.emplace(e.name());

            } else if ((e.type() == ex_type::closure_def) ||
                       (e.type() == ex_type::function_def)) {
                // e.add_score(_scoring_rules.statement_line_count(line_cnt));
                e.add_score(_scoring_rules.function_parameter_count(e.param_count()));
            }
        }
        push_extent(line,
                        std::string("block{} ") + std::to_string(_block_level),
                        ex_type::curly_open);
        line.remove_prefix(1);
    }
}

void cxx_source_processor::cxx_analyze_curly_close(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == '}') {
        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        if (get_current_extent().type() == ex_type::angle_open) {
            pop_extent(true);
        }

        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        if (get_current_extent().type() != ex_type::curly_open) {
            throw processing_exception("curly brace mismatch", _filename, get_line_number() + 1,
                                       _line_data, _line_data.length() - line.length());
        }

        auto block_start_line = get_current_extent().line_num();

        pop_extent(true);
        if (auto &e = get_current_extent(); e.type() == ex_type::namespace_def) {
            _namespace.pop();
            pop_extent(true);

        } else {
            add_line_scores(1, block_start_line + 1, get_line_number());
            auto score = score_line_range(e.line_num(), get_line_number() + 1);

            if (e.type() == ex_type::class_def) {
                add_class_score(e.name(), e.line_num(), get_line_number(), e.score() + score);
                pop_extent(true);
                _namespace.pop();
            } else if (e.type() == ex_type::function_def) {
                add_function_score(e.name(), e.line_num(), get_line_number(), e.score() + score);
                pop_extent();
            } else if (e.type() == ex_type::flow_expr) {
                pop_extent(true);
            }
            --_block_level;
        }
        line.remove_prefix(1);
    }
}




void cxx_source_processor::cxx_analyze_paren_open(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == '(') {
        if (auto& e = get_current_extent();
            (e.type() == ex_type::element) ||
            (e.type() == ex_type::function_def)) {
            auto name = e.name() + "()";
            auto data = e.data();

            if (!_namespace.empty()) {
                name = _namespace.top() + "::" + name;
            }

            pop_extent(true);
            push_extent(data, name, ex_type::function_def);
            push_extent(line, "function(", ex_type::function_param);

        } else if (e.type() == ex_type::operator_overload) {
            auto data = e.data();
            auto line_num = e.line_num();
            pop_extent(false);
            push_extent(data, "operator()", line_num, ex_type::operator_overload);
        } else {
            push_extent(line, "'('", ex_type::paren_open);
        }

        ++_block_level;
        line.remove_prefix(1);
    }
}


void cxx_source_processor::cxx_analyze_paren_close(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == ')') {
        bool add_param = false;
        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);

            if (get_current_extent().type() == ex_type::angle_open) {
                pop_extent(true);
                if (get_current_extent().type() == ex_type::element) {
                    pop_extent(true);
                }
            } else {
                add_param = true;
            }
        }

        if (auto& e = get_current_extent();
            e.type() == ex_type::function_param) {

            if(add_param) {
                e.add_param();
            }

            add_line_scores(e.score(), e.line_num() + 1, get_line_number());


            auto param_count = e.param_count();
            pop_extent(true);

            auto& p = get_current_extent();

            assert((p.type() == ex_type::function_def) ||
                   (p.type() == ex_type::closure_def));

            // Don't score the parameter count just yet.  This maybe a function
            // call or prototype.
            p.set_param_count(param_count);

            // FIXME: If function call or prototype and semicolon is on a
            // separate line, this does not properly score the statement line
            // count!
            p.add_score(_scoring_rules.statement_line_count(get_line_number() - p.line_num()));

        } else if (e.type() == ex_type::operator_overload) {
            auto data = e.data();
            auto line_num = e.line_num();
            pop_extent(false);
            push_extent(data, "operator()", line_num, ex_type::function_def);

        } else if (e.type() == ex_type::paren_open) {
            pop_extent(true);

        } else {
            throw processing_exception("parenthesis mismatch", _filename, get_line_number() + 1,
                                       _line_data, _line_data.length() - line.length());
        }
        --_block_level;
    }
}



void cxx_source_processor::cxx_analyze_square_open(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == '[') {
        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        push_extent(line, "[", ex_type::square_open);
        ++_block_level;
        line.remove_prefix(1);
    }
}


void cxx_source_processor::cxx_analyze_square_close(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == ']') {
        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        while (get_current_extent().type() == ex_type::angle_open) {
            pop_extent(true);
        }

        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        if (get_current_extent().type() != ex_type::square_open) {
            throw processing_exception("square bracket mismatch", _filename, get_line_number() + 1,
                                       _line_data, _line_data.length() - line.length());
        }
        pop_extent(true);
        --_block_level;
    }
}



void cxx_source_processor::cxx_analyze_colon(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == ':') {
        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        auto& e = get_current_extent();
        if (e.type() == ex_type::namespace_def) {
            if ((line.length() > 1) && (line[1] == ':')) {
                auto name = e.name() + "::";
                pop_extent(true);
                push_extent(line, name, ex_type::namespace_def);

                line.remove_prefix(2);
            } else {
                throw processing_exception("namespace scoping", _filename, get_line_number() + 1,
                                           _line_data, _line_data.length() - line.length());
            }
        }
    }
}


void cxx_source_processor::cxx_analyze_semicolon(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == ';') {

        auto e = &get_current_extent();
        while ((e->type() != ex_type::file) &&
               (e->type() != ex_type::curly_open) &&
               (e->type() != ex_type::paren_open)) {

            if ((e->type() == ex_type::function_def) ||
                (e->type() == ex_type::class_def)) {
                auto line_cnt = get_line_number() - e->line_num();
                pop_extent(false);
                e = &get_current_extent();
                e->add_score(_scoring_rules.statement_line_count(line_cnt));
            } else {
                pop_extent(true);
                e = &get_current_extent();
            }
        }
    }
}


void cxx_source_processor::cxx_analyze_comma(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == ',') {
        if (auto& e = get_current_extent();
            (e.type() == ex_type::element) ||
            (e.type() == ex_type::statement) ||
            (e.type() == ex_type::class_def)){
            pop_extent(true);
        }

        if (auto& e = get_current_extent();
            (e.type() == ex_type::closure_def) ||
            (e.type() == ex_type::function_param) ||
            (e.type() == ex_type::template_param)) {
            e.add_param();
        }
    }
}


void cxx_source_processor::cxx_analyze_namespace(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    static constexpr auto namespace_token = std::string_view("namespace");

    if (check_keyword_token(line, namespace_token)) {
        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        std::string name;

        if (!_namespace.empty()) { name = _namespace.top() + "::"; }

        push_extent(line, name, ex_type::namespace_def);
        line.remove_prefix(namespace_token.length());

    } else if (get_current_extent().type() == ex_type::namespace_def) {
        // Remove the old "namespace_def" context to replace it with the
        // one that has the real namespace name.
        auto name = get_current_extent().name();
        auto score = get_current_extent().score();
        pop_extent(false);

        auto name_len = find_name_end(line);

        name += std::string(line.substr(0, name_len));

        push_extent(line, name, ex_type::namespace_def);
        get_current_extent().add_score(score);
        line.remove_prefix(name_len);
    }
}


void cxx_source_processor::cxx_analyze_class(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    static constexpr auto class_token = std::string_view("class");
    static constexpr auto struct_token = std::string_view("struct");

    auto is_class = check_keyword_token(line, class_token);
    auto is_struct = check_keyword_token(line, struct_token);

    if ((is_class || is_struct)) {
        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        std::string name;

        if (!_namespace.empty()) { name = _namespace.top() + "::"; }

        push_extent(line, name, ex_type::class_def);
        line.remove_prefix(is_class ? class_token.length() : struct_token.length());

    } else if (get_current_extent().type() == ex_type::class_def) {
        // Remove the old "class_def" context to replace it with the
        // one that has the real class name (with namespace prefix).
        auto name = get_current_extent().name();
        auto score = get_current_extent().score();
        pop_extent(false);

        auto name_len = find_name_end(line);

        name += std::string(line.substr(0, name_len));

        push_extent(line, name, ex_type::class_def);
        get_current_extent().add_score(score);
        line.remove_prefix(name_len);
    }
}


void cxx_source_processor::cxx_analyze_template(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    static constexpr auto template_token = std::string_view("template");

    if (check_keyword_token(line, template_token) ) {
        if (get_current_extent().type() == ex_type::element) {
            pop_extent(true);
        }

        push_extent(line, "template definition", ex_type::template_def);
        line.remove_prefix(template_token.length());
    }
}


void cxx_source_processor::cxx_analyze_flow_control(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    static constexpr std::initializer_list<std::string_view> flow_tokens = {
        "if", "while", "do", "for", "else", "try", "catch", "switch"
    };

    auto token = match_token(line, flow_tokens);

    if (!token.empty()) {
        push_extent(line, std::string(token), ex_type::flow_expr);
        line.remove_prefix(token.length());
    }
}

void cxx_source_processor::cxx_analyze_pre_processor(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    if (line[0] == '#') {
        line.remove_prefix(1);
        skip_ws(line);    // Some code puts spaces between the "#" and directive word.
        auto word_len = find_name_end(line);
        auto name = std::string(line.substr(0, word_len));
        push_extent(line, name, ex_type::pre_processor);
        line.remove_prefix(word_len);
    }
}


void cxx_source_processor::cxx_analyze_operator_overload(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    static constexpr std::initializer_list<std::string_view> operator_token = {
        "operator"
    };

    auto token = match_token(line, operator_token);

    if (!token.empty()) {
        push_extent(line, "operator", ex_type::operator_overload);
        line.remove_prefix(token.length());
    }
}


void cxx_source_processor::cxx_analyze_noise_tokens(std::string_view& line)
{
    if (_in_string || _in_comment) { return; }

    static constexpr std::initializer_list<std::string_view> noise_tokens = {
        "constexpr"
    };

    auto token = match_token(line, noise_tokens);

    if (!token.empty()) {
        line.remove_prefix(token.length());
    }
}
#endif

}
