/**
 * @headerfile ""
 *
 * Define the C/C++ operator function definition extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _cxx_extent_operator_func_def_h
#define _cxx_extent_operator_func_def_h

#include "extent_function_def.h"

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
class extent_operator_func_def: public extent_function_def {
  public:
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_operator_func_def(cxx_source_processor& processor):
        extent_function_def(processor, "operator"),
        _is_func_operator(false),
        _have_operator(false)
    {}

    virtual ~extent_operator_func_def() final = default;

    extent_operator_func_def(extent_operator_func_def&& other) = default;
    extent_operator_func_def& operator=(extent_operator_func_def&& other) = default;


    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed.
     */
    virtual process_return process(std::string_view line) final;

    extent_operator_func_def() = delete;
    extent_operator_func_def(const extent_operator_func_def& other) = delete;
    extent_operator_func_def& operator=(const extent_operator_func_def& other) = delete;

  private:
    bool _is_func_operator;
    bool _have_operator;
};

}

#endif
