#ifndef _extent_file_h
#define _extent_file_h
/**
 * @file
 *
 * Define the file extent class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */


#include "extent.h"

class extent_file: public extent {
  public:
    using extent::extent;
};


#endif
