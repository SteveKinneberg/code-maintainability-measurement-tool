/**
 * @file ""
 *
 * Unit test file for testing the scoring of templates in code.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#include "scoring_test.h"


class ScoringTestTemplate: public ScoringTestBase { };


TEST_F(ScoringTestTemplate, SimpleFunction) {
    run_test(
        { { 3, "template <class T> void func(T t) { }" } },
        {},
        { { "function func", 3 } });
}

TEST_F(ScoringTestTemplate, NestedTemplate) {
    run_test(
        { { 4, "template <template<class> class T> void func(T t) { }" } },
        {},
        { { "function func", 4 } });
}

TEST_F(ScoringTestTemplate, DefaultTemplatedType) {
    run_test(
        {   { 2, "template <class T, class U = vector<T>>" },
            { 1, "class X {};" } },
        { { "class X", 3 } },
        {});
}


TEST_F(ScoringTestTemplate, ComplexEnableIf) {
    run_test(
        {   { 40, "template <class T, int S," },
            {  1, "          typename std::enable_if<" },
            {  1, "              std::is_trivially_destructable<T>{} &&" },
            {  1, "              (S > 10) &&" },
            {  1, "              (std::is_class<T>{} || std::is_union<T>{}" },
            {  1, "          int>::type = 0>" },
            {  1, "class X {};" } },
        { { "class X", 46 } },
        {});
}
