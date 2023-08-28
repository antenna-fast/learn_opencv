#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <sstream>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;



int main() {
    std::cout << "test remap ..." << std::endl;

    size_t frame_idx = 22;
    size_t str_full  = 5;

    string root_path = "/home/antenna/ssd/ImageProcess/pic_undisto/qingdao_0530";

    // load original image
    // stringstream ss;
    // ss << setfill('0') << setw(str_full) << frame_idx ;
    // frame = ss.str();
    // cout << "frame: " << frame << endl;    

    std::string img_path_0 = root_path + "/10.1.6.165_1/00022.png";  // 0
    std::string img_path_1 = root_path + "/10.1.6.10_1/00022.png";   // 1

    // 27
    // std::string img_path = root_path + "/10.1.6.10_1/00027.png";      // 1

    cv::Mat src = cv::imread(img_path_0);
    if (src.empty())
    {
        std::cout << "Cannot read image: " << img_path_0 << std::endl;
        return -1;
    }

    cv::Mat src_1 = cv::imread(img_path_1);
    if (src_1.empty())
    {
        std::cout << "Cannot read image: " << img_path_1 << std::endl;
        return -1;
    }

    // Undistortion
    // std::string root_path = "/home/antenna/ssd/ImageProcess/pic_undisto/qingdao_0530";  // cali undisto image
    string map_path_1 = root_path + "/map_10.yaml";
    string map_path_2 = root_path + "/map_165.yaml";

    cv::Mat map1, map2;
    cv::Mat map21, map22;

    // read map from file
    FileStorage fs_map_in;  // 10
    fs_map_in.open(map_path_1, FileStorage::READ);
    fs_map_in["map1"] >> map1;
    fs_map_in["map2"] >> map2;
    fs_map_in.release();

    FileStorage fs_map_in_2;  // 165
    fs_map_in_2.open(map_path_2, FileStorage::READ);
    fs_map_in_2["map1"] >> map21;
    fs_map_in_2["map2"] >> map22;
    fs_map_in_2.release();

    cv::remap(src, src, map21, map22, cv::INTER_LINEAR);  // 0: 165
    cv::remap(src_1, src_1, map1, map2, cv::INTER_LINEAR);  // 1: 10


    // load pano image
    std::string pano_path = root_path + "/res_rt/00027.png";
    cv::Mat pano = cv::imread(pano_path);
    if (pano.empty())
    {
        std::cout << "Cannot read image: " << pano_path << std::endl;
        return -1;
    }

    // create map
    // cv::Mat map_x(src.size(), CV_32FC1);
    // cv::Mat map_y(src.size(), CV_32FC1);
    // update_map(0, map_x, map_y);

    // read yaml
    std::string yaml_root_path = "/home/antenna/ssd/ImageProcess/ImageStitch/build/";
    FileStorage fs(yaml_root_path + "0.yml", FileStorage::READ);
    // FileStorage fs(yaml_root_path + "1.yml", FileStorage::READ);

    Mat uxmap, uymap;
    fs["uxmap"] >> uxmap;
    fs["uymap"] >> uymap;
    fs.release();

    std::cout <<  "map_x size: " << uxmap.size() << std::endl;
    std::cout <<  "map_y size: " << uymap.size() << std::endl;
    
    vector<Point2i> src_points;
    for (size_t i = 0; i < 10; i++)
    {
        int x = 150 + i * 100;  // col
        int y = 200;            // row
        Point2i point(x, y);
        src_points.push_back(point);
    }
    
    // draw circle on original image
    Scalar line_Color(0, 0, 255);//Color of the circle
    for (size_t i = 0; i < src_points.size(); i++)
    {
        circle(src, src_points.at(i), 20, line_Color, 7);
    }
    
    cv::imwrite("ori_circle.png", src);

    // get maped points
    vector<Point2i> dst_points;
    for (size_t i = 0; i < src_points.size(); i++)
    {
        auto x = src_points.at(i).x;
        auto y = src_points.at(i).y;

        auto x_dst = uxmap.at<float>(y, x) ;//+ 1095;  // at(row, col)
        auto y_dst = uymap.at<float>(y, x) ;//- 1478; 

        std::cout << "remap point idx: " << x_dst << ", " << y_dst << std::endl;

        Point2i point(x_dst, y_dst);
        dst_points.push_back(point);
    }

    // auto dx = 1095 - 561;
    // auto dx = 561;

    // auto dy = 0;

    auto dx = 0;
    auto dy = 2919 - 2908;

    Scalar line_Color_map(0, 255, 0); //Color of the circle

    // Point center_map((int)x_idx + dx, (int)y_idx + dy);  //Declaring the center point
    // Point center_map((int)x_ori + dx, (int)y_ori + dy);  //Declaring the center point
    // std::cout << "center_map: " << center_map << std::endl;

    // apply remap
    cv::Mat dst(src.size(), src.type());
    // cv::remap( src, dst, uxmap, uymap, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0) );
    cv::remap( src, dst, uxmap, uymap, INTER_LINEAR, BORDER_REFLECT );

    // draw circle on mappen image
    for (size_t i = 0; i < dst_points.size(); i++)
    {
        circle(dst, dst_points.at(i), 10, line_Color_map, -1);
    }

    // circle(dst, center_map, 10, line_Color_map, -1);
    cv::imwrite("remapped.jpg", dst);

    // pano
    // circle(pano, center_map, 10, line_Color_map, -1);
    // cv::imwrite("mapped_circle.jpg", pano);

    return 0;
}
