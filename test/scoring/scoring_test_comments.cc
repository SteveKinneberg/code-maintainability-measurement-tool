/**
 * @file ""
 *
 * Unit test file for testing the scoring of blank lines in code.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#include "scoring_test.h"


class ScoringTestComment: public ScoringTestBase { };

TEST_F(ScoringTestComment, BasicCComment) {
    run_test(
        { { 1, "/* comment */"} },
        {},
        {});
}

TEST_F(ScoringTestComment, BasicCxxComment) {
    run_test(
        { { 1, "// comment "} },
        {},
        {});
}

TEST_F(ScoringTestComment, MultilineCComment) {
    run_test(
        {   { 1, "/*"},
            { 1, " * comment"},
            { 1, " */"} },
        {},
        {});
}

TEST_F(ScoringTestComment, CCommentInCxxComment) {
    run_test(
        { { 1, "// /* comment */"} },
        {},
        {});
}

TEST_F(ScoringTestComment, CxxCommentInCComment) {
    run_test(
        { { 1, "/* // comment */"} },
        {},
        {});
}

TEST_F(ScoringTestComment, CCommentStartInCComment) {
    run_test(
        { { 1, "/* /* comment */"} },
        {},
        {});
}

TEST_F(ScoringTestComment, EscCharCommentInCComment) {
    run_test(
        { { 1, "/* comment \\*/"} },
        {},
        {});
}
