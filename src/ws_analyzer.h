#ifndef _ws_analyzer_h
#define _ws_analyzer_h
/**
 * @file
 *
 * Declare the whitespace analyzer class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */


#include <string_view>

#include "score_types.h"
#include "scoring_rules.h"

/**
 * This functor class will analyze and provide scoring for indentation, long
 * lines and trailing whitespace.
 */
class ws_analyzer {
  public:
    /** Indication of possibly expected indentation changes. */
    enum class indent_change {
        expect_none,        ///< No change in indentation is expected.
        expect_increase,    ///< Indentation is expected to increase.
        expect_decrease,    ///< Indentation is expected to decrease.
        allow_increase,     ///< Indentation can increase or stay the same.
        allow_decrease,     ///< Indentation can decrease or stay the same.
        allow_any           ///< Any change to indentation is allowed.
    };

    /** Default constructor. */
    ws_analyzer(const scoring_rules& rules): _scoring_rules(rules) {}

    /**
     * Function call operator that analyzes a line of code and returns a score
     * and the number of characters in the indentation.
     *
     * @param[in,out] line              The line to analyze.
     * @param         ic                The expected indentation change
     *                                  (defaults to allow_any).
     * @param         ignore_indent     Ignore all line indentation.
     *
     * @return  The score for the indentation.
     */
    score_t operator()(std::string_view& line,
                       indent_change ic = indent_change::allow_any,
                       bool ignore_indent = false);

    ws_analyzer() = delete;
    ws_analyzer(ws_analyzer&&) = delete;
    ws_analyzer(const ws_analyzer&) = delete;
    ws_analyzer& operator=(ws_analyzer&&) = delete;
    ws_analyzer& operator=(const ws_analyzer&) = delete;

  private:
    const scoring_rules& _scoring_rules;    ///< Reference to the scoring rules.
    std::string_view _prev_indent;          ///< Indentation for the previous line of code.
};

#endif
