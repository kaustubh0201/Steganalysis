# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'

import numpy as np
import cv2 as ocv
import scipy.stats as scs
import matplotlib.pyplot as plt
import cmath



def countHomog(b):
    nb = len(b)
    y = 0
    for i in range(1, nb):
        y += b[i] * b [i - 1] + (1 - b[i]) * (1 - b[i - 1])

    return y / nb



img_r = ocv.imread("./ee_pup.jpg", ocv.IMREAD_GRAYSCALE)
img = img_r.flatten()
img = [8, 0, 5, 0, 5, 8, 9, 5, 0, 4, 4, 5, 7, 3, 6 , 4]


vFlip = []

for i in img:
    if i % 2 == 0:
        vFlip.append(i+1)
    else:
        vFlip.append(i-1)

        



def ColorCut(x, y, z):
    a = []
    for i in range(0, len(x)):
        if(x[i] == y or x[i] == z):
            a.append(x[i] % 2)

    return a


def quadratic_solution(a, b, c):    
    d = (b**2) - (4 * a * c)
    sol1 = ((-b - cmath.sqrt(d)) / (2*a))
    sol2 = ((-b + cmath.sqrt(d)) / (2*a))

    return sol2.real, sol1.real        



def calcR_half(ZdR):
    result = 0
    p = 2
    
    print(len(ZdR))
    for k in range(1, len(ZdR)):
        hk = 0
        # pragma omp for shared(hk, result) num_threads(8) reduction(+:hk) 
        for j in range(0, len(ZdR)-k):
            tempA = ZdR[j] + ZdR[j+k]
            hk += countHomog(tempA)
        
        result += hk/p
        p *= 2

        print(k)
    return result
            


Z = []
Zd = []
ZdR = []
Zdd = []


for k in range(0,128):
    Z += ColorCut(img, 2*k, 2*k+1)

for k in range(0, 127):
    auxVal = ColorCut(img, 2*k + 1, 2*k + 2)
    Zd += auxVal
    ZdR.append(auxVal)
    Zdd += ColorCut(vFlip, 2*k + 1, 2*k + 2)

ZdR.append(ColorCut(img, 255, 1))
Zd += ColorCut(img, 255, 1)
Zdd += ColorCut(vFlip, 255, 1)



D_Beta = countHomog(Z) - countHomog(Zd)
D_1_Beta = countHomog(Z) - countHomog(Zdd)
D_half = calcR_half(ZdR)

print(D_Beta, D_1_Beta, D_half)

a = 4 * D_half
b = D_1_Beta - D_Beta - a
c = D_Beta

print(a, b, c)

betaP, betaM = quadratic_solution(a,b,c)

beta =  min(betaP, betaM)

print(beta)







