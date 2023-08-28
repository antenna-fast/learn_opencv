#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    if(argc != 1) {
        std::cout << " argc not match, Usage: " << std::endl;
        return -1;
    }

    std::cout << "running: " << argv[0] << std::endl;

    // read the first image
    string img_root_path = "/home/antenna/ssd/Dataset/KITTI/data_odometry_gray/dataset/sequences/00";
    string first_img_path = img_root_path + "/" + "image_0/" + "000000.png";
    string second_img_path = img_root_path + "/" + "image_1/" + "000000.png";

    cv::Mat first_img = cv::imread(first_img_path);
    if(first_img.empty())
    {
        std::cout << "first_img.empty! " << std::endl;
        return -1;
    }

    cv::Mat second_img = cv::imread(second_img_path);
    if(second_img.empty())
    {
        std::cout << "second_img.empty! " << std::endl;
        return -1;
    }

    cv::Mat first_img_gray;
    cv::Mat second_img_gray;

    // cv::cvtColor(first_img, first_img_gray, CV_RGB2GRAY);
    cv::cvtColor(first_img, first_img_gray, COLOR_RGB2GRAY);
    cv::cvtColor(second_img, second_img_gray, COLOR_RGB2GRAY);

    vector<cv::Point2f> keypoints_1, keypoints_2, pointCopy;
    size_t num_keypoints = 100;
    size_t MIN_DIST = 10;
    cv::Mat mask;

    cv::goodFeaturesToTrack(first_img_gray, keypoints_1, num_keypoints, 0.01, MIN_DIST, mask);
    // draw keypoints
    for (int i = 0; i < keypoints_1.size(); i++)  
    {
        cv::circle(first_img, keypoints_1[i], 1, cv::Scalar(0, 0, 255), 2);
    }
    cv::imwrite("good_features.jpg", first_img);


    vector<uchar> status;
    vector<float> err;
    // prevImg, nextImg, InputArray prevPts, 
    // InputOutputArray nextPts, OutputArray status, OutputArray err,

    // status: output status vector (of unsigned chars); each element of the vector is set to 1 if
    //         the flow for the corresponding features has been found, otherwise, it is set to 0.
    cv::calcOpticalFlowPyrLK(first_img_gray, second_img_gray, keypoints_1, keypoints_2, status, err, cv::Size(21, 21), 3);
    // draw keypoints
    for (int i = 0; i < keypoints_2.size(); i++)  
    {
        if(status[i])  // track success
        {
            cv::circle(second_img, keypoints_2[i], 1, cv::Scalar(0, 0, 255), 2);
        }
    }
    cv::imwrite("good_features_track.jpg", second_img);

    return 0;
}