# Import necessary libraries
import cv2
import numpy as np

# Read an image
img_path = "/home/antenna/Desktop/02-21-20221212_164839.jpg"
output_img_path = "/home/antenna/Desktop/polygon.jpg"

img = cv2.imread(img_path)

# Define an array of endpoints of triangle
points = np.array([[160, 130], 
                    [350, 130], 
                    [250, 300]])

# Use fillPoly() function and give input as
# image, end points,color of polygon
# Here color of polygon will blue
cv2.fillPoly(img, pts=[points], color=(255, 0, 0))

cv2.imwrite(output_img_path, img)

# # Displaying the image
# cv2.imshow("Triangle", img)

# # wait for the user to press any key to
# # exit window
# cv2.waitKey(0)

# # Closing all open windows
# cv2.destroyAllWindows()

