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
    img_path = "/home/antenna/ssd/Mall/zgcyh/b2/2022.11.21.09.42.07/image/mve_info/0_0.png"
    img = cv2.imread(img_path)
    cv2.imwrite("ori.png", img)
    after = adjust_gamma(img, gamma=2.2)
    cv2.imwrite("gamma.png", after)