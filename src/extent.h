/**
 * @headerfile ""
 *
 * Define the extent base class.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _extent_h
#define _extent_h

#include <array>
#include <functional>
#include <memory>
#include <ostream>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeindex>

#include "debug_util.h"
#include "score_types.h"

#include "language_processor.h"

/**
 * hold state information for an extent of source code while that extent is
 * being processed.  This is useful for scoring things that may extend
 * across several lines.  It can also be useful for processing a statement
 * that maybe one of many on a single line.
 *
 * This is a base class that is meant to be sub-classed to track additional
 * information about a given extent.  All sub-classes are expected to provide
 * static factory method with the following prototye:
 *
 * @code
 *     std::tuple<extent_ptr, std::size> check_token(extent_processor& processor,
 *                                                   std::string_view line);
 * @endcode
 *
 * If the beginning of 'line' matches the extent then a valid extent_ptr will be
 * returned which can be pushed on to the extent stack and the number of
 * characters in the matching token will be return in the second value of the
 * std::<tuple>.  If the beginning of 'line' does not match then extent_ptr will
 * be nullptr and the second value will be 0.  A check_token operation may
 * identify characters that should be consumed without the need for creating an
 * extent.  In such cases, it will return a nullptr and the number of characters
 * processed.
 */
class extent {
  public:
    using post_process = std::function<void(void)>;
    using check_return = std::tuple<bool, std::size_t>;
    using process_return = std::tuple<std::size_t, post_process>;

    static check_return check_token(language_processor& processor[[maybe_unused]],
                                    std::string_view line[[maybe_unused]])
    {
        return { false, 0 };
    }

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     */
    extent(language_processor& processor, std::string&& name);

    /**
     * Constructor.
     *
     * @param name      Name given to the extent.  Typically the
     *                  class/struct or function name.
     */
    extent(language_processor& processor, const std::string& name);

    virtual ~extent() = default;

    extent(extent&& other) = default;
    extent& operator=(extent&& other) = default;

    /**
     * Update the extent's score with the given value.
     *
     * @param delta     Value to add to the score.
     */
    virtual void add_score(score_t delta)
    {
        _score += delta;
    }

    /**
     * Get the extent's name.
     *
     * @return  The extent's name.
     */
    virtual const std::string& name() const { return _name; }

    /**
     * Get the extent's score.
     *
     * @return  The extent's score.
     */
    auto score() const { return _score; }

    /**
     * Get the extent's starting line number.
     *
     * @return  The extent's starting line number.
     */
    auto start_line_num() const { return _start; }

    /**
     * Get the extent's ending line number.
     *
     * @return  The extent's ending line number.
     */
    auto end_line_num() const { return _end; }

    /**
     * Get the number of lines in the extent.
     *
     * @return  The number of lines in the extent.
     */
    virtual std::size_t line_count() const;

    /**
     * Processes the given line according to the current extent.  The number of
     * characters processed are returned.
     *
     * @param line  Source line to process.
     *
     * @return  The number of characters processed and a pointer to the post
     *          processing function.
     */
    virtual process_return process(std::string_view line[[maybe_unused]])
    {
        return { 0, nullptr };
    }


    void set_body_info(score_t body_score, std::size_t body_start, std::size_t body_end)
    {
        _has_body = true;
        _body_score = body_score;
        _body_start = body_start;
        _body_end = body_end;
    }

    void import_body_info(const std::tuple<bool, score_t, std::size_t, std::size_t>& bi)
    {
        std::tie(_has_body, _body_score, _body_start, _body_end) = bi;
    }

    auto body_info() const
    {
        return std::make_tuple(_has_body, _body_score, _body_start, _body_end);
    }

    extent() = delete;
    extent(const extent& other) = delete;
    extent& operator=(const extent& other) = delete;

  protected:
    language_processor& _processor;     ///< Extent processor reference.
    std::string _name;                  ///< Extent name.
    std::size_t _start;                 ///< Extent starting line number.
    std::size_t _end;                   ///< Extent ending line number.
    score_t _score;                     ///< Extent score.

    bool _has_body;
    score_t _body_score;
    std::size_t _body_start;
    std::size_t _body_end;

    virtual int line_count_adjust() const { return 0; }

    virtual void complete() { _end = _processor.get_line_number(); }
};



using extent_ptr = std::unique_ptr<extent>; ///< Convenience type alias for pointers to extents.

inline std::ostream& operator<<(std::ostream& os, const extent& e)
{
    return os << typeid(e) << " (" << e.name() << ": " << e.score() << ")";
}

inline std::ostream& operator<<(std::ostream& os, const extent_ptr& e)
{
    return os << *e;
}



#endif
