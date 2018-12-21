#ifndef _scoring_rules_h
#define _scoring_rules_h
/**
 * @file
 *
 * Define the score rules class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <array>
#include <functional>
#include <map>
#include <numeric>
#include <ratio>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "score_types.h"


/**
 * This class centralizes all the scoring rules for each scoring categories.
 * This should make changing the scoring balance easier.
 */
class scoring_rules {
  public:
    enum class rule {
        boolean_op_cnt,
        flow_ctrl_cnt,
        func_param_cnt,
        goto_distance,
        indent_lvl,
        line_cnt,
        line_length,
        mixed_indent,
        scalar_op_cnt,
        stmt_line_cnt,
        templ_param_cnt,
        trailing_ws,
        wrong_indent_lvl
    };

    using score_fn1_t = std::function<score_t(std::size_t)>;
    using score_fn2_t = std::function<score_t(std::size_t, std::size_t)>;

    /**
     * Convenience type for scoring function type definition.
     */
    using score_op = std::variant<score_t, score_fn1_t, score_fn2_t>;

    using rule_table = std::map<rule, score_op>;


    class pow {
      public:
        pow(std::uintmax_t p): _power(p) {}

        score_t operator()(std::size_t v)
        {
            score_t s = 1;
            for(auto p = _power; p > 0; --p) { s *= v; }
            return s;
        }

      private:
        const std::uintmax_t _power;
    };


    class scale {
      public:
        scale(std::intmax_t n, std::intmax_t d = 1):
            _num(n / std::gcd(n, d)),
            _den(d / std::gcd(n, d))
        {}

        score_t operator()(std::size_t v) { return (v * _num) / _den; }

      private:
        const std::intmax_t _num;
        const std::intmax_t _den;
    };



    static auto pow_op(std::uintmax_t p) { return score_fn1_t{pow{p}}; }


    static auto scale_op(std::intmax_t n, std::intmax_t d = 1)
    {
        return score_fn1_t{scale(n, d)};
    }





    scoring_rules();

    scoring_rules(rule_table&& updates): scoring_rules() { update_rules(std::move(updates)); }

    void update_rules(rule_table&& updates) { _rule_table.merge(std::move(updates)); }


    auto boolean_operator_count(std::size_t count) const { return call_scorer(rule::boolean_op_cnt ,count); }
    auto flow_control_count(std::size_t count) const { return call_scorer(rule::flow_ctrl_cnt, count); }
    auto function_parameter_count(std::size_t count) const { return call_scorer(rule::func_param_cnt, count); }
    auto goto_distance(std::size_t goto_line, std::size_t label_line) const { return call_scorer(rule::goto_distance, goto_line, label_line); }
    auto inconsistent_indent() const { return call_scorer(rule::wrong_indent_lvl); }
    auto indent_level(std::uint32_t level = 1) const { return call_scorer(rule::indent_lvl, level); }
    auto line_count(std::size_t count) const { return call_scorer(rule::line_cnt, count); }
    auto line_length(std::size_t length) const { return call_scorer(rule::line_length, length); }
    auto mixed_indent() const { return call_scorer(rule::mixed_indent); }
    auto scalar_operator_count(std::size_t count) const { return call_scorer(rule::scalar_op_cnt, count); }
    auto statement_line_count(std::size_t count) const { return call_scorer(rule::stmt_line_cnt, count); }
    auto template_parameter_count(std::size_t count) const { return call_scorer(rule::templ_param_cnt, count); }
    auto trailing_whitespace(std::size_t count) const { return call_scorer(rule::trailing_ws, count); }

  private:
    rule_table _rule_table;

    template<class... Args>
    static score_t eval_score(const score_op& s, const Args&... args)
    {
        static_assert(sizeof... (args) <= 2);

        if constexpr (sizeof... (args) == 0) {
                return std::get<score_t>(s);
            }

        if constexpr (sizeof... (args) == 1) {
                auto fn = std::get<score_fn1_t>(s);
                return std::invoke(fn, args...);
            }

        if constexpr (sizeof... (args) == 2) {
                auto fn = std::get<score_fn2_t>(s);
                return std::invoke(fn, args...);
            }
        return 0;
    }

    template<class... Args>
    score_t call_scorer(rule s, const Args&... args) const
    {
        return eval_score(_rule_table.at(s), args...);
    }
};



#endif
