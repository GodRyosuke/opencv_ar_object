set (PROJECT_SRC
    Manager.cpp
    Renderer.cpp
    Shader.cpp
    VertexArray.cpp
    Texture.cpp
    Mesh.cpp
    Animation.cpp
    Skeleton.cpp
    Util.cpp
    Actor/Actor.cpp
    Actor/TestSprite.cpp
    Actor/Capture.cpp
    Actor/ARMarker.cpp
    Actor/DebugActor.cpp
    Actor/UnityChan.cpp
    Actor/Grid.cpp
    Actor/Axis.cpp
    Component/Component.cpp
    Component/SpriteComponent.cpp
    Component/MeshComponent.cpp
    Component/SkinMeshComponent.cpp
    Component/VideoComponent.cpp
)
set (include_dirs
    ${glfw3_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${ASSIMP_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
)
set (libraries
    ${glfw3_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    glad
)
if (USE_REALSENSE)
list(APPEND include_dirs ${realsense2_INCLUDE_DIR})
list(APPEND libraries ${realsense2_LIBRARY})
endif()

add_library(main_lib ${PROJECT_SRC})
target_include_directories(main_lib PUBLIC
    ${include_dirs}
#     ${glfw3_INCLUDE_DIR}
#     ${OpenCV_INCLUDE_DIRS}
#     ${PROJECT_SOURCE_DIR}/include
#     ${ASSIMP_INCLUDE_DIRS}
#     ${GLM_INCLUDE_DIR}
# if (USE_REALSENSE)
#     ${realsense2_INCLUDE_DIR}
# endif()
)
target_link_libraries(main_lib
    ${libraries}
#     ${glfw3_LIBRARIES}
#     ${OpenCV_LIBRARIES}
#     ${ASSIMP_LIBRARIES}
#     glad
# if (USE_REALSENSE)
#     ${realsense2_LIBRARY}
# endif()
)