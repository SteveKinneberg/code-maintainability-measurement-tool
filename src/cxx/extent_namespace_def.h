#ifndef _cxx_extent_namespace_def_h
#define _cxx_extent_namespace_def_h
/**
 * @file
 *
 * Define the C/C++ namespace definition extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold extent state information for a namespace extent.
 */
class extent_namespace_def: public extent_cxx {
  public:
    /**
     * Check if the beginning of 'line' matches a namespace extent.
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
    extent_namespace_def(cxx_source_processor& processor):
        extent_cxx(processor, "")
    {}

    /** Destructor. */
    virtual ~extent_namespace_def() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_namespace_def(extent_namespace_def&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_namespace_def& operator=(extent_namespace_def&& other) = default;

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

    extent_namespace_def() = delete;
    extent_namespace_def(const extent_namespace_def& other) = delete;
    extent_namespace_def& operator=(const extent_namespace_def& other) = delete;

  private:
};

}

#endif
