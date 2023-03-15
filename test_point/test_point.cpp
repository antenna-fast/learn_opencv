#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"


using namespace std;
using namespace cv;


int main(){
    cout << "test point ..." << endl;

    // generate points
    std::vector<cv::Point2d> vPoint2d;
    for (size_t i = 0; i < 5; i++)
    {
        cv::Point2d tmp;
        tmp.x = i;
        tmp.y = i+1;
        vPoint2d.push_back(tmp);
    }

    // pick points    
    for (size_t i = 0; i < 5; i++)
    {
        cout << i << " " << "x: " << vPoint2d.at(i).x << " y: " << vPoint2d.at(i).y << endl;
    }

    return 0;
}
