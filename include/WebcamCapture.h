//
// Created by morzharetto on 4/6/19.
//

#ifndef OPENCV_LAB_WEBCAMCAPTURE_H
#define OPENCV_LAB_WEBCAMCAPTURE_H

#include <opencv2/opencv.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void BindCVMat2GLTexture(cv::Mat& image, GLuint& imageTexture);

class WebcamCapture {

private:
    cv::VideoCapture capture;

public:
    void captureFrame(cv::Mat &frame);
    WebcamCapture();

};


#endif //OPENCV_LAB_WEBCAMCAPTURE_H
