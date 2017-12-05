/**
 * @file ""
 *
 * C/C++ class/struct definition extent processor implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent.h"


extent::extent(language_processor& processor, std::string&& name):
    _processor(processor),
    _name(std::move(name)),
    _start(processor.get_line_number()),
    _end(_start),
    _score(0),
    _has_body(false),
    _body_score(0),
    _body_start(0),
    _body_end(0)
{}


extent::extent(language_processor& processor, const std::string& name):
    _processor(processor),
    _name(name),
    _start(processor.get_line_number()),
    _end(_start),
    _score(0),
    _has_body(false),
    _body_score(0),
    _body_start(0),
    _body_end(0)
{}

std::size_t extent::line_count() const
{
    return (_end - _start) - (_body_end - _body_start) + 1 + line_count_adjust();
}
