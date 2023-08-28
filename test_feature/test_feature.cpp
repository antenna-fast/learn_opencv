#include <iostream>
#include <fstream>

#include "opencv2/core.hpp"
// #ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

const char* keys =
    "{ help h |                  | Print help message. }"
    "{ input1 | box.png          | Path to input image 1. }"
    "{ input2 | box_in_scene.png | Path to input image 2. }";


int main( int argc, char* argv[] )
{
    CommandLineParser parser( argc, argv, keys );

    // input path
    // read images
    // std::string left_img_path{samples::findFile( parser.get<String>("input1") )};
    // std::string right_img_path{samples::findFile( parser.get<String>("input2") )};

    string img_root_path = "/home/antenna/ssd/Dataset/KITTI/data_odometry_gray/dataset/sequences/00";
    string left_img_path = img_root_path + "/" + "image_0/" + "000000.png";
    string right_img_path = img_root_path + "/" + "image_1/" + "000000.png";

    // output path
    // string left_kps_path  = "left_kps.txt"; 
    // string right_kps_path = "right_kps.txt"; 
    string left_right_kps_path = "left_right_kps.txt"; 

    ofstream kps_f;
    kps_f.open(left_right_kps_path);

    Mat img1 = imread(left_img_path , IMREAD_GRAYSCALE );
    Mat img2 = imread(right_img_path, IMREAD_GRAYSCALE );
    if ( img1.empty() || img2.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        parser.printMessage();
        return -1;
    }

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create( minHessian );
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute( img1, noArray(), keypoints1, descriptors1 );
    detector->detectAndCompute( img2, noArray(), keypoints2, descriptors2 );

    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > knn_matches;  // <queryIdx, <trainIdx, <DMatch>>>
    matcher->knnMatch( descriptors1, descriptors2, knn_matches, 2 );  // query, train 
    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.5f;
    std::vector<DMatch> good_matches;
    size_t trainIdx = 0;
    size_t queryIdx = 0;

    vector<Point2d> left_img_points;
    vector<Point2d> right_img_points;

    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        //knn_matches[i][0]: nearest data pair
        //knn_matches[i][1]: sub-nearest data pair
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
            trainIdx = knn_matches[i][0].trainIdx;  // left image kps index
            queryIdx = knn_matches[i][0].queryIdx;  // right image kps index            
            cout << "match queryIdx: " << queryIdx << " i: " << i << endl;
            cout << "match trainIdx: " << trainIdx << ", position: " << keypoints1[queryIdx].pt << endl;  // i: queryIdx
            
            Point2d left_point(keypoints1[queryIdx].pt.x,  keypoints1[queryIdx].pt.y);
            Point2d right_point(keypoints2[trainIdx].pt.x, keypoints2[trainIdx].pt.y);
            left_img_points.push_back(left_point);
            right_img_points.push_back(right_point);

            kps_f << left_point.x << " " << left_point.y << " " <<right_point.x << " " << right_point.y << "\n";        
        }
    }

    kps_f.close();

    //-- Draw matches
    Mat img_matches;
    drawMatches( img1, keypoints1, 
                 img2, keypoints2, 
                 good_matches, img_matches, Scalar::all(-1),
                 Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    // img, CvPoint center, int radius, CvScalar color,
    for (size_t i = 0; i < left_img_points.size(); i++)
    {
        cv::circle(img1, left_img_points.at(i), 5, Scalar(255, 0, 0));
        cv::circle(img2, right_img_points.at(i), 5, Scalar(255, 0, 0));
    }

    //-- Show detected matches
    // imshow("Good Matches", img_matches );
    // imshow("LeftView", img1 );
    // imshow("RightView", img2 );

    // waitKey();

    // write to file
    cv::imwrite("left_feature.png", img1);
    cv::imwrite("right_feature.png", img2);

    // two view images witch match result 
    cv::imwrite("img_matches.png", img_matches);

    return 0;
}
