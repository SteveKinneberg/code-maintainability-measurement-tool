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
 * Hold extent state information for a string.
 */
class extent_string: public extent {
  public:
    /// Kinds of strings.
    enum class style {
        single_quote,   ///< Single quote strings.
        double_quote    ///< Double quote strings.
    };

    /**
     * Check if the beginning of 'line' matches a string extent.
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
     * @param s             String style.
     */
    extent_string(language_processor& processor, style s):
        extent(processor,
               (s == style::single_quote) ? "single quote string" : "double quote string"),
        _style(s)
    {}

    /** Destructor. */
    virtual ~extent_string() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_string(extent_string&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_string& operator=(extent_string&& other) = default;

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

    extent_string() = delete;
    extent_string(const extent_string& other) = delete;
    extent_string& operator=(const extent_string& other) = delete;

  private:
    style _style;   ///< The string style being processed.
};


#endif
