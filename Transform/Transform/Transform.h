#ifndef TRANSFORM_TRANSFORM
#define TRANSFORM_TRANSFORM
#include "Util.h"
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgcodecs.hpp>

using namespace cv;
class Transform
{
protected:
    int imgWidth;
    int imgHeight;
    int colorbit;
    int cornerWidth = 7;
    int cornerHeight = 7;
    cv::Vec3b white;
    cv::Vec3b black;
  
public:
    int pixelStride;
    int width;
    int height;
    int colorLevel;
public:
    Transform(int width = 100, int height = 100, int pixelStride = 15,int colorLevel = 16);
    void convert(pByte bytes, int length, std::string imageName);
    int bytes();
    int bits();
    int validBits();
    int validBytes();
    void drawCorner(cv::Mat& mat);
    void drawTopLeftCorner(cv::Mat& mat, int x, int y, int value);
    void drawTopRightCorner(cv::Mat& mat, int x, int y, int value);
    void drawBottomLeftCorner(cv::Mat& mat, int x, int y, int value);
    inline void fillPixel(Mat& mat, int x, int y, Vec3b& fillColor);
    Byte fetch(pByte bytes, int bitPos, int stride);
public:
    inline static Transform Default() {
        return Transform(50,50,15,8);
    }
    inline static Transform Small() {
        return Transform(30,30,31,8);
    }
    inline static Transform Medium() {
        return Transform(100, 100, 31, 16);
    }
    inline static Transform Large() {
        return Transform(100, 100, 7, 32);
    }
};

#endif
