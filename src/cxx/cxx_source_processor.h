#ifndef _cxx_cxx_source_processor_h
#define _cxx_cxx_source_processor_h
/**
 * @file
 *
 * Declare the C/C++ source file processor class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "../language_processor.h"


namespace cxx {

/**
 * Implementation of `language_processor` for C/C++ code.
 */
class cxx_source_processor: public language_processor {
  public:
    /**
     * Check if this source processor can analyze the specified file.  This
     * implementation currently only checks file name extensions.
     *
     * @param filename      Name of the file.
     * @param source_data   Contents of the file.
     */
    static std::int32_t file_check(std::string_view filename, std::string_view source_data);

    /**
     * Constructor.
     *
     * @param rules         Scoring rules for the language being processed.
     * @param filename      Name of the file to be processed.
     * @param source_data   Contents of the file to be processed.
     */
    cxx_source_processor(const scoring_rules& rules,
                         std::string&& filename,
                         std::string&& source_data):
        language_processor(rules, std::move(filename), std::move(source_data))
    {
        _namespace.push(""); // Initialize with global namespace.
    }

    /** Destructor. */
    ~cxx_source_processor() final;

    /**
     * Move constructor.
     *
     * @param other     Reference to the source instance to move.
     */
    cxx_source_processor(cxx_source_processor&& other) = default;

    /**
     * Move assignment operator.
     *
     * @param other     Reference to the source instance to move.
     *
     * @return  Reference to '*this'.
     */
    cxx_source_processor& operator=(cxx_source_processor&& other) = default;

    /**
     * Get the current namespace.
     *
     * @return  The current namespace.
     */
    const std::string& current_namespace() const { return _namespace.top(); }

    /**
     * Add a namespace to the namespace stack.
     *
     * @param name      Namespace to push onto the namespace stack.
     */
    void push_namespace(std::string&& name) { _namespace.emplace(std::move(name)); }

    /** Remove the top namespace from the namespace stack. */
    void pop_namespace() { _namespace.pop(); }

    cxx_source_processor() = delete;
    cxx_source_processor(const cxx_source_processor&) = delete;
    cxx_source_processor& operator=(const cxx_source_processor&) = delete;

  private:
    std::stack<std::string> _namespace;     ///< The namespace stack.
    std::string_view _line_data;            ///< The current line being processed.

    std::map<std::string_view, std::size_t> gotos;      ///< Set of goto targets and the goto line number.
    std::map<std::string_view, std::size_t> labels;     ///< Set of labels an their line number.

    /**
     * Process a line of C/C++ code.
     *
     * @param line      String view of the line to process.
     *
     * @return  The number of unprocessed characters.  Any value greater than 0
     *          indicates an error and where processing stopped.
     */
    std::size_t process_line(std::string_view line) final;
};

}
#endif
