/**
 * @file ""
 *
 * Unit test file for testing the scoring of blank lines in code.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#include "scoring_test.h"


class ScoringTestClass: public ScoringTestBase {};


TEST_F(ScoringTestClass, ForwardDeclaration) {
    run_test(
        { { 1, "class foo;" } },
        {},
        {});
}

TEST_F(ScoringTestClass, EmptyOneLine) {
    run_test({ { 1, "class foo {};" } },
             { { "class foo", 1 } },
             {});
}

TEST_F(ScoringTestClass, Empty1) {
    run_test(
        {   { 1, "class foo {" },
            { 1, "};" } },
        { { "class foo", 2 } },
        {});
}

TEST_F(ScoringTestClass, Empty2) {
    run_test(
        {   { 1, "class foo" },
            { 1, "{};" } },
        { { "class foo", 2 } },
        {});
}

TEST_F(ScoringTestClass, Empty3) {
    run_test(
        {   { 1, "class foo" },
            { 1, "{" },
            { 1, "};" } },
        { { "class foo", 3 } },
        {});
}

TEST_F(ScoringTestClass, EmptyWithExtraNewLines1) {
    run_test(
        {   { 5, "class foo {" },
            { 1, "}" },
            { 1, "" },
            { 1, ";" } },
        { { "class foo", 8 } },
        {});
}

TEST_F(ScoringTestClass, EmptyWithExtraNewLines2) {
    run_test(
        {   { 2, "class foo" },
            { 1, "{" },
            { 1, "}" },
            { 1, ";" } },
        { { "class foo", 5 } },
        {});
}

TEST_F(ScoringTestClass, EmptyWithExtraNewLines3) {
    run_test(
        {   { 5, "class foo" },
            { 1, "" },
            { 1, "{" },
            { 1, "}" },
            { 1, ";" } },
        { { "class foo", 9 } },
        {});
}

TEST_F(ScoringTestClass, EmptyWithExtraNewLines4) {
    run_test(
        {   { 5, "class foo" },
            { 1, "{" },
            { 1, "}" },
            { 1, "" },
            { 1, ";" } },
        { { "class foo", 9 } },
        {});
}

TEST_F(ScoringTestClass, EmptyWithExtraNewLines5) {
    run_test(
        {   { 10, "class" },
            { 1, "foo {" },
            { 1, "}" },
            { 1, "" },
            { 1, ";" } },
        { { "class foo", 14 } },
        {});
}

TEST_F(ScoringTestClass, EmptyWithExtraNewLines6) {
    run_test(
        {   { 10, "class" },
            { 1, "foo" },
            { 1, "{" },
            { 1, "}" },
            { 1, "" },
            { 1, ";" } },
        { { "class foo", 15 } },
        {});
}

TEST_F(ScoringTestClass, EmptyWithLabel) {
    run_test(
        {   { 1, "class foo {" },
            { 1, "} label;" } },
        { { "class foo", 2 } },
        {});
}

TEST_F(ScoringTestClass, EmptyWithLabelAndSemicolonOnSeparateLines) {
    run_test(
        {   { 5, "class foo {" },
            { 1, "}" },
            { 1, "label" },
            { 1, ";" } },
        { { "class foo", 8 } },
        {});
}

TEST_F(ScoringTestClass, NestedEmptyStructs) {
    run_test(
        {   { 1, "struct foo {" },
            { 2, "    struct bar {" },
            { 2, "    };" },
            { 1, "};" } },
        {   { "class foo", 6 },
            { "class foo::bar", 2 } },
        {});
}

TEST_F(ScoringTestClass, Anonymous) {
    run_test(
        {   { 1, "class {" },
            { 1, "};" } },
        { { "class <anonymous>", 2 } },
        {});
}

TEST_F(ScoringTestClass, NestedAnonymousStructs) {
    run_test(
        {   { 1, "struct {" },
            { 2, "    struct {" },
            { 2, "    };" },
            { 1, "};" } },
        {   { "class <anonymous>", 6 },
            { "class <anonymous>::<anonymous>", 2 } },
        {});
}


TEST_F(ScoringTestClass, DISABLED_Basic) {
    run_test(
        {   { 1, "class foo {" },
            { 2, "  public:" },
            { 2, "    foo() {}" },
            { 2, "    ~foo() {}" },
            { 2, "  private:" },
            { 2, "    int var;" },
            { 1, "};" } },
        { { "class foo", 12 } },
        {});
}

