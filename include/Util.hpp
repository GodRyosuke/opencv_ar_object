#pragma once

#include <utility>
#include <iostream>

namespace Util {
void Print() {}
template <class Head, class... Tail>
void Print(Head&& arg1, Tail&&... arg_end)
{
#ifndef NDEBUG
    std::cout << arg1;
    Print(std::forward<Tail>(arg_end)...);
#endif
}
}