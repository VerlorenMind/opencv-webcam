cmake_minimum_required(VERSION 3.13)
project(opencv_lab)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})

add_executable(opencv_lab main.cpp)
target_link_libraries(opencv_lab ${OPENGL_gl_LIBRARY} glfw)

