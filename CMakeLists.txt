cmake_minimum_required(VERSION 3.26)
project(GraphicalProgramming_7002810)

set(CMAKE_CXX_STANDARD 20)

Include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.8
        GIT_SHALLOW 1
)
FetchContent_MakeAvailable(glfw)
FetchContent_GetProperties(glfw)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8
        GIT_SHALLOW 1
)
FetchContent_MakeAvailable(glm)
FetchContent_GetProperties(glm)

find_package(OpenGL REQUIRED)

include_directories(include lib/gl3w/include)

file(GLOB_RECURSE SOURCES "main.cpp" "src/*.cpp" "lib/gl3w/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} glfw glm OpenGL::GL)
