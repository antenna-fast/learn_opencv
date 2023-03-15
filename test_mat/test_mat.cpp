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
    const int raw = 100;
    const int col = 3;
    
    cv::Mat a(raw, col, CV_32F);
    
    return 0;
}
