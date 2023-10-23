# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
import numpy as np
import cv2 as ocv
import scipy.stats as scs
import matplotlib.pyplot as plt


img_r = ocv.imread("./pup.jpg", ocv.IMREAD_GRAYSCALE)
#print(img_r)

img = np.ndarray.flatten(img_r)
start = 201
end = 300
print(img[start:end])

groupSize = 4096

#print(len(img))

img =  np.append(img, np.full(groupSize -  len(img) % groupSize, 0))

#print(len(img))

blocks = np.split(img, len(img) // groupSize)
#print(blocks)

dof = 128
alpha = 0.05


def observed(lst):
    obs = []

    for i in range(0, len(lst) - 1):
        obs.append(lst[i]+1)

    return obs


def expected(lst):
    exp = []

    for i in range(0, len(lst) - 1, 2):
        exp.append((lst[i] + lst[i + 1]) / 2)

    return exp


def calcChiSquare(block, size, dof):

    auxX = [0 for x in range(128)]
    auxY = [0 for x in range(128)]

    for j in range(0, size):
        c = block[j]
        if(c % 2 == 0):
            auxX[c//2] += 1
        else:
            auxY[c//2] += 1

    T = []

    auxZ = []
    for i in range(0, 128):
        if(auxX[i] + auxY[i] != 0):
            T.append(auxX[i])
            auxZ.append((auxX[i] + auxY[i])/2)

    
    chi, p = scs.chisquare(T, auxZ)

    return p



img_r = ocv.imread("./pup.jpg", ocv.IMREAD_GRAYSCALE)
img = np.ndarray.flatten(img_r)

arr_len = len(img)


p_values = []
for i in range(1, 101, 10):
    size = int(arr_len * i / 100)
    block = img[0:size]
    p_values.append(1 - calcChiSquare(block, size, dof))

print(p_values)
