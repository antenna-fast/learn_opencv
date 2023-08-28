#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;


void update_map( int &ind, Mat &map_x, Mat &map_y )
{
    for( int i = 0; i < map_x.rows; i++ )
    {
        for( int j = 0; j < map_x.cols; j++ )
        {
            switch( ind )
            {
            case 0:
                if( j > map_x.cols*0.25 && j < map_x.cols*0.75 && i > map_x.rows*0.25 && i < map_x.rows*0.75 )
                {
                    map_x.at<float>(i, j) = 2*( j - map_x.cols*0.25f ) + 0.5f;
                    map_y.at<float>(i, j) = 2*( i - map_x.rows*0.25f ) + 0.5f;
                }
                else
                {
                    map_x.at<float>(i, j) = 0;
                    map_y.at<float>(i, j) = 0;
                }
                break;
            case 1:  // invert vertical direction
                map_x.at<float>(i, j) = (float)j;
                map_y.at<float>(i, j) = (float)(map_x.rows - i);
                break;
            case 2:  // invert horizontal direction
                map_x.at<float>(i, j) = (float)(map_x.cols - j);
                map_y.at<float>(i, j) = (float)i;
                break;
            case 3:
                map_x.at<float>(i, j) = (float)(map_x.cols - j);
                map_y.at<float>(i, j) = (float)(map_x.rows - i);
                break;
            default:
                break;
            } // end of switch
        }
    }
    ind = (ind+1) % 4;
}

int main() {
    std::cout << "test remap ..." << std::endl;

    // load a image
    // std::string img_path = "0.jpg";
    std::string img_path = "/home/antenna/ssd/ImageProcess/pic_undisto/qingdao_0530/10.1.6.10_1/00010.png";

    cv::Mat src = cv::imread(img_path);
    if (src.empty())
    {
        std::cout << "Cannot read image: " << img_path << std::endl;
        return -1;
    }

    // create map
    cv::Mat map_x(src.size(), CV_32FC1);
    cv::Mat map_y(src.size(), CV_32FC1);    

    // int i = 1;  // vertical reflect (turn image upside down)
    // int i = 2;  // horizontal reflect
    int i = 3;  // horizontal+vertical reflect

    update_map(i, map_x, map_y);

    // std::cout << "map_x: \n" << map_x << std::endl;
    // std::cout << "map_y: \n" << map_y << std::endl;

    std::cout <<  "map_x size: " << map_x.size() << std::endl;
    std::cout <<  "map_y size: " << map_y.size() << std::endl;
    
    size_t x_ori = 100;  // x: col
    size_t y_ori = 20;   // y: row 
    
    // draw circle on original image
    Scalar line_Color(0, 0, 255);//Color of the circle
    Point center((int)x_ori, (int)y_ori);  //Declaring the center point
    circle(src, center, 20, line_Color, -1);
    
    cv::imwrite("1_c.png", src);

    // size_t x_idx = map_x.at<float>(x_ori, y_ori);
    // size_t y_idx = map_y.at<float>(x_ori, y_ori);
    
    // mapped location
    size_t x_idx = map_x.at<float>(y_ori, x_ori);  // at(row, col)
    size_t y_idx = map_y.at<float>(y_ori, x_ori);

    std::cout << "mapped point: " << x_idx << ", " << y_idx << std::endl;

    Scalar line_Color_map(0, 255, 0);//Color of the circle
    Point center_map((int)x_idx, (int)y_idx);  //Declaring the center point

    // apply remap
    cv::Mat dst(src.size(), src.type());
    // cv::remap( src, dst, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0) );
    cv::remap( src, dst, map_x, map_y, INTER_LINEAR, BORDER_REPLICATE, Scalar(0, 0, 0) );

    circle(dst, center_map, 10, line_Color_map, -1);

    cv::imwrite("res.jpg", dst);

    return 0;
}
