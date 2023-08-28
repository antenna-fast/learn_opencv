#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;


int main(){
    cout << "test xml" << endl;

    // generate points
    const int raw = 5;
    const int col = 3;
    
    /*
    Matrix row/col define: 
         col1 col2 ... col  (y)
    row1
    row2
    .
    .
    row
    
    (x)
    */
    cv::Mat a(raw, col, CV_32FC1);
    a.at<float>(1, 0) = 1000.0;
    cout << "a: \n" << a << endl;

    // write yaml
    FileStorage fs_write("test.xml", FileStorage::WRITE);
    fs_write << "mat_a" << a;
    fs_write.release();

    // read yaml
    FileStorage fs("test.xml", FileStorage::READ);
    Mat mat_a;
    fs["mat_a"] >> mat_a;
    fs.release();

    std::cout << "mat_a: \n" << mat_a << std::endl;
    
    return 0;
}
