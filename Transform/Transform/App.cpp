
#include "App.h"
#include "opencv2/core.hpp"
#include <opencv2/imgcodecs.hpp>
#include "CLI11.hpp"
#include "Convert.h"
#include <iostream>

/// <summary>
///  en --width 128 --height 128 --file design.rar
///  re --filepath ./output
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int cli(int argc, char** argv) {
    CLI::App app("Transform data to data table");
    bool flag;
    app.add_flag("-f,--flag", flag, "a flag option");

    auto display = app.add_subcommand("display","Display data image");

    auto enconding = app.add_subcommand("en", "enconding data");
    //Enconding
    int width = 0;
    enconding->add_option("-w,--width", width, "Image width");
    int height = 0;
    enconding->add_option("-t,--height", height, "Image width");
    std::string file;
    enconding->add_option("-f,--file", file, "file data");
    enconding->callback([&]() {
        if (width != 0 && height != 0) {
            std::cout << "Image width:" << width << " Image height:" << height << std::endl;
            if (!file.empty()) {
                std::cout << "File:" << file << std::endl;
                Convert convert(width, height);
                convert.Enconding(file.c_str());
            }
        }
    });
    //Refining
    auto refining = app.add_subcommand("re", "refining data");
    std::string filePath;
    refining->add_option("-f,--filepath", filePath, "file path");
    refining->callback([&]() {
        if (!filePath.empty()) {
            Convert::Refining(filePath.c_str());
        }
    });

    CLI11_PARSE(app, argc, argv);
   
    return 0;
}

bool match(const char* path) {
    return true;
}


//export module App;
//import <iostream>;
//import <fstream>;
//import <opencv2/core.hpp>;
//import <opencv2/imgcodecs.hpp>;
//
//import "CLI11.hpp";

//#include "opencv2/core.hpp"
//#include <opencv2/imgcodecs.hpp>
//#include <fstream>

//#include "CLI11.hpp"
//using namespace std;
//using namespace cv;
//namespace App {
//    export int cli(int argc, char** argv) {
//        CLI::App app("Transform data to data table");
//        //cv::Mat mat;
//
//        int width = 0;
//        app.add_option("-w,--width", width, "an integer value")->capture_default_str();
//
//        bool flag;
//        app.add_flag("-f,--flag", flag, "a flag option");
//        std::cout << "test";
//
//        CLI11_PARSE(app, argc, argv);
//        std::cout << "OKOKOK" << std::endl;
//        return 0;
//    }
//}
