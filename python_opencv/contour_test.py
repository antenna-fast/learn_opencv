import numpy as np
import cv2


img_path = '/mnt/data/yaohualiu/PublicDataset/wildtrack_dataset/Wildtrack_dataset/Image_subsets/C1/00000000.png'

# 找出下图的轮廓线
im = cv2.imread(img_path)
imgray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
ret, thresh = cv2.threshold(imgray, 127, 255, 0)
contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

