#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    if(argc != 1) {
        std::cout << " argc not match, Usage: " << std::endl;
        return -1;
    }

    std::cout << "running: " << argv[0] << std::endl;

    
    // matches_info.H = findHomography(src_points, dst_points, RANSAC);

    return 0;
}