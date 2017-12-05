/**
 * @headerfile ""
 *
 * Define the C/C++ function definition extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _cxx_extent_function_def_h
#define _cxx_extent_function_def_h

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
class extent_function_def: public extent_cxx {
  public:
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_function_def(cxx_source_processor& processor, std::string&& name):
        extent_cxx(processor, std::move("function " + name)),
        _param_count(0)
    {}

    virtual ~extent_function_def() = default;

    extent_function_def(extent_function_def&& other) = default;
    extent_function_def& operator=(extent_function_def&& other) = default;

    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed.
     */
    virtual process_return process(std::string_view line);

    void set_param_count(std::uint32_t count) { _param_count += count; }
    auto param_count() const { return _param_count; }

    extent_function_def() = delete;
    extent_function_def(const extent_function_def& other) = delete;
    extent_function_def& operator=(const extent_function_def& other) = delete;

  private:
    std::uint32_t _param_count;
};

}

#endif
