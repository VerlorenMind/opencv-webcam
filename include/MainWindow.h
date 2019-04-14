#ifndef OPENCV_LAB_MAINWINDOW_H
#define OPENCV_LAB_MAINWINDOW_H

#include <cstdio>
#include <iostream>
#include <cstdarg>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define TITLE "OpenCV Lab"
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "WebcamOutput.h"
#include "Shader.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include "lib/nuklear.h"
#include "lib/nuklear_glfw_gl3.h"


void error_callback(int error, const char* description);

class MainWindow {
private:
    GLFWwindow *win;
    WebcamOutput::EffectSettings settings;
    int width = 0, height = 0;
    struct nk_context *ctx;
public:
    MainWindow();
    ~MainWindow();
    void mainLoop();
};

#endif //OPENCV_LAB_MAINWINDOW_H
