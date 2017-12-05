/**
 * @headerfile ""
 *
 * Define the C/C++ statement extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _cxx_extent_statement_h
#define _cxx_extent_statement_h

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
class extent_statement: public extent_cxx {
  public:
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_statement(cxx_source_processor& processor):
        extent_cxx(processor, "statement"),
        _end_on_block_end(false),
        _end_on_semicolon(false),
        _record_class_score(false),
        _record_function_score(false)
    {}

    virtual ~extent_statement() final = default;

    extent_statement(extent_statement&& other) = default;
    extent_statement& operator=(extent_statement&& other) = default;

    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed.
     */
    virtual process_return process(std::string_view line) final;

    void set_end_on_block_end() { _end_on_block_end = true; }
    void set_end_on_semicolon() { _end_on_semicolon = true; }

    void record_class_score(const std::string& name) { _record_class_score = true; _inner_name = name; }
    void record_function_score(const std::string& name) { _record_function_score = true; _inner_name = name; }

    extent_statement() = delete;
    extent_statement(const extent_statement& other) = delete;
    extent_statement& operator=(const extent_statement& other) = delete;

  private:
    bool _end_on_block_end;
    bool _end_on_semicolon;

    bool _record_class_score;
    bool _record_function_score;

    std::string _inner_name;
};

}

#endif
