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

add_executable(opencv_lab src/main.cpp
        include/lib/nuklear.h
        include/lib/nuklear_glfw_gl3.h
        src/WebcamCapture.cpp
        include/WebcamCapture.h
        include/Shader.h src/Shader.cpp
        include/utility.h src/utility.cpp
        include/Logger.h src/Logger.cpp
        include/LoggerInit.h
        include/MainWindow.h
        src/MainWindow.cpp
        include/WebcamOutput.h
        src/WebcamOutput.cpp
        include/ImageEffects.h
        src/ImageEffects.cpp)

target_link_libraries(opencv_lab ${OPENGL_LIBRARIES} glfw GLEW ${OpenCV_LIBS} )

