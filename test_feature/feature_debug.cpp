#if ENABLE_FEATURE_DEBUG
        // Save feature matching result
        std::cout << "feature_find_imgs.size: " << feature_find_imgs.size() << ",  pairwise_matches_.size(): " << pairwise_matches_.size() << std::endl;
        for (size_t match_idx = 0; match_idx < pairwise_matches_.size(); match_idx++)
        {
            // pick a pair of image match
            cv::detail::MatchesInfo match_info = pairwise_matches_.at(match_idx);
            int src_img_idx = match_info.src_img_idx;
            int dst_img_idx = match_info.dst_img_idx;
            int num_inliers = match_info.num_inliers;
            if(src_img_idx == -1 || dst_img_idx == -1 || num_inliers < 20)  // some simple filter
            {
                continue;
            }

            std::cout << "src_img_idx: " << src_img_idx << ",  " << dst_img_idx << ", inlier: " << match_info.num_inliers << std::endl;

            // features
            detail::ImageFeatures src_features = features_.at(src_img_idx);
            detail::ImageFeatures dst_features = features_.at(dst_img_idx);
            // keypoints
            std::vector<KeyPoint> src_keypoints = src_features.keypoints;
            std::vector<KeyPoint> dst_keypoints = dst_features.keypoints;

            //-- Draw pairwise-matches
            Mat img_matches;

            // Transform match_info: uchar -> char format
            std::vector<char> mask_inlier(match_info.inliers_mask.size());
            for (size_t i = 0; i < mask_inlier.size(); i++)
            {
                mask_inlier[i] = (char)match_info.inliers_mask[i];
            }

            drawMatches( feature_find_imgs.at(src_img_idx), src_keypoints,
                        feature_find_imgs.at(dst_img_idx), dst_keypoints,
                        match_info.matches, img_matches, Scalar::all(-1),
                        Scalar::all(-1), mask_inlier, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );  // with outlier filter
                        //  Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );  // without outlier filter

            // save match images
            string save_path = "test_match_" + to_string(src_img_idx) + "_" + to_string(dst_img_idx);
            cv::imwrite(save_path + ".jpg", img_matches);

            // save feature's key points
            ofstream kps_f;
            kps_f.open(save_path + ".txt");

            // draw match points on image to check keypoints position
            UMat src_draw_img = feature_find_imgs.at(src_img_idx).clone();
            UMat dst_draw_img = feature_find_imgs.at(dst_img_idx).clone();

            for (size_t i = 0; i < match_info.matches.size(); i++)
            {
                int trainIdx = match_info.matches[i].trainIdx;  // left image kps index
                int queryIdx = match_info.matches[i].queryIdx;  // right image kps index

                if(match_info.inliers_mask[i])
                {
                    // src
                    Scalar line_Color(0, 0, 255);//Color of the circle
                    Point center((int)src_keypoints[queryIdx].pt.x, (int)src_keypoints[queryIdx].pt.y);  //Declaring the center point
                    circle(src_draw_img, center, 5, line_Color);
                    // dst
                    Scalar line_Color_dst(0, 255, 0);//Color of the circle
                    Point center_dst((int)dst_keypoints[trainIdx].pt.x, (int)dst_keypoints[trainIdx].pt.y);  //Declaring the center point
                    circle(dst_draw_img, center_dst, 5, line_Color_dst);

                    kps_f << src_keypoints[queryIdx].pt.x << " " << src_keypoints[queryIdx].pt.y << " " << dst_keypoints[trainIdx].pt.x << " " << dst_keypoints[trainIdx].pt.y << "\n";
                }
            }

            cv::imwrite("test_keypoints_" + to_string(src_img_idx) + "_" + to_string(dst_img_idx) + "_src.jpg", src_draw_img);
            cv::imwrite("test_keypoints_" + to_string(src_img_idx) + "_" + to_string(dst_img_idx) + "_dst.jpg", dst_draw_img);

            kps_f.close();
        }
#endif
