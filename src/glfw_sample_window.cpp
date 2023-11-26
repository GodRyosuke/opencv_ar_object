#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "opencv2/opencv.hpp"
#include <opencv2/aruco/charuco.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
// #include <opencv2/aruco.hpp>

#include "Definitions.hpp"

namespace {
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
    
void glfwsample()
{
    if (!glfwInit())
    {
        // Initialization failed
        printf("failed to initialize glfw\n");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    const int screenWidth = 640;
    const int screenHeight = 480;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "wander", NULL, NULL);
    if (!window)
    {
        printf("error: failed to crate glfw window\n");
        return;
        // Window or OpenGL context creation failed
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

    const aiScene* m_pScene;
    // Assimp::Importer m_Importer;    // Importer保持せんかったら、Sceneも保持できない!!

    glfwSetKeyCallback(window, key_callback);

    GLuint vertex_buffer = 0;
    glGenBuffers(1, &vertex_buffer);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0, 0.5, 0.7, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, screenWidth, screenHeight);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

}
}

int main(int argc, char* argv[])
{
    std::cout << "abcd" << std::endl;
    // gladLoadGL();


	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	// glGetError();
    glfwsample();
    
    
    
    // int xpos, ypos;
    // glfwGetMousePos(&xpos, &ypos);
    return 0;
}