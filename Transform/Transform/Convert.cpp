#include "Convert.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using namespace cv;
Convert::Convert(UShort width, UShort height) {
	this->width = width;
	this->height = height;
    this->capacity = this->width * this->height * 3 - (1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 4 + 2);
}

void Convert::Enconding(const char* fileName) {
	//¡öwith¡õheight¡öindex¡õtotal¡ölength end¡õ¡ö
	cv::Mat mat(32, 32, CV_8UC3, cv::Scalar::all(168));
	std::ifstream ifs(fileName, std::ifstream::binary);
	// get pointer to associated buffer object
    std::filebuf* pbuf = ifs.rdbuf();

    // get file size using buffer's members
    std::size_t fileSize = pbuf->pubseekoff(0, ifs.end, ifs.in);
    pbuf->pubseekpos(0, ifs.in);

    // allocate memory to contain file data
    char* buffer = new char[fileSize];
    // get file data
    pbuf->sgetn(buffer, fileSize);
    ifs.close();

    //enconding
    auto total = fileSize / this->capacity;
    UInt32 left = fileSize % this->capacity;
    if (left != 0)
        total += 1;
    //output
    for (int i = 0; i < total; i++) {
        bool islast = ((i == total) && (left!=0));
        cv::Mat mat(32, 32, CV_8UC3, cv::Scalar::all(168));
        //write head
        auto pData = mat.data;
        *pData = 0;
        pData += 1;
        UShort* pWidth = (UShort*)pData;
        *pWidth = this->width;
        pData += 2;
        UShort* pHeight = (UShort*)pData;
        *pHeight = this->height;
        pData += 2;
        UShort* pIndex = (UShort*)pData;
        *pIndex = i;
        pData += 2;
        UShort* pTotal = (UShort*)pData;
        *pTotal = total;
        pData += 2;
        UInt32* pLength = (UInt32*)pData;
        if (islast)
            *pLength = left;
        else
            *pLength = this->capacity;
        pData += 4;
        //write data
        if (islast)
            memcpy(pData, buffer, left);
        else
            memcpy(pData, buffer, this->capacity);
        //write end
        pData += this->capacity;
        *pData = 0xff;
        pData += 1;
        pData = 0;
        //save to file
        std::ostringstream imageName;
        imageName << i << '-' << total << ".bmp";
        cv::imwrite(imageName.str(), mat);
    }
}

void Convert::Decoding() {

}
