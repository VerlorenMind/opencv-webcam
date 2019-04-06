cmake_minimum_required(VERSION 3.13)
project(opencv_lab)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

add_executable(opencv_lab src/main.cpp include/nuklear.h include/nuklear_glfw_gl3.h src/WebcamCapture.cpp include/WebcamCapture.h)

target_link_libraries(opencv_lab ${OPENGL_LIBRARIES} glfw GLEW ${OpenCV_LIBS} )

