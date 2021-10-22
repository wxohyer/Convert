#include "Convert.h"
#include <opencv2/imgcodecs.hpp>
#include <map>
#include <tuple>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

using namespace std;
using namespace std::filesystem;
using namespace cv;

/// <summary>
///  constructor
///  ¡öwith¡õheight¡öindex¡õtotal¡ölength end¡õ¡ö
/// </summary>
/// <param name="width">¿í</param>
/// <param name="height">¸ß</param>
Convert::Convert(UShort width, UShort height) {
	this->width = width;
	this->height = height;
	this->capacity = static_cast<size_t>(this->width) * static_cast<size_t>(this->height) * 3 - (1 + 2 + 1 + 2 + 1 + 2 + 1 + 2 + 1 + 4 + 2);
}

///// <summary>
/////  match header
///// </summary>
///// <param name="ptr"></param>
///// <returns></returns>
//bool Convert::Match(const uchar* ptr) {
//	
//}

/// <summary>
///  enconding
/// </summary>
/// <param name="fileName"></param>
void Convert::Enconding(const char* fileName) {
	//path
	path output(".\\output");
	if (!exists(output)) {
		cout << "not exit directory" << endl;
		create_directory(output);
	}else {
		remove_all(output);
		create_directory(output);
	}

	//std::ifstream ifs(fileName, std::ifstream::binary);
	//// get pointer to associated buffer object
	//std::filebuf* pbuf = ifs.rdbuf();

	//// get file size using buffer's members
	//std::size_t fileSize = pbuf->pubseekoff(0, ifs.end, ifs.in);
	//pbuf->pubseekpos(0, ifs.in);

	//// allocate memory to contain file data
	//char* buffer = new char[fileSize];
	//auto bufferPos = buffer;
	//// get file data
	//pbuf->sgetn(buffer, fileSize);
	//ifs.close();

	////enconding
	//auto total = fileSize / this->capacity;
	//UInt32 left = fileSize % this->capacity;
	//if (left != 0)
	//	total += 1;
	//output
	//for (int i = 0; i < total; i++) {
	//	bool islast = ((i == total-1) && (left != 0));
	//	cv::Mat mat(this->width, this->height, CV_8UC3, cv::Scalar::all(168));
	//	//write head
	//	auto pData = mat.data;
	//	*pData = 0;
	//	pData += 1;
	//	UShort* pWidth = (UShort*)pData;
	//	*pWidth = this->width;
	//	pData += 2;
	//	*pData = 0xff;
	//	pData += 1;
	//	UShort* pHeight = (UShort*)pData;
	//	*pHeight = this->height;
	//	pData += 2;
	//	*pData = 0x00;
	//	pData += 1;
	//	UShort* pIndex = (UShort*)pData;
	//	*pIndex = i;
	//	pData += 2;
	//	*pData = 0xff;
	//	pData += 1;
	//	UShort* pTotal = (UShort*)pData;
	//	*pTotal = total;
	//	pData += 2;
	//	*pData = 0x00;
	//	pData += 1;
	//	UInt32* pLength = (UInt32*)pData;
	//	if (islast)
	//		*pLength = left;
	//	else
	//		*pLength = this->capacity;
	//	pData += 4;
	//	auto dataLength = this->capacity;
	//	if (islast)
	//		dataLength = left;
	//	//Wriete data
	//	memcpy(pData, bufferPos, dataLength);
	//	bufferPos += dataLength;
	//	//Write end
	//	pData += this->capacity;
	//	*pData = 0xff;
	//	pData += 1;
	//	*pData = 0;
	//	//save to file
	//	std::ostringstream imageName;
	//	imageName << ".\\output\\" << i << '-' << total << ".bmp";
	//	cv::imwrite(imageName.str(), mat);
	//}
	//delete []buffer;
}
 /// <summary>
 ///  Refining
 /// </summary>
 /// <param name="dataPath"></param>
 void Convert::Refining(const char* dataPath) {
	path dir(dataPath);
	if (!exists(dir))
		return;
	directory_entry entry(dir);
	if (entry.status().type() != file_type::directory)
		return;

	std::map<int, std::tuple<char*,int>> datas;
	directory_iterator list(dir);
	for (auto& it : list) {
		std::cout << it.path().filename() << std::endl;
		auto img = cv::imread(it.path().string());
		if (!img.empty()) {
			//scan image
			auto ptr = img.data;
			while (ptr != img.dataend)
			{
				if (Convert::Match(ptr)) {
					auto index = *(UShort*)(ptr + 7);
					auto datasize = *(UInt32*)(ptr + 13);
					char* buffer = new char[datasize];
					memcpy(buffer, ptr + 17, datasize);
					datas.insert(pair<int, tuple<char*, int>>(index, make_tuple(buffer, datasize)));
					break;
				}
				else {
					ptr++;
				}
			}
		}
	}
	std::ofstream ofs("./refining.data", ofstream::binary);
	if (!ofs.is_open())
		return;
	for (auto& it : datas) {
		std::cout << it.first;
		ofs.write(std::get<0>(it.second), std::get<1>(it.second));
		delete[] get<0>(it.second);
	}
	ofs.close();
}

 