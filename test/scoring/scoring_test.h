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

/**
 * Scoring test base class.
 *
 * This serves as the base class for all scoring test code.
 */
class ScoringTestBase: public ::testing::Test {
  public:
    /// Score for a line of test sample code.
    using test_sample_line = std::pair<score_t, std::string_view>;

    /// Lines of test sample code.
    using test_sample_code = std::vector<test_sample_line>;

    /// Scores for extents of test sample code.
    using test_extent_scores = std::vector<std::pair<std::string_view, score_t>>;

    /** Destructor. */
    virtual ~ScoringTestBase() = default;

  protected:
    static std::unique_ptr<scoring_rules> rules;    ///< Scoring rules.

    /// Create the scoring rules in side the Google Test SetUpTestCase method.
    static void SetUpTestCase() {
        rules = std::make_unique<scoring_rules>();
    }

    /// Destroy the scoring rules in side the Google Test TearDownTestCase method.
    static void TearDownTestCase() {
        rules = nullptr;
    }

    /**
     * Check extent scores for any extent type with their respective expected score.
     *
     * @param extent_type           String describing the extent type.
     * @param extent_scores         Computed extent scores for the test sample code.
     * @param top_extent_scores     Top computed extent scores for the test sample code.
     * @param exp_extent_scores     Expected scores for each extent in the test case.
     */
    static void check_extent_scores(std::string_view extent_type,
                                    const extent_scores_t& extent_scores,
                                    const top_extent_scores_t& top_extent_scores,
                                    const test_extent_scores& exp_extent_scores);

    /**
     * Sum of all extent scores for the test sample code.
     *
     * @param code      Scores and test sample code for the test sample code.
     *
     * @return  The sum of all extent scores.
     */
    static score_t total_score(const test_sample_code& code)
    {
        return std::accumulate(code.begin(), code.end(),
                               0,
                               [](score_t total, const test_sample_line& line) {
                                   return total + line.first;
                               });
    }

    /**
     * Merge each line of the test sample code into a single string.
     *
     * The test case code must provide a list where each entry is a score for a
     * line of test sample code along with that line of test sample code.  This
     * function merges each of those lines into a single string that the CMMT
     * code being tested can evaluate.
     *
     * @param code      Scores and test sample code for the test sample code.
     *
     * @return  The merged results of all test sample code lines.
     */
    static std::string concat_code(const test_sample_code& code)
    {
        return std::accumulate(code.begin(), code.end(),
                               std::string(),
                               [](std::string result, const test_sample_line& line) {
                                   return result + std::string(line.second) + "\n";
                               });
    }

    /**
     * Perform the actual scoring test.
     *
     * @param test_code             Sample code to test along with expected scores for each line.
     * @param exp_class_scores      Expected scores for each class/struct in the test sample code.
     * @param exp_function_scores   Expected scores for each function in the test sample code.
     */
    void run_test(const test_sample_code& test_code,
                  const test_extent_scores& exp_class_scores,
                  const test_extent_scores& exp_function_scores);

};

#endif
