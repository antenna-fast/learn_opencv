import os
import cv2
import numpy as np


K_array = [
        np.array([-0.1, 0.01, 0, 0]),  # cam1
        np.array([-0.1,  0.01, 0, 0]),    # cam2
        np.array([-0.12,  0.013, 0, 0]),    # cam3
        np.array([-0.11, 0.015, 0, 0]),    # cam4
        np.array([-0.11, 0.013, 0, 0])     # cam5     
]


if __name__ == "__main__":
    root_path = "/home/antenna/ssd/ImageProcess/pic"

    # root_path = "/home/antenna/ssd/ImageProcess/test"

    undisto_path = root_path + "/../pic_undisto"
    
    for fidx in range(1, 45000):
        print("fidx: ", fidx)

        frame = str(fidx).rjust(6, '0') + ".jpg"

        for i in range(1, 6):
            camera = "img{}".format(i)
            img_path = os.path.join(root_path, camera, frame)  # ref

            img_1 = cv2.imread(img_path)
            img_shape = img_1.shape

            P = np.array(
                [[img_shape[1]/2/1.3, 0            , img_shape[1]/2], 
                [0                 , img_shape[0]/2, img_shape[0]/2], 
                [0                 , 0             , 1]
                ])

            img_res = cv2.undistort(img_1, P, K_array[i-1])
            
            res_path = "{}/../undisto/{}".format(root_path, camera)
            try: 
                os.makedirs(res_path)
            except:
                pass
            
            # print("{} exist.".format(res_path))
            cv2.imwrite(res_path+"/{}".format(frame), img_res)
