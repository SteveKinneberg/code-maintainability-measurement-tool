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
    /**
     * Rule IDs for all the scoring rules.
     */
    enum class rule {
        boolean_op_cnt,     ///< Rule ID for binary operator counts.
        flow_ctrl_cnt,      ///< Rule ID for flow control counts.
        func_param_cnt,     ///< Rule ID for function parameter counts.
        goto_distance,      ///< Rule ID for distances between goto's and their target labels.
        indent_lvl,         ///< Rule ID for indentation level.
        line_cnt,           ///< Rule ID for line counts.
        line_length,        ///< Rule ID for line lengths.
        mixed_indent,       ///< Rule ID for mixed indentation.
        scalar_op_cnt,      ///< Rule ID for scalar operator counts.
        stmt_line_cnt,      ///< Rule ID for statement line counts.
        templ_param_cnt,    ///< Rule ID for template parameter counts.
        trailing_ws,        ///< Rule ID for trailing whitespace.
        wrong_indent_lvl    ///< Rule ID for wrong indentation level.
    };

    /// Function reference type for scoring functions that take one count or line number value.
    using score_fn1_t = std::function<score_t(std::size_t)>;

    /// Function reference type for scoring functions that take two count or line number values.
    using score_fn2_t = std::function<score_t(std::size_t, std::size_t)>;

    /// Convenience type for scoring function type definition.
    using score_op = std::variant<score_t, score_fn1_t, score_fn2_t>;

    /// The rule table - a map of rule IDs to scoring function.
    using rule_table = std::map<rule, score_op>;

    /**
     * Functor to compute an exponential score.
     *
     * Specfically, \f$v^p\f$ where \f$v\f$ is a count value from the code being
     * evaluated and \f$p\f$ is the power. \f$p\f$ must be a positive integer.
     */
    class pow {
      public:
        /**
         * Constructor,
         *
         * @param p     Exponent for computing the power.
         */
        pow(std::uintmax_t p): _power(p) {}

        /**
         * Function call operator to raise the specified value to the power
         * passed into the constructor.
         *
         * @param v     Count value to compute the power score with.
         *
         * @return  The result of \f$v^p\f$.
         */
        score_t operator()(std::size_t v)
        {
            score_t s = 1;
            for(auto p = _power; p > 0; --p) { s *= v; }
            return s;
        }

      private:
        const std::uintmax_t _power;    ///< Exponent value.
    };

    /**
     * Functor to compute a scalar score.
     *
     * Specifically, \f$v \frac {n}{d}\f$ where \f$v\f$ is a count value from
     * the code being evaluated and \f$\frac {n}{d}\f$ is the scaling factor.
     * Note that \f$d = 1\f$ by default.
     */
    class scale {
      public:
        /**
         * Constructor,
         *
         * @param n     Numerator value for the scaling factor.
         * @param d     Denomiator value for the scaling factor. (Default: 1)
         */
        scale(std::intmax_t n, std::intmax_t d = 1):
            _num(n / std::gcd(n, d)),
            _den(d / std::gcd(n, d))
        {}

        /**
         * Function call operator to apply a scaling factor passed into the
         * constructor to the specified value.
         *
         * @param v     Count value to compute scale with.
         *
         * @return  The result of \f$v \frac {n}{d}\f$.
         */
        score_t operator()(std::size_t v) { return (v * _num) / _den; }

      private:
        const std::intmax_t _num;   ///< Scaling numerator value.
        const std::intmax_t _den;   ///< Scaling denominator value.
    };

    /**
     * Factory method to create exponential scoring operators.
     *
     * @param p     Exponent for computing the power.
     *
     * @return  A functor instance for computing the desired exponential score.
     */
    static auto pow_op(std::uintmax_t p) { return score_fn1_t{pow{p}}; }

    /**
     * Factory method to create scalar scoring operators.
     *
     * @param n     Numerator value for the scaling factor.
     * @param d     Denomiator value for the scaling factor. (Default: 1)
     *
     * @return  A functor instance for computing the desired exponential score.
     */
    static auto scale_op(std::intmax_t n, std::intmax_t d = 1)
    {
        return score_fn1_t{scale(n, d)};
    }

    /**
     * Default constructor.
     *
     * Pre-populates the scoring rule table with default scoring rules.
     */
    scoring_rules();

    /**
     * Constructor that takes a properly formulated initializer list for
     * overriding the default the scoring rules.
     *
     * @param updates   The rules to apply to the rule table.
     */
    scoring_rules(rule_table&& updates): scoring_rules() { update_rules(std::move(updates)); }

    /**
     * Update the rule table from a properly formulated initializer list for
     * specifying the scoring rules.
     *
     * @param updates   The rules to apply to the rule table.
     */
    void update_rules(rule_table&& updates) { _rule_table.merge(std::move(updates)); }

    /**
     * Compute boolean operator score.
     *
     * @param count     Number of boolean operators.
     *
     * @return   The score.
     */
    auto boolean_operator_count(std::size_t count) const { return call_scorer(rule::boolean_op_cnt, count); }

    /**
     * Compute flow control score.
     *
     * @param count     Number of flow control statements.
     *
     * @return   The score.
     */
    auto flow_control_count(std::size_t count) const { return call_scorer(rule::flow_ctrl_cnt, count); }

    /**
     * Compute function parameter score.
     *
     * @param count     Number of function parameters.
     *
     * @return   The score.
     */
    auto function_parameter_count(std::size_t count) const { return call_scorer(rule::func_param_cnt, count); }

    /**
     * Compute goto distance score.
     *
     * @param goto_line     Line number of the goto statement.
     * @param label_line    Line number of the target goto label.
     *
     * @return   The score.
     */
    auto goto_distance(std::size_t goto_line, std::size_t label_line) const { return call_scorer(rule::goto_distance, goto_line, label_line); }

    /**
     * Compute inconsistent indentation score.
     *
     * @return   The score.
     */
    auto inconsistent_indent() const { return call_scorer(rule::wrong_indent_lvl); }

    /**
     * Compute indentation level score.
     *
     * @param level     Number of indentation levels. (Default: 1)
     *
     * @return   The score.
     */
    auto indent_level(std::uint32_t level = 1) const { return call_scorer(rule::indent_lvl, level); }

    /**
     * Compute line count score.
     *
     * @param count     Number of lines.
     *
     * @return   The score.
     */
    auto line_count(std::size_t count) const { return call_scorer(rule::line_cnt, count); }

    /**
     * Compute line length score.
     *
     * @param length    Length of the line.
     *
     * @return   The score.
     */
    auto line_length(std::size_t length) const { return call_scorer(rule::line_length, length); }

    /**
     * Compute mixed indentation score.
     *
     * @return   The score.
     */
    auto mixed_indent() const { return call_scorer(rule::mixed_indent); }

    /**
     * Compute scalar operator count score.
     *
     * @param count     Number of scalar operators.
     *
     * @return   The score.
     */
    auto scalar_operator_count(std::size_t count) const { return call_scorer(rule::scalar_op_cnt, count); }

    /**
     * Compute statement line count score.
     *
     * @param count     Number of lines in the statement.
     *
     * @return   The score.
     */
    auto statement_line_count(std::size_t count) const { return call_scorer(rule::stmt_line_cnt, count); }

    /**
     * Compute template parameter count score.
     *
     * @param count     Number of template parameters.
     *
     * @return   The score.
     */
    auto template_parameter_count(std::size_t count) const { return call_scorer(rule::templ_param_cnt, count); }

    /**
     * Compute trailing whitespace score.
     *
     * @param count     Number of trailing whitespace characters.
     *
     * @return   The score.
     */
    auto trailing_whitespace(std::size_t count) const { return call_scorer(rule::trailing_ws, count); }

  private:
    rule_table _rule_table;     ///< The scoring rule table.

    /**
     * Helper template method that looks up the scoring operator function to call.
     *
     * Helper template method to invoke the scoring function with the correct
     * number of arguments.
     *
     * @param r         Rule ID of the scoring rule to apply.
     * @param args      Arguments to pass to the scoring function.
     *
     * @return  The score.
     */
    template<class... Args>
    score_t call_scorer(rule r, const Args&... args) const
    {
        static_assert(sizeof... (args) <= 2);
        auto scorer_variant = _rule_table.at(r);

        if constexpr (sizeof... (args) == 0) {
            return std::get<score_t>(scorer_variant);

        } else if constexpr (sizeof... (args) == 1) {
            auto fn = std::get<score_fn1_t>(scorer_variant);
            return std::invoke(fn, args...);

        } else if constexpr (sizeof... (args) == 2) {
            auto fn = std::get<score_fn2_t>(scorer_variant);
            return std::invoke(fn, args...);
        }
        return 0;
    }
};

#endif
