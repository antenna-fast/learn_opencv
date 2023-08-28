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

    string img_path = "/home/antenna/ssd/ImageProcess/pic_undisto/res_rt/018739.jpg"; 
    
    cv::UMat img;
    // cv::UMat gain_map(img.size(), );

    img = cv::imread(img_path).getUMat(ACCESS_READ);


    return 0;
}
