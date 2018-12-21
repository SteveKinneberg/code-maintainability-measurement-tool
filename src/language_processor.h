#ifndef _language_processor_h
#define _language_processor_h
/**
 * @file
 *
 * Declare the source file processor class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <cassert>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <type_traits>

#include "scoring_rules.h"

#include "debug_util.h"

class extent;
// #include "extent.h"

class language_processor;
using language_processor_ptr = std::unique_ptr<language_processor>;

/**
 * Base class that provides the interface for analyzing a single source code
 * file.
 */
class language_processor {
  public:
    /**
     * Constructor.
     *
     * @param filename      Name of the file to be processed.
     * @param source_data   Contents of the file to be processed.
     */
    language_processor(const scoring_rules& rules,
                       std::string&& filename,
                       std::string&& source_data);

    /** Destructor. */
    virtual ~language_processor();

    void analyze();

    const auto& filename() const { return _filename; }
    const auto& source_data() const { return _source_data; }
    const auto& rules() const { return _scoring_rules; }

    auto file_score() const { return _file_score; }
    auto line_scores() { return std::move(_line_scores); }
    auto class_scores() { return std::move(_class_scores); }
    auto top_class_scores() { return std::move(_top_class_scores); }
    auto function_scores() { return std::move(_function_scores); }
    auto top_function_scores() { return std::move(_top_function_scores); }


    void set_file_score(score_t score) { _file_score = score; }
    void add_line(std::string_view line)
    {
        _line_scores.emplace_back(std::make_pair(line, 1));
    }
    auto get_line_count() const { return _line_scores.size(); }
    auto get_line_number() const { return get_line_count() - 1; }
    auto line_score(std::size_t line_num) const
    {
        return _line_scores[line_num].second;
    }
    void add_line_score(score_t score, std::size_t line_num)
    {
        _line_scores[line_num].second += score;
    }
    void add_line_scores(score_t score, std::size_t start, std::size_t end)
    {
        while (start < end) {
            _line_scores[start++].second += score;
        }
    }


    score_t score_line_range(std::size_t start, std::size_t end)
    {
        return std::accumulate(std::next(_line_scores.begin(), start),
                               std::next(_line_scores.begin(), end),
                               0,
                               [] (score_t a, const line_score_t& s) {
                                   return a + s.second;
                               });
    }

    void add_class_score(const std::string& name,
                         std::size_t start,
                         std::size_t end,
                         score_t score);
    void add_function_score(const std::string& name,
                            std::size_t start,
                            std::size_t end,
                            score_t score);

    extent& get_current_extent();

    template<class T>
    void push_extent(T&& e){
        static_assert(std::is_base_of<extent, T>::value);
        _process_extents.emplace(std::make_unique<T>(std::move(e)));
    }


    void pop_extent(bool propogate_score = false);



    language_processor() = delete;
    language_processor(const language_processor&) = delete;
    language_processor& operator=(const language_processor&) = delete;

  protected:
    using extent_ptr = std::unique_ptr<extent>;
    std::stack<extent_ptr> _process_extents;    ///< Collection of partially processed extents.

    /**
     * Utility function for checking if the file extension matches one of the
     * entries in the given list.  Each entry in the list must begin with ".".
     *
     * @param ext_list  An std::initializer_list<> with all the extensions to check.
     *
     * @return  A value to be added to the overall confidence computation
     *          indicating whether an extension matches or not:
     *            -   0: The file name does not have an extension.
     *            -  50: The file name extension matches an entry in the list.
     *            - -25: The file name extension does not match any entry in the list.
     */
    static std::int32_t extension_match(std::string_view filename,
                                        const std::initializer_list<const char* const>& ext_list);

    virtual std::size_t process_line(std::string_view line) = 0;


  private:
    const std::string& _filename;               ///< Name of the file being processed.
    const std::string& _source_data;            ///< Contents of the file being processed.
    const scoring_rules& _scoring_rules;        ///< Scoring rule table

    score_t _file_score;                        ///< Total score for the entire file.
    line_scores_t _line_scores;                 ///< Scores for each line in the file.

    extent_scores_t _class_scores;              ///< Scores for each class in the file.
    top_extent_scores_t _top_class_scores;      ///< Scores for each class in the file sorted by score.

    extent_scores_t _function_scores;           ///< Scores for each function in the file.
    top_extent_scores_t _top_function_scores;   ///< Scores for each function in the file sorted by score.

};

#endif
