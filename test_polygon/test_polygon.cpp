#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp" 
#include "opencv2/imgproc/imgproc.hpp"


using namespace std;
using namespace cv;

int main(){
    cout << "polygon test ..." << endl;

    /// Create an image 
    // Mat src = Mat::zeros( Size(4096, 8192), CV_8UC1 );
    Mat src = Mat::zeros( Size(8192, 4096), CV_8UC1 );

    // string img_path = "/home/antenna/ssd/TongZhou/test_data/Cam/Cache/000000.jpg";
    // Mat src = cv::imread(img_path, IMREAD_GRAYSCALE);

    /// Create a sequence of points to make a contour:
    vector<Point2f> vert;
    // vert.push_back(Point2f(50, 50));
    // vert.push_back(Point2f(50, 100));
    // vert.push_back(Point2f(100, 50));

    vert.push_back(Point2f(0, 2547));
    vert.push_back(Point2f(404, 2553));
    vert.push_back(Point2f(1087, 2749));
    vert.push_back(Point2f(1852, 2964));
    vert.push_back(Point2f(1984, 3179));
    vert.push_back(Point2f(3369, 3027));
    vert.push_back(Point2f(3855, 2661));
    vert.push_back(Point2f(4412, 2673));
    vert.push_back(Point2f(4860, 3097));
    vert.push_back(Point2f(6365, 3198));
    vert.push_back(Point2f(6320, 2989));
    vert.push_back(Point2f(7142, 2686));
    vert.push_back(Point2f(7711, 2528));
    vert.push_back(Point2f(8192, 2496));
    vert.push_back(Point2f(8192, 4096));
    vert.push_back(Point2f(0, 4096));

    /// Draw polygon in src
    for( int j = 0; j < vert.size(); j++ )
    { 
        line( src, vert[j],  vert[(j+1)%vert.size()], Scalar( 255 ), 3, 8 ); 
    }

    // string source_window = "Source";
    // namedWindow( source_window );
    // while (1)
    // {
    //     cv::imshow( "source_window", src );
    //     waitKey(30);
    // }
    cv::imwrite("test.png", src);

    /// Get the contours
    vector<vector<Point> > contours; 
    vector<Vec4i> hierarchy;
    Mat src_copy = src.clone();

    findContours( src_copy, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    Point2f testPoint = Point2f(2, 4090);  // in, return -1
    // Point2f testPoint = Point2f(2, 2);  // out, return 1

    float res = pointPolygonTest( contours[0], testPoint, false); 
    cout << "res: " << res << endl;

    /// Calculate the distances to the contour
    // Mat raw_dist( src.size(), CV_32FC1 );

    // for( int j = 0; j < src.rows; j++ )
    // { 
    //     for( int i = 0; i < src.cols; i++ )
    //     { 
    //         raw_dist.at<float>(j,i) = pointPolygonTest( contours[0], Point2f(i,j), true ); 
    //     }
    // }

    // for( int j = 0; j < src.rows; j++ )
    // { 
    //     for( int i = 0; i < src.cols; i++ )
    //         {
    //         if( raw_dist.at<float>(j,i) < 0 )
    //           { drawing.at<Vec3b>(j,i)[0] = 255 - (int) abs(raw_dist.at<float>(j,i))*255/minVal; }
    //         else if( raw_dist.at<float>(j,i) > 0 )
    //           { drawing.at<Vec3b>(j,i)[2] = 255 - (int) raw_dist.at<float>(j,i)*255/maxVal; }
    //         else
    //           { drawing.at<Vec3b>(j,i)[0] = 255; drawing.at<Vec3b>(j,i)[1] = 255; drawing.at<Vec3b>(j,i)[2] = 255; }
    //     }
    // }

    /// Create Window and show your results
    // char* source_window = "Source";
    // namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    // imshow( source_window, src );
    // namedWindow( "Distance", CV_WINDOW_AUTOSIZE );
    // imshow( "Distance", drawing );

    // waitKey(0);
 
    return 0;
}

