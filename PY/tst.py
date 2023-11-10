import cv2
import numpy as np
a=cv2.imread('bz1.jpeg')
b=np.ones((101,101,3))
b=a[220:400,250:350]
# a[0:40,0:40]=b
cv2.imshow('o',a)
cv2.waitKey()
cv2.destroyAllWindows()
