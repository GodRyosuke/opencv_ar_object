#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace InputEvent {
struct Data {
    GLFWwindow* window;
    const int key;
    const int scancode;
    const int action;
    const int mods;
};
}