from PIL import Image
import math
import sys
from matplotlib import pyplot as plt
import cv2
import scipy.stats

# return the value of each pixel in the image
def splitpixels(img):
    pixRow = []
    pix = []
    pixNum = 0

    # getting the pixels from the image
    pixels = img.getdata()

    for pixel in pixels:
        # png
        if isinstance(pixel, int):
            # appending the value of each pixel in pixRow
            pixRow.append(pixel)

        # jpg
        else:
            pixRow.append(pixel[0])
        
        # counting the no.of pixels 
        pixNum += 1

        # If one rows is completed, then we append pixRow to pix and 
        # clear pixRow for next row of pixels
        if pixNum % img.size[0] == 0:
            pix.append(pixRow)
            pixRow = []
    
    print(pixNum, "pixels")

    return pix


def groupmask(gmask, mask):

    # gmask-->group
    # initialise the new mask variable
    newgroupmask = []

    # adds to the list for the new mask
    # copied group to newgroupmask
    for line in gmask:
        newgroupmask.append(list(line))

    # sets the row and column values
    totalrow = len(newgroupmask)
    totalcolumn = len(newgroupmask[0])

    # applying the mask on the group
    for row in range(0, totalrow):
        for column in range(0, totalcolumn):
            if newgroupmask[row][column] % 2 == 0:
                newgroupmask[row][column] += mask[row][column]
            else:
                newgroupmask[row][column] -= mask[row][column]

    # returns the calculated mask
    return newgroupmask

# finding the difference between the pixel values in the group-->sum(abs(x(i+1)-x(i-1)))
def discrimination_function(group):

    # initialise variables
    amount = 0
    totalrow = len(group)
    totalcolumn = len(group[0])


    # cycles through the columns using the discrimination function
    for row in range(0, totalrow):
        for column in range(0, totalcolumn):
            if column < (totalcolumn - 1):
                amount += abs(group[row][column] - group[row][column + 1])
    # cycles through the rows using the discrimination function
    for column in range(0, totalcolumn):
        for row in range(0, totalrow):
            if row < (totalrow - 1):
                amount += abs(group[row][column] - group[row + 1][column])
    return amount

def breakimage(imagearray, maskk, position):
    # position is the start of the group
    # initiate a new list
    brokeimage = []

    # copy mask rows into brokeimage
    # brokeimage = [[0, 1, 0]]
    for line in maskk:
        brokeimage.append(list(line))

    # cycles through the image with the chosen mask to break the image
    for temprow in range(0, len(maskk)):
        for tempcol in range(0, len(maskk[0])):
            brokeimage[temprow][tempcol] = imagearray[temprow + position[0]][tempcol + position[1]]

    return brokeimage


