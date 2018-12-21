#ifndef _cmmt_exception_h
#define _cmmt_exception_h
/**
 * @file
 *
 * Define the CMMT exception base class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <exception>
#include <string>

/**
 * CMMT base exception.
 */
class cmmt_exception: public std::exception {
  public:
    /**
     * Constructor.
     *
     * @param msg   The exception message.
     */
    cmmt_exception(const std::string& msg): _msg(msg) {}

    /**
     * Implementation of the what() method that returns the exception message
     * string contents.
     *
     * @return  Pointer to the contents of the exception message string.
     */
    const char* what() const noexcept override { return _msg.c_str(); }

  private:
    const std::string _msg;     ///< The exception message.
};


#endif
