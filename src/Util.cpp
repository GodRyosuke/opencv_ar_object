#include "Util.hpp"

namespace Util {
glm::mat4 ToGlmMat4(aiMatrix4x4& aiMat)
{
    return glm::transpose(glm::make_mat4(&aiMat.a1));
}

glm::mat4 ToGlmMat4(aiMatrix3x3& aiMat)
{
    return glm::transpose(glm::make_mat3(&aiMat.a1));
}

std::vector<std::string> Split(std::string src, std::string split_str)
{
    // std::string replaced_path = std::regex_replace(path, std::regex("\\\\"), "/");
    size_t pos = src.find(split_str);
    size_t last_pos = 0;
    std::string sub_path = src;
    std::vector<std::string> dir_names;
    while (1) {
        if (pos == std::string::npos) {
            dir_names.push_back(sub_path);
            break;
        }
        std::string dir_name = sub_path.substr(0, pos);
        sub_path = sub_path.substr(pos + 1);
        // last_pos = pos;
        pos = sub_path.find(split_str);
        dir_names.push_back(dir_name);
    }

    return dir_names;
}
}