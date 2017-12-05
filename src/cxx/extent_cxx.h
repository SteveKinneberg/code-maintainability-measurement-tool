/**
 * @headerfile ""
 *
 * Define the extent_cxx base class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _cxx_extent_cxx_h
#define _cxx_extent_cxx_h

#include <cassert>

#include "../extent.h"
#include "cxx_source_processor.h"

#include "../debug_util.h"


namespace cxx {

class extent_cxx: public extent {
  public:
    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     */
    extent_cxx(cxx_source_processor& processor, const std::string&& name):
        extent(processor, std::move(name)),
        _cxx_processor(processor),
        _line_count_adjust(0)
    {}

    virtual ~extent_cxx() = default;

    extent_cxx(extent_cxx&& other) = default;
    extent_cxx& operator=(extent_cxx&& other) = default;


    void set_line_count_adjust(int adjustment) { _line_count_adjust = adjustment; }

    extent_cxx(const extent_cxx& other) = delete;
    extent_cxx& operator=(const extent_cxx& other) = delete;

  protected:
    cxx_source_processor& _cxx_processor;
    int _line_count_adjust;

    virtual int line_count_adjust() const { return _line_count_adjust; }
};


}


#endif
