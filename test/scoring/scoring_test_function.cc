/**
 * @file ""
 *
 * Unit test file for testing the scoring of blank lines in code.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#include "scoring_test.h"


class ScoringTestFunction: public ScoringTestBase { };

TEST_F(ScoringTestFunction, Prototype1) {
    run_test(
        { { 1, "void foo();" } },
        {},
        {});
}

TEST_F(ScoringTestFunction, Prototype2) {
    run_test(
        { { 1, "void foo(int a);" } },
        {},
        {});
}

TEST_F(ScoringTestFunction, Prototype3) {
    run_test(
        { { 1, "const string foo(int a, int b);" } },
        {},
        {});
}

TEST_F(ScoringTestFunction, Trivial) {
    run_test(
        { { 1, "void foo() {}" } },
        {},
        { { "function foo", 1 } });
}

TEST_F(ScoringTestFunction, WithOneParameter) {
    run_test(
        { { 2, "void foo(int a) {}" } },
        {},
        { { "function foo", 2 } });
}

TEST_F(ScoringTestFunction, WithThreeParameter) {
    run_test(
        { { 10, "void foo(int a, int b, int c) {}" } },
        {},
        { { "function foo", 10 } });
}

TEST_F(ScoringTestFunction, Typical) {
    run_test(
        {   { 1, "void foo()" },
            { 1, "{" },
            { 2, "    /* return s + to_string(a); */" },
            { 1, "}" } },
        {},
        { { "function foo", 5 } });
}

TEST_F(ScoringTestFunction, TypicalSnuggedOpenBrace) {
    run_test(
        {   { 1, "void foo() {" },
            { 2, "    /* return s + to_string(a); */" },
            { 1, "}" } },
        {},
        { { "function foo", 4 } });
}

TEST_F(ScoringTestFunction, TypicalWithExtraNewlines1) {
    run_test(
        {   { 2, "void foo()" },
            { 1, "" },
            { 1, "{" },
            { 2, "    /* return s + to_string(a); */" },
            { 1, "}" } },
        {},
        { { "function foo", 7 } });
}

TEST_F(ScoringTestFunction, TypicalWithExtraNewlines2) {
    run_test(
        {   { 5, "void foo()" },
            { 1, "" },
            { 1, "" },
            { 1, "{" },
            { 2, "    /* return s + to_string(a); */" },
            { 1, "}" } },
        {},
        { { "function foo", 11 } });
}

TEST_F(ScoringTestFunction, TypicalWithTwoParametersAndReturn) {
    run_test(
        {   { 5, "const string foo(const string s, int a)" },
            { 1, "{" },
            { 2, "    /* return s + to_string(a); */" },
            { 1, "}" } },
        {},
        { { "function foo", 9 } });
}

TEST_F(ScoringTestFunction, ClassMethod) {
    run_test(
        {   { 5, "const string foo::bar(const string s, int a)" },
            { 1, "{" },
            { 2, "    /* return s + to_string(a); */" },
            { 1, "}" } },
        {},
        { { "function foo::bar", 9 } });
}

TEST_F(ScoringTestFunction, DISABLED_ClassMethodWithOddLineBreaks) {
    run_test(
        {   { 17, "const string foo" },
            { 1, "::" },
            { 1, "bar()" },
            { 1, "" },
            { 1, "{" },
            { 2, "    /* do something */" },
            { 1, "}" } },
        {},
        { { "function foo::bar", 24 } });
}

TEST_F(ScoringTestFunction, DISABLED_Destructor) {
    run_test(
        {   { 5, "foo::~foo()" },
            { 1, "{" },
            { 2, "    /* do something */" },
            { 1, "}" } },
        {},
        { { "function foo::bar", 9 } });
}

TEST_F(ScoringTestFunction, DISABLED_DestructorWithOddLineBreaks) {
    run_test(
        {   { 5, "foo" },
            { 5, "::" },
            { 5, "~" },
            { 5, "foo()" },
            { 1, "{" },
            { 2, "    /* do something */" },
            { 1, "}" } },
        {},
        { { "function foo::bar", 9 } });
}
