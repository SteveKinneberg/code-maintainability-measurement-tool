/**
 * @file ""
 *
 * Unit test file for testing the scoring of blank lines in code.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#include "scoring_test.h"


class ScoringTestUsingStatement: public ScoringTestBase { };


TEST_F(ScoringTestUsingStatement, Simple) {
    run_test(
        { { 1, "using foo;"} },
        {},
        {});
}

TEST_F(ScoringTestUsingStatement, Namespace1) {
    run_test(
        { { 1, "using namespace foo;"} },
        {},
        {});
}

TEST_F(ScoringTestUsingStatement, Namespace2) {
    run_test(
        { { 1, "using namespace foo::bar;"} },
        {},
        {});
}

TEST_F(ScoringTestUsingStatement, MultiLine1) {
    run_test(
        {   { 2, "using"},
            { 1, "foo;"} },
        {},
        {});
}

TEST_F(ScoringTestUsingStatement, MultiLine2) {
    run_test(
        {   { 2, "using foo"},
            { 1, ";"} },
        {},
        {});
}

TEST_F(ScoringTestUsingStatement, MultiLine3) {
    run_test(
        {   { 26, "using"},
            { 1, "namespace"},
            { 1, "foo"},
            { 1, "::"},
            { 1, "bar"},
            { 1, ";"}},
        {},
        {});
}

TEST_F(ScoringTestUsingStatement, CComment1) {
    run_test(
        { { 1, "using /* comment */ foo;"} },
        {},
        {});
}

TEST_F(ScoringTestUsingStatement, CComment2) {
    run_test(
        {   { 2, "using /* comment"},
            { 1, " more comment */ foo;"} },
        {},
        {});
}

TEST_F(ScoringTestUsingStatement, CxxComment) {
    run_test(
        {   { 2, "using // comment"},
            { 1, "foo;"} },
        {},
        {});
}
