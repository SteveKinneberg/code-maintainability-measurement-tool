#ifndef _source_processor_h
#define _source_processor_h
/**
 * @file
 *
 * Declare the source file processor class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include <memory>
#include <stack>
#include <string>
#include <string_view>

#include "scoring_rules.h"


class language_processor;
/// Convenience type for a pointer to a language processor.
using language_processor_ptr = std::unique_ptr<language_processor>;

class source_processor;
/// Convenience type for a pointer to a source processor.
using source_processor_ptr = std::unique_ptr<source_processor>;


/**
 * Base class that provides the interface for analyzing a single source code
 * file.
 */
class source_processor {
  public:
    /**
     * Constructor.
     *
     * @param rules         Scoring rules for the source being processed.
     * @param filename      Name of the file to be processed.
     * @param source_data   Contents of the file to be processed.
     */
    source_processor(const scoring_rules& rules,
                     std::string&& filename,
                     std::string&& source_data);

    /** Destructor. */
     ~source_processor();

    /** Default move constructor. */
    source_processor(source_processor&& other) = default;

    /** Default move assignment operator. */
    source_processor& operator=(source_processor&& other) = default;

    /**
     * Analyze the file contents.  Results are stored in the various scoring
     * member variables of *this class.
     */
    void analyze();

    /**
     * Report the total score for the entire file.
     *
     * @return  The total score for the entire file.
     */
    score_t file_score() const;

    /**
     * Report the scores for each line in the file.
     *
     * @return  The scores for each line in the file.
     */
    line_scores_t line_scores();

    /**
     * Report the scores for each class in the file sorted by class name.
     *
     * @return  The scores for each class in the file.
     */
    extent_scores_t class_scores();

    /**
     * Report the scores for each class in the file sorted by score rather than
     * by class name.
     *
     * @return  The scores for each class in the file.
     */
    top_extent_scores_t top_class_scores();

    /**
     * Report the scores for each function in the file sorted by function name.
     *
     * @return  The scores for each function in the file.
     */
    extent_scores_t function_scores();

    /**
     * Report the scores for each function in the file sorted by score rather
     * than by class name.
     *
     * @return  The scores for each function in the file.
     */
    top_extent_scores_t top_function_scores();

    /**
     * Indicate if '*this' has an implementation when used in any bool context.
     */
    operator bool() const { return bool(_processor_impl); }

    source_processor() = delete;
    source_processor(const source_processor&) = delete;
    source_processor& operator=(const source_processor&) = delete;

  private:
    language_processor_ptr _processor_impl;     ///< Pointer to the actual processor implementation.
};
#endif
