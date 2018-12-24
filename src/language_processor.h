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
/// Convenience type for a pointer to an extent.
using extent_ptr = std::unique_ptr<extent>;

class language_processor;
/// Convenience type for a pointer to a language processor.
using language_processor_ptr = std::unique_ptr<language_processor>;

/**
 * Base class that provides the interface for analyzing a single source code
 * file.
 *
 * Every sub-class must define a static method 'file_check' that takes a
 * filename and source data to compute a confidence level that the language
 * processor can successfully compute a score.  The prototype for that static
 * method is as follows:
 *
 * @code
 * static std::int32_t file_check(std::string_view filename, std::string_view source_data);
 * @endcode
 *
 * Both the file name and contents are provided.  At the very minimum,
 * implementations of 'file_check()' should call 'extension_match()'.  A more
 * advanced implementation can look at the file contents for keywords or
 * patterns that are commonly found in files the parser understands.  Advanced
 * analysis should also include negative checks for elements that are know to
 * not be part of the language.  This helps in disambiguating between similar
 * languages.
 */
class language_processor {
  public:
    /**
     * Constructor.
     *
     * @param rules         Scoring rules for the language being processed.
     * @param filename      Name of the file to be processed.
     * @param source_data   Contents of the file to be processed.
     */
    language_processor(const scoring_rules& rules,
                       std::string&& filename,
                       std::string&& source_data);

    /** Destructor. */
    virtual ~language_processor();

    /**
     * Analyze the source code and evaluate the scoring.
     */
    void analyze();

    /**
     * Get the name of the file being processed.
     *
     * @return  The name of the file being processed.
     */
    const auto& filename() const { return _filename; }

    /**
     * Get the source to be processed.
     *
     * @return  The source to be processed.
     */
    const auto& source_data() const { return _source_data; }

    /**
     * Get the scoring rules for the file being processed.
     *
     * @return  The scoring rules for the file being processed.
     */
    const auto& rules() const { return _scoring_rules; }

    /**
     * Get score for the file.
     *
     * @return  The score for the file.
     */
    auto file_score() const { return _file_score; }

    /**
     * Get score for each line in the file.
     *
     * @return  The score for each line in the file.
     */
    auto line_scores() { return move_scores(_line_scores); }

    /**
     * Get score for each class/struct in the file.
     *
     * @return  The score for each class/struct in the file.
     */
    auto class_scores()  { return move_scores(_class_scores); }

    /**
     * Get score for highest scoring class/struct in the file.
     *
     * @return  The score for each class/struct in the file.
     */
    auto top_class_scores() { return move_scores(_top_class_scores); }

    /**
     * Get score for each function in the file.
     *
     * @return  The score for each function in the file.
     */
    auto function_scores() { return move_scores(_function_scores); }

    /**
     * Get score for highest scoring function in the file.
     *
     * @return  The score for each function in the file.
     */
    auto top_function_scores() { return move_scores(_top_function_scores); }

    /**
     * Set the score for the file.
     *
     * @param score     The file score.
     */
    void set_file_score(score_t score) { _file_score = score; }

    /**
     * Add a line for the line scoring.
     *
     * @param line      The line of source being added.
     */
    void add_line(std::string_view line)
    {
        _line_scores.emplace_back(std::make_pair(line, 1));
    }

    /**
     * Get the number of lines added for the source file.
     *
     * @return  Number of lines in the source file.
     */
    auto get_line_count() const { return _line_scores.size(); }

    /**
     * Get the line number of the last line to be added.
     *
     * @return  Line number of the last line added.
     */
    auto get_line_number() const { return get_line_count() - 1; }

    /**
     * Get the score for the specified line.
     *
     * @param line_num      Line number to get the score for.
     *
     * @return  Score for the given line number.
     */
    auto line_score(std::size_t line_num) const
    {
        return _line_scores[line_num].second;
    }

