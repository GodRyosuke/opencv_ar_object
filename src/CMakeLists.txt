set (PROJECT_SRC
    Manager.cpp
    Renderer.cpp
    Shader.cpp
    VertexArray.cpp
    Texture.cpp
    Mesh.cpp
    Skeleton.cpp
    Util.cpp
    Actor/Actor.cpp
    Actor/TestSprite.cpp
    Actor/Capture.cpp
    Actor/ARMarker.cpp
    Actor/DebugActor.cpp
    Actor/UnityChan.cpp
    Actor/Grid.cpp
    Component/Component.cpp
    Component/SpriteComponent.cpp
    Component/MeshComponent.cpp
    Component/VideoComponent.cpp
    # Component/CameraComponent.cpp
)
add_library(main_lib ${PROJECT_SRC})
target_include_directories(main_lib PUBLIC
    ${glfw3_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${ASSIMP_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
)
target_link_libraries(main_lib
    ${glfw3_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${ASSIMP_LIBRARIES}
    glad
)