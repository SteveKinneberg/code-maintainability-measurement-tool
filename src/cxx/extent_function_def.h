#ifndef _cxx_extent_function_def_h
#define _cxx_extent_function_def_h
/**
 * @file
 *
 * Define the C/C++ function definition extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold extent state information for a function definition.
 */
class extent_function_def: public extent_cxx {
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
     * @param name          Function name given to the extent.
     */
    extent_function_def(cxx_source_processor& processor, std::string&& name):
        extent_cxx(processor, std::move("function " + name)),
        _param_count(0)
    {}

    /** Destructor. */
    virtual ~extent_function_def() = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_function_def(extent_function_def&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_function_def& operator=(extent_function_def&& other) = default;

    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed and a pointer to the post
     *          processing function.
     */
    virtual process_return process(std::string_view line) override;

    /**
     * Adds the number of function parameters to the parameter count.
     *
     * @param count     Number of parameters to add to the parameter count. (Default: 1)
     */
    void set_param_count(std::uint32_t count) { _param_count += count; }

    /**
     * Get the function parameter count.
     */
    auto param_count() const { return _param_count; }

    extent_function_def() = delete;
    extent_function_def(const extent_function_def& other) = delete;
    extent_function_def& operator=(const extent_function_def& other) = delete;

  private:
    std::uint32_t _param_count;     ///< Number of function parameters.
};

}

#endif
