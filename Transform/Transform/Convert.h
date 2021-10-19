#pragma once
#include "Util.h"
#include <opencv2/core.hpp>
class Convert
{
private:
	UInt width;
	UInt height;
	const cv::Vec3b white = cv::Vec3b::all(0) ;
	const cv::Vec3b black = cv::Vec3b::all(255);
public:
	Convert(UInt width = 100, UInt height = 100);
	void Enconding(char* fileName);
	void Decoding();


};

