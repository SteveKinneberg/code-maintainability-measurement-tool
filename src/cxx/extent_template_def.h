#ifndef _cxx_extent_template_def_h
#define _cxx_extent_template_def_h
/**
 * @file
 *
 * Define the C/C++ template definition extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

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
class extent_template_def: public extent_cxx {
  public:
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_template_def(cxx_source_processor& processor):
        extent_cxx(processor, "template "),
        _have_open_angle_bracket(false),
        _param_count(0)
    {}

    virtual ~extent_template_def() final = default;

    extent_template_def(extent_template_def&& other) = default;
    extent_template_def& operator=(extent_template_def&& other) = default;

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

    extent_template_def() = delete;
    extent_template_def(const extent_template_def& other) = delete;
    extent_template_def& operator=(const extent_template_def& other) = delete;

  private:
    bool _have_open_angle_bracket;
    std::uint32_t _param_count;
};

}

#endif
