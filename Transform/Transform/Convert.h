#pragma once
#include "Util.h"
#include <opencv2/core.hpp>
class Convert
{
private:
	UInt32 width;
	UInt32 height;
	std::size_t capacity;

	const cv::Vec3b white = cv::Vec3b::all(0) ;
	const cv::Vec3b black = cv::Vec3b::all(255);
public:
	Convert(UShort width = 100, UShort height = 100);
	bool Match(const uchar* ptr);
	void Enconding(const char* fileName);
	void Decoding();
	//static method
	inline static Convert Defualt() {
		return Convert(256, 256);
	}
};

