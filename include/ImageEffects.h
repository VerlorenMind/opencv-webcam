#ifndef OPENCV_LAB_IMAGEEFFECTS_H
#define OPENCV_LAB_IMAGEEFFECTS_H

#include <opencv2/opencv.hpp>

#include "Logger.h"

struct ImageEffectsContext {
    int brightness;
    float contrast;
    int gaussianKernelSize;
    int erosionKernelSize;
    int dilationKernelSize;
    cv::MorphShapes erosionShapes;
    cv::MorphShapes dilationShapes;
};

class ImageEffect {
protected:
    std::string name;
public:
    ImageEffect();
    virtual void operator()(cv::Mat &image, ImageEffectsContext& ctx);
    std::string getName();
};

class BrightnessContrastImageEffect: public ImageEffect {
public:
    BrightnessContrastImageEffect();
    virtual void operator()(cv::Mat &image, ImageEffectsContext& ctx);
};

class GaussianFilterImageEffect: public ImageEffect {
public:
    GaussianFilterImageEffect();
    virtual void operator()(cv::Mat &image, ImageEffectsContext& ctx);
};

class ErosionImageEffect: public ImageEffect {
public:
    ErosionImageEffect();
    virtual void operator()(cv::Mat &image, ImageEffectsContext& ctx);
};

class DilationImageEffect: public ImageEffect {
public:
    DilationImageEffect();
    virtual void operator()(cv::Mat &image, ImageEffectsContext& ctx);
};

typedef std::vector<ImageEffect*> ImageEffectList;

class ImageEffectsProcessor {
private:
    ImageEffectList effects;
public:
    ImageEffectsProcessor();
    ImageEffectsProcessor(ImageEffectList &list);
    ~ImageEffectsProcessor();

    void enable(ImageEffect *effect);
    void disable(std::string name);
    void insertBefore(ImageEffect *effect, std::string insertBefore);
    void swapEffects(std::string effect1, std::string effect2);
    void applyEffects(cv::Mat& image);
    void setContext(ImageEffectsContext ctx);

    ImageEffectsContext ctx;
};

#endif //OPENCV_LAB_IMAGEEFFECTS_H
