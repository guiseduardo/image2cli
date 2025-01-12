#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

#include "utils.hpp"

int main(int argc, char* argv[])
{
    if (argc <= 1)
        throw std::runtime_error("An image path is required");

    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    std::cout << std::endl;
    image2cli::print_image_in_console(argv[1], size.ws_col, size.ws_row - 1);
    return EXIT_SUCCESS;
}
