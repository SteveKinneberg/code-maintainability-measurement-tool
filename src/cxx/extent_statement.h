#ifndef _cxx_extent_statement_h
#define _cxx_extent_statement_h
/**
 * @file
 *
 * Define the C/C++ statement extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_cxx.h"


namespace cxx {

/**
 * Hold extent state information for a regular C/C++ statement.
 */
class extent_statement: public extent_cxx {
  public:
    /**
     * Check if the beginning of 'line' matches a regular C/C++ statement extent.
     *
     * @param processor     Reference to the current language processor.
     * @param line          String view into the unprocessed part of the current
     *                      line being processed.
     *
     * @return  Whether this extent processor handles the next extent and how
     *          many characters were consumed.
     */
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param processor     Reference to the current language processor.
     */
    extent_statement(cxx_source_processor& processor):
        extent_cxx(processor, "statement"),
        _end_on_block_end(false),
        _end_on_semicolon(false),
        _record_class_score(false),
        _record_function_score(false)
    {}

    /** Destructor. */
    virtual ~extent_statement() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_statement(extent_statement&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_statement& operator=(extent_statement&& other) = default;

    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed and a pointer to the post
     *          processing function.
     */
    virtual process_return process(std::string_view line) final;

    /**
     * Set detection of extent end to be a block end (i.e., '}').
     */
    void set_end_on_block_end() { _end_on_block_end = true; }

    /**
     * Set detection of extent end to be a semicolon (i.e., ';').
     */
    void set_end_on_semicolon() { _end_on_semicolon = true; }

    /**
     * Set recording a class/struct statement score.
     *
     * @param name      Inner name to save.
     */
    void record_class_score(const std::string& name) { _record_class_score = true; _inner_name = name; }

    /**
     * Set recording a function statement score.
     *
     * @param name      Inner name to save.
     */
    void record_function_score(const std::string& name) { _record_function_score = true; _inner_name = name; }

    extent_statement() = delete;
    extent_statement(const extent_statement& other) = delete;
    extent_statement& operator=(const extent_statement& other) = delete;

  private:
    bool _end_on_block_end;         ///< Indicates if a statement ends with a block end.
    bool _end_on_semicolon;         ///< Indicates if a statement ends with a semicolon.
    bool _record_class_score;       ///< Indicates that a class/struct score should be recorded.
    bool _record_function_score;    ///< Indicates that a function score should be recorded.
    std::string _inner_name;        ///< Inner name of the class/struct or function being scored.
};

}
#endif
