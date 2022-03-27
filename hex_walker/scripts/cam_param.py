# !/usr/bin/python3
import cv2
import numpy as np

# all_camera_idx_available = []
#
# for camera_idx in range(10):
#     cap = cv2.VideoCapture(f"/dev/video{camera_idx}")
#     if cap.isOpened():
#         print(f'Camera index available: {camera_idx}')
#         all_camera_idx_available.append(camera_idx)
#         cap.release()
#
# exit()
vid1 = cv2.VideoCapture("/dev/bus/usb/001/013")
# vid2 = cv2.VideoCapture("/dev/bus/usb/001/017")
# file = open('/dev/video6', 'rb')
# vid1 = cv2.VideoCapture(4)
vid2 = cv2.VideoCapture(6)
while(True):

    # Capture the video frame
    # by frame
    ret1, frame1 = vid1.read()
    ret2, frame2 = vid2.read()

    if ret1:
    # Display the resulting frame
        cv2.imshow('frame1', frame1)
        print(f'frame 1 shape {frame1.shape}')
    if ret2:
        cv2.imshow('frame2', np.flip(frame2, axis=(0,1)))
        print(f'frame 2 shape {frame2.shape}')


    # the 'q' button is set as the
    # quitting button you may use any
    # desired button of your choice
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# After the loop release the cap object
vid1.release()
vid2.release()
# file.close()
# Destroy all the windows
cv2.destroyAllWindows()
