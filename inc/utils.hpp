#ifndef IMAGE2CLI_UTILS_HPP
#define IMAGE2CLI_UTILS_HPP

#include <iostream>
#include <string>
#include <iomanip>

#include <opencv2/opencv.hpp>

namespace image2cli
{
    std::string rgb_to_foreground(int r, int g, int b);
    std::string rgb_to_background(int r, int g, int b);
    void print_image_in_console(std::string image_path, size_t max_width, size_t max_height);
}

#endif
