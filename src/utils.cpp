#include "utils.hpp"

std::string image2cli::rgb_to_foreground(int r, int g, int b)
{
    std::stringstream ss;
    ss << "\033[38;2;" << r << ';' << g << ';' << b << "m";
    return ss.str();
}

std::string image2cli::rgb_to_background(int r, int g, int b)
{
    std::stringstream ss;
    ss << "\033[48;2;" << r << ';' << g << ';' << b << "m";
    return ss.str();
}

void image2cli::print_image_in_console(std::string image_path, size_t max_width, size_t max_height)
{
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);

    if (image.empty())
        throw std::runtime_error("Could not open or find the image.");

    float scale = std::min(2 * (float)max_height / (float)image.rows, (float)max_width / (float)image.cols);

    cv::Mat fitted_image;
    cv::resize(image, fitted_image, cv::Size(scale * (float)image.cols, scale * (float)image.rows));

    /*-----------------------------------------------*/

    std::cout << "\r";
    for (int rowp = 0; rowp < fitted_image.rows; rowp += 2)
    {
        for (int col = 0; col < fitted_image.cols; ++col)
        {
            cv::Vec3b p1 = fitted_image.at<cv::Vec3b>(rowp, col);
            cv::Vec3b p2 = fitted_image.at<cv::Vec3b>(rowp + 1, col);
            std::cout << rgb_to_foreground(p1[2], p1[1], p1[0]) << rgb_to_background(p2[2], p2[1], p2[0]) << "▀";
        }
        std::cout << "\033[0m" << std::endl;
    }

    if (fitted_image.rows % 2 == 1)
    {
        int row = fitted_image.rows - 1;
        for (int col = 0; col < fitted_image.cols; ++col)
        {
            cv::Vec3b p = fitted_image.at<cv::Vec3b>(row, col);
            std::cout << rgb_to_foreground(p[2], p[1], p[0]) << "▀";
        }
        std::cout << "\033[0m" << std::endl;
    }
}
