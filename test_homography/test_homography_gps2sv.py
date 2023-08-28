import os
import numpy as np
import cv2
import open3d as o3d


if __name__ == "__main__":

    root_path = "/home/antenna/Downloads/rtk"

    # load images to check gps points projection
    src_img_path = os.path.join(root_path, "L.png")
    src_img = cv2.imread(src_img_path)

    # load gps points
    gps_points_path = os.path.join(root_path, "gps_list.txt")
    gps_points = np.loadtxt(gps_points_path)
    gps_points = gps_points[:, :2]
    gps_points = np.hstack((gps_points, np.ones((len(gps_points), 1))))

    print("src_points: ", gps_points.shape)
    # print("src_points: \n", src_points)

    SV2GPS_H = np.loadtxt(root_path + "/TransToGps_L.txt")
    GPS2SV = np.linalg.inv(SV2GPS_H)

    # project gps to sv using H
    dst_points = np.dot(GPS2SV, gps_points.transpose()).transpose() 

    src_to_dst_res = []
    for i in range(len(dst_points)):
        src_to_dst = dst_points[i]
        src_to_dst = [src_to_dst[0] / src_to_dst[2], src_to_dst[1] / src_to_dst[2]]
        print(src_to_dst)

        src_to_dst_res.append(src_to_dst)

    src_to_dst_res = np.array(src_to_dst_res)
    np.savetxt(root_path + "/gps2sv.txt", src_to_dst_res)

    