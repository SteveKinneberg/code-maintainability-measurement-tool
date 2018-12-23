/**
 * @file ""
 *
 * Extent file processor class method implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "language_processor.h"

#include <cassert>

#include "extent.h"
#include "extent_file.h"
#include "processing_exception.h"

#include "debug_util.h"


language_processor::language_processor(const scoring_rules& rules,
                                       std::string&& filename,
                                       std::string&& source_data):
    _filename(std::move(filename)),
    _source_data(std::move(source_data)),
    _scoring_rules(rules),
    _file_score(0)
{
    push_extent(extent_file(*this, _filename));
}

language_processor::~language_processor()
{
    _process_extents.pop();
}

std::int32_t language_processor::extension_match(std::string_view filename,
                                                 const std::initializer_list<const char* const>& ext_list)
{
    auto ext_start = filename.rfind('.');
    if (ext_start == filename.npos) {
        return 0;
    }

    auto filename_extension = std::string_view(filename.data() + ext_start,
                                               filename.length() - ext_start);

    if (std::any_of(ext_list.begin(), ext_list.end(),
                    [&file_ext = filename_extension](std::string_view e) {
                        return file_ext == e;
                    })) {
        return 50;
    }

    return -25;
}



void language_processor::add_class_score(const std::string& name,
                                         std::size_t start,
                                         std::size_t end,
                                         score_t score)
{
    score += score_line_range(start, end + 1);

    _class_scores.emplace(std::move(extent_score_info(name,
                                                      _filename,
                                                      start,
                                                      end,
                                                      score)));
    _top_class_scores.emplace(std::move(extent_score_info(name,
                                                          _filename,
                                                          start,
                                                          end,
                                                          score)));
}

void language_processor::add_function_score(const std::string& name,
                                            std::size_t start,
                                            std::size_t end,
                                            score_t score)
{
    score += score_line_range(start, end + 1);

    _function_scores.emplace(std::move(extent_score_info(name,
                                                         _filename,
                                                         start,
                                                         end,
                                                         score)));
    _top_function_scores.emplace(std::move(extent_score_info(name,
                                                             _filename,
                                                             start,
                                                             end,
                                                             score)));
}


extent& language_processor::get_current_extent()
{
    assert(!_process_extents.empty());
    return *_process_extents.top();
}

void language_processor::pop_extent(bool propogate_score)
{
    assert(!_process_extents.empty());

    auto& e = get_current_extent();
    auto score = e.score();

    if (propogate_score) {
        _process_extents.pop();
        get_current_extent().add_score(score);
    } else {
        add_line_score(score, e.start_line_num());
        _process_extents.pop();
    }
}


void language_processor::analyze()
{
    auto source = std::string_view(_source_data);

    while (!source.empty()) {
        auto line_end = source.find("\n");
        auto line = source.substr(0, line_end);
        // std::cout << "\n------------------------------------------------------------\n"
        //           << line
        //           << "\n------------------------------------------------------------\n";

        add_line(line);

        if (auto remainder = process_line(line);
            remainder > 0) {
            throw processing_exception("parsing error while processing " + get_current_extent().name(),
                                       filename(), get_line_number() + 1,
                                       line, line.length() - remainder);
        }

        source.remove_prefix(line.length() + 1);

        // SJK << get_current_extent().name()
        //     << " | " << std::setw(5) << get_current_extent().score()
        //     << " | " << _line_data << std::endl;

    }
    while (typeid(get_current_extent()) != typeid(extent_file)) {
        // TODO: penalize "incomplete" files
        pop_extent(true);
    }


    auto sc = score_line_range(0, get_line_count());
    // SJK << "total line score: " << sc << std::endl;
    // SJK << "adding score for all the lines of code in the file: " << sc << std::endl;
    get_current_extent().add_score(sc);

    set_file_score(get_current_extent().score());
}

