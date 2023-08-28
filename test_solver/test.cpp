#include <iostream>

#include "opencv2/calib3d/calib3d_c.h"
#include "opencv2/core/cvdef.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {
    if(argc != 1) {
        std::cout << " argc not match, Usage: " << std::endl;
        return -1;
    }

    std::cout << "running: " << argv[0] << std::endl;
    
    // TODO
    
    // Compute number of correspondences
    int total_num_matches_ = 0;
    for (size_t i = 0; i < edges_.size(); ++i)
        total_num_matches_ += static_cast<int>(pairwise_matches[edges_[i].first * num_images_ +
                                                                edges_[i].second].num_inliers);
    // std::cout << "CvLevMarq solver Start" << std::endl; 
    CvLevMarq solver(num_images_ * num_params_per_cam_,
                     total_num_matches_ * num_errs_per_measurement_,
                     cvTermCriteria(term_criteria_));
    // std::cout << "CvLevMarq solver End" << std::endl; 

    Mat err, jac;
    CvMat matParams = cvMat(cam_params_);
    cvCopy(&matParams, solver.param);

    // iteration
    int iter = 0;
    for(;;)
    {
        const CvMat* _param = 0;
        CvMat* _jac = 0;
        CvMat* _err = 0;

        bool proceed = solver.update(_param, _jac, _err);
        // std::cout << "  BA error: " << std::sqrt(err.dot(err)) << std::endl;

        cvCopy(_param, &matParams);

        if (!proceed || !_err)
            break;

        if (_jac)
        {
            calcJacobian(jac);
            CvMat tmp = cvMat(jac);
            cvCopy(&tmp, _jac);
        }

        if (_err)
        {
            calcError(err);  // err are calculated here
            LOG_CHAT(".");
            iter++;
            CvMat tmp = cvMat(err);
            cvCopy(&tmp, _err);
        }
    }

    LOGLN("");
    LOGLN("Bundle adjustment, final RMS error: " << std::sqrt(err.dot(err) / total_num_matches_));
    LOGLN("Bundle adjustment, iterations done: " << iter);

    // Check if all camera parameters are valid
    bool ok = true;
    for (int i = 0; i < cam_params_.rows; ++i)
    {
        if (cvIsNaN(cam_params_.at<double>(i,0)))
        {
            ok = false;
            break;
        }
    }
    if (!ok)
        return false;

    return 0;
}