/**
 * @headerfile ""
 *
 * Define the score type and various scoring containers.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _score_types_processor_h
#define _score_types_processor_h

#include <set>
#include <string>
#include <string_view>
#include <vector>

using score_t = std::uint32_t;
using line_score_t = std::pair<std::string_view, score_t>;
using line_scores_t = std::vector<line_score_t>;

class extent_score_info {
  public:
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

    std::string_view name() const { return _name; }
    std::string_view filename() const { return _filename; }
    std::size_t start_line() const { return _start_line; }
    std::size_t end_line() const { return _end_line; }
    score_t score() const { return _score; }

  private:
    const std::string _name;
    const std::string _filename;
    const std::size_t _start_line;
    const std::size_t _end_line;
    const score_t _score;

};

struct extent_name_sort {
    bool operator()(const extent_score_info& lhs, const extent_score_info& rhs) const
    {
        return lhs.name() < rhs.name();
    }
};

struct extent_score_sort {
    bool operator()(const extent_score_info& lhs, const extent_score_info& rhs) const
    {
        return lhs.score() > rhs.score();
    }
};

using extent_scores_t = std::set<extent_score_info, extent_name_sort>;
using top_extent_scores_t = std::set<extent_score_info, extent_score_sort>;

#endif
