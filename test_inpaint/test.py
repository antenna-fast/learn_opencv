import cv2
import numpy as np

if __name__ == "__main__":
    img = cv2.imread("0_0.png")
    # img = cv2.imread("0_0.png", cv2.IMREAD_GRAYSCALE)

    # create mask
    img_g = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    ret, th1 = cv2.threshold(img_g, 180, 230, cv2.THRESH_BINARY)
    # th1 = cv2.cvtColor(th1, cv2.COLOR_BGR2GRAY)
    cv2.imwrite("th1.png", th1)


    # timg = cv2.inpaint(img, th1, 9, cv2.INPAINT_NS)  # img, mask
    timg = cv2.inpaint(img, th1, 30, cv2.INPAINT_TELEA)  # img, mask

    cv2.imwrite("timg.png", timg)

    m_img = cv2.medianBlur(img, 5)
    cv2.imwrite("m_img.png", m_img)



    laplacian = cv2.Laplacian(img, cv2.CV_64F)
    sobelx = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=5)
    sobely = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=5)
    cv2.imwrite("laplacian.png", laplacian)

    cv2.imwrite("sobelx.png", sobelx)
    cv2.imwrite("sobely.png", sobely)

    # compute the gradient magnitude and orientation
    magnitude = np.sqrt((sobelx ** 2) + (sobely ** 2))
    cv2.imwrite("magnitude.png", magnitude)
