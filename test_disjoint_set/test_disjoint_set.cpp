// DisjointSets
// it can be used in pair-wise feature matching to build a fully panorama

#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
// #include "opencv2/features2d.hpp"
#include <opencv2/core/mat.hpp>

#include "opencv2/stitching/detail/util.hpp"

using namespace std;
using namespace cv;


int main(int argc, char** argv) 
{
    if(argc != 1) 
    {
        std::cout << " argc not match, Usage: " << std::endl;
        return -1;
    }

    std::cout << "running: " << argv[0] << std::endl;
    
    int num_images = 3;

    cv::detail::DisjointSets comps(num_images);

    for (int i = 0; i < num_images; ++i)
    {
        for (int j = i + 1; j < num_images; ++j)
        {
            // some filter condition
            // if (pairwise_matches[i*num_images + j].confidence < conf_threshold)
            // if (j < i)
            //     continue;

            int comp1 = comps.findSetByElem(i);  // find operation
            int comp2 = comps.findSetByElem(j);

            std::cout << "i: " << i << ", comp1: " << comp1 << ",  j: " << j << ", comp2: " << comp2 << std::endl;

            if (comp1 != comp2)
            {
                comps.mergeSets(comp1, comp2);  // merge operation
            }
        }
    }


    int max_comp = static_cast<int>(std::max_element(comps.size.begin(), comps.size.end()) - comps.size.begin());
    std::cout << "max_comp: " << max_comp << std::endl;

    // index filter
    std::vector<int> indices;           // indices we want to preserve
    std::vector<int> indices_removed;   // indices we want to remove
    for (int i = 0; i < num_images; ++i)  // judge if index i belong to the max component
        if (comps.findSetByElem(i) == max_comp)
            indices.push_back(i);
        else
            indices_removed.push_back(i);


    return 0;
}
