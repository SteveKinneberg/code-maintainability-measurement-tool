/**
 * @headerfile ""
 *
 * Define the C/C++ parenthetical extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _cxx_extent_paren_h
#define _cxx_extent_paren_h

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold state information for an extent of source code while that extent is
 * being processed.  This is useful for scoring things that may extend
 * across several lines.  It can also be useful for processing a statement
 * that maybe one of many on a single line.
 *
 * This is a base class that can be sub-classed to track additional information
 * about a given extent.
 */
class extent_paren: public extent_cxx {
  public:
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_paren(cxx_source_processor& processor):
        extent_cxx(processor, "paren " + std::to_string(_level)),
        _param_count(0)
    {}

    virtual ~extent_paren() final = default;

    extent_paren(extent_paren&& other) = default;
    extent_paren& operator=(extent_paren&& other) = default;


    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed.
     */
    virtual process_return process(std::string_view line) final;

    void count_param(std::uint32_t count = 1) { _param_count += count; }

    auto param_count() const { return _param_count; }

    extent_paren() = delete;
    extent_paren(const extent_paren& other) = delete;
    extent_paren& operator=(const extent_paren& other) = delete;

  private:
    static std::uint32_t _level;
    std::uint32_t _param_count;
};

}

#endif
