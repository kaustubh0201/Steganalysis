import numpy as np
import cv2 as ocv
import scipy.stats as scs
import matplotlib.pyplot as plt
import cmath

def quadratic_solution(a, b, c):    
    d = (b**2) - (4 * a * c)
    sol1 = ((-b - cmath.sqrt(d)) / (2*a))
    sol2 = ((-b + cmath.sqrt(d)) / (2*a))

    return sol2.real, sol1.real 

img_r = ocv.imread("./edge_img.png", ocv.IMREAD_GRAYSCALE)
threshold = 0

dimensions = img_r.shape
img_height = dimensions[0]
img_width = dimensions[1]

P = []

for i in range(0, img_height):
    for j in range(0, img_width-1):
        c1 = img_r[i, j]
        c2 = img_r[i, j + 1]
        P.append((c1, c2))

x = y = kp = 0

for k in range(0, (img_height * (img_width-1))):
    (r, s) = P[k]
    if(((s % 2 == 0) and (r < s)) or ((s % 2 != 0) and (r > s))):
        x += 1
    elif(((s % 2 == 0) and (r > s)) or ((s % 2 != 0) and (r < s))):
        y += 1
    elif((s // 2) == (r // 2)):
        kp += 1

if kp == 0:
    print("SPA Failed because k = 0")

a = 2 * kp
b = 2 * (2 * x - img_height*(img_width-1))
c = y - x

betaP, betaM = quadratic_solution(a, b, c)
beta = min(betaP, betaM)
print("Change Rate of Stego Image: " + str(beta))