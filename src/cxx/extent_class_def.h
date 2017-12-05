/**
 * @headerfile ""
 *
 * Define the C/C++ class/struct definition extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _cxx_extent_class_def_h
#define _cxx_extent_class_def_h

#include "extent_cxx.h"

namespace cxx {

/**
 * Hold state information for an extent of source code while that extent is
 * being processed.  This is useful for scoring things that may extend
 * across several lines.  It can also be useful for processing a statement
 * that maybe one of many on a single line.
 *
 * This is a base class that can be sub-classed to track additional information
 * about a given extent.
 */
class extent_class_def: public extent_cxx {
  public:
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_class_def(cxx_source_processor& processor):
        extent_cxx(processor, "class "),
        _base_classes(false)
    {}

    virtual ~extent_class_def() final = default;

    extent_class_def(extent_class_def&& other) = default;
    extent_class_def& operator=(extent_class_def&& other) = default;



    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed.
     */
    virtual process_return process(std::string_view line) final;

    extent_class_def() = delete;
    extent_class_def(const extent_class_def& other) = delete;
    extent_class_def& operator=(const extent_class_def& other) = delete;

  private:
    bool _base_classes;

    void update_name(std::string_view n);
};

}

#endif
