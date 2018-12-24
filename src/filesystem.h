#ifndef _filesystem_h
#define _filesystem_h
/**
 * @file
 *
 * Abstract support for C++17 std::filesystem by bringing boost::filesystem into
 * the std namespace if USE_BOOST_FS is defined.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "config.h"

#include <fstream>
#include <set>


#if defined(HAVE_FILESYSTEM)
// C++17 filesystem support.
#include <filesystem>

#elif defined(HAVE_LIBBOOST_FILESYSTEM)
#include <boost/filesystem.hpp>
namespace std {
namespace filesystem {
// Map everything from the boost::filesystem namespace into the std::filesystem
// namespace.
using namespace boost::filesystem;
}
}
#endif  // defined(HAVE_FILESYSTEM)


#if defined(HAVE_POSIX_FADVISE)
#include <fcntl.h>
/**
 * Enable file read optimization/
 *
 * @param is    Input file stream to optimize reads from.
 */
inline void optimize_read(std::ifstream& is)
{
    posix_fdadvise(is.rdbuf()->fd(), 0, 0, POSIX_FADV_SEQUENTIAL);
}
#else   // defined(HAVE_POSIX_FADVISE)
/// @cond IGNORE
inline void optimize_read(std::ifstream& is[[maybe_unused]]) {}
/// @endcond
#endif  // defined(HAVE_POSIX_FADVISE)

/// Convenience type for an ordered set of paths.
using paths_t = std::set<std::filesystem::path>;

/**
 * Utility function to read an entire file into a string
 *
 * @param p     Path to the file to read.
 */
std::string read_file(const std::filesystem::path& p);

#endif
