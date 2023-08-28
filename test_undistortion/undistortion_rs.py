import os
import cv2
import numpy as np


if __name__ == "__main__":
    root_path = "/home/antenna/ssd/Mall/colormap/2023.08.10.10.59.48/mve_info"

    frame = "0_0.png"

    # cam0:
    # distortion_coeffs: [0.3509820376220978, 0.1613432051236743, -0.03701740875758452, 0.1050650531955296]
    # intrinsics: [0, 1036.77711495113, 1043.841433589064, 1023.205398456701, 1035.297790890757]

    fx = 1036.77711495113;
    fy = 1043.841433589064
    cx = 1023.205398456701
    cy = 1035.297790890757

    for i in range(0, 1):
        camera = "{}".format(i)

        img_path = os.path.join(root_path, frame)

        img = cv2.imread(img_path)
        img_shape = img.shape
 
        cameraMatrix = np.array(
            [[cx, 0 , cx], 
            [0  , fy, cy], 
            [0  , 0 , 1]
            ])
        distCoeffs = np.array([0.3509820376220978, 0.1613432051236743, -0.03701740875758452, 0.1050650531955296])

        img_res = cv2.undistort(img, cameraMatrix, distCoeffs)

        cv2.imwrite(os.path.join(root_path, "res_" + frame), img_res)
