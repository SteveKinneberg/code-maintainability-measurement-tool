#ifndef _score_types_processor_h
#define _score_types_processor_h
/**
 * @file
 *
 * Define the score type and various scoring containers.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <set>
#include <string>
#include <string_view>
#include <vector>

/// The score type.
using score_t = std::uint32_t;

/// Line score associating a line of source with a score.
using line_score_t = std::pair<std::string_view, score_t>;

/// A list of line scores (for a file or extent).
using line_scores_t = std::vector<line_score_t>;

/**
 * Scoring information for an extent suitable for presentation to the user.
 */
class extent_score_info {
  public:
    /**
     * Constructor.
     *
     * @param name          Name of the extent (typically, function or class/stuct name).
     * @param filename      Name of the file the extent appears in.
     * @param start_line    Starting line of the extent.
     * @param end_line      Ending line of the extent.
     * @param score         The score for the extent.
     */
    extent_score_info(const std::string& name,
                      const std::string& filename,
                      std::size_t start_line,
                      std::size_t end_line,
                      score_t score):
        _name(std::move(name)),
        _filename(filename),
        _start_line(start_line),
        _end_line(end_line),
        _score(score)
    {}

    /**
     * Get the name of the extent.
     *
     * @return  The name of the extent.
     */
    std::string_view name() const { return _name; }

    /**
     * Get the file name of the extent.
     *
     * @return  The file name of the extent.
     */
    std::string_view filename() const { return _filename; }

    /**
     * Get the starting line of the extent.
     *
     * @return  The starting line of the extent.
     */
    std::size_t start_line() const { return _start_line; }

    /**
     * Get the ending line of the extent.
     *
     * @return  The ending line of the extent.
     */
    std::size_t end_line() const { return _end_line; }

    /**
     * Get the score of the extent.
     *
     * @return  The score of the extent.
     */
    score_t score() const { return _score; }

  private:
    const std::string _name;        ///< Name of the extent (typically, function or class/stuct name).
    const std::string _filename;    ///< Name of the file the extent appears in.
    const std::size_t _start_line;  ///< Starting line of the extent.
    const std::size_t _end_line;    ///< Ending line of the extent.
    const score_t _score;           ///< The score for the extent.
};

/**
 * Functor used to sort extent scores by extent name.
 */
struct extent_name_sort {
    /**
     * Function call operator that compares 2 extent names.
     *
     * @param lhs   Left hand side of '<' operator for comparison.
     * @param rhs   Right hand side of '<' operator for comparison.
     */
    bool operator()(const extent_score_info& lhs, const extent_score_info& rhs) const
    {
        return lhs.name() < rhs.name();
    }
};

/**
 * Functor used to sort extent scores by extent score.
 */
struct extent_score_sort {
    /**
     * Function call operator that compares 2 extent scores.
     *
     * @param lhs   Left hand side of '<' operator for comparison.
     * @param rhs   Right hand side of '<' operator for comparison.
     */
    bool operator()(const extent_score_info& lhs, const extent_score_info& rhs) const
    {
        return lhs.score() > rhs.score();
    }
};

/// Convenience type for a set of extent scores sorted by extent name.
using extent_scores_t = std::set<extent_score_info, extent_name_sort>;

/// Convenience type for a set of extent scores sorted by extent score.
using top_extent_scores_t = std::set<extent_score_info, extent_score_sort>;

#endif
