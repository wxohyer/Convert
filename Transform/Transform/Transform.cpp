#include "Transform.h"
#include <iostream>
using namespace std;
using namespace cv;
int Transform::bytes() {
    return width * height * 3 * ((int)log2(colorLevel)) / 8;
}
int Transform::bits() {
    return width * height * 3 * (int)log2(colorLevel);
}
int Transform::validBits() {
    return (width * height - (cornerHeight + 1) * (cornerWidth + 1)) * 3 * (int)log2(colorLevel);
}
int Transform::validBytes() {
    return validBits() / 8;
}
Transform::Transform(int width, int height, int pixelStride,int colorLevel) {
    this->width = width;
    this->height = height;
    this->pixelStride = pixelStride;
    this->imgHeight = height * pixelStride;
    this->imgWidth = width * pixelStride;
    this->colorLevel = colorLevel;
    colorbit =(int)log2(colorLevel);
    white = Vec3b::all(255);
    black = Vec3b::all(0);
    std::cout << "Bytes:" << bytes() << endl;
}
void Transform::drawCorner(Mat& mat) {
    drawTopLeftCorner(mat, 0, 0, 100);
    drawBottomLeftCorner(mat,height - cornerHeight,0,100);
    drawTopRightCorner(mat, 0, width - cornerWidth,100);
}
//绘制角点
void Transform::drawTopLeftCorner(Mat& mat, int x, int y, int value) {
    //draw col
    for (int i = 0; i < cornerWidth; i++) {
        int xcoord = (x + i) * pixelStride;
        fillPixel(mat, xcoord, y * pixelStride, black);
        fillPixel(mat, xcoord, (y+1) * pixelStride, white);
        fillPixel(mat, xcoord, (y + cornerHeight-2) * pixelStride, white);
        fillPixel(mat, xcoord, (y + cornerHeight-1)* pixelStride, black);
        fillPixel(mat, xcoord, (y + cornerHeight)*pixelStride, white);
    }
    //draw row
    for (int i = 1; i < cornerHeight-1; i++) {
        int ycoord = (y + i) * pixelStride;
        fillPixel(mat, x * pixelStride, ycoord, black);
        fillPixel(mat, (x+1) * pixelStride, ycoord, white);
        fillPixel(mat, (x + cornerHeight-2) * pixelStride, ycoord, white);
        fillPixel(mat, (x + cornerHeight - 1) * pixelStride, ycoord, black);
    }
    for (int i = 0; i < cornerHeight+1; i++) {
        int ycoord = (y + i) * pixelStride;
        fillPixel(mat, (x + cornerHeight) * pixelStride, ycoord, white);
    }

    //draw core
    int corewidth = (cornerWidth - 4) * pixelStride;
    Rect rect((x + 2) * pixelStride, (y + 2) * pixelStride, corewidth, corewidth);
    auto coreRoi = mat(rect);
    coreRoi.setTo(Scalar(0,0,255));
    imshow("core", coreRoi);

    //coreRoi.setTo(Scalar(0, 0, 0));
    //imshow("tesss", coreRoi);
}

void Transform::drawTopRightCorner(cv::Mat& mat, int x, int y, int value) {
    //draw col
    for (int i = 0; i < cornerWidth; i++) {
        int xcoord = (x + i) * pixelStride;
        fillPixel(mat, xcoord, y * pixelStride, black);
        fillPixel(mat, xcoord, (y + 1) * pixelStride, white);
        fillPixel(mat, xcoord, (y + cornerHeight - 2) * pixelStride, white);
        fillPixel(mat, xcoord, (y + cornerHeight - 1) * pixelStride, black);
        fillPixel(mat, xcoord, (y-1) * pixelStride, white);
    }
    //draw row
    for (int i = 1; i < cornerHeight - 1; i++) {
        int ycoord = (y + i) * pixelStride;
        fillPixel(mat, x * pixelStride, ycoord, black);
        fillPixel(mat, (x + 1) * pixelStride, ycoord, white);
        fillPixel(mat, (x + cornerHeight - 2) * pixelStride, ycoord, white);
        fillPixel(mat, (x + cornerHeight - 1) * pixelStride, ycoord, black);
    }
    //last row
    for (int i = -1; i < cornerHeight; i++) {
        int ycoord = (y + i) * pixelStride;
        fillPixel(mat, (x + cornerHeight) * pixelStride, ycoord, white);
    }

    //draw core
    int corewidth = (cornerWidth - 4) * pixelStride;
    Rect rect((y + 2) * pixelStride, (x + 2) * pixelStride, corewidth, corewidth);
    auto coreRoi = mat(rect);
    coreRoi.setTo(Scalar(0,255,0));
    imshow("core", coreRoi);
}
void Transform::drawBottomLeftCorner(cv::Mat& mat, int x, int y, int value) {
    //draw col
    for (int i = 0; i < cornerWidth; i++) {
        int xcoord = (x + i) * pixelStride;
        fillPixel(mat, xcoord, y * pixelStride, black);
        fillPixel(mat, xcoord, (y + 1) * pixelStride, white);
        fillPixel(mat, xcoord, (y + cornerHeight - 2) * pixelStride, white);
        fillPixel(mat, xcoord, (y + cornerHeight - 1) * pixelStride, black);
        fillPixel(mat, xcoord, (y + cornerHeight) * pixelStride, white);
    }
    //draw row
    //top row
    for (int i = 0; i < cornerHeight+1; i++) {
        int ycoord = (y + i) * pixelStride;
        fillPixel(mat, (x-1) * pixelStride, ycoord, white);
    }
    for (int i = 1; i < cornerHeight - 1; i++) {
        int ycoord = (y + i) * pixelStride;
        fillPixel(mat, x * pixelStride, ycoord, black);
        fillPixel(mat, (x + 1) * pixelStride, ycoord, white);
        fillPixel(mat, (x + cornerHeight - 2) * pixelStride, ycoord, white);
        fillPixel(mat, (x + cornerHeight - 1) * pixelStride, ycoord, black);
    }
    for (int i = 0; i < cornerHeight; i++) {
        int ycoord = (y + i) * pixelStride;
        fillPixel(mat, (x + cornerHeight-1) * pixelStride, ycoord, black);
    }
    ////draw core
    int corewidth = (cornerWidth - 4) * pixelStride;
    Rect rect((y + 2) * pixelStride, (x + 2) * pixelStride,corewidth, corewidth);
    auto coreRoi = mat(rect);
    coreRoi.setTo(Scalar(255,0,0));
    imshow("core", coreRoi);
}

