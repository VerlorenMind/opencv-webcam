#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION

#include "MainWindow.h"

MainWindow::MainWindow() {

    settings.brightness = 0;

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
    WebcamOutput webcamOutput({0, 0, WINDOW_WIDTH, WINDOW_HEIGHT});
    bool erosionDilationOrder = false;
    const char **shapes = new const char*[3];
    shapes[0] = const_cast<char *>("Rectangle");
    shapes[1] = const_cast<char *>("Cross");
    shapes[2] = const_cast<char *>("Ellipse");
    int selectedErosionShape = 0;
    int selectedDilationShape = 0;
    while (!glfwWindowShouldClose(win)) {
        /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame();

        if(nk_begin(ctx, "Effects", nk_rect(50, 50 , 350, 350),NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
                                                               NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctx, 130);
                nk_label(ctx, "Brightness:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 170);
                nk_slider_int(ctx, -100, &webcamOutput.effects.ctx.brightness, 100, 1);
            }
            nk_layout_row_end(ctx);
            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctx, 130);
                nk_label(ctx, "Contrast:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 170);
                nk_slider_float(ctx, 0.f, &webcamOutput.effects.ctx.contrast, 3.f, 0.01f);
            }
            nk_layout_row_end(ctx);
            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctx, 130);
                nk_label(ctx, "Gaussian Filter:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 170);
                nk_slider_int(ctx, 1, &webcamOutput.effects.ctx.gaussianKernelSize, 61, 2);
            }
            nk_layout_row_end(ctx);

            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctx, 150);
                if (nk_option_label(ctx, "Erosion", !erosionDilationOrder)) {
                    erosionDilationOrder = false;
                    webcamOutput.effects.swapEffects("ErosionImageEffect", "DilationImageEffect");
                };
                nk_layout_row_push(ctx, 150);
                if (nk_option_label(ctx, "Dilation", erosionDilationOrder)) {
                    erosionDilationOrder = true;
                    webcamOutput.effects.swapEffects("ErosionImageEffect", "DilationImageEffect");
                };
            }
            nk_layout_row_end(ctx);

            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctx, 130);
                nk_label(ctx, "Kernel Size:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 170);
                if(!erosionDilationOrder) {
                    nk_slider_int(ctx, 1, &webcamOutput.effects.ctx.erosionKernelSize, 61, 2);
                }
                else {
                    nk_slider_int(ctx, 1, &webcamOutput.effects.ctx.dilationKernelSize, 61, 2);
                }
            }
            nk_layout_row_end(ctx);

            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctx, 150);
                if (nk_option_label(ctx, "Erosion", erosionDilationOrder)) {
                    erosionDilationOrder = true;
                    webcamOutput.effects.swapEffects("ErosionImageEffect", "DilationImageEffect");
                };
                nk_layout_row_push(ctx, 150);
                if (nk_option_label(ctx, "Dilation", !erosionDilationOrder)) {
                    erosionDilationOrder = false;
                    webcamOutput.effects.swapEffects("ErosionImageEffect", "DilationImageEffect");
                };
            }
            nk_layout_row_end(ctx);

            nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctx, 130);
                nk_label(ctx, "Kernel Size:", NK_TEXT_LEFT);
                nk_layout_row_push(ctx, 170);
                if(erosionDilationOrder) {
                    nk_slider_int(ctx, 1, &webcamOutput.effects.ctx.erosionKernelSize, 61, 2);
                }
                else {
                    nk_slider_int(ctx, 1, &webcamOutput.effects.ctx.dilationKernelSize, 61, 2);
                }
            }
            nk_layout_row_end(ctx);

            nk_layout_row_dynamic(ctx, 30, 1);
            selectedErosionShape = nk_combo(ctx, shapes, 3, selectedErosionShape, 25, nk_vec2(200,200));
            switch(selectedErosionShape) {
                case 0:
                    webcamOutput.effects.ctx.erosionShapes = cv::MORPH_RECT;
                    break;
                case 1:
                    webcamOutput.effects.ctx.erosionShapes = cv::MORPH_CROSS;
                    break;
                case 2:
                    webcamOutput.effects.ctx.erosionShapes = cv::MORPH_ELLIPSE;
                    break;
            }

            nk_layout_row_dynamic(ctx, 30, 1);
            selectedDilationShape = nk_combo(ctx, shapes, 3, selectedDilationShape, 25, nk_vec2(200,200));
            switch(selectedDilationShape) {
                case 0:
                    webcamOutput.effects.ctx.dilationShapes = cv::MORPH_RECT;
                    break;
                case 1:
                    webcamOutput.effects.ctx.dilationShapes = cv::MORPH_CROSS;
                    break;
                case 2:
                    webcamOutput.effects.ctx.dilationShapes = cv::MORPH_ELLIPSE;
                    break;
            }
        }
        nk_end(ctx);


        /* Draw */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetWindowSize(win, &width, &height);

        webcamOutput.setViewport({0, 0, width, height});
        webcamOutput.draw();

        glViewport(0, 0, width, height);
        nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
    }
    delete[] shapes;
}

MainWindow::~MainWindow() {
    nk_glfw3_shutdown();
    glfwTerminate();
}


void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}