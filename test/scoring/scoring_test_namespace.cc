/**
 * @file ""
 *
 * Unit test file for testing the scoring of blank lines in code.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#include "scoring_test.h"


class ScoringTestNamespace: public ScoringTestBase { };


TEST_F(ScoringTestNamespace, Anonymous) {
    run_test(
        { { 1, "namespace {}"} },
        {},
        {});
}


TEST_F(ScoringTestNamespace, Basic) {
    run_test(
        {   { 1, "namespace foo {"},
            { 1, "}"} },
        {},
        {});
}

TEST_F(ScoringTestNamespace, Nested) {
    run_test(
        {   { 1, "namespace foo {"},
            { 1, "namespace bar {}"},
            { 1, "}"} },
        {},
        {});
}

TEST_F(ScoringTestNamespace, Cxx17) {
    run_test(
        {   { 1, "namespace foo::bar {"},
            { 1, "}"} },
        {},
        {});
}

TEST_F(ScoringTestNamespace, Cxx17StyleSplitAcrossLines) {
    run_test(
        {   { 5, "namespace foo"},
            { 1, "::"},
            { 1, "bar {"},
            { 1, "}"} },
        {},
        {});
}
