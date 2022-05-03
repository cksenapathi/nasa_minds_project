# !/usr/bin/python3

import cv2 as cv
import numpy as np
import glob

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
# # vid1 = cv2.VideoCapture("/dev/bus/usb/001/013")
# # vid2 = cv2.VideoCapture("/dev/bus/usb/001/017")
# # file = open('/dev/video6', 'rb')
# # vid1 = cv2.VideoCapture(4)
# # vid2 = cv2.VideoCapture(6)
# # while(True):
#
#     # Capture the video frame
#     # by frame
#     # ret1, frame1 = vid1.read()
#     # ret2, frame2 = vid2.read()
#
#     if ret1:
#     # Display the resulting frame
#         cv2.imshow('frame1', frame1)
#         print(f'frame 1 shape {frame1.shape}')
#     if ret2:
#         cv2.imshow('frame2', np.flip(frame2, axis=(0,1)))
#         print(f'frame 2 shape {frame2.shape}')
#
#
#     # the 'q' button is set as the
#     # quitting button you may use any
#     # desired button of your choice
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break
#
# # After the loop release the cap object
# vid1.release()
# vid2.release()
# # file.close()
# # Destroy all the windows
# cv2.destroyAllWindows()
criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((8*6,3), np.float32)
objp[:,:2] = np.mgrid[0:6,0:8].T.reshape(-1,2)

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

images = glob.glob('Webcam/*.jpg')
for fname in images:
    print(fname)
    img = cv.imread(fname)
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # Find the chess board corners
    ret, corners = cv.findChessboardCorners(gray, (6,8), None)
    # If found, add object points, image points (after refining them)
    if ret == True:
        objpoints.append(objp)
        corners2 = cv.cornerSubPix(gray,corners, (11,11), (-1,-1), criteria)
        imgpoints.append(corners)
        # Draw and display the corners
        # cv.drawChessboardCorners(img, (6,8), corners2, ret)
        # cv.imshow('img', img)
        # cv.waitKey(0)

ret, mtx, dist, rvecs, tvecs = cv.calibrateCamera(objpoints, imgpoints,
    gray.shape[::-1], None, None)
img = cv.imread('test.jpg')
print(f'image shape from camera {img.shape}')
h,  w = img.shape[:2]
newcameramtx, roi = cv.getOptimalNewCameraMatrix(mtx, dist, (w,h), 1, (w,h))
dst = cv.undistort(img, mtx, dist, None, newcameramtx)
# crop the image
x, y, w, h = roi
dst = dst[y:y+h, x:x+w]
cv.imwrite('calibresult.png', dst)
cv.imshow('calibrated image', dst)
cv.waitKey()

print(newcameramtx)
print()
print(mtx)
print()
print(dist)
print()
print(np.asarray(rvecs))
print()
print(np.asarray(tvecs))

cv.destroyAllWindows()
