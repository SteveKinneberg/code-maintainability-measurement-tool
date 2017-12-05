/**
 * @file ""
 *
 * Unit test file for testing the scoring of classes/structs in code.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#include "scoring_test.h"

#include <sstream>


#include "extent.h"
#include "cxx/cxx_source_processor.h"

namespace {

template<class T>
std::string scores_to_string(const T& scores)
{
    if (scores.empty()) {
        return "<no scores recorded>";
    }

    auto score_to_string = [] (const extent_score_info& info) {
        return std::string("'") + std::string(info.name()) + "' (" + std::to_string(info.score()) + ")";
    };

    return std::accumulate(std::next(scores.cbegin()),
                           scores.cend(),
                           score_to_string(*scores.cbegin()),
                           [score_to_string] (std::string& s, const extent_score_info& info) {
                               return (s + ", '" + score_to_string(info));
                           });
}


testing::AssertionResult line_score_eq(const char* expectedExpr,
                                       const char* actualExpr,
                                       const ScoringTestBase::test_sample_line& expected,
                                       const ScoringTestBase::test_sample_line& actual)
{
    if ((actual.first == expected.first) && (actual.second == expected.second)){
        return testing::AssertionSuccess();
    }

    std::stringstream ss;
    std::stringstream msg;

    ss.str("");
    ss << expected.first << " | " << expected.second;
    auto expectedStr = ss.str();

    ss.str("");
    ss << actual.first << " | " << actual.second;
    auto actualStr = ss.str();

    msg << "      Expected: " << expectedExpr;
    if(expectedStr != expectedExpr) {
        msg << "\n      Which is: " << expectedStr;
    }
    msg << "\nTo be equal to: " << actualExpr;
    if(actualStr != actualExpr) {
        msg << "\n      Which is: " << actualStr;
    }

    return testing::AssertionFailure() << msg.str();
}


#define EXPECT_LINE_EQ(exp, act)  EXPECT_PRED_FORMAT2(line_score_eq, exp, act)

}

void ScoringTestBase::check_extent_scores(std::string_view extent_type,
                                      const extent_scores_t& extent_scores,
                                      const top_extent_scores_t& top_extent_scores,
                                      const test_extent_scores& exp_extent_scores)
{
    EXPECT_EQ(extent_scores.size(), exp_extent_scores.size()) << "Extent type: " << extent_type;
    EXPECT_EQ(top_extent_scores.size(), exp_extent_scores.size()) << "Extent type: " << extent_type;

    for (const auto& [name, score]: exp_extent_scores) {
        auto exp_extent = extent_score_info(std::string(name), "<test>", 0, 0, score);

        auto it = extent_scores.find(exp_extent);
        ASSERT_NE(it, extent_scores.end())
                << "looking for '" << name << "' (" << score << ") in: "
                << scores_to_string(extent_scores);
        auto extent_score = it->score();
        EXPECT_EQ(extent_score, score) << "Extent type: " << extent_type;

        auto [start, end] = top_extent_scores.equal_range(exp_extent);
        ASSERT_NE(start, top_extent_scores.end()) << "looking for " << name
                                                  << " with score: " << score;
        EXPECT_TRUE(std::any_of(start, end, [&name] (const extent_score_info& info) {
                    return name == info.name();
                })) << "looking for '" << name << "' (" << score << ") in: "
                    << scores_to_string(top_extent_scores);
    }
}


void ScoringTestBase::run_test(const test_sample_code& test_code,
                           const test_extent_scores& exp_class_scores,
                           const test_extent_scores& exp_function_scores)
{
    auto full_code = concat_code(test_code);

    auto processor = cxx::cxx_source_processor(*rules, "<test>", std::move(full_code));

    processor.analyze();

    auto file_score = processor.file_score();
    auto line_scores = processor.line_scores();

    EXPECT_EQ(file_score, total_score(test_code));
    EXPECT_EQ(line_scores.size(), test_code.size());

    for(auto [ls_it, tc_it] = std::make_pair(line_scores.cbegin(), test_code.cbegin());
        (ls_it != line_scores.cend()) && (tc_it != test_code.cend());
        ++ls_it, ++tc_it) {

        auto [ls_line, ls_score] = *ls_it;
        auto [tc_score, tc_line] = *tc_it;

        auto act_line = std::pair<score_t, std::string_view>(ls_score, ls_line);
        auto exp_line = std::pair<score_t, std::string_view>(tc_score, tc_line);

        EXPECT_LINE_EQ(act_line, exp_line);
    }

    check_extent_scores("class",
                        processor.class_scores(),
                        processor.top_class_scores(),
                        exp_class_scores);

    check_extent_scores("function",
                        processor.function_scores(),
                        processor.top_function_scores(),
                        exp_function_scores);
}

static std::unique_ptr<scoring_rules> rules;

std::unique_ptr<scoring_rules> ScoringTestBase::rules = nullptr;
