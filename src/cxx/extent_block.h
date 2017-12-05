/**
 * @headerfile ""
 *
 * Define the C/C++ block extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _cxx_extent_block_h
#define _cxx_extent_block_h

#include <memory>
#include <string_view>

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
class extent_block: public extent_cxx {
  public:
    static check_return check_token(language_processor& processor, std::string_view line);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     * @param line_num  Current line number.
     */
    extent_block(cxx_source_processor& processor):
        extent_cxx(processor, "block " + std::to_string(_level))
    {}

    virtual ~extent_block() final = default;

    extent_block(extent_block&& other) = default;
    extent_block& operator=(extent_block&& other) = default;


    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed.
     */
    virtual process_return process(std::string_view line) final;

    extent_block() = delete;
    extent_block(const extent_block& other) = delete;
    extent_block& operator=(const extent_block& other) = delete;

  private:
    static std::uint32_t _level;
};

}

#endif
