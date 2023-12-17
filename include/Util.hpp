#pragma once

#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/postprocess.h> // Post processing flags

#include "Shader.hpp"

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

template <typename ... Args>
void Printf(const char *format, Args const & ... args) {
#ifndef NDEBUG
    printf(format, args ...);
#endif
}

glm::mat4 ToGlmMat4(aiMatrix4x4& aiMat);
glm::mat4 ToGlmMat4(aiMatrix3x3& aiMat);
std::vector<std::string> Split(std::string src, std::string split_str);

}
