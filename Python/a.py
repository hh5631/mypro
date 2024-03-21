import cv2
import threading
from moviepy.editor import VideoFileClip
def imgShow(name, img):
        img = cv2.imread('bz1.jpeg');

        img2 = cv2.resize(img, (640, 480));

        cv2.imshow(name, img2);

        cv2.waitKey(0);

        cv2.destroyAllWindows();

def videoShow(name, vedio):

        vc = cv2.VideoCapture(vedio)
        cv2.namedWindow(name, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(name, 640, 480);
        while True:
                rval, frame = vc.read()
                if not rval:
                        break
                cv2.imshow(name, frame)
                
                key = cv2.waitKey(1)
                if key == 27:  # If 'Esc' key is pressed, exit the loop
                        break
        cv2.destroyAllWindows();

def cameraShow(name):
        vc = cv2.VideoCapture(0)
        cv2.namedWindow(name, cv2.WINDOW_NORMAL)
        cv2.resizeWindow(name, 640, 480);
        while True:
                rval, frame = vc.read()
                if not rval:
                        break
                cv2.imshow(name, frame)
                
                key = cv2.waitKey(1)
                if key & 0xFF == ord('q'):  # If 'Esc' key is pressed, exit the loop
                        break
        vc.release();
        cv2.destroyAllWindows();

import cv2
import threading

def saveVideo(path):
        vc = cv2.VideoCapture(0)
        fourcc = cv2.VideoWriter_fourcc(*'XVID')
        vw = cv2.VideoWriter(path, fourcc, 20.0, (640, 480))
        # 定时器回调函数，用于停止录制视频
        start_time = cv2.getTickCount();
        while vc.isOpened():
                rval, frame = vc.read()
                if not rval:
                        break
                vw.write(frame)
                cv2.waitKey(1)
                       # 计算录制时间
                current_time = cv2.getTickCount()
                elapsed_time = (current_time - start_time) / cv2.getTickFrequency()
                # 如果录制时间达到10秒，关闭程序
                if elapsed_time > 10:
                        vw.release()
                        vc.release()
                        cv2.destroyAllWindows()
                        break       
        vw.release()
        vc.release()
        cv2.destroyAllWindows()
        

saveVideo('a.mp4')