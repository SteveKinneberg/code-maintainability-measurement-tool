/**
 * @file ""
 *
 * Source file processor class method implementations.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */

#include "source_processor.h"

#include "language_processor.h"
#include "cxx/cxx_source_processor.h"
#include "extent.h"

using check_file_fn = std::int32_t(*)(std::string_view, std::string_view);
using processor_factory = language_processor_ptr(*)(const scoring_rules& rules,
                                                    std::string&& filename,
                                                    std::string&& source_data);
using check_factory_t = std::pair<check_file_fn, processor_factory>;

template<class T>
language_processor_ptr factory(const scoring_rules& rules,
                               std::string&& filename,
                               std::string&& source_data)
{
    return std::move(std::make_unique<T>(rules, std::move(filename), std::move(source_data)));
}

template<class T>
constexpr check_factory_t check_factory()
{
    return { T::file_check, factory<T> };
}


static constexpr auto processor_factories = {
    check_factory<cxx::cxx_source_processor>(),
};





language_processor_ptr create_processor(const scoring_rules& rules,
                                        std::string&& filename,
                                        std::string&& source_data)
{
    processor_factory factory = nullptr;

    std::int32_t max_confidence = 0;
    for (auto& p: processor_factories) {
        auto& check_file = std::get<0>(p);
        auto confidence = check_file(filename, source_data);
        if (confidence > max_confidence) {
            max_confidence = confidence;
            factory = std::get<1>(p);
        }
    }

    if (factory) {
        return factory(rules, std::move(filename), std::move(source_data));
    }
    return nullptr;
}


source_processor::source_processor(const scoring_rules& rules,
                                   std::string&& filename,
                                   std::string&& source_data):
    _processor_impl(create_processor(rules, std::move(filename), std::move(source_data)))
{}


source_processor::~source_processor()
{}

void source_processor::analyze()
{
    _processor_impl->analyze();
}

score_t source_processor::file_score() const
{
    return _processor_impl->file_score();
}

line_scores_t source_processor::line_scores()
{
    return std::move(_processor_impl->line_scores());
}

extent_scores_t source_processor::class_scores()
{
    return std::move(_processor_impl->class_scores());
}

top_extent_scores_t source_processor::top_class_scores()
{
    return std::move(_processor_impl->top_class_scores());
}

extent_scores_t source_processor::function_scores()
{
    return std::move(_processor_impl->function_scores());
}

top_extent_scores_t source_processor::top_function_scores()
{
    return std::move(_processor_impl->top_function_scores());
}
