
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/calib3d.hpp>


using namespace std;
using namespace cv;

void check(cv::Mat points_3d)
{
    size_t rows = points_3d.rows;
    size_t cols = points_3d.cols;
    size_t channels = points_3d.channels();
    std::cout << "rows: " << rows << ", cols: " << cols << ", channels: " << channels << std::endl;    
}

int main(int argc, char** argv) {
    if(argc != 1) {
        std::cout << " argc not match, Usage: " << std::endl;
        return -1;
    }

    std::cout << "running: " << argv[0] << std::endl;
    
    string root_path = "/home/antenna/Desktop/parking_data/cali_test";

    string intrinsic_dir = root_path + "/intrinsic-calib.yml";
    string extrinsic_dir = root_path + "/extrinsic-calib.yml";
    string points_dir = root_path + "/2d3dpoints.yml";

    // read 2d-3d points
    cv::FileStorage fs(points_dir, FileStorage::READ);
    cv::Mat points_2d, points_3d;
    fs["imgpoints"] >> points_2d;
    fs["objpoints"] >> points_3d;
    fs.release();

    // read intrinsic
    cv::FileStorage fs_intrinsic(intrinsic_dir, FileStorage::READ);
    cv::Mat K, D;
    fs_intrinsic["camera_matrix"] >> K;
    fs_intrinsic["distortion_coefficients"] >> D;
    fs_intrinsic.release();

    // read extrinsic
    // cv::FileStorage fs_extrinsic(extrinsic_dir, FileStorage::READ);
    // cv::Mat rvec, tvec;
    // fs_extrinsic["rvec"] >> rvec;
    // fs_extrinsic["tvec"] >> tvec;
    // fs_extrinsic.release();

    size_t cols = points_3d.cols;

    check(points_2d);
    // check(points_3d);
    
    points_3d = points_3d.reshape(1, cols);  // reshape(channel, rows), the cols can be determanted automatically
    check(points_3d);

    // std::cout << points_3d << std::endl;
    cv::Point3d test_point =  points_3d.at<cv::Point3d>(0);
    std::cout << "test_point: " << test_point << std::endl;
    
    // objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec, 
    // useExtrinsicGuess = false, iterationsCount = 100,
    // reprojectionError = 8.0, confidence = 0.99, inliers = noArray(), 
    // flags = SOLVEPNP_ITERATIVE

    cv::Mat rvec, tvec;
    cv::Mat inliers;
    bool solved = cv::solvePnPRansac(points_3d, points_2d, K, D, rvec, tvec, false, 100, 8.0F, 0.99, inliers, SOLVEPNP_ITERATIVE);

    std::cout << "tvec: \n" << tvec << std::endl;

    return 0;
}
