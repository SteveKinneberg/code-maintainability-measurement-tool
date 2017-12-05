/**
 * @file ""
 *
 * Project class method implementations.
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "filesystem.h"

#include "cmmt_exception.h"

/**
 * Exception thrown as the result of a file read failure.
 */
class file_read_exception: public cmmt_exception {
  public:
    using cmmt_exception::cmmt_exception;
};

std::string read_file(const std::filesystem::path& fpath)
{
    std::ifstream is{fpath.string(), std::ios::ate};
    if(!is) {
        throw file_read_exception(std::string("Failed to open: ") + fpath.string());
    }

    optimize_read(is);

    std::string data;

    is.seekg(0, std::ios::end);
    auto size = is.tellg();
    data.resize(size);
    is.seekg(0, std::ios::beg);

    if(!is.read(&data[0], size)) {
        throw file_read_exception(std::string("Failed to read: ") + fpath.string());
    }

    return data;
}
