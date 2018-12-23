#ifndef _cxx_extent_template_def_h
#define _cxx_extent_template_def_h
/**
 * @file
 *
 * Define the C/C++ template definition extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold extent state information for a template definition.
 */
class extent_template_def: public extent_cxx {
  public:
    /**
     * Check if the beginning of 'line' matches a template definition extent.
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
    extent_template_def(cxx_source_processor& processor):
        extent_cxx(processor, "template "),
        _have_open_angle_bracket(false),
        _param_count(0)
    {}

    /** Destructor. */
    virtual ~extent_template_def() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_template_def(extent_template_def&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_template_def& operator=(extent_template_def&& other) = default;

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
     * Adds the number of template parameters to the parameter count.
     *
     * @param count     Number of parameters to add to the parameter count. (Default: 1)
     */
    void count_param(std::uint32_t count = 1) { _param_count += count; }

    /**
     * Get the template parameter count.
     */
    auto param_count() const { return _param_count; }

    extent_template_def() = delete;
    extent_template_def(const extent_template_def& other) = delete;
    extent_template_def& operator=(const extent_template_def& other) = delete;

  private:
    bool _have_open_angle_bracket;  ///< Indicates if '<' has been processed.
    std::uint32_t _param_count;     ///< Number of template parameters.
};

}
#endif
