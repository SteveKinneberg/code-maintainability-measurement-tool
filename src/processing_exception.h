#ifndef _processing_exception_h
#define _processing_exception_h
/**
 * @file
 *
 * Declare source code processing exception class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <string_view>

#include "cmmt_exception.h"

/**
 * Exception thrown when parsing source code fails.
 */
class processing_exception: public cmmt_exception {
  public:
    /**
     * Constructor.
     *
     * @param msg           Exception message.
     * @param filename      Name of file being processed where the error was encountered.
     * @param line_num      Line number in the file where the error was encountered.
     * @param line          Source line with the error.
     * @param pos           Position within the line where the error was encountered.
     */
    processing_exception(std::string_view msg,
                         std::string_view filename, std::size_t line_num,
                         std::string_view line, std::size_t pos):
        cmmt_exception(build_message(msg, filename, line_num, line, pos))
    {}

  private:
    /**
     * Helper function to build the actual exception message suitable for
     * displaying to a user.
     *
     * @param msg           Exception message.
     * @param filename      Name of file being processed where the error was encountered.
     * @param line_num      Line number in the file where the error was encountered.
     * @param line          Source line with the error.
     * @param pos           Position within the line where the error was encountered.
     */
    static std::string build_message(std::string_view msg,
                                     std::string_view filename, std::size_t line_num,
                                     std::string_view line, std::size_t pos);
};



#endif
