#ifndef _cxx_extent_cxx_h
#define _cxx_extent_cxx_h
/**
 * @file
 *
 * Define the extent_cxx base class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <cassert>

#include "../extent.h"
#include "cxx_source_processor.h"

#include "../debug_util.h"


namespace cxx {

/**
 * Base class for C/C++ extents.
 */
class extent_cxx: public extent {
  public:
    /**
     * Constructor.
     *
     * @param processor     Reference to the C/C++ source processor.
     * @param name          Name given to the extent.  Typically the
     *                      class/struct or function name.
     */
    extent_cxx(cxx_source_processor& processor, const std::string&& name):
        extent(processor, std::move(name)),
        _cxx_processor(processor),
        _line_count_adjust(0)
    {}

    /** Destructor. */
    virtual ~extent_cxx() = default;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    extent_cxx(extent_cxx&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    extent_cxx& operator=(extent_cxx&& other) = default;

    /**
     * Set the line count adjustment for this extent.
     *
     * @param adjustment    Adjustment to the line count.
     */
    void set_line_count_adjust(int adjustment) { _line_count_adjust = adjustment; }

    extent_cxx(const extent_cxx& other) = delete;
    extent_cxx& operator=(const extent_cxx& other) = delete;

  protected:
    cxx_source_processor& _cxx_processor;   ///< Reference to the C/C++ processor.
    int _line_count_adjust;                 ///< Adjustment to the line count.

    virtual int line_count_adjust() const override  { return _line_count_adjust; }
};

}
#endif
