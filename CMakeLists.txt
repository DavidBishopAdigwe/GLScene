cmake_minimum_required(VERSION 3.15)
project(GLScene LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 👇 FIND OPENGL FIRST - This is crucial
find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

set (DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/dependencies)

set(GLAD_DIR ${DEPENDENCIES_DIR}/glad)

set(GLM_DIR ${DEPENDENCIES_DIR}/glm)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89.9
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v6.0.1
)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Don't build assimp tools" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Don't build tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Don't install" FORCE)
FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# 👇 YOUR PROJECT SOURCES
set(SOURCES
        main.cpp
        Objects/Sources/Shader.cpp
        Objects/Sources/VAO.cpp
        Objects/Sources/VBO.cpp
        Objects/Sources/EBO.cpp
        Objects/Sources/Texture.cpp
        Objects/Sources/Camera.cpp
        Objects/Sources/Light.cpp
        Objects/Sources/Vertex.cpp
        Main/Engine.cpp
        Main/Scene.cpp
        stb/stb.cpp
        Core/Sources/Material.cpp
        Core/Sources/MaterialInstance.cpp
        Core/Sources/Mesh.cpp
        Core/Sources/Entity.cpp
        Core/Sources/Components.cpp
        Core/Headers/Components.h
        ${GLAD_DIR}/glad.c  # Local glad.c
)

set(IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        Objects/General/Vertex.h
)

add_executable(GLScene ${SOURCES} ${IMGUI_SOURCES} "Core/FileManager.h" "Core/ResourceManager.h" "Core/Parser.h" "Shapes/Headers/Grid.h" "Math/Math.h")

target_include_directories(GLScene PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${DEPENDENCIES_DIR}
        ${GLAD_DIR}
        ${GLM_DIR}# GLAD headers
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${stb_SOURCE_DIR}
)

target_compile_definitions(GLScene PRIVATE
        IMGUI_DEFINE_MATH_OPERATORS
        GLFW_INCLUDE_NONE
)

target_link_libraries(GLScene PRIVATE
        glfw
        assimp
        ${OPENGL_LIBRARIES}
)
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG")
endif()

if(WIN32)
    target_link_libraries(GLScene PRIVATE opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(GLScene PRIVATE GL)
endif()