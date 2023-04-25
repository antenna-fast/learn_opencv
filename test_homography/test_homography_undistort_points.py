import os
import numpy as np
import cv2


if __name__ == "__main__":
    root_path = "/home/antenna/ssd/ImageProcess/ImageStitch/build"

    src = 3
    dst = 2

    src_img_path = os.path.join(root_path, "test_keypoints_{}_{}_src.jpg".format(src, dst))
    dst_img_path = os.path.join(root_path, "test_keypoints_{}_{}_dst.jpg".format(src, dst))

    # load images to check feature points projection
    src_img = cv2.imread(src_img_path)
    dst_img = cv2.imread(dst_img_path)

    # load keypoints
    src_dst_points_path = os.path.join(root_path, "test_match_{}_{}.txt".format(src, dst))
    src_dst_points = np.loadtxt(src_dst_points_path)

    src_points = np.array([src_dst_points[:, 0:2]], dtype=float)
    dst_points = np.array([src_dst_points[:, 2:4]], dtype=float)
    print("src_points.shape: ", src_points.shape)
    
    min_dist = 999999

    img_shape = dst_img.shape

    k1 = -0.3402540568533989
    k2 = 0.1353356068503937
    p1 = 0.0008434793770756303
    p2 = -0.0007039357556693586

    # k1 = -0.1
    # k2 = 0.01

    distCoeffs = np.array([k1, k2, p1, p2])

    min_focal = 1258
    max_focal = 1850
    focal_range = np.linspace(min_focal, max_focal, max_focal - min_focal)

    cx = 964 
    cy = 605
    # cx = 978 
    # cy = 585

    src_to_dst_res = []

    # iter focal
    # for f in focal_range[:2]:
    for f in focal_range:
        camMat = np.array(
                        [[f, 0, cx], 
                         [0, f, cy], 
                         [0, 0, 1]
                        ])

        # Undistort Points
        undis_points2d_src = cv2.undistortPoints(src_points, camMat, distCoeffs)[0]
        undis_points2d_dst = cv2.undistortPoints(dst_points, camMat, distCoeffs)[0]
        # print("undis_points2d_src: ", undis_points2d_src.shape)
        # print("undis_points2d_dst: ", undis_points2d_dst.shape)

        # Calculate homography using undistorted points
        H, s = cv2.findHomography(undis_points2d_src, undis_points2d_dst, cv2.RANSAC, 4)

        undis_points2d_src = np.hstack((undis_points2d_src, np.ones((len(undis_points2d_src), 1))))

        # Project undistorted src to dst using H
        src_to_dst_points = np.dot(H, undis_points2d_src.transpose()).transpose() 
        # print("src_to_dst_points: ", src_to_dst_points.shape)

        # Get project MSE to metric distortion
        dist = 0
        for i in range(len(src_to_dst_points)):
            dst_point = undis_points2d_dst[i]
            # dst_point = dst_points[0][i]

            src_to_dst = src_to_dst_points[i]
            src_to_dst = [src_to_dst[0] / src_to_dst[2], src_to_dst[1] / src_to_dst[2]]

            src_to_dst = src_points[0][i]

            x_diff = src_to_dst[0] - dst_point[0]
            y_diff = src_to_dst[1] - dst_point[1]

            dist += np.sqrt((x_diff * x_diff) + (y_diff * y_diff))

            # src_to_dst_res.append(src_to_dst)

            # draw transformed points onto dst image 
            # dst_img = cv2.circle(dst_img, (int(src_to_dst[0]), int(src_to_dst[1])), 2, (0, 0, 255), -1)

        if(dist < min_dist):
            min_dist = dist
            print("min_dist: {},  focal: {}".format(min_dist, f))

    print("min_dist: {},  focal: {} ".format(min_dist, f))
