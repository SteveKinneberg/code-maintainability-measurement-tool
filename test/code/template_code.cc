#include <type_traits>

template<template<class> class T1, class T2, int V1, class T4>
class X {
    class XX {

    };
};

template<class T, int S,
         typename std::enable_if<
             std::is_trivially_destructible<T>{} &&
             (S > 10) &&
             (std::is_class<T>{} || std::is_union<T>{}),
         int>::type = 0>
class Y1 {
};

template<class T, int S,
         typename std::enable_if<
             std::is_trivially_destructible<T>{} &&
             (S < 10) &&
             (std::is_class<T>{} || std::is_union<T>{}),
         int>::type = 0>
class Y2 {
};

template<class T, int S,
         typename std::enable_if<
             (std::is_trivially_destructible<T>{} &&
              (S > 10) &&
              (std::is_class<T>{} || std::is_union<T>{})),
         int>::type = 0>
class Z1 { class ZZ1 {

}; };

template<class T, int S,
         typename std::enable_if<
             (std::is_trivially_destructible<T>{} &&
              (S < 10) &&
              (std::is_class<T>{} || std::is_union<T>{})),
         int>::type = 0>
class Z2 {
};



/*
 * if close_paren
 *     while context == open_angle
 *         open_angle is less_than operator
 *         pop context until context == open_paren
 *
 * if close_angle and context = open_paren
 *     close_angle is greater_than operator
 */


template<class T, int S,
         typename std::enable_if<
             (std::is_trivially_destructible<typename std::enable_if<(S > 10), T>::type>{} &&
              (S < 10) &&
              (std::is_class<T>{} || std::is_union<T>{})),
                 int>::type = 0>
class QQQ {
};


template<>
class WWW<int> {};
