/**
 * @headerfile ""
 *
 * Abstract support for C++17 std::filesystem by bringing boost::filesystem into
 * the std namespace if USE_BOOST_FS is defined.
 *
 * Copyright 2017 Steve Kinneberg <steve.kinneberg@gmail.com>
 */
#ifndef _array_h
#define _array_h

#include "config.h"

#if defined(HAVE_STD__EXPERIMENTAL__MAKE_ARRAY)

#include <experimental/array>

namespace std {
using namespace experimental;
}


#elif !defined(HAVE_STD__MAKE_ARRAY)

#include <type_traits>

/*
 * NOTE:
 *
 * The following implementation of make_array() was taken from cppreference.com
 * under the terms of the Creative Commons Attribution-Sharealike 3.0 license
 * (CC-BY-SA).  License terms can be found here:
 * http://en.cppreference.com/w/Cppreference:Copyright/CC-BY-SA
 *
 * This code was adapted from the code found at this page:
 * http://en.cppreference.com/w/cpp/experimental/make_array
 */
namespace std {
namespace details {
template<class> struct is_ref_wrapper : std::false_type {};

template<class T> struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

template<class T>
using not_ref_wrapper = std::negation<is_ref_wrapper<std::decay_t<T>>>;

template <class D, class...> struct return_type_helper { using type = D; };

template <class... Types>
struct return_type_helper<void, Types...> : std::common_type<Types...> {
    static_assert(std::conjunction_v<not_ref_wrapper<Types>...>,
                  "Types cannot contain reference_wrappers when D is void");
};

template <class D, class... Types>
using return_type = std::array<typename return_type_helper<D, Types...>::type,
                               sizeof...(Types)>;
}

template < class D = void, class... Types>
constexpr details::return_type<D, Types...> make_array(Types&&... t) {
    return {std::forward<Types>(t)... };
}

}

#endif

#endif
