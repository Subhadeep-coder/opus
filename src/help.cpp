#include<iostream>

void print_usage() {
    std::cout << "Usage: opus -h <host> -U <username>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h <host>      Server host address" << std::endl;
    std::cout << "  -U <username>  Username for authentication" << std::endl;
}