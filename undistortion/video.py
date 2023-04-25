import os
import cv2

path = '/home/antenna/ssd/ImageProcess/pic_undisto/res_seamless'
filelist = os.listdir(path)
filelist = sorted(filelist)

fps = 25  # 视频每秒24帧
size = (6400, 1080)  # 需要转为视频的图片的尺寸
# 可以使用cv2.resize()进行修改

video = cv2.VideoWriter(path + "/../" + "combine_6400_1080.avi", cv2.VideoWriter_fourcc('X','V','I','D'), fps, size)#创建.avi
# 视频保存在当前目录下

for item in filelist:
    print(item)
    if item.endswith('.jpg'):
        # 找到路径中所有后缀名为.jpeg的图片
        item = os.path.join(path,item)

        img = cv2.imread(item)
        img = cv2.resize(img,(6400, 1080))#resize图片

        #print(img)
        video.write(img)

video.release()
cv2.destroyAllWindows()
