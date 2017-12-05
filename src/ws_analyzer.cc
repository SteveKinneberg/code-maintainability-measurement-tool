/**
 * @file ""
 *
 * Whitespace analyzer class method implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "ws_analyzer.h"

#include <iostream>
#define SJK std::cout << "SJK - " << __FUNCTION__ << ": " << __LINE__ << ": "



static auto trailing_whitespace_count(const std::string_view& s)
{
    return s.length() - std::min(s.length(), s.find_last_not_of(" \t") + 1);
}


static auto check_indent_inconsistency(std::string_view ip,
                                       std::string_view prev_ip,
                                       ws_analyzer::indent_change ic)
{
    auto common_len = std::min(ip.length(), prev_ip.length());
    auto is_same_start = ip.compare(0, common_len, prev_ip, 0, common_len);
    auto is_same_level = ip.length() == prev_ip.length();
    auto is_increased_level = ip.length() > prev_ip.length();
    auto is_decreased_level = ip.length() < prev_ip.length();

    bool penalty = false;

    switch (ic) {
    case ws_analyzer::indent_change::expect_none:
        penalty = is_same_level && is_same_start;
        break;

    case ws_analyzer::indent_change::expect_increase:
        penalty = is_increased_level && is_same_start;
        break;

    case ws_analyzer::indent_change::expect_decrease:
        penalty = is_decreased_level && is_same_start;
        break;

    case ws_analyzer::indent_change::allow_increase:
        penalty = !is_decreased_level && is_same_start;
        break;

    case ws_analyzer::indent_change::allow_decrease:
        penalty = !is_increased_level && is_same_start;
        break;

    case ws_analyzer::indent_change::allow_any:
        break;
    }

    return penalty;
}

score_t ws_analyzer::operator()(std::string_view& line,
                                indent_change ic,
                                bool ignore_indent)
{
    auto indent_pattern = line.substr(0, line.find_first_not_of(" \t"));
    score_t score = 0;

    score += _scoring_rules.line_length(line.length());
    score += _scoring_rules.trailing_whitespace(trailing_whitespace_count(line));

    if (!ignore_indent) {
        if (line.find(" \t") < std::string_view::npos) {
            score += _scoring_rules.mixed_indent();
        }

        if (check_indent_inconsistency(indent_pattern, _prev_indent, ic)) {
            score += _scoring_rules.inconsistent_indent();
        }

        _prev_indent = indent_pattern;
    }

    line.remove_prefix(indent_pattern.length());

    return score;
}
