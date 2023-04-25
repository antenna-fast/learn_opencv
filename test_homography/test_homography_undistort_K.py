import os
import numpy as np
import cv2


def undistort(u, v, k1, k2, k3, p1, p2, fx, fy, cx, cy):
    x = (u-cx)/fx
    y = (v-cy)/fy
    x2 = x*x
    y2 = y*y
    xy = x*y
    r2 = x2 + y2
    x_radial = x * (1 + k1*r2 + k2*r2*r2 + k3*r2*r2*r2)
    y_radial = y * (1 + k1*r2 + k2*r2*r2 + k3*r2*r2*r2)
    x_tangential = 2*p1*xy + p2*(r2 + 2*x2)
    y_tangential = 2*p2*xy + p1*(r2 + 2*y2)
    xd = x_radial + x_tangential
    yd = y_radial + y_tangential
    u = xd*fx + cx 
    v = yd*fy + cy
    return u, v


# TODO
def undistort_point(point2d, camMat, distCoeffs):
    points2d = np.array([point2d], dtype=np.float)
    # src, cameraMatrix, distCoeffs
    undis_points2d = cv2.undistortPoints(points2d, camMat, distCoeffs)
    return undis_points2d.reshape(-1)
    

if __name__ == "__main__":
    root_path = "/home/antenna/ssd/ImageProcess/ImageStitch/build"

    src = 2
    dst = 3

    src_img_path = os.path.join(root_path, "test_keypoints_{}_{}_src.jpg".format(src, dst))
    dst_img_path = os.path.join(root_path, "test_keypoints_{}_{}_dst.jpg".format(src, dst))

    src_img = cv2.imread(src_img_path)

    # load keypoints
    src_dst_points_path = os.path.join(root_path, "test_match_{}_{}.txt".format(src, dst))
    src_dst_points = np.loadtxt(src_dst_points_path)

    src_points = src_dst_points[:, 0:2]
    dst_points = src_dst_points[:, 2:4]

    # load images to check feature points projection
    dst_img = cv2.imread(dst_img_path)
    
    src_to_dst_res = []

    K = [-1, -1, 0, 0]  # k1, k2, p1, p2
    k1 = K[0]
    k2 = K[1]

    min_dist = 999999
    
    k1_num = 100
    k2_num = 100

    k1_range = np.linspace(-0.3, -0.9, k1_num)
    k2_range = np.linspace(0.1, 0.8, k2_num)
    
    img_shape = dst_img.shape

    # iter possible k1, k2
    for k1 in k1_range:
        for k2 in k2_range:
            # k1 = -0.3402540568533989
            # k2 = 0.1353356068503937

            # k1 = -0.1
            # k2 = 0.01

            camMat = np.array(
                [[img_shape[1]/2/1.3, 0            , img_shape[1]/2], 
                [0                 ,  img_shape[0]/2, img_shape[0]/2], 
                [0                 ,  0             , 1]
                ])

            distCoeffs = np.array([k1, k2, 0, 0])

            # undistort
            undis_points2d_src = cv2.undistortPoints(src_points, camMat, distCoeffs)[0]
            undis_points2d_dst = cv2.undistortPoints(dst_points, camMat, distCoeffs)[0]

            # calculate homography using undistorted points
            H, s = cv2.findHomography(undis_points2d_src, undis_points2d_dst, cv2.RANSAC, 4)
            print("H: \n", H)

            undis_points2d_src = np.hstack((undis_points2d_src, np.ones((len(src_points), 1))))

            # project undistorted src to dst using H
            src_to_dst_points = np.dot(H, undis_points2d_src.transpose()).transpose() 

            # get project MSE to metric distortion
            dist = 0
            for i in range(len(src_to_dst_points)):

                # undistort src points and dst points
                
                dst_point = dst_points[i]
                src_point = src_points[i]

                src_to_dst = src_to_dst_points[i]
                src_to_dst = [src_to_dst[0] / src_to_dst[2], src_to_dst[1] / src_to_dst[2]]
                
                
                x_diff = src_to_dst[0] - dst_point[0]
                y_diff = src_to_dst[1] - dst_point[1]

                dist += np.sqrt((x_diff * x_diff) + (y_diff * y_diff))

                # src_to_dst_res.append(src_to_dst)

                # draw transformed points onto dst image 
                # dst_img = cv2.circle(dst_img, (int(src_to_dst[0]), int(src_to_dst[1])), 2, (0, 0, 255), -1)
                # dst_img = cv2.circle(dst_img, (int(dst_u), int(dst_v)), 2, (0, 0, 255), -1)

            print("dist: ", dist)

            if(dist < min_dist):
                min_dist = dist
                print("min_dist: {},  k1: {} , k2: {} ".format(min_dist, k1, k2))

                cv2.imwrite("src_{}_to_dst_{}.jpg".format(src, dst), dst_img)

                # warpImg = cv2.warpPerspective(src_img, H_inv, (src_img.shape[1], src_img.shape[0]))  # size not ok
                # cv2.imwrite("warp_src_{}_to_dst_{}.jpg".format(src, dst), warpImg)

                # using best distortation to undistort image for visualization

                # return best distortation

    print("min_dist: {},  k1: {} , k2: {} ".format(min_dist, k1, k2))
