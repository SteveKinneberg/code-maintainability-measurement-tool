#ifndef _cxx_extent_comment_h
#define _cxx_extent_comment_h
/**
 * @file
 *
 * Define the C/C++ comment extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <memory>
#include <string_view>

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold extent state information for a comment.
 */
class extent_comment: public extent_cxx {
  public:
    /**
     * Check if the beginning of 'line' matches a comment extent.
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
    extent_comment(cxx_source_processor& processor):
        extent_cxx(processor, "C style comment")
    {}

    /** Destructor. */
    virtual ~extent_comment() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_comment(extent_comment&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_comment& operator=(extent_comment&& other) = default;

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

    extent_comment() = delete;
    extent_comment(const extent_comment& other) = delete;
    extent_comment& operator=(const extent_comment& other) = delete;
};

}
#endif
