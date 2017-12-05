/**
 * @file ""
 *
 * C/C++ source file processor class method implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "processing_exception.h"


std::string processing_exception::build_message(std::string_view msg,
                                                std::string_view filename, std::size_t line_num,
                                                std::string_view line, std::size_t pos)
{
    std::string s("Failed to process \"");

    s += filename;
    s += "\" at line ";
    s += std::to_string(line_num);
    if (!msg.empty()) {
        s += ": ";
        s += msg;
    }
    s += '\n';

    s += line;
    s += '\n';

    while (pos--) {
        if ((line[0] == ' ') || (line[0] == '\t')) {
            s += line[0];
        } else {
            s += ' ';
        }
        line.remove_prefix(1);
    }
    s += '^';

    return s;
}
