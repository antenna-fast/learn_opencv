#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
    Mat image = Mat::zeros(480, 640, CV_8UC1 );
    circle(image, cv::Point2f(320, 240), 320, Scalar(255), -1, 8, 0);
    imwrite("fisheye_mask.jpg", image);
    // imshow("circle", image);
    // waitKey(0);
    return 0;
}
