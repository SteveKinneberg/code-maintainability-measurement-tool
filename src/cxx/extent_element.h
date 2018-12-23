#ifndef _cxx_extent_element_h
#define _cxx_extent_element_h
/**
 * @file
 *
 * Define the C/C++ element extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <memory>
#include <string_view>

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold extent state information for a basic elements.
 *
 * A basic element is pretty much any token that is not handled by any of the
 * other C/C++ extents.  It primarily exists for counting parameters and
 * building up names with namespace delimiters.
 */
class extent_element: public extent_cxx {
  public:
    /**
     * Check if the beginning of 'line' matches a function definition extent.
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
     * @param name          Element name given to the extent.
     */
    extent_element(cxx_source_processor& processor, std::string_view name):
        extent_cxx(processor, std::string(name)),
        _continue_name(false)
    {}

    /** Destructor. */
    virtual ~extent_element() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_element(extent_element&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_element& operator=(extent_element&& other) = default;

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

    extent_element() = delete;
    extent_element(const extent_element& other) = delete;
    extent_element& operator=(const extent_element& other) = delete;

  private:
    /**
     * Indicates that a name is being built up that include namespace
     * delimiters.
     */
    bool _continue_name;
};

}
#endif
