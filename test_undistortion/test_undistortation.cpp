#include <iostream>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080


int main(int argc, char** argv) {
    if(argc != 1) {
        std::cout << " argc not match, Usage: " << std::endl;
        return -1;
    }

    std::cout << "running: " << argv[0] << std::endl;
    
    string img_path;
    img_path = "/home/antenna/ssd/ImageProcess/pic_undisto/qingdao/10_2/00001.png";

    cv::Mat img = cv::imread(img_path);
    if(img.empty())
    {
        std::cout << "Empty image, check your file: " << img_path << std::endl;
        return -1;
    }
    cv::imwrite("img.png", img);


    // load camera intrinsic and distortation parameter

// cam_10
// [2294.1510727205114, 2280.798626201654, 971.7575562402385, 563.281062493135]
    cv::Mat cameraMatrix(3, 3, CV_32F);
    cameraMatrix.at<float>(0, 0) = 2294.1510727205114;  // fx  img_shape[1]/2/1.3
    cameraMatrix.at<float>(1, 1) = 2280.798626201654;  // fy  img_shape[0]/2
    cameraMatrix.at<float>(0, 2) = 971.7575562402385;  // px
    cameraMatrix.at<float>(1, 2) = 563.281062493135;  // py
    cameraMatrix.at<float>(2, 2) = 1; 

// cam_10
// [-0.780052432125019, 0.9602412619423992, -0.001122695641929897, -0.0024221898460297076]

    cv::Mat distCoeffs(4, 1, CV_32F);
    distCoeffs.at<float>(0, 0) = -0.780052432125019;
    distCoeffs.at<float>(1, 0) = 0.9602412619423992;
    distCoeffs.at<float>(2, 0) = -0.001122695641929897;
    distCoeffs.at<float>(3, 0) = -0.0024221898460297076;

    // image un-distortation

    // Method 1: cv::undistort,  this is too slow! 
    cv::Mat img_undisto;
    cv::undistort(img, img_undisto, cameraMatrix, distCoeffs);
    // cv::undistort(img_ori.getUMat(ACCESS_READ), img_undisto, cameraMatrix, distCoeffs);
    cv::imwrite("undisto_0.png", img_undisto);

    // Method 2: remap
    // cameraMatrix, distCoeffs, R, newCameraMatrix, size, m1type, map1, 
    cv::Mat img_undisto_2;

    cv::Size size(FRAME_WIDTH, FRAME_HEIGHT);
    cv::Mat map1, map2;
    
    cv::Matx33f R_tmp;
    for (size_t i = 0; i < 3; i++)
    {
        R_tmp(i, i) = 1;
    }
    std::cout << "R_tmp: \n" << R_tmp << std::endl;

    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, R_tmp, cameraMatrix, size, 0, map1, map2);
    
    // InputArray src, OutputArray dst, InputArray map1, InputArray map2, int interpolation, 
    cv::remap(img, img_undisto_2, map1, map2, cv::INTER_LINEAR);

    std::cout << "map1: " << map1 << std::endl;
    
    cv::imwrite("undisto_remap.png", img_undisto_2);

    return 0;
}