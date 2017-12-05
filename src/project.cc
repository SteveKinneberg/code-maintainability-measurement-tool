/**
 * @file ""
 *
 * Project class method implementations.
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "project.h"

#include <exception>
#include <iomanip>
#include <fstream>
#include <regex>

#include "filesystem.h"
#include "source_processor.h"

#include <iostream>
#ifndef SJK
#define SJK std::cout << "SJK - " << __FUNCTION__ << ": " << __LINE__ << ": "
#endif

void project::process_files(const paths_t& paths)
{
    for (auto& path: paths) {
        auto filename = path.string();
        auto source_data = read_file(filename);

        auto processor = source_processor(_scoring_rules, std::move(filename), std::move(source_data));

        if (processor) {
            processor.analyze();
            _fdata.emplace(filename, std::move(source_data)).first;

            auto file_score = processor.file_score();
            _file_scores.emplace(filename, file_score);
            _top_file_scores.emplace(file_score, filename);

            _line_scores.emplace(filename, std::move(processor.line_scores()));

            _class_scores.merge(processor.class_scores());
            _top_class_scores.merge(processor.top_class_scores());

            _function_scores.merge(processor.function_scores());
            _top_function_scores.merge(processor.top_function_scores());

            _project_total += file_score;
        }
    }
}


void project::report_text(std::ostream& os) const
{
    const auto score_width = std::max(std::to_string(_project_total).length(),
                                      std::string_view("score").length());


    os << std::setw(score_width) << "Score" << " | "<< "Files" << std::endl
       << "========================================" << std::endl;
    for (const auto& [filename, score]: _file_scores) {
        const auto& lines = _line_scores.at(filename);
        for(const auto& [l, s]: lines) {
            os << std::setw(score_width) << s << " | " << l << std::endl;
        }
        os << "----------------------------------------" << std::endl
           << std::setw(score_width) << score << " | " << filename << std::endl
           << "========================================" << std::endl;
    }

    os << std::setw(score_width) << "Score" << " | "<< "Struct/Classes" << std::endl
       << "----------------------------------------" << std::endl;
    for (const auto& score_info: _class_scores) {
        os << std::setw(score_width) << score_info.score() << " | "
           << score_info.name() << "    ("
           << score_info.filename() << ":"
           << score_info.start_line() << ")" << std::endl;
    }

    os << "========================================" << std::endl
       << std::setw(score_width) << "Score" << " | "<< "Functions" << std::endl
       << "----------------------------------------" << std::endl;
    for (const auto& score_info: _function_scores) {
        os << std::setw(score_width) << score_info.score() << " | "
           << score_info.name() << "    ("
           << score_info.filename() << ":"
           << score_info.start_line() << ")" << std::endl;
    }

    os << "========================================" << std::endl
       << std::setw(score_width) << _project_total << " | Project Total" << std::endl;
}

