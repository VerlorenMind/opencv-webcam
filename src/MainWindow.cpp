#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION

#include "MainWindow.h"

MainWindow::MainWindow() {

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
}

void MainWindow::mainLoop() {
    WebcamCapture capture;
    cv::Mat frame;
    GLuint texid;
    GLuint readFboId;
    Shader fragShader("Fragment", GL_FRAGMENT_SHADER, Shader::SourceType::fromFile, "../assets/shaders/webcamstream/shader.frag");
    Shader vertShader("Vertex", GL_VERTEX_SHADER, Shader::SourceType::fromFile, "../assets/shaders/webcamstream/shader.vert");
    ShaderProgram prog("MainWindowShader", {&fragShader, &vertShader});
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

        readFboId = 0;
        glGenFramebuffers(1, &readFboId);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, texid, 0);
        glBlitFramebuffer(0, 0, frame.cols, frame.rows,
                          0, 0, width, height,
                          GL_COLOR_BUFFER_BIT, GL_LINEAR);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &readFboId);

        glViewport(0, 0, width, height);
        // glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
    }
}

MainWindow::~MainWindow() {
    nk_glfw3_shutdown();
    glfwTerminate();
}


void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}