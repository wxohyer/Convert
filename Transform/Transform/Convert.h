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
	
	void Enconding(const char* fileName);
	void static Refining(const char* dataPath);
	/// <summary>
	///  Default
	/// </summary>
	/// <returns></returns>
	inline static Convert Defualt() {
		return Convert(256, 256);
	}
	/// <summary>
	///  match head
	/// </summary>
	/// <returns></returns>
	inline static bool Match(const uchar* ptr) {
		if (*ptr == 0x00 && *(ptr + 3) == 0xff
			&& *(ptr + 6) == 0x00
			&& *(ptr + 9) == 0xff
			&& *(ptr + 12) == 0x00) {
			auto width = *(UShort*)(ptr + 1);
			auto height = *(UShort*)(ptr + 4);
			auto capacity = static_cast<size_t>(width) * static_cast<size_t>(height) * 3;
			if (*(ptr + capacity - 1) == 0x00
				&& *(ptr + capacity - 2) == 0xff) {
				return true;
			}else {
				return false;
			}
		}else {
			return false;
		}
	}
};

