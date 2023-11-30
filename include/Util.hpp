// #pragma once
#ifndef UTIL_H_
#define UTIL_H_

#include <utility>
#include <iostream>
#include <cassert>

namespace Util {
inline void Print() {}
template <class Head, class... Tail>
inline void Print(Head&& arg1, Tail&&... arg_end)
{
#ifndef NDEBUG
    std::cout << arg1;
    Print(std::forward<Tail>(arg_end)...);
#endif
}
}

#endif