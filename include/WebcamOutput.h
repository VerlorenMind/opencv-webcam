#ifndef OPENCV_LAB_WEBCAMOUTPUT_H
#define OPENCV_LAB_WEBCAMOUTPUT_H

#include "WebcamCapture.h"
#include "Shader.h"
#include "ImageEffects.h"

struct ViewportRectangle {
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
};

void BindCVMat2GLTexture(cv::Mat& image, GLuint& imageTexture);

class WebcamOutput {

public:
    struct EffectSettings {
        float brightness;
    };

    explicit WebcamOutput(ViewportRectangle port);
    ~WebcamOutput();
    void setViewport(ViewportRectangle port);
    void draw();

    ImageEffectsProcessor effects;

private:
    cv::Mat frame;
    GLuint texid;
    GLuint readFboId;
    WebcamCapture capture;
    ViewportRectangle port;
    ShaderProgram *program;
};

#endif //OPENCV_LAB_WEBCAMOUTPUT_H
