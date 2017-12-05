/**
 * @headerfile ""
 *
 * Define the C/C++ comment extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _cxx_extent_using_statement_h
#define _cxx_extent_using_statement_h

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
class extent_using_statement: public extent_cxx {
  public:
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_using_statement(cxx_source_processor& processor):
        extent_cxx(processor, "using ...")
    {}

    virtual ~extent_using_statement() final = default;

    extent_using_statement(extent_using_statement&& other) = default;
    extent_using_statement& operator=(extent_using_statement&& other) = default;


    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed.
     */
    virtual process_return process(std::string_view line) final;

    extent_using_statement() = delete;
    extent_using_statement(const extent_using_statement& other) = delete;
    extent_using_statement& operator=(const extent_using_statement& other) = delete;

  private:
};

}

#endif
