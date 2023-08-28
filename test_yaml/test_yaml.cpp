#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include <opencv2/core/mat.hpp>

#include "load_yaml.h"


using namespace std;
using namespace cv;

/**
bool read_yaml(string yaml_path, vector<double>& disto, vector<double> &intri)
{

    ifstream in_file;
    in_file.open(yaml_path);

    if(in_file.fail())
    {
        cout << "failed to open file: " << yaml_path << endl;
        return false;
    }

    disto.resize(4);
    intri.resize(4);

    string line;
    int row_counter = 0; 

    while (!in_file.eof())
    {
        row_counter ++;
        getline(in_file, line);

        if(row_counter == 4)
        {
            istringstream str_line(line);
            string str;

            // std::cout << "str_line: " << line << std::endl;
            
            size_t pos = line.find("[");
            line = line.substr(pos+1, line.size() - pos -2); 
            cout << line << endl;

            for (size_t i = 0; i < 3; i++)
            {
                pos = line.find(", ");
                string p0 = line.substr(0, pos);
                line = line.substr(pos+2);

                disto.at(i) = stod(p0);
                std::cout << "disto: " << disto.at(i) << std::endl;
            }
            disto.at(3) = stod(line);

            std::cout << "disto: " << disto.at(3) << std::endl;
        }

        if(row_counter == 6)
        {
            istringstream str_line(line);
            string str; 

            size_t pos = line.find("[");
            line = line.substr(pos+1, line.size() - pos -2); 
            cout << line << endl;

            for (size_t i = 0; i < 3; i++)
            {
                pos = line.find(", ");
                string p0 = line.substr(0, pos);
                line = line.substr(pos+2);

                intri.at(i) = stod(p0);
                std::cout << "intri: " << intri.at(i) << std::endl;
            }
            intri.at(3) = stod(line);
            std::cout << "intri: " << intri.at(3) << std::endl;
        }        
    }

    return true;
}
*/

int main(){
    cout << "test yaml" << endl;

    // generate points
    const int raw = 5;
    const int col = 3;
    
    /*
    Matrix row/col define: 
         col1 col2 ... col  (y)
    row1
    row2
    .
    .
    row
    
    (x)
    */

    // cv::Mat a(raw, col, CV_32FC1);
    // a.at<float>(1, 0) = 1000.0;
    // cout << "a: \n" << a << endl;

    // write yaml
    // FileStorage fs_write("test.xml", FileStorage::WRITE);
    // fs_write << "mat_a" << a;
    // fs_write.release();

    // read yaml
    string yaml_path = "/home/antenna/ssd/App/Learn/learn_opencv/test_yaml/build/pano-camchain.yaml";
    // FileStorage fs(yaml_path, FileStorage::READ);
    // cv::Mat disto;
    // fs["cam0"]["distortion_coeffs"] >> disto;
    // fs.release();

    // std::cout << "disto: \n" << disto << std::endl;
    
    vector<double> disto;
    vector<double> intri;

    read_yaml(yaml_path, disto, intri);

    std::cout << "disto: " << std::endl;
    for (size_t i = 0; i < disto.size(); i++)
    {
        std::cout << disto.at(i) << std::endl;
    }

    std::cout << "intri: " << std::endl;
    for (size_t i = 0; i < intri.size(); i++)
    {
        std::cout << intri.at(i) << std::endl;
    }

    return 0;
}
