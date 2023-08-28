import numpy as np
import cv2

if __name__ == "__main__":
    bgr_img = cv2.imread("0_0.png")
    hsv_img = cv2.cvtColor(bgr_img, cv2.COLOR_BGR2HSV)
    cv2.imwrite("hsv.png", hsv_img)

    print("hsv_img: ", hsv_img.shape)

    h = hsv_img[:, :, 0]
    s = hsv_img[:, :, 1]
    v = hsv_img[:, :, 2]

    print("v: ", v.shape)

    cv2.imwrite("h.png", h)
    cv2.imwrite("s.png", s)
    cv2.imwrite("v.png", v)