void Transform::fillPixel(Mat& mat, int x, int y, Vec3b& fillColor) {
    for (int i = 0; i < pixelStride; i++) {
        for (int j = 0; j < pixelStride; j++) {
            auto& color = mat.at<Vec3b>(x + i, y + j);
            color = fillColor;
        }
    }
}
Byte Transform::fetch(PByte bytes, int bitPos, int stride) {
    int bytePos = bitPos / 8;
    int left = bitPos % 8;
    Byte result = 0;
    if ((left + stride) > 8) {
        Byte byte = bytes[bytePos];
        Byte nextByte = bytes[bytePos++];
        Byte mask1 = 0;
        for (int i = 0; i < left; i++) {
            mask1 += (Byte)pow(2, 8 - i);
        }
        int need = left + stride - 8;
        Byte mask2 = 0;
        for (int i = 0; i < need; i++) {
            mask2 += (Byte)pow(2, 8 - i);
        }

        result = byte & (~mask1) << need + nextByte & mask2 >> (8 - need);
        if (result >= 16) {
            std::cout << "Error" << endl;
        }
    }
    else {
        Byte byte = bytes[bytePos];
        Byte mask1 = 0;
        for (int i = 0; i < left; i++) {
            mask1 += (Byte)pow(2, 8 - i);
        }
        Byte mask2 = 0;
        int leftcount = 8 - left - stride;
        for (int i = 0; i < leftcount; i++) {
            mask2 += (Byte)pow(2, i);
        }
        Byte mask = ~(mask1 | mask2);
        result = mask & byte;
        result = result >> (8 - stride);

        if (result >= 16) {
            std::cout << "Error" << endl;
        }
    }
    return result;
}

void Transform::convert(PByte bytes, int length, std::string imageName) {
    Mat mat(imgWidth, imgHeight, CV_8UC3,Scalar::all(168));  
    auto take = [=](const int bitPos, int stride)->Byte {
        int bytePos = bitPos / 8;
        int left = bitPos % 8;
        Byte result = 0;
        if ((left + stride) > 8) {
            Byte byte = bytes[bytePos];
            Byte nextByte = bytes[bytePos++];
            Byte mask1 = 0;
            for (int i = 0; i < left; i++) {
                mask1 += (Byte)pow(2, 8 - i);
            }
            int need = left + stride - 8;
            Byte mask2 = 0;
            for (int i = 0; i < need; i++) {
                mask2 += (Byte)pow(2, 8 - i);
            }

            result = byte & (~mask1) << need + nextByte & mask2 >> (8 - need);
            if (result >= 16) {
                std::cout << "Error" << endl;
            }
        }
        else {
            Byte byte = bytes[bytePos];
            Byte mask1 =0;
            for (int i = 0; i < left; i++) {
                mask1 += (Byte)pow(2, 8 - i);
            }
            Byte mask2 =0;
            int leftcount = 8 - left - stride;
            for (int i = 0; i < leftcount; i++) {
                mask2 += (Byte)pow(2, i);
            }
            Byte mask = ~(mask1 | mask2);
            result = mask & byte;
            result = result >> (8-stride);

            if (result >= 16) {
                std::cout << "Error" << endl;
            }
        }
        return result;
    };
    int scale = 256 / colorLevel;
    int bitIndex = 0;
    for (int row = 0; row < width; row++) {
        for (int col = 0; col < height; col++) {
            int rowPos = row * pixelStride;
            int colPos = col * pixelStride;
            Byte b = take(bitIndex, colorbit);
            bitIndex += colorbit;
            Byte g = take(bitIndex, colorbit);
            bitIndex += colorbit;
            Byte r = take(bitIndex, colorbit);
            bitIndex += colorbit;

            for (int i = 0; i < pixelStride; i++) {
                for (int j = 0; j < pixelStride; j++) {
                    auto& color = mat.at<Vec3b>(rowPos + i, colPos + j);
                    color[0] = b * scale;
                    color[1] = g * scale;
                    color[2] = r * scale;
                }
            }
            if (bitIndex / 8 > length) {
                goto finish;
            }
        }
    }
finish:
    drawCorner(mat);
    imwrite(imageName, mat);
    imshow("test",mat);
    waitKey();
}

