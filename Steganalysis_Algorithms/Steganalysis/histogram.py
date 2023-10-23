from cv2 import cv2
import numpy as np
import matplotlib.pyplot as plt
import math

img_post=cv2.imread('./critter.jpg')
img_post_stego=cv2.imread('./rev9_img.png')
img_post=cv2.resize(img_post, (800,600))
img_post_stego=cv2.resize(img_post_stego, (800,600))
print(img_post.shape) # 600 rows, 800 cols, 3 channels

#converting the img into binary
img_gray=cv2.cvtColor(img_post, cv2.COLOR_BGR2GRAY)

img_gray_stego=cv2.cvtColor(img_post_stego, cv2.COLOR_BGR2GRAY)

print(img_gray.shape)

arr=np.zeros(shape=(256,1))
rows=img_post.shape[0]
cols=img_post.shape[1]
for i in range(rows):
  for j in range(cols):
    #read kth position and add 1 in it
    k=img_gray[i,j]
    arr[k,0]=arr[k,0]+1


#we get total no of values for each pixel in the image in normal image
print(arr)


#for stego image
arr_stego=np.zeros(shape=(256,1))
for i in range(rows):
  for j in range(cols):
    #read kth position and add 1 in it
    k=img_gray_stego[i,j]
    arr_stego[k,0]=arr_stego[k,0]+1

#we get total no of values for each pixel in the image in normal image
print(arr_stego)

plt.plot(arr)
plt.title("Histogram for cover image")
plt.xlabel("Pixel value")
plt.ylabel("Frequency of occurance")
plt.show()

plt.plot(arr_stego)
plt.title("Histogram for stego image created by Edge detection based algorithm")
plt.xlabel("Pixel value")
plt.ylabel("Frequency of occurance")
plt.show()

#finding the difference between the 2 frequency arrays
diff_arr=arr-arr_stego
diff_arr=np.round(diff_arr, 2)

plt.plot(diff_arr)
plt.title("Difference plot for Edge detection based algorithm")
plt.xlabel("Color coordinate")
plt.ylabel("Frequency of pixel difference")
plt.show()