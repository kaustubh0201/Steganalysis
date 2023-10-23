#freq of each 0's,(black) 1's ...255's(white) is plotted
from cv2 import cv2
import numpy as np
import matplotlib.pyplot as plt
import math

img_post=cv2.imread('./pup.jpg')
img_post_stego=cv2.imread('./edg2_img.png')

img_post=cv2.resize(img_post, (800,600))
img_post_stego=cv2.resize(img_post_stego, (800,600))

#converting the img into binary
img_gray=cv2.cvtColor(img_post, cv2.COLOR_BGR2GRAY)

img_gray_stego=cv2.cvtColor(img_post_stego, cv2.COLOR_BGR2GRAY)


#enhancing the whites to be lighter and the greys to darker
#img_gray=cv2.convertScaleAbs(img_gray, alpha=1.10, beta=-20) #alpha is contrast, beta is brightness

#creating a numpy array of 0s
arr=np.zeros(shape=(256,1))
rows=img_post.shape[0]
cols=img_post.shape[1]
for i in range(rows):
  for j in range(cols):
    #read kth position and add 1 in it
    k=img_gray[i,j]
    arr[k,0]=arr[k,0]+1


#we get total no of values for each pixel in the image in normal image

#for stego image
arr_stego=np.zeros(shape=(256,1))
for i in range(rows):
  for j in range(cols):
    #read kth position and add 1 in it
    k=img_gray_stego[i,j]
    arr_stego[k,0]=arr_stego[k,0]+1

#we get total no of values for each pixel in the image in normal image


#finding the difference between the 2 frequency arrays
diff_arr=arr-arr_stego
diff_arr=np.round(diff_arr, 2)

#minr=abs(math.floor(np.amin(diff_arr)))
#maxr=abs(math.floor(np.amax(diff_arr))) 

#freq_diff=np.zeros(shape=(minr,maxr))
#creating histogram of difference
#for i in range(diff_arr.shape[0]):
#  k=diff_arr[i]
#  k=math.floor(k)
#  k=abs(k)
#  freq_diff[k,0]=freq_diff[k,0]+1

count_pairs=0

for i in range(diff_arr.shape[0]-1):
  p1=diff_arr[i]
  p2=diff_arr[i+1]
  if(p1 != p2 and p1==abs(p2)):
    count_pairs=count_pairs+1

dist=0

for i in range(arr.shape[0]):
  p=arr[i]
  q=arr_stego[i]
  dist=dist+(pow((p-q),2))/(pow((p+q),2))

print("Probability of being a steganoimage: " + str(dist*100))