#include <utility>
#include "ImageEffects.h"

ImageEffect::ImageEffect() {
    name = "ImageEffect";
}

std::string ImageEffect::getName() {
    return name;
}

void ImageEffect::operator()(cv::Mat &image, ImageEffectsContext &ctx) {

}

BrightnessContrastImageEffect::BrightnessContrastImageEffect() {
    name = "BrightnessContrastImageEffect";
}

void BrightnessContrastImageEffect::operator()(cv::Mat& image, ImageEffectsContext& ctx) {
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                image.at<cv::Vec3b>(y,x)[c] =
                        cv::saturate_cast<uchar>(ctx.contrast*image.at<cv::Vec3b>(y,x)[c] + ctx.brightness);
            }
        }
    }
}

GaussianFilterImageEffect::GaussianFilterImageEffect() {
    name = "GaussianFilterImageEffect";
}

void GaussianFilterImageEffect::operator()(cv::Mat &image, ImageEffectsContext &ctx) {
    if(ctx.gaussianKernelSize < 3) {
        return;
    }
    else if(ctx.gaussianKernelSize % 2 == 0){
        logger.write("Filter %s: invalid kernel size = %i", name.data(), ctx.gaussianKernelSize);
    }
    else {
        cv::GaussianBlur(image, image, cv::Size(ctx.gaussianKernelSize, ctx.gaussianKernelSize), 0, 0);
    }
}

ErosionImageEffect::ErosionImageEffect() {
    name = "ErosionImageEffect";
}

void ErosionImageEffect::operator()(cv::Mat &image, ImageEffectsContext &ctx) {
    if(ctx.erosionKernelSize < 3) {
        return;
    }
    else if(ctx.erosionKernelSize % 2 == 0){
        logger.write("Filter %s: invalid kernel size = %i", name.data(), ctx.erosionKernelSize);
    }
    else {
        cv::Mat element = cv::getStructuringElement(ctx.erosionShapes,
                                                    cv::Size(ctx.erosionKernelSize, ctx.erosionKernelSize),
                                                    cv::Point(ctx.erosionKernelSize / 2, ctx.erosionKernelSize / 2));
        cv::erode(image, image, element);
    }
}

DilationImageEffect::DilationImageEffect() {
    name = "DilationImageEffect";
}

void DilationImageEffect::operator()(cv::Mat &image, ImageEffectsContext &ctx) {
    if(ctx.dilationKernelSize < 3) {
        return;
    }
    else if(ctx.dilationKernelSize % 2 == 0){
        logger.write("Filter %s: invalid kernel size = %i", name.data(), ctx.dilationKernelSize);
    }
    else {
        cv::Mat element = cv::getStructuringElement(ctx.dilationShapes,
                                                    cv::Size(ctx.dilationKernelSize, ctx.dilationKernelSize),
                                                    cv::Point(ctx.dilationKernelSize / 2, ctx.dilationKernelSize / 2));
        cv::dilate(image, image, element);
    }
}

ImageEffectsProcessor::ImageEffectsProcessor() {
    ctx.brightness = 0;
    ctx.contrast = 1.0f;
    ctx.gaussianKernelSize = 0;
    ctx.erosionKernelSize = 0;
    ctx.dilationKernelSize = 0;
}

ImageEffectsProcessor::ImageEffectsProcessor(ImageEffectList &list) : ImageEffectsProcessor() {
    effects = std::move(list);
}

void ImageEffectsProcessor::enable(ImageEffect *effect) {
    bool found = false;
    for(auto iter = effects.begin(); iter != effects.end(); ++iter) {
        if((*iter)->getName() == effect->getName()){
            found = true;
            break;
        }
    }
    if(!found)
    {
        effects.push_back(effect);
    }
}

void ImageEffectsProcessor::applyEffects(cv::Mat &image) {
    for(auto iter = effects.begin(); iter != effects.end(); ++iter) {
        (**iter)(image, ctx);
    }
}

ImageEffectsProcessor::~ImageEffectsProcessor() {
    for(auto iter = effects.begin(); iter != effects.end(); ++iter) {
        delete *iter;
    }
}

void ImageEffectsProcessor::setContext(ImageEffectsContext ctx) {
    this->ctx = ctx;
}

void ImageEffectsProcessor::disable(std::string name) {
    for(auto iter = effects.begin(); iter != effects.end(); ++iter) {
        if((*iter)->getName() == name) {
            effects.erase(iter);
            break;
        }
    }
}

void ImageEffectsProcessor::swapEffects(std::string effect1, std::string effect2) {

    int first = -1;
    int second = -1;
    for(int i = 0; i < effects.size(); ++i) {
        if(effects[i]->getName() == effect1) {
            first = i;
        }
        if(effects[i]->getName() == effect2) {
            second = i;
        }
    }
    if(first != -1 && second != -1) {
        std::swap(effects[first], effects[second]);
    }
}
