#include "WebcamCapture.h"
#include "Logger.h"

WebcamCapture::WebcamCapture() {
    capture.open(0);
    if(!capture.isOpened()) {
        logger.write("Can't open webcam feed!\n");
    }
}

void WebcamCapture::captureFrame(cv::Mat &frame) {
    capture >> frame;
}