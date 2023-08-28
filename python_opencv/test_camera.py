import os
import cv2

root_path = "/home/antenna/Downloads"

# video_name = "ref"
# video_name = "0"
video_name = "90"

video_path = os.path.join(root_path, "video-" + video_name + ".mp4")
# img_path = os.path.join(root_path, "ref")
img_path = os.path.join(root_path, video_name)

cap = cv2.VideoCapture(video_path)

# Check if camera opened successfully
if (cap.isOpened()== False): 
    print("Error opening video stream or file")

# Read until video is completed
frame_counter = 0
while(cap.isOpened()):
  # Capture frame-by-frame
  ret, frame = cap.read()
  if ret == True:
 
    # Display the resulting frame
    # cv2.imshow('Frame',frame)
    cv2.imwrite(os.path.join(img_path, str(frame_counter) + ".png"), frame)
    frame_counter += 1 

    # Press Q on keyboard to  exit
    if cv2.waitKey(25) & 0xFF == ord('q'):
      break
 
  # Break the loop
  else: 
    break
 
# When everything done, release the video capture object
cap.release()
 
# Closes all the frames
cv2.destroyAllWindows()
