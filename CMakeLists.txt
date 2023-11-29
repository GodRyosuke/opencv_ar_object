cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project ("opencv_ar_object" VERSION 0.0)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    if(CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(CEF_PLATFORM "linux64")
    else()
        set(CEF_PLATFORM "linux32")
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    if(CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(CEF_PLATFORM "windows64")
    else()
        set(CEF_PLATFORM "windows32")
    endif()
endif()

# if(MSVC)
#     set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   /DBUILD_TYPE=\\\"Debug\\\"")
#     set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DBUILD_TYPE=\\\"Release\\\"")
# else()
#     set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -DBUILD_TYPE=\\\"Debug\\\"")
#     set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DBUILD_TYPE=\\\"Release\\\"")
# endif()

# third party libraries
if (MSVC)
    set (glfw3_DIR "${PROJECT_SOURCE_DIR}/dependencies/glfw/lib/cmake/glfw3")
    set (OpenCV_DIR "${PROJECT_SOURCE_DIR}/dependencies/opencv/${CMAKE_BUILD_TYPE}")
endif()
# glfw
find_package(glfw3 CONFIG REQUIRED)
if (MSVC)
    set (glfw3_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/dependencies/glfw/include")
    set (glfw3_LIBRARIES "${PROJECT_SOURCE_DIR}/dependencies/glfw/lib/glfw3.lib")
else()
    set (glfw3_LIBRARIES GL glfw)
endif()
# opencv
find_package(OpenCV CONFIG REQUIRED)
# assimp
find_package(assimp CONFIG REQUIRED)
#glad
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/include)
# glm
set (GLM_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/dependencies/glm)

# cmake configurations
set (BUILD_PATH "${PROJECT_SOURCE_DIR}/build")
set (ASSET_PATH "${PROJECT_SOURCE_DIR}/assets/")
set (SHADER_PATH "${PROJECT_SOURCE_DIR}/shaders/")
set (PROJECT_BUILD_TYPE ${CMAKE_BUILD_TYPE})
configure_file(
    ${PROJECT_SOURCE_DIR}/include/Definitions.hpp.in 
    ${PROJECT_SOURCE_DIR}/include/Definitions.hpp
    @ONLY
)

# 実行ファイル作成
add_subdirectory(src)
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} main_lib)

# glfwを使ってwindow表示サンプル
add_executable("glfw_sample_window" "src/glfw_sample_window.cpp")
target_include_directories(glfw_sample_window PUBLIC
    ${glfw3_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${ASSIMP_INCLUDE_DIRS}
)
target_link_libraries(glfw_sample_window
    ${glfw3_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    glad
)


if (MSVC)
    install(DIRECTORY "${OpenCV_DIR}/x64/vc17/bin/" DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}
        FILES_MATCHING PATTERN "*.dll")
endif()
