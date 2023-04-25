#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
// #include "opencv2/features2d.hpp"
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;


int main(){
    cout << "test Mat" << endl;

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
    cv::Mat a(raw, col, CV_32F);
    a.at<float>(1, 0) = 10.0;

    cout << "a: \n" << a << endl;

    return 0;
}
