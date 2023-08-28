import os
import cv2
import numpy as np


if __name__ == "__main__":
    root_path = "/home/antenna/ssd/ImageStitching/test"

    # camera = "img1"
    # camera = "img2"
    # camera = "img3"

    frame = "000010.jpg"

    # for i in range(1, 4):  # [1, 3]
    for i in range(1, 2):  # 1
    # for i in range(3, 4):  # 3
    # for i in range(4, 5):  # 4
    # for i in range(5, 6):  # 5
        camera = "img{}".format(i)

        # img_path_1 = os.path.join(root_path, "img1", frame)  # ref
        img_path_1 = os.path.join(root_path, camera, frame)  # ref

        img_1 = cv2.imread(img_path_1)
        img_shape = img_1.shape

        P = np.array(
            [[img_shape[1]/2/1.3, 0            , img_shape[1]/2], 
            [0                 , img_shape[0]/2, img_shape[0]/2], 
            [0                 , 0             , 1]
            ])
        K = np.array([-0.09, 0.01, 0.0, 0])  # cam1
        # K = np.array([-0.1, 0.01, 0, 0])  # cam3
        # K = np.array([-0.09, 0.01, 0, 0])  # cam4
        # K = np.array([-0.11, 0.01, 0, 0])  # cam5

        img_res = cv2.undistort(img_1, P, K)
        cv2.imwrite("res_{}_{}".format(camera, frame), img_res)
