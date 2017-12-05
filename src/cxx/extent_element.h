/**
 * @headerfile ""
 *
 * Define the C/C++ element extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _cxx_extent_element_h
#define _cxx_extent_element_h

#include <memory>
#include <string_view>

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
class extent_element: public extent_cxx {
  public:
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_element(cxx_source_processor& processor, std::string_view name):
        extent_cxx(processor, std::string(name)),
        _continue_name(false)
    {}

    virtual ~extent_element() final = default;

    extent_element(extent_element&& other) = default;
    extent_element& operator=(extent_element&& other) = default;


    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed.
     */
    virtual process_return process(std::string_view line) final;

    extent_element() = delete;
    extent_element(const extent_element& other) = delete;
    extent_element& operator=(const extent_element& other) = delete;

  private:
    bool _continue_name;
};

}

#endif
