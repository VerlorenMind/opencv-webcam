#ifndef OPENCV_LAB_WEBCAMCAPTURE_H
#define OPENCV_LAB_WEBCAMCAPTURE_H

#include <opencv2/opencv.hpp>

#include <GL/glew.h>

void BindCVMat2GLTexture(cv::Mat& image, GLuint& imageTexture);

class WebcamCapture {

private:
    cv::VideoCapture capture;
public:
    void captureFrame(cv::Mat &frame);
    WebcamCapture();

};



#endif //OPENCV_LAB_WEBCAMCAPTURE_H
