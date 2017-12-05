/**
 * @headerfile ""
 *
 * Implement C/C++ processing utility functions.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <algorithm>
#include <locale>

#include "../debug_util.h"

namespace {

const auto locale = std::locale("C");

}

namespace cxx {

std::string_view match_keyword(std::string_view line,
                               std::initializer_list<std::string_view>&& keywords)
{
    auto word_end = std::find_if_not(line.begin(), line.end(), [](char c) {
            return std::isalpha(c, locale);
        });
    auto word = line.substr(0, word_end - line.begin());

    auto token_it = std::find(keywords.begin(), keywords.end(), word);

    if (token_it != keywords.end()) {
        return *token_it;
    }
    return std::string_view();
}


std::string get_name(std::string_view line)
{
    auto word_end = std::find_if_not(line.begin(), line.end(), [](char c) {
            return ((c == '_') || std::isalpha(c, locale));
        });

    return std::string(line.substr(0, word_end - line.begin()));
}

std::string_view get_operator(std::string_view line)
{
    // Three character "operators"
    static constexpr std::initializer_list<std::string_view> operators3 = {
        "<<=", ">>=",
        "->*",
        "..."
    };

    // Two character "operators"
    static constexpr std::initializer_list<std::string_view> operators2 = {
        "::",
        "++", "--",
        "+=", "-=", "*=", "/=",
        "%=", "&=", "|=", "^=",
        "<<", ">>",
        "==", "!=", "<=", ">=",
        "&&", "||",
        ".*", "->",
        "[[", "]]"
    };

    // One character "operators"
    static constexpr std::initializer_list<std::string_view> operators1 = {
        "+", "-", "*", "/", "%",
        "&", "|", "^", "~", "!",
        "<", ">",
        "?", ":",
        ".", ",", ";",
        "(", ")", "{", "}", "[", "]"
    };

    if (line.length() >= 3) {
        auto it = std::find(operators3.begin(), operators3.end(), line.substr(0, 3));
        if (it != operators3.end()) {
            return *it;
        }
    }

    if (line.length() >= 2) {
        auto it = std::find(operators2.begin(), operators2.end(), line.substr(0, 2));
        if (it != operators2.end()) {
            return *it;
        }
    }

    if (line.length() >= 1) {
        auto it = std::find(operators1.begin(), operators1.end(), line.substr(0, 1));
        if (it != operators1.end()) {
            return *it;
        }
    }

    return std::string_view();
}

}
