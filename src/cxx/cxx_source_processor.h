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

class cxx_source_processor: public language_processor {
  public:
    static std::int32_t file_check(std::string_view filename, std::string_view source_data);

    cxx_source_processor(const scoring_rules& rules,
                         std::string&& filename,
                         std::string&& source_data):
        language_processor(rules, std::move(filename), std::move(source_data))
    {
        _namespace.push(""); // global namespace
    }

    ~cxx_source_processor() final;

    cxx_source_processor(cxx_source_processor&&) = default;
    cxx_source_processor& operator=(cxx_source_processor&&) = default;

    const std::string& current_namespace() const { return _namespace.top(); }

    void push_namespace(std::string&& name) { _namespace.emplace(std::move(name)); }
    void pop_namespace() { _namespace.pop(); }

    cxx_source_processor() = delete;
    cxx_source_processor(const cxx_source_processor&) = delete;
    cxx_source_processor& operator=(const cxx_source_processor&) = delete;

  private:
    std::stack<std::string> _namespace;
    std::string_view _line_data;

    std::map<std::string_view, std::size_t> gotos;
    std::map<std::string_view, std::size_t> labels;

    std::size_t process_line(std::string_view line) final;
};

}

#endif
