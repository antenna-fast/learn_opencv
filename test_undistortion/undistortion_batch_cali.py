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

            img = cv2.imread(img_path)
            img_shape = img.shape

            # [1851.533570072246, 1851.703074846014, 964.7358704944479, 605.5603915306388]
            # [1258.7186850468274, 1259.094307788846, 978.385184011695, 585.6191698993645]
            # focal = 1393
            focal = 1258
            cx = 978
            cy = 585
            P = np.array(
                [[focal, 0    , cx], 
                [0     , focal, cy], 
                [0     , 0    , 1]
                ])

            # K = np.array([-0.3428271988870185, 0.10140538695565529, 0.0008434793770756303, -0.0007039357556693586])
            K = np.array([-0.3402540568533989, 0.1353356068503937, -0.0014236269930090234, 0.0006181272446327531])

            img_res = cv2.undistort(img, P, K)
            
            res_path = "{}/{}".format(undisto_path, camera)
            try: 
                os.makedirs(res_path)
            except:
                pass
            
            # print("{} exist.".format(res_path))
            cv2.imwrite(res_path+"/{}".format(frame), img_res)
