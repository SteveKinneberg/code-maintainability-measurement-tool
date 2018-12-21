#ifndef _scoring_test_h
#define _scoring_test_h
/**
 * @file
 *
 * Define the scoring Google Test class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <gtest/gtest.h>

#include <algorithm>
#include <string_view>

#include "score_types.h"
#include "scoring_rules.h"

class ScoringTestBase: public ::testing::Test {
  public:
    using test_sample_line = std::pair<score_t, std::string_view>;
    using test_sample_code = std::vector<test_sample_line>;
    using test_extent_scores = std::vector<std::pair<std::string_view, score_t>>;

    virtual ~ScoringTestBase() = default;

  protected:
    static std::unique_ptr<scoring_rules> rules;

    static void SetUpTestCase() {
        rules = std::make_unique<scoring_rules>();
    }
    static void TearDownTestCase() {
        rules = nullptr;
    }

    static void check_extent_scores(std::string_view extent_type,
                                    const extent_scores_t& extent_scores,
                                    const top_extent_scores_t& top_extent_scores,
                                    const test_extent_scores& exp_extent_scores);

    static score_t total_score(const test_sample_code& code)
    {
        return std::accumulate(code.begin(), code.end(),
                               0,
                               [](score_t total, const test_sample_line& line) {
                                   return total + line.first;
                               });
    }

    static std::string concat_code(const test_sample_code& code)
    {
        return std::accumulate(code.begin(), code.end(),
                               std::string(),
                               [](std::string result, const test_sample_line& line) {
                                   return result + std::string(line.second) + "\n";
                               });
    }

    void run_test(const test_sample_code& test_code,
                  const test_extent_scores& exp_class_scores,
                  const test_extent_scores& exp_function_scores);

};




#endif
