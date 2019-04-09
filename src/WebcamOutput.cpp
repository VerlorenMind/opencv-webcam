
#include <WebcamOutput.h>

#include "WebcamOutput.h"

void BindCVMat2GLTexture(cv::Mat& image, GLuint& imageTexture)
{
    if(image.empty()) {
        std::cout << "image empty" << std::endl;
    } else {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glGenTextures(1, &imageTexture);
        glBindTexture(GL_TEXTURE_2D, imageTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set texture clamping method
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

        glTexImage2D(GL_TEXTURE_2D,         // Type of texture
                     0,                   // Pyramid level (for mip-mapping) - 0 is the top level
                     GL_RGB,              // Internal colour format to convert to
                     image.cols,          // Image width  i.e. 640 for Kinect in standard mode
                     image.rows,          // Image height i.e. 480 for Kinect in standard mode
                     0,                   // Border width in pixels (can either be 1 or 0)
                     GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                     GL_UNSIGNED_BYTE,    // Image data type
                     image.ptr());        // The actual image data itself
    }
}

WebcamOutput::WebcamOutput(ViewportRectangle port) {
    Shader fragShader("Fragment", GL_FRAGMENT_SHADER, Shader::SourceType::fromFile, "../assets/shaders/webcamstream/shader.frag");
    Shader vertShader("Vertex", GL_VERTEX_SHADER, Shader::SourceType::fromFile, "../assets/shaders/webcamstream/shader.vert");
    program = new ShaderProgram("MainWindowShader", {&fragShader, &vertShader});
    this->port = port;

    glGenFramebuffers(1, &readFboId);
}

void WebcamOutput::setViewport(ViewportRectangle port) {
    this->port = port;
}

void WebcamOutput::draw() {
    glViewport(port.x, port.y, port.width, port.height);

    program->use();

    capture.captureFrame(frame);

    cv::flip(frame, frame, 0);

    /* Draw */

    BindCVMat2GLTexture(frame, texid);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, texid, 0);
    glBlitFramebuffer(0, 0, frame.cols, frame.rows,
                      0, 0, port.width, port.height,
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

WebcamOutput::~WebcamOutput() {
    glDeleteFramebuffers(1, &readFboId);
}


