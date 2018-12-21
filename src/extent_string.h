#ifndef _extent_string_h
#define _extent_string_h
/**
 * @file
 *
 * Define the string extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <memory>
#include <string_view>

#include "extent.h"
#include "score_types.h"

/**
 * Hold state information for an extent of source code while that extent is
 * being processed.  This is useful for scoring things that may extend
 * across several lines.  It can also be useful for processing a statement
 * that maybe one of many on a single line.
 *
 * This is a base class that can be sub-classed to track additional information
 * about a given extent.
 */
class extent_string: public extent {
  public:
    enum class style { single_quote, double_quote };

    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_string(language_processor& processor, style s):
        extent(processor,
               (s == style::single_quote) ? "single quote string" : "double quote string"),
        _style(s)
    {}

    virtual ~extent_string() final = default;

    extent_string(extent_string&& other) = default;
    extent_string& operator=(extent_string&& other) = default;


    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed.
     */
    virtual process_return process(std::string_view line) final;

    extent_string() = delete;
    extent_string(const extent_string& other) = delete;
    extent_string& operator=(const extent_string& other) = delete;

  private:
    style _style;
};


#endif
