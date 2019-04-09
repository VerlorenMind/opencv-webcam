#ifndef OPENCV_LAB_WEBCAMOUTPUT_H
#define OPENCV_LAB_WEBCAMOUTPUT_H

#include "WebcamCapture.h"
#include "Shader.h"

struct ViewportRectangle {
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
};

void BindCVMat2GLTexture(cv::Mat& image, GLuint& imageTexture);

class WebcamOutput {
private:
    cv::Mat frame;
    GLuint texid;
    GLuint readFboId;
    WebcamCapture capture;
    ViewportRectangle port;
    ShaderProgram *program;
public:
    WebcamOutput(ViewportRectangle port);
    ~WebcamOutput();
    void setViewport(ViewportRectangle port);
    void draw();
};

#endif //OPENCV_LAB_WEBCAMOUTPUT_H
