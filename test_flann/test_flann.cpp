#include <iostream>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"


using namespace std;
using namespace cv;

int main(){
    cout << "test flann ..." << endl;

    const int width(3), height(500);
    const int k = 5;
    Mat d1 = Mat_<_Float32>(Size(width, 2));        // Size(width, height)
    Mat d2 = Mat_<_Float32>(Size(width, height));   // Size(width, height)
    cout << "d1: \n" << d1 << endl;

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            _Float32  z = i * width + j;
            d2.at<_Float32>(i, j) = _Float32(z);
        }
    }
    cout << "d2: \n" << d2 << endl;

    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > knn_matches;
    matcher->knnMatch( d1, d2, knn_matches, k );

    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        cout << "distance " << i << ": ";
        for (size_t j = 0; j < k; j++)
        {
            cout << knn_matches[i][j].distance << " ";
        }
        cout << endl;
    }

    return 0;
}
