import cv2
import numpy as np

if __name__ == "__main__":
    img = cv2.imread("0_0.png")
    # img = cv2.imread("0_0.png", cv2.IMREAD_GRAYSCALE)

    # create mask
    img_g = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    cv2.imwrite("img_g.png", img_g)

    ret, th1 = cv2.threshold(img_g, 160, 230, cv2.THRESH_BINARY)
    
    # th1 = cv2.cvtColor(th1, cv2.COLOR_BGR2GRAY)
    cv2.imwrite("th1.png", th1)

    # inv_weight = 255/img_g
    inv_weight = img_g.copy()
    # cv2.imwrite("inv_weight.png", inv_weight)
    # inv_weight = np.array([inv_weight, inv_weight, inv_weight])

    # timg = inv_weight * img_g
    timg = img.copy()

    img_shape = img.shape

    for i in range(img_shape[0]):
        for j in range(img_shape[1]):
            # timg[i][j] = timg[i][j] * inv_weight[i][j]

            if(th1[i][j] != 0):
                # print(th1[i][j])
            #     timg[i][j] = timg[i][j] * 0.6

            # if(img_g[i][j] != 0):
                inv_weight[i][j] = 255 - img_g[i][j]

                # timg[i][j] = inv_weight[i][j]/255 * timg[i][j] + timg[i][j]
                timg[i][j] = timg[i][j] - 0.3 * img_g[i][j] 
                
    cv2.imwrite("timg.png", timg)
    cv2.imwrite("inv_weight.png", inv_weight)
