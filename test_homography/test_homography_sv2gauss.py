import os
import numpy as np
import cv2

np.set_printoptions(suppress=True)

if __name__ == "__main__":

    root_path = "/home/antenna/ssd/rtk"

    src_img_path = os.path.join(root_path, "L.png")
    # dst_img_path = os.path.join(root_path, "dst_L.jpg")

    # load images to check feature points projection
    src_img = cv2.imread(src_img_path)

    # load src points
    src_points_path = os.path.join(root_path, "l_points.txt")
    # src_points_path = os.path.join(root_path, "r_points.txt")
    src_points = np.loadtxt(src_points_path, delimiter=',')
    print("src_points: ", src_points.shape)

    # load dst points
    dst_points_path = os.path.join(root_path, "wgs_to_gauss_new.txt")
    dst_points = np.loadtxt(dst_points_path)
    dst_points_z = dst_points[:9, 2:3].squeeze()
    dst_points = dst_points[:9, :2]
    print("dst_points: ", dst_points.shape)
    # print("dst_points: ", dst_points)

    # calculate homography using undistorted points
    H, s = cv2.findHomography(src_points, dst_points, cv2.RANSAC, 4)
    print("H: \n", H)
    np.savetxt(root_path + "/TransToGauss_L.txt", H)

    src_points = np.hstack((src_points, np.ones((len(src_points), 1))))
    # print("src_points: \n", src_points)

    # project src to dst using H
    src_to_dst_points = np.dot(H, src_points.transpose()).transpose() 

    src_to_dst_res = []
    for i in range(len(src_to_dst_points)):
        src_point = src_points[i]

        src_to_dst = src_to_dst_points[i]
        src_to_dst = [src_to_dst[0] / src_to_dst[2], src_to_dst[1] / src_to_dst[2], dst_points_z[i]]
        print(src_to_dst)

        src_to_dst_res.append(src_to_dst)

    src_to_dst_res = np.array(src_to_dst_res)
    np.savetxt(root_path + "/sv2gauss.txt", src_to_dst_res, fmt='%f')

    # Warp src image using homography
    # warpImg = cv2.warpPerspective(src_img, H, (src_img.shape[1], src_img.shape[0]))
    # cv2.imwrite("test.png", warpImg)
