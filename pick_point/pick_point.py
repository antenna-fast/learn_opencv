import cv2
import numpy as np

WIN_NAME = 'pick_points'

polygon_vertex = []
point_counter = 0

def onmouse_pick_points(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        global point_counter
        # print("vert[%d] = Point2f(%d, %d);" % (point_counter, x, y))
        # print("[%d, %d]," % (x, y))
        print("%d, %d" % (x, y))

        point_counter += 1

        cv2.drawMarker(param, (x, y), (0, 0, 255), 1)
        polygon_vertex.append([x, y])


if __name__ == '__main__':
    # img_path = "/home/antenna/ssd/TongZhou/test_data/Cam/Cache/000000.jpg"
    img_path = "/home/antenna/Downloads/rtk/L.png"
    # img_path = "/home/antenna/Downloads/rtk/R.png"

    image = cv2.imread(img_path)

    cv2.namedWindow(WIN_NAME, 0)
    cv2.setMouseCallback(WIN_NAME, onmouse_pick_points, image)
    while True:
        cv2.imshow(WIN_NAME, image)
        key = cv2.waitKey(30)
        if key == 27:  # ESC
            break
    cv2.destroyAllWindows()