    /**
     * Add points to a line of source code.
     *
     * @param score     Points to add to the specified line.
     * @param line_num  Line number to add points to.
     */
    void add_line_score(score_t score, std::size_t line_num)
    {
        _line_scores[line_num].second += score;
    }

    /**
     * Add points to a rang of lines.
     *
     * @param score     Points to add to the specified line.
     * @param start     Starting line number to add points to.
     * @param end       One past the last line number to add points to.
     */
    void add_line_scores(score_t score, std::size_t start, std::size_t end)
    {
        while (start < end) { _line_scores[start++].second += score; }
    }

    /**
     * Get the total number of points for range of lines.
     *
     * @param start     Starting line number for computing the score.
     * @param end       One past the last line number for computing the score.
     */
    score_t score_line_range(std::size_t start, std::size_t end)
    {
        return std::accumulate(std::next(_line_scores.begin(), start),
                               std::next(_line_scores.begin(), end),
                               0,
                               [] (score_t a, const line_score_t& s) {
                                   return a + s.second;
                               });
    }

    /**
     * Add a class with its score and line range.
     *
     * @param name      Name of the class to add.
     * @param start     Starting line of the class.
     * @param end       One past the last line of the class.
     * @param score     The score for the class.
     */
    void add_class_score(const std::string& name,
                         std::size_t start,
                         std::size_t end,
                         score_t score);

    /**
     * Add a function with its score and line range.
     *
     * @param name      Name of the function to add.
     * @param start     Starting line of the class.
     * @param end       One past the last line of the class.
     * @param score     The score for the class.
     */
    void add_function_score(const std::string& name,
                            std::size_t start,
                            std::size_t end,
                            score_t score);

    /**
     * Get a reference to the current extent.
     *
     * @return  A reference to the current extent.
     */
    extent& get_current_extent();

    /**
     * Push the next extent to be processed on to the extent stack.
     *
     * @tparam T    Extent type being pushed.
     *
     * @param e     The extent being pushed.
     */
    template<class T>
    void push_extent(T&& e){
        static_assert(std::is_base_of<extent, T>::value);
        _process_extents.emplace(std::make_unique<T>(std::move(e)));
    }

    /**
     * Remove the current extent from the extent stack while optionally
     * propagating the current extent's score to the next extent on the stack.
     *
     * @param propogate_score   Indicates if score should be propagated. (Default: false)
     */
    void pop_extent(bool propogate_score = false);

    language_processor() = delete;
    language_processor(const language_processor&) = delete;
    language_processor& operator=(const language_processor&) = delete;

  protected:
    std::stack<extent_ptr> _process_extents;    ///< Collection of partially processed extents.

    /**
     * Utility function for checking if the file extension matches one of the
     * entries in the given list.  Each entry in the list must begin with ".".
     *
     * @param filename      The file name to check.
     * @param ext_list      An std::initializer_list<> with all the extensions to check.
     *
     * @return  A value to be added to the overall confidence computation
     *          indicating whether an extension matches or not:
     *            -   0: The file name does not have an extension.
     *            -  50: The file name extension matches an entry in the list.
     *            - -25: The file name extension does not match any entry in the list.
     */
    static std::int32_t extension_match(std::string_view filename,
                                        const std::initializer_list<const char* const>& ext_list);

    /**
     * Abstract method to actually process a line of source.  Language specific
     * sub-classes, must implement this.
     *
     * @param line      String view of the line to process.
     *
     * @return  The number of unprocessed characters.  Any value greater than 0
     *          indicates an error and where processing stopped.
     */
    virtual std::size_t process_line(std::string_view line) = 0;


  private:
    /**
     * Helper template function to efficiently move score containers.
     *
     * @tparam T        Type of the score container.
     *
     * @param from      Score container to move from.
     *
     * @return  A newly instantiated 'T' containing the scores from 'from'.
     */
    template <typename T> T move_scores(T& from)
    {
        T to{};
        std::swap(from, to);
        return to;
    }

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
