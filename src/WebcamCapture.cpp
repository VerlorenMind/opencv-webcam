//
// Created by morzharetto on 4/6/19.
//

#include "WebcamCapture.h"

/*void BindCVMat2GLTexture(cv::Mat& image, GLuint& imageTexture)
{
    if(image.empty()){
        std::cout << "image empty" << std::endl;
    }else{
        //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
}*/

WebcamCapture::WebcamCapture() {
    capture.open(0);
    if(!capture.isOpened()) {
        std::cerr << "Can't open webcam feed!" <<std::endl;
    }
}

void WebcamCapture::captureFrame(cv::Mat &frame) {
    capture >> frame;
}