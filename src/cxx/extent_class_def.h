#ifndef _cxx_extent_class_def_h
#define _cxx_extent_class_def_h
/**
 * @file
 *
 * Define the C/C++ class/struct definition extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold extent state information for a class/struct definition.
 */
class extent_class_def: public extent_cxx {
  public:
    /**
     * Check if the beginning of 'line' matches a class/struct extent.
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
    extent_class_def(cxx_source_processor& processor):
        extent_cxx(processor, "class "),
        _base_classes(false)
    {}

    /** Destructor. */
    virtual ~extent_class_def() final = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_class_def(extent_class_def&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_class_def& operator=(extent_class_def&& other) = default;

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

    extent_class_def() = delete;
    extent_class_def(const extent_class_def& other) = delete;
    extent_class_def& operator=(const extent_class_def& other) = delete;

  private:
    bool _base_classes;     ///< Indicates that the class/struct definition includes base classes.

    /**
     * Updates the class name.
     *
     * @param n     String view for the name.
     */
    void update_name(std::string_view n);
};

}
#endif
