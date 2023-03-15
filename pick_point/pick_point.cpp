
// TODO

#include <iostream>
#include <string>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/opencv.hpp>
 
using namespace cv;
using namespace std;
 
void on_Mouse(int event, int x, int y, int flags, void *data) {
    // cv::Mat tempImage;
    // tempImage = src.clone();

	if (event == EVENT_LBUTTONDOWN)            
	    cout << "x: " << x << "y: "<< y << endl;
        cv::drawMarker(img, Point2d(x, y), (0, 255, 0))

    // if event == cv2.EVENT_LBUTTONDOWN:
    //     print('x = %d, y = %d' % (x, y))
    //     cv2.drawMarker(param, (x, y), (0, 255, 0))
}

         
int main(int argc, char **argv)
{
    string img_path = "/home/antenna/ssd/TongZhou/test_data/Cam/Cache/000000.jpg";
	Mat src = imread(img_path);
	
	namedWindow("src");//创建窗口
 
	setMouseCallback("src", on_Mouse);  //窗口名必须和之前创建窗口同名
    // cv2::setMouseCallback(WIN_NAME, onmouse_pick_points, image)

    while (1)
    {
    	imshow("src", src);

    }
    
	waitKey();

}
