#ifndef _cxx_extent_paren_h
#define _cxx_extent_paren_h
/**
 * @file
 *
 * Define the C/C++ parenthetical extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold extent state information for a parenthetical extent.
 */
class extent_paren: public extent_cxx {
  public:
    /**
     * Check if the beginning of 'line' matches a parenthetical extent.
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
    extent_paren(cxx_source_processor& processor):
        extent_cxx(processor, "paren " + std::to_string(_level)),
        _param_count(0)
    {}

    /** Destructor. */
    virtual ~extent_paren() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_paren(extent_paren&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_paren& operator=(extent_paren&& other) = default;

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
     * Adds the number of function parameters to the parameter count.
     *
     * @param count     Number of parameters to add to the parameter count. (Default: 1)
     */
    void count_param(std::uint32_t count = 1) { _param_count += count; }

    /**
     * Get the function parameter count.
     */
    auto param_count() const { return _param_count; }

    extent_paren() = delete;
    extent_paren(const extent_paren& other) = delete;
    extent_paren& operator=(const extent_paren& other) = delete;

  private:
    static std::uint32_t _level;    ///< Count of open parentheses.
    std::uint32_t _param_count;     ///< Number of function parameters.
};

}
#endif
