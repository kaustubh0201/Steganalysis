from subprocess import call
import os

flag1 = "-e"

algo1 = "PVD_greyscale"
algo2 = "PVD_4px" # e4_
algo3 = "Edge_LSB" # ee_
algo4 = "Reversible_DCT" # er_

dataFile = "./med_file.txt"
size = os.path.getsize(dataFile)

imgPath = "./pup.jpg"

call(["./stegano", flag1, algo2, dataFile, str(size), imgPath])