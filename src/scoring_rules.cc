/**
 * @file ""
 *
 * Provide the default scoring rules.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */


#include "scoring_rules.h"

using sr = scoring_rules;
using r = sr::rule;

scoring_rules::scoring_rules():
    _rule_table{
    { r::boolean_op_cnt,   sr::pow_op(2) },
    { r::flow_ctrl_cnt,    sr::scale_op(1) },
    { r::func_param_cnt,   sr::pow_op(2) },
    { r::goto_distance,    sr::score_fn2_t{ [] (std::size_t g, std::size_t l) {
                if (l > g) { return sr::scale{1}(l - g); }
                return sr::scale{5}(g - l);
            }} },
    { r::indent_lvl,       sr::scale_op(1)},
    { r::line_cnt,         sr::scale_op(1) },
    { r::line_length, sr::score_fn1_t{ [] (std::size_t v) {
                score_t s = 0;
                if (v >  60) { s += sr::scale{ 1, 2}(v - 60); }
                if (v >  80) { s += sr::scale{ 1}(v - 80); }
                if (v > 100) { s += sr::scale{ 5}(v - 100); }
                if (v > 120) { s += sr::scale{10}(v - 120); }
                return s;
            }} },
    { r::mixed_indent,     score_t{ 100 } },
    { r::scalar_op_cnt,    sr::scale_op(1) },
    { r::stmt_line_cnt,    sr::score_fn1_t{ [] (std::size_t v) { return sr::pow{2}(v - 1); }} },
    { r::templ_param_cnt,  sr::pow_op(2) },
    { r::trailing_ws,      sr::scale_op(10) },
    { r::wrong_indent_lvl, score_t{ 100 } }
}
{}