def analyseLSBs(imageBox, mask, neg_mask):

    # r(p/2), s(p/2), r(1-p/2), s(1-p/2) and their negations
    r_p2 = 0
    s_p2 = 0
    r_1p2 = 0
    s_1p2 = 0
    neg_r_p2 = 0
    neg_s_p2 = 0
    neg_r_1p2 = 0
    neg_s_1p2 = 0
    
    # getting the dimesions of the image
    imageRow = len(imageBox)
    # 0th element is a list so it tells us the no.of columns
    imageCol = len(imageBox[0])
    
    # getting the dimensions of the mask
    maskRow = len(mask)
    maskCol = len(mask[0])

    num = float((imageRow - imageRow % maskRow) / maskRow * (imageCol - imageCol % maskCol) / maskCol)

    # 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1  
    # 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1
    # 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1
    # 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1

    numCount = 0

    for row in range(0, imageRow):
        for column in range(0, imageCol):

            if (row + 1) % maskRow == 0:
                if (column + 1) % maskCol == 0:
                    # this is the start of the group
                    pos = [row - maskRow + 1, column - maskCol + 1]

                    # number of groups
                    numCount += 1
                    
                    # taking the mask group
                    breakimagebox = breakimage(imageBox, mask, pos)

                    flip_box = []
                    # copying the breakimagebox in flip_box
                    for line in breakimagebox:
                        flip_box.append(list(line))
                    
                    # flipping the values of the pixels in the group using F1 operation(0<->1,2<->3 ,..., 254<->255)
                    for fliprow in range(0, len(breakimagebox)):
                        for flipcolumn in range(0, len(breakimagebox[0])):
                            if breakimagebox[fliprow][flipcolumn] % 2 == 0:
                                flip_box[fliprow][flipcolumn] += 1
                            elif breakimagebox[fliprow][flipcolumn] % 2 == 1:
                                flip_box[fliprow][flipcolumn] += -1

                    # The below code is used to find the no of singular groups and regular groups when there is 50% and 100% embedding(code 214-262)
                    # f(G)
                    # applying the discrimination function on the group
                    discr_breakimagebox = discrimination_function(breakimagebox)

                    # f(G_m)                     
                    # applied the mask on the group and then applied the discrimination function
                    discr_mask_breakimagebox = discrimination_function(groupmask(breakimagebox, mask))
                    
                    # f(G_(-m))
                    # applying negative mask on the group and applying the discrimination function
                    discr_neg_mask_breakimagebox = discrimination_function(groupmask(breakimagebox, neg_mask))

                    # f(F(G))
                    # applying the discrimination function on the flipped group
                    discr_flip_box = discrimination_function(flip_box)
                    
                    # f(F(G)_m)
                    # applying the mask on the flipped pixel group and then applying the discrimination function
                    discr_mask_flip_box = discrimination_function(groupmask(flip_box, mask))
                    
                    # f(F(G)_(-m))
                    # applying the negative mask on the flipped group and then applying the discrimination function
                    discr_neg_mask_flip_box = discrimination_function(groupmask(flip_box, neg_mask))
                    
                    # normal image 
                    # f(G) > f(G_m)  -> singular group
                    if discr_breakimagebox > discr_mask_breakimagebox:
                        s_p2 += 1
                    # f(G_m) > f(G) -> regular group
                    elif discr_breakimagebox < discr_mask_breakimagebox:
                        r_p2 += 1

                    # f(G) > f(G_(-m)) -> negative singular group
                    if discr_breakimagebox > discr_neg_mask_breakimagebox:
                        neg_s_p2 += 1
                    # f(G_(-m)) > f(G) -> negative regular group
                    elif discr_breakimagebox < discr_neg_mask_breakimagebox:
                        neg_r_p2 += 1
                    

                    # flipped image with mask m
                    # image group F1 operation followed by mask and discrimination
                    # f(F(G)) > f(F(G)_(m))
                    if discr_flip_box > discr_mask_flip_box:
                        s_1p2 += 1
                    elif discr_flip_box < discr_mask_flip_box:
                        r_1p2 += 1
                    
                    # flipped image with mask -m
                    if discr_flip_box < discr_neg_mask_flip_box:
                        neg_r_1p2 += 1
                    elif discr_flip_box > discr_neg_mask_flip_box:
                        neg_s_1p2 += 1

    if num == 0:
        return 0
    
    # finding the message length by solving the quadratic
    d0 = float(r_p2 - s_p2) / num
    dn0 = float(neg_r_p2 - neg_s_p2) / num
    d1 = float(r_1p2 - s_1p2) / num
    dn1 = float(neg_r_1p2 - neg_s_1p2) / num

    a = 2 * (d1 + d0)
    b = (dn0 - dn1 - d1 - 3 * d0)
    c = (d0 - dn0)

    if b * b < 4 * a * c:
        # avoid negative root
        message_length = 0
    elif a == 0:
        # avoid deviding by zero
        message_length = 0
    else:
        # x = (-b+- sqrt(b^2-4ac))/2a, quadratic
        quadratic_solution1 = (-b + math.sqrt(b * b - 4 * a * c)) / (2 * a)
        quadratic_solution2 = (-b - math.sqrt(b * b - 4 * a * c)) / (2 * a)
        if abs(quadratic_solution1) < abs(quadratic_solution2):
            quadratic_solution = quadratic_solution1
        else:
            quadratic_solution = quadratic_solution2
        # p = x/(x−1/2), where p is message length
        message_length = abs(quadratic_solution / (quadratic_solution - 0.50))

    return message_length

def image_analyser(img, chosen_mask):
    # chosen_mask = [[0, 1, 0]]
    neg_mask = []


    # neg_mask = [[0, 1, 0]]
    for l in chosen_mask:
        neg_mask.append(list(l))

    # neg_mask = [[0, -1, 0]]
    for r in range(len(neg_mask)):
        for c in range(len(neg_mask[0])):
            if neg_mask[r][c] == 1:
                neg_mask[r][c] = -1
            elif neg_mask[r][c] == -1:
                neg_mask[r][c] = 1

    pix = splitpixels(img)

    # displays the size of the chosen mask
    print("")
    print("Mask size = ", len(chosen_mask[0]), "x", len(chosen_mask))

    # analyses the pixels to determine what percent of them may contain embedded content
    print("")
    print("Analysing LSBs")
    gpercent = analyseLSBs(pix, chosen_mask, neg_mask)

    # controls any errors within the calculations
    print("")
    # if message length is 0
    if gpercent == 0:
        print("Unable to calculate the percent of the pixels")

        print("")
        encodedpercent = "?"
    else:
        # calculates and displays the total percentage of pixels that are likely to be encoded
        encodedpercent = gpercent
        print("")
        totalpercent = (encodedpercent * 100)
        print("Total Percent of pixels likely to contain embedded data: ", round(totalpercent, 2), "%")
        width, height = img.size
        totalpix = int(width) * int(height)
        # the size of the file is calculated by multiplying the percent of encoded pixels by the total number of pixels

        data = ((encodedpercent * totalpix))
        print("Approximately ", round(data, 2), " bits of data")

    return encodedpercent

# File path is defined 
filename = "./rev_img.png"

# opened the image file
image_filename = Image.open(filename)

# mask defined 
m0 = [[0, 1, 0]]

image_analyser(image_filename, m0)