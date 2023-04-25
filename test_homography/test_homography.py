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


if __name__ == "__main__":

    # res = "disto"
    res = "undisto"

    root_path = "/home/antenna/ssd/ImageProcess/ImageStitch/build/" + res

    src = 3
    dst = 2

    src_img_path = os.path.join(root_path, "test_keypoints_{}_{}_src.jpg".format(src, dst))
    dst_img_path = os.path.join(root_path, "test_keypoints_{}_{}_dst.jpg".format(src, dst))

    # load images to check feature points projection
    src_img = cv2.imread(src_img_path)
    dst_img = cv2.imread(dst_img_path)
    dst_img_proj = cv2.imread(dst_img_path)

    # load keypoints
    src_dst_points_path = os.path.join(root_path, "test_match_{}_{}.txt".format(src, dst))
    src_dst_points = np.loadtxt(src_dst_points_path)
    # print("src_dst_points: \n", src_dst_points)

    src_points = src_dst_points[:, 0:2]
    dst_points = src_dst_points[:, 2:4]
    print("src_points: ", src_points.shape)

    # calculate homography using undistorted points
    H, s = cv2.findHomography(src_points, dst_points, cv2.RANSAC, 4)
    print("H: \n", H)

    
    src_to_dst_res = []

    src_points = np.hstack((src_points, np.ones((len(src_points), 1))))
    # print("src_points: \n", src_points)

    # project src to dst using H
    src_to_dst_points = np.dot(H, src_points.transpose()).transpose() 

    img_shape = dst_img.shape

    min_dist = 999999
    
    # get project MSE to metric distortion
    dist = 0
    for i in range(len(src_to_dst_points)):
        dst_point = dst_points[i]
        src_point = src_points[i]

        src_to_dst = src_to_dst_points[i]
        src_to_dst = [src_to_dst[0] / src_to_dst[2], src_to_dst[1] / src_to_dst[2]]
        
        x_diff = src_to_dst[0] - dst_point[0]
        y_diff = src_to_dst[1] - dst_point[1]

        dist += np.sqrt((x_diff * x_diff) + (y_diff * y_diff))

        # draw transformed points onto dst image 
        dst_img_projimg = cv2.circle(dst_img_proj, (int(src_to_dst[0]), int(src_to_dst[1])), 2, (0, 0, 255), -1)
 
        src_img = cv2.circle(src_img, (int(src_point[0]), int(src_point[1])), 2, (0, 0, 255), -1)
        dst_img = cv2.circle(dst_img, (int(dst_point[0]), int(dst_point[1])), 2, (0, 255, 0), -1)

    print("dist: ", dist)
    print("mean dist: ", dist/len(src_to_dst_points))
    
    cv2.imwrite("res_" + res + "/src_{}_to_dst_src_disto.jpg".format(src, dst), src_img)
    cv2.imwrite("res_" + res + "/src_{}_to_dst_dst_disto.jpg".format(src, dst), dst_img)
    cv2.imwrite("res_" + res + "/src_{}_to_dst_{}_proj.jpg".format(src, dst), dst_img_proj)

    # Warp src image using homography
    warpImg = cv2.warpPerspective(src_img, H, (src_img.shape[1], src_img.shape[0]))  # size not ok
    cv2.imwrite("res_" + res + "/warp_src_{}_to_dst_{}.jpg".format(src, dst), warpImg)
