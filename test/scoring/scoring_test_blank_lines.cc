/**
 * @file ""
 *
 * Unit test file for testing the scoring of blank lines in code.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#include "scoring_test.h"

class ScoringTestFile: public ScoringTestBase {};


TEST_F(ScoringTestFile, Empty) {
    run_test({}, {}, {});
}

TEST_F(ScoringTestFile, BlankLines) {
    run_test(
        {   { 1, ""},
            { 1, ""},
            { 1, ""} },
        {},
        {});
}
