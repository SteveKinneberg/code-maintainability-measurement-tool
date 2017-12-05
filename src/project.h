/**
 * @headerfile ""
 *
 * Declare the project class.
 *
 * This provides a top-level container to collect all the extents as they are
 * processed for final analysis reporting.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _project_h
#define _project_h

#include <ostream>
#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

// #include "extent.h"
#include "filesystem.h"
#include "score_types.h"
#include "scoring_rules.h"
#include "source_processor.h"

/**
 * This collects the scores for all the extents and provides an overall
 * score for the given run of cmmt.
 */
class project {
  public:
    /** Project extent constructor. */
    project(): _report_all(true), _report_top_score(10), _project_total(0) {}

    /**
     * Project extent constructor.
     *
     * @param paths     List of files to process.
     */
    void process_files(const paths_t& paths);

    /**
     * Report the maintainability score  to the appropriate stream.
     *
     * @param os  The output stream to send scoring information to.
     */
    void report_text(std::ostream& os) const;

    /*
     * TODO: Future reporting facilities:
     *    void report_html() const;
     *    void report_markdown() const;
     *    void report_wiki() const;
     *    void report_json() const;
     *    void report_xml() const;
     */

  private:
    /** Convenience type alias for storing file data. */
    using file_data_t = std::map<std::string, std::string>;

    /** Convenience type alias for storing file scores. */
    using file_scores_t = std::map<std::string, score_t>;

    /** Convenience type alias for storing file scores sorted by score. */
    using top_file_scores_t = std::multimap<score_t, std::string, std::greater<score_t>>;

    /** Convenience type alias for storing line scores for each file. */
    using file_line_scores_t = std::map<std::string, line_scores_t>;

    bool _report_all;                   ///< Flag to indicate reporting all scores.
    std::uint32_t _report_top_score;    ///< How may of the top scores to report (if not all).

    /**
     * Source files and their data.  This provides the buffer storage for the
     * source file contents for analysis.
     */
    file_data_t _fdata;

    score_t _project_total;                     ///< Total score for the entire project.
    file_scores_t _file_scores;                 ///< Scores for each file.
    top_file_scores_t _top_file_scores;         ///< Scores for each file sorted by score.
    file_line_scores_t _line_scores;            ///< Scores for each line in all files.
    extent_scores_t _class_scores;              ///< Scores for each class.
    top_extent_scores_t _top_class_scores;      ///< Scores for each class sorted by score.
    extent_scores_t _function_scores;           ///< Scores for each function.
    top_extent_scores_t _top_function_scores;   ///< Scores for each function sorted by score.

    scoring_rules _scoring_rules;
};


#endif
