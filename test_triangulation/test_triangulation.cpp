#include <iostream>
#include <vector>
#include <fstream>
#include "opencv2/core.hpp"

#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/sfm/triangulation.hpp>

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

void img2cam(Point2d img_point, Mat K)
{
}


int main(int argc, char** argv)
{
    if(argc != 1)
    {
        cout << " argc not match, Usage: " << endl;
        return -1;
    }

    std::cout << "running: " << argv[0] << std::endl;
    
    // load camera intrinsic (to project image points to unit focal plane)
    string camera_intrinsic_path = "../../camera_pose/build/relative_camera_pose.yml";

    string res_path = "points.txt";
    ofstream res;
    res.open(res_path);

    FileStorage fs;
    fs.open(camera_intrinsic_path, FileStorage::READ);
    Mat K, R, t;
    fs["K"] >> K;
    fs["R"] >> R;   // Read cv::Mat
    fs["t"] >> t;
    
    cout << "K: \n" << K << endl;
    cout << "camera 2 R: \n" << R << endl;

    // load 2d image key points
    string kps_file_path = "/home/antenna/App/Learn/learn_opencv/feature/build/left_right_kps.txt";
    ifstream kps;
    kps.open(kps_file_path);

    vector<vector<float>> vvKps;
    read_txt(kps_file_path, vvKps, 4);
    
    // cv::Mat left_kps(vvKps.size(), 2, CV_32F);  // row, col, type
    // cv::Mat right_kps(vvKps.size(), 2, CV_32F);
    vector<Point2f> left_kps, right_kps;
    for (size_t i = 0; i < vvKps.size(); i++)
    {
        Point2f p;
        // pixel coordinate -> unique plane
        p.x = (vvKps.at(i)[0] - K.at<float>(0, 2)) / K.at<float>(0, 0);
        p.y = (vvKps.at(i)[1] - K.at<float>(1, 2)) / K.at<float>(1, 1);
        left_kps.push_back(p);

        p.x = (vvKps.at(i)[2] - K.at<float>(0, 2)) / K.at<float>(0, 0);
        p.y = (vvKps.at(i)[3] - K.at<float>(1, 2)) / K.at<float>(1, 1);
        right_kps.push_back(p);

        // cout << "left_kps x: " << left_kps.at<float>(i, 0) << ", y: " << left_kps.at<float>(i, 1) << ", right: x: " << right_kps.at<float>(i, 0) << ", y: " << right_kps.at<float>(i, 1) << endl;
    }

    // load camera pose
    cv::Mat T1 = (Mat_<float>(3, 4) <<
		1.0, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0);
    cout << "T1: \n" << T1 << endl;

    cv::Mat T2 = (Mat_<float>(3, 4) <<
		R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2), t.at<double>(0),
		R.at<double>(1, 0), R.at<double>(1, 1), R.at<double>(1, 2), t.at<double>(1),
		R.at<double>(2, 0), R.at<double>(2, 1), R.at<double>(2, 2), t.at<double>(2));
    cout << "T2: \n" << T2 << endl;

    // build 4x4 projection matrix
    // projection_matrix = K * [R|t]
    cv::Mat leftProjectionMatrix(3, 4, CV_32F);
    cv::Mat rightProjectionMatrix(3, 4, CV_32F);

    // leftProjectionMatrix = K * T1;
    // rightProjectionMatrix = K * T2;

    // triangulation
    Mat points_4d;
    // cv::triangulatePoints(leftProjectionMatrix, rightProjectionMatrix, left_kps, right_kps, points_4d);
    cv::triangulatePoints(T1, T2, left_kps, right_kps, points_4d);
    cout << "point num: " << points_4d.size() << ",  " << points_4d.cols << endl;

    Mat p_tmp;
    vector<Point3d> points;
    for (int i = 0; i < points_4d.cols; i++)
    {
        // cout << "i: " << i << "  points_4d: " << points_4d.at<float>(i) << endl;

        p_tmp = points_4d.col(i);
        // cout << "p_tmp: \n" << p_tmp << ",  p_tmp.size: " << p_tmp.size << endl;

        p_tmp /= p_tmp.at<float>(3, 0);
        // cout << "after p_tmp: \n" << p_tmp << ",  p_tmp.size: " << p_tmp.size << endl;

        Point3d p(
            p_tmp.at<float>(0, 0),
            p_tmp.at<float>(1, 0),
            p_tmp.at<float>(2, 0)
        );
        points.push_back(p);

        res << p.x << " " << p.y << " " << p.z << "\n";
        // cout << i << endl;
    }

    res.close();

    return 0;
}