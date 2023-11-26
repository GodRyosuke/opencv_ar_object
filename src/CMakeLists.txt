set (PROJECT_SRC
    Manager.cpp
    Renderer.cpp
)
add_library(main_lib ${PROJECT_SRC})
target_include_directories(main_lib PUBLIC
    ${glfw3_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${ASSIMP_INCLUDE_DIRS}
)
target_link_libraries(main_lib
    ${glfw3_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    glad
)