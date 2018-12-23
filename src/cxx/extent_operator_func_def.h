#ifndef _cxx_extent_operator_func_def_h
#define _cxx_extent_operator_func_def_h
/**
 * @file
 *
 * Define the C/C++ operator function definition extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_function_def.h"

namespace cxx {

/**
 * Hold extent state information for an operator function definition.
 */
class extent_operator_func_def: public extent_function_def {
  public:
    /**
     * Check if the beginning of 'line' matches an operator function definition extent.
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
    extent_operator_func_def(cxx_source_processor& processor):
        extent_function_def(processor, "operator"),
        _is_func_operator(false),
        _have_operator(false)
    {}

    /** Destructor. */
    virtual ~extent_operator_func_def() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_operator_func_def(extent_operator_func_def&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_operator_func_def& operator=(extent_operator_func_def&& other) = default;

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

    extent_operator_func_def() = delete;
    extent_operator_func_def(const extent_operator_func_def& other) = delete;
    extent_operator_func_def& operator=(const extent_operator_func_def& other) = delete;

  private:
    bool _is_func_operator;     ///< Indicates if the extent is a function operator.
    bool _have_operator;        ///< Indicates processing of 'operator' keyword.
};

}
#endif
