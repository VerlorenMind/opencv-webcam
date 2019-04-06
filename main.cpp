#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define TITLE "OpenCV Lab"

#include <cstdio>
#include <iostream>
#include <GLFW/glfw3.h>


void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main() {
    static GLFWwindow *win;

    if (!glfwInit()) {
        fprintf(stderr, "[GFLW] failed to init!\n");
        exit(1);
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE, NULL, NULL);
    if(!win) {
        fprintf(stderr, "[GFLW] failed to create window!\n");
        exit(1);
    }
    glfwMakeContextCurrent(win);

    while (!glfwWindowShouldClose(win)) {

    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}