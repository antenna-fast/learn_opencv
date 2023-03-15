// we want to recover camera pose from feature matching

#include <iostream>
#include <fstream>
#include <vector>

#include "opencv2/core.hpp"
#include <opencv2/core/mat.hpp>
// #ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
// #include "opencv2/features2d.hpp"
// #include "opencv2/xfeatures2d.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;


template <typename T>
bool read_txt(std::string file_path, vector<vector<T>>& data, size_t num_per_line)
{
    ifstream in_file;
    in_file.open(file_path);

    if(in_file.fail())
    {
        cout << "failed to open file: " << file_path << endl;
        return false;
    }

    string line;
    while (!in_file.eof())
    {
        getline(in_file, line);
        if(line.empty())
        {
            break;
        }
        // cout << "line: " << line << " : ";
        istringstream str_line(line);
        string str;

        vector<T> vLineData(num_per_line);

        for (size_t i = 0; i < num_per_line; i++)
        {
            getline(str_line, str, ' ');
            // cout << " " << str << " ";
            vLineData.at(i) = std::stof(str);
            // vLineData.at(i) = std::stod(str);
        }
        // cout << vLineData.at(0) << " " << vLineData.at(1) << endl;
        data.emplace_back(vLineData); 
    }
    return true;
}


int main(int argc, char** argv) {
    if(argc != 1){
        cout << " argc not match, Usage: " << endl;
        return -1;
    }

    std::cout << "running: " << argv[0] << std::endl;
    
    // load camera intrinsic
    string camera_intrinsic_path = "/home/antenna/ssd/Dataset/KITTI/data_odometry_gray/dataset/sequences/00/calib.txt";
    float fx = 718.856;
    float fy = 718.856;
    float cx = 607.1928;
    float cy = 185.2157;
    cv::Mat K(3, 3, CV_32F);
    K.at<float>(0, 0) = fx;
    K.at<float>(1, 1) = fy;
    K.at<float>(0, 2) = cx;
    K.at<float>(1, 2) = cy;

    // load 2d points
    string kps_file_path = "/home/antenna/App/Learn/learn_opencv/feature/build/left_right_kps.txt";

    ifstream kps;
    kps.open(kps_file_path);

    vector<vector<float>> vvKps;
    read_txt(kps_file_path, vvKps, 4);
    
    cv::Mat left_kps(vvKps.size(), 2, CV_32F);  // row, col, type
    cv::Mat right_kps(vvKps.size(), 2, CV_32F);

    for (size_t i = 0; i < vvKps.size(); i++)
    {
        // cout << vvKps.at(i).at(0) << endl;
        left_kps.at<float>(i, 0) = vvKps.at(i)[0];
        left_kps.at<float>(i, 1) = vvKps.at(i)[1];
        right_kps.at<float>(i, 0) = vvKps.at(i)[2];
        right_kps.at<float>(i, 1) = vvKps.at(i)[3];
        // cout << "left_kps x: " << left_kps.at<float>(i, 0) << ", y: " << left_kps.at<float>(i, 1) << ", right: x: " << right_kps.at<float>(i, 0) << ", y: " << right_kps.at<float>(i, 1) << endl;
    }

    // recover pose by homography matrix model
    // cv::findHomography();

    // recover pose by essential matrix(contain camera intrinsic)
    // points1, points2, cameraMatrix, method=RANSAC, prob = 0.999, double threshold = 1.0, mask = noArray() 
    cv::Mat E = cv::findEssentialMat(left_kps, right_kps, K);
    cout << "E: \n" << E << endl;

    cv::Mat R1, R2, t;
    cv::decomposeEssentialMat(E, R1, R2, t);

    cout << "R1: \n" << R1 << endl;
    cout << "t: \n" << t << endl;

    FileStorage fs("relative_camera_pose.yml", FileStorage::WRITE);
    // or:
    // FileStorage fs;
    // fs.open(filename, FileStorage::WRITE);
    fs << "K" << K;     // cv::Mat
    fs << "R" << R1;    // cv::Mat
    fs << "t" << t;
    fs.release();       // explicit close
    // cout << "Write Done." << endl;
    // recover pose by fundamental matrix model
    // without camera intrinsic
    // cv::findFundamentalMat();

    return 0;
}