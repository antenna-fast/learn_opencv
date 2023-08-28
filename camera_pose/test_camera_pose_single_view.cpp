// we want to recover camera pose from feature matching

#include <iostream>
#include <fstream>
#include <vector>
// #include <assert>

#include "opencv2/core.hpp"
#include <opencv2/core/mat.hpp>
// #ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
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


// TODO: image coordinate -> unique focal coordinate
// x = X / Z
// y = Y / Z
void world_to_cam(Point3d &world_point )
{

}


// TODO: decompose Essential matrix


// Calculates rotation matrix to euler angles
// The result is the same as MATLAB except the order
// of the euler angles ( x and z are swapped ).
Vec3f rotationMatrixToEulerAngles(Mat &R)
{
    // assert(isRotationMatrix(R));
 
    float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0) );
 
    bool singular = sy < 1e-6; // If
 
    float x, y, z;
    if (!singular)
    {
        x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
        y = atan2(-R.at<double>(2,0), sy);
        z = atan2(R.at<double>(1,0), R.at<double>(0,0));
    }
    else
    {
        x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
        y = atan2(-R.at<double>(2,0), sy);
        z = 0;
    }
    return Vec3f(x, y, z);
}

int main(int argc, char** argv)
{
    if(argc != 1)
    {
        std::cout << " argc not match, Usage: " << std::endl;
        return -1;
    }

    std::cout << "running: " << argv[0] << std::endl;
    
    // load camera intrinsic
    std::string camera_intrinsic_path = "/home/antenna/ssd/Dataset/KITTI/data_odometry_gray/dataset/sequences/00/calib.txt";
    float fx = 718.856;
    float fy = 718.856;
    float cx = 607.1928;
    float cy = 185.2157;
    cv::Mat K(3, 3, CV_32F);  // wor, col
    K.at<float>(0, 0) = fx;
    K.at<float>(1, 1) = fy;
    K.at<float>(0, 2) = cx;
    K.at<float>(1, 2) = cy;
    K.at<float>(2, 2) = 1;
    std::cout << "K: \n" << K << std::endl;

    // load 2d points
    std::string kps_file_path = "/home/antenna/App/Learn/learn_opencv/feature/build/left_right_kps.txt";

    std::ifstream kps;
    kps.open(kps_file_path);

    std::vector<std::vector<float>> vvKps;
    read_txt(kps_file_path, vvKps, 4);
    
    cv::Mat left_kps(vvKps.size(), 2, CV_32F);  // (row, col, type)
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
    std::cout << "E: \n" << E << std::endl;

    cv::Mat R1, R2, t;
    // cv::decomposeEssentialMat(E, R1, R2, t);  // Note: coordinate NOT right!

    // Mat E, R, t;
    cv::Mat mask;
    // E = findEssentialMat(alignedLeft.points, alignedRight.points, focal, pp, RANSAC, 0.999, 1.0, mask);

    //Find Pright camera matrix from the essential matrix
    //Cheirality check (all points are in front of camera) is performed internally.
    double focal = fx;
    Point2d pp(cx, cy);
    cv::recoverPose(E, left_kps, right_kps, R1, t, focal, pp, mask);  // Note: coordinate OK!

    std::cout << "R1: \n" << R1 << endl;
    // cout << "R2: \n" << R2 << endl;
    std::cout << "t: \n" << t << endl;

    cv::Vec3f angles = rotationMatrixToEulerAngles(R1);
    std::cout << "angles: " << angles << std::endl;

    // cv::Vec3f angles_2 = rotationMatrixToEulerAngles(R2);
    // std::cout << "angles2: " << angles_2 << std::endl;

    cv::FileStorage fs("relative_camera_pose.yml", cv::FileStorage::WRITE);
    // or:
    // FileStorage fs;
    // fs.open(filename, FileStorage::WRITE);
    fs << "K" << K;     // cv::Mat
    fs << "R" << R1;    // cv::Mat
    // fs << "R2" << R2;    // cv::Mat

    // Mat t to t_vec
    cv::Vec3f t_vec;
    for (size_t i = 0; i < 3; i++)
    {
        t_vec[i] = t.at<double>(i, 0);
    }
    
    fs << "t" << t_vec;

    fs << "angles" << angles;    
    fs.release();       // explicit close
    std::cout << "Write Done." << endl;
    
    // recover pose by fundamental matrix model
    // without camera intrinsic
    // cv::findFundamentalMat();

    return 0;
}