import os
import cv2
import numpy as np

def adjust_gamma(image, gamma=1.0):
    # build a lookup table mapping the pixel values [0, 255] to
    # their adjusted gamma values
    invGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** invGamma) * 255
        for i in np.arange(0, 256)]).astype("uint8")
    # apply gamma correction using the lookup table
    return cv2.LUT(image, table)

if __name__ == "__main__":

    root_path = "/home/antenna/ssd/Mall/zgcyh/b2/2022.11.21.09.42.07/image"
    # root_path = "/home/antenna/ssd/Mall/icpark/B1/2023.05.19.04.07.45/image"

    # src_path = "/home/antenna/ssd/Mall/zgcyh/b2/2022.11.21.09.42.07/image/mve_info_2.2"
    src_path = root_path + "/mve_info_ori"
    dst_path = root_path + "/mve_info"
    images_name = sorted(os.listdir(src_path))
    # print("images_name: \n", images_name)
    for img_name in images_name:
        if(".png" not in img_name):
            continue
        
        print(img_name)

        img_path = os.path.join(src_path, img_name)
        img = cv2.imread(img_path)
        # cv2.imwrite("ori.png", img)
        # after = adjust_gamma(img, gamma=0.4)  # 2.2 -> 1
        # after = adjust_gamma(img, gamma=2.2)
        # after = adjust_gamma(img, gamma=1.8)
        after = adjust_gamma(img, gamma=1.6)
        # after = adjust_gamma(img, gamma=1.4)

        dst_img_path = os.path.join(dst_path, img_name)
        cv2.imwrite(dst_img_path, after)
