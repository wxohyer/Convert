#include "opencv2/core.hpp"
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <fstream>
#include "Transform.h"
#include "Convert.h"
#include "CLI11.hpp"
using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    CLI::App app("Transform data to data table");

    int width = 0;
    app.add_option("-w,--width", width, "an integer value")->capture_default_str();

    bool flag;
    app.add_flag("-f,--flag", flag, "a flag option");

    CLI11_PARSE(app, argc, argv);
    std::cout << width << endl;
    auto convert = Convert::Defualt();
    convert.Enconding("transform.rar");

    return 0;
}

//int main(int argc, char* argv[]) {
//	/*wchar_t curPath[512];
//	auto path = _wgetcwd(curPath,512);
//	std::wcout << curPath << std::endl;
//	Mat mat(128, 128, CV_8UC3, Scalar::all(168));
//	auto pData = mat.data;
//	for (int i = 0; i < 100; i++) {
//		pData[i] = i;
//	}
//	auto size = mat.size;
//	imwrite("test.bmp", mat);
//	imshow("test.bmp", mat);
//	waitKey(0);*/
//	return 0;
//}

//int main(int argc, char* argv[])
//{
//    for (int i = 0; i < argc; i++) {
//        std::cout << argv[i] << endl;
//    }
//    std::cout << "Transform!\n";
//    Transform tt;
//    Transform&& transform = Transform::Default();
//    std::cout << "Bytes:" << transform.bytes() << endl;
//    //tansform.convert(nullptr, 0, "test.bmp");
//    std::ifstream ifs("Transform.rar", std::ifstream::binary);
//
//    // get pointer to associated buffer object
//    std::filebuf* pbuf = ifs.rdbuf();
//
//    // get file size using buffer's members
//    std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
//    pbuf->pubseekpos(0, ifs.in);
//
//    // allocate memory to contain file data
//    char* buffer = new char[size];
//
//    // get file data
//    pbuf->sgetn(buffer, size);
//
//    ifs.close();
//
//    // write content to stdout
//    // std::cout.write(buffer, size);
//
//    if (transform.bytes() > size) {
//        transform.convert((PByte)buffer, size, "Image1.png");
//    }
//
//    delete[] buffer;
//}

//std::ifstream ifs("test.txt", std::ifstream::binary);
//
//// get pointer to associated buffer object
//std::filebuf* pbuf = ifs.rdbuf();
//
//// get file size using buffer's members
//std::size_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
//pbuf->pubseekpos(0, ifs.in);
//
//// allocate memory to contain file data
//char* buffer = new char[size];
//
//// get file data
//pbuf->sgetn(buffer, size);
//
//ifs.close();
//
//// write content to stdout
//std::cout.write(buffer, size);
//
//delete[] buffer;
