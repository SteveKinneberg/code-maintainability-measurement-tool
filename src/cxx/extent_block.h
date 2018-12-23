#ifndef _cxx_extent_block_h
#define _cxx_extent_block_h
/**
 * @file
 *
 * Define the C/C++ block extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <memory>
#include <string_view>

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold extent state information for a C/C++ code block.
 */
class extent_block: public extent_cxx {
  public:
    /**
     * Check if the beginning of 'line' matches a C/C++ code block extent.
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
    extent_block(cxx_source_processor& processor):
        extent_cxx(processor, "block " + std::to_string(_level))
    {}

    /** Destructor. */
    virtual ~extent_block() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_block(extent_block&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_block& operator=(extent_block&& other) = default;

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

    extent_block() = delete;
    extent_block(const extent_block& other) = delete;
    extent_block& operator=(const extent_block& other) = delete;

  private:
    static std::uint32_t _level;    ///< Count of open curly braces.
};

}

#endif
