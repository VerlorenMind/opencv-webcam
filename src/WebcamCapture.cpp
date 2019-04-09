#include "WebcamCapture.h"

WebcamCapture::WebcamCapture() {
    capture.open(0);
    if(!capture.isOpened()) {
        std::cerr << "Can't open webcam feed!" <<std::endl;
    }
}

void WebcamCapture::captureFrame(cv::Mat &frame) {
    capture >> frame;
}