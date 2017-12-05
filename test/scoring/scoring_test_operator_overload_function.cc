/**
 * @file ""
 *
 * Unit test file for testing the scoring of blank lines in code.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#include "scoring_test.h"



class ScoringTestOperatorOverloadFunction: public ScoringTestBase { };


TEST_F(ScoringTestOperatorOverloadFunction, Prototype1) {
    run_test(
        { { 1, "bool operator<(int lhs, int rhs);" } },
        {},
        {});
}


TEST_F(ScoringTestOperatorOverloadFunction, Prototype2) {
    run_test(
        { { 1, "bool operator()();" } },
        {},
        {});
}

TEST_F(ScoringTestOperatorOverloadFunction, Prototype3) {
    run_test(
        { { 1, "operator bool();" } },
        {},
        {});
}


TEST_F(ScoringTestOperatorOverloadFunction, LessOperator) {
    run_test(
        { { 5, "bool operator<(int lhs, int rhs) {}" } },
        {},
        { { "function operator <", 5 } });
}


TEST_F(ScoringTestOperatorOverloadFunction, FunctionOperator) {
    run_test(
        { { 1, "bool operator()() {}" } },
        {},
        { { "function operator ()", 1 } });
}

TEST_F(ScoringTestOperatorOverloadFunction, CastOperator) {
    run_test(
        { { 1, "operator bool() {}" } },
        {},
        { { "function operator bool", 1 } });
}
