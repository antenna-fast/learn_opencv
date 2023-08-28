#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
// #include "opencv2/features2d.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


int main(){
    cout << "test Mat" << endl;

    // 1. define mat data
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
    a.at<float>(1, 0) = 1000.0;  // at<type>(row, col)
    std::cout << "a: \n" << a << std::endl;

    // 2. mat transpose
    cv::Mat aT;
    cv::transpose(a, aT);
    std::cout << "aT: \n" << aT << std::endl;

    // 3. matrix production
    // Matrix * Matrix
    Mat mat_multiply = a * aT;
    std::cout << "mat_multiply: \n" << mat_multiply << std::endl;

    // Matrix * Vector
    cv::Mat v(3, 1, CV_32FC1);
    v.at<float>(0, 0) = 1;
    v.at<float>(1, 0) = 1;
    v.at<float>(1, 0) = 1;

    std::cout << a * v << std::endl;

    // 4. change channel
    // methord 1: cvtColor
    Mat b;
    // a.convertTo(b, CV_32FC3);  // NOTE: it will only change data type, but NOT channels
    cv::cvtColor(a, b, cv::COLOR_GRAY2BGR);       // 1. change the number of channels
    b.convertTo(b, CV_32FC3); // 2. change type to float and preserve scale
    // b.convertTo(b, CV_32FC3, 1/255.0); // 2. change type to float and scale    

    cout << "b: \n" << b << endl;
    cout << "b.channels: \n" << b.channels() << endl;

    // methord 2: merge channel
    cv::Mat out(a.size(), CV_32FC3);
    cv::Mat in[] = {a, a, a};
    cv::merge(in, 3, out);
    cout << "out: \n" << out << endl;
    cv::imwrite("t.png", out);

    Mat img = cv::imread("t.png");
    cout << "img: \n" << img << endl;

    return 0;
}
