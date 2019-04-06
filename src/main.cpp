#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define TITLE "OpenCV Lab"
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include <cstdio>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

#include "WebcamCapture.h"

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

/* static const struct
{
    float x, y;
    float r, g, b;
} vertices[6] =
{
    { -1.f, -1.f, 0.f, 0.f, 0.f },
    {  0.f, -1.f, 0.f, 0.f, 0.f },
    { -1.f,  0.f, 0.f, 0.f, 0.f },
    { -1.f,  0.f, 0.f, 0.f, 0.f },
    {  0.f, -1.f, 0.f, 0.f, 0.f },
    {  0.f,  0.f, 0.f, 0.f, 0.f },
};*/

int main() {
    static GLFWwindow *win;
    int width = 0, height = 0;
    struct nk_context *ctx;
    struct nk_colorf bg;
    WebcamCapture capture;
    cv::Mat frame;
    GLuint texid;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "[GFLW] failed to init!\n");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE, NULL, NULL);
    if(!win) {
        fprintf(stderr, "[GFLW] failed to create window!\n");
        exit(1);
    }
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }

    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);


    {struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        nk_glfw3_font_stash_end();}
    while (!glfwWindowShouldClose(win)) {
        /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame();

        if(nk_begin(ctx, "Effects", nk_rect(50, 50 , 250, 220),NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                                         NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                fprintf(stdout, "button pressed\n");
        }
        nk_end(ctx);

        capture.captureFrame(frame);

        /* Draw */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetWindowSize(win, &width, &height);

        // Creating texture

        glGenTextures(1, &texid);
        glBindTexture(GL_TEXTURE_2D, texid);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set texture clamping method
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexImage2D(GL_TEXTURE_2D,       // Type of texture
                     0,                   // Pyramid level (for mip-mapping) - 0 is the top level
                     GL_RGB,              // Internal colour format to convert to
                     frame.cols,          // Image width  i.e. 640 for Kinect in standard mode
                     frame.rows,          // Image height i.e. 480 for Kinect in standard mode
                     0,                   // Border width in pixels (can either be 1 or 0)
                     GL_BGR,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                     GL_UNSIGNED_BYTE,    // Image data type
                     (GLvoid *) frame.ptr());        // The actual image data itself

        glViewport(0, 0, width, height);
        glClearColor(bg.r, bg.g, bg.b, bg.a);

        nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}