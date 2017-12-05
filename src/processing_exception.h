/**
 * @headerfile ""
 *
 * Declare Cmmt exception base class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _processing_exception_h
#define _processing_exception_h

#include <string_view>

#include "cmmt_exception.h"

class processing_exception: public cmmt_exception {
  public:
    processing_exception(std::string_view msg,
                         std::string_view filename, std::size_t line_num,
                         std::string_view line, std::size_t pos):
        cmmt_exception(build_message(msg, filename, line_num, line, pos))
    {}

  private:
    static std::string build_message(std::string_view msg,
                                     std::string_view filename, std::size_t line_num,
                                     std::string_view line, std::size_t pos);
};



#endif
