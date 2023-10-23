# The equations used for the Chi-Square test were provided by A. Westfeld and A. Pfitzmann,
# "Attacks on Steganographic Systems", International Workshop on Information Hiding, vol. 1768, 1999, pp. pp 61-76.
# all work on the Chi-Square test draws from their research
#
# The RS Steganalysis method is based on the work of J. Fridrich, M. Goljan and Rui Du, " Reliable Detection of
# LSB Steganography in Color and Grayscale Images," IEEE MultiMedia, vol. 8, no. 4, pp. 22-28, Oct.-Dec. 2001.
#
# The RS Steganalysis method is an editied version of code written by Brandon Hurst
# B. Hurst, "RS Steganalysis", Youtube.com, 2014. [Online].
# Available: https://www.youtube.com/watch?v=J28_FXEGZlw&ab_channel=NYUCyFor
#
# Code from RS Steganalysis by Brandon Hurst was used in this project
from PIL import Image
import math
import sys
from matplotlib import pyplot as plt
import cv2
import scipy.stats


# this function displays the main menu that the user uses to choose which test to run
def mainmenu():
    print("**********MAIN MENU**********")
    print()
    print("Please select an option.")
    print()
    print("1 : Histogram Analysis")
    print("2 : Chi-Square Test")
    print("3 : RS Steganalysis")
    print("4 : Exit")


# this function generates a histogram, comparing the pixel values and no. pixels between two images
def histogram(filename1, filename2):
    # reads the images, based on the filename
    img1 = cv2.imread(filename1)
    img2 = cv2.imread(filename2)

    # initiate the variables
    x1 = []
    y1 = []
    x2 = []
    y2 = []
    calc_dist = []
    dif = 0

    # calculates the histogram to plot, for both images
    histo1 = cv2.calcHist([img1], [0], None, [256], [0, 256])
    histo2 = cv2.calcHist([img2], [0], None, [256], [0, 256])

    # plots the histogram and adds the labels for both lines
    plt.plot(histo1, label="Suspect Image")
    plt.plot(histo2, label="Original Image")

    # adds the axis labels
    plt.xlabel("Pixel Value")
    plt.ylabel("No. Pixels")

    # adds the key
    plt.legend(loc='upper left')

    # creates a list of the values in the x axis for the histograms
    for n in range(256):
        x1.append(n)
        x2.append(n)

    # adds the pixel values from image 1 to a list
    for xy in histo1:
        y1.append(xy[0])

    # adds the pixel values from image 2 to a list
    for xy in histo2:
        y2.append(xy[0])

    # changes the type of content in the lists to int
    x1 = [int(i) for i in x1]
    y1 = [int(i) for i in y1]
    x2 = [int(i) for i in x2]
    y2 = [int(i) for i in y2]

    # calculates the difference in the y values, as the x values are a constant 0-256
    for i in range(256):
        dist = y2[i] - y1[i]
        # adds the difference values to a list
        calc_dist.append(dist)
        # calculates the total number of pixels that have been altered
        if calc_dist[i] != 0:
            dif += 1

    # calculates the percentage of pixels that have been altered and rounds the values to 2 decimal places
    percent = (dif / len(calc_dist)) * 100
    r_percent = round(percent, 2)
    print("Total number of changes detected: ", dif, "/", len(calc_dist), " pixels have been altered (", r_percent, "%)")

    # displays the histogram of image 1 and 2
    plt.show()

    # plots, labels and displays the difference values
    plt.plot(x1, calc_dist, label="Difference")
    plt.xlabel("Pixel Value")
    plt.ylabel("Difference")
    plt.show()


# def LSBs(filename):
#
#     img = Image.open(filename)
#     pix = img.load()
#     lsb = []
#     for x in range(img.size[0]):
#         for y in range(img.size[1]):
#             val = pix[x, y]
#
#             if val[0] % 2 == 0:
#                 red = 0
#             else:
#                 red = 1
#
#             if val[1] % 2 == 0:
#                 green = 0
#             else:
#                 green = 1
#
#             if val[2] % 2 == 0:
#                 blue = 0
#             else:
#                 blue = 1
#             rgb = red, green, blue
#             lsb.append(list(rgb))
#
#     # f = open("lsbs.txt",'w')
#     # f.write(str(lsb))
#     # f.close()
#
#     return lsb

# this function calculates the observed values for the chi-square test, maths from Westfeld and Pfitzmann
def observed(lst):
    obs = []

    for i in range(0, len(lst) - 1):
        obs.append(lst[i]+1)

    return obs


# this function calculates the expected values for the chi-square test, maths from Westfeld and Pfitzmann
def expected(lst):
    exp = []

    for i in range(0, len(lst) - 1, 2):
        exp.append((lst[i] + lst[i + 1]) / 2)

    return exp


# this function calculates the chi and p values
def chitest(lst):

    # takes the observed and expected values from the corresponding function
    obs = observed(lst)
    exp = expected(obs)

    # takes every second value from the list of observed values
    obs_even = obs[::2]

    # calculate the chi and p values, using a pre built chi squared function
    chi, pval = scipy.stats.chisquare(obs_even, exp)

    return chi, pval


# this function creates a histogram for the chi-square test and displays the value
def chi2(img):

    # calculate the histogram of the image
    hist = img.histogram()

    # calculate the red, green and blue values of the image, from the histogram
    rchi, rp = chitest(hist[0:255])
    gchi, gp = chitest(hist[256:511])
    bchi, bp = chitest(hist[512:767])

    # display the calculated values and work out the total percentage
    print()
    print()
    print("RED: chi = ", round(rchi, 2), " Probability = ", round(rp * 100, 2), "%")
    print()
    print("GREEN: chi = ", round(gchi, 2), " Probability = ", round(gp * 100, 2), "%")
    print()
    print("BLUE: chi = ", round(bchi, 2), " Probability =  ", round((bp * 100), 2), "%")
    print()
    total = round(((rp + gp + bp) / 3) * 100, 2)
    print()
    print("TOTAL PERCENT: ", total, "%")


# this function creates a discrimination function
# which is used to calculate the smoothness and regularity of a group of pixels
# based on RS steganalysis project by Brandon Hurst
def discrimination_function(group):

    # initialise variables
    amount = 0
    totalrow = len(group)
    totalcolumn = len(group[0])

    # the discrimination function is based on the work of friedrich et al
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

# based on RS steganalysis project by Brandon Hurst
# this applies the mask to the group of pixels
def groupmask(gmask, mask):

    # initialise the new mask variable
    newgroupmask = []

    # adds to the list for the new mask
    for line in gmask:
        newgroupmask.append(list(line))

    # sets the row and column values
    totalrow = len(newgroupmask)
    totalcolumn = len(newgroupmask[0])

    # adds/subtracts from the group depending on if it is divisible by 2 (mod2)
    for row in range(0, totalrow):
        for column in range(0, totalcolumn):
            if newgroupmask[row][column] % 2 == 0:
                newgroupmask[row][column] += mask[row][column]
            else:
                newgroupmask[row][column] -= mask[row][column]

    # returns the calculated mask
    return newgroupmask


# this function takes an array of pixel values and returns a small group based on the provided position
# based on RS steganalysis project by Brandon Hurst
def breakimage(imagearray, maskk, position):

    # initiate a new list
    brokeimage = []

    # adds each line to the list
    for line in maskk:
        brokeimage.append(list(line))

    # cycles through the image with the chosen mask to break the image
    for temprow in range(0, len(maskk)):
        for tempcol in range(0, len(maskk[0])):
            brokeimage[temprow][tempcol] = imagearray[temprow + position[0]][tempcol + position[1]]

    return brokeimage


# this function takes an image and splits the colours into separate arrays
# based on RS steganalysis project by Brandon Hurst
def splitpixels(img):

    # initialises number of pixels and the list for each colour
    pixnum = 0
    redrow = []
    greenrow = []
    bluerow = []
    red = []
    green = []
    blue = []

    # the get data function is used to retrieve the value of each pixel in the image
    rgb = img.getdata()

    # calculates the total number of pixels in the image by multiplying the width by the height, and displays it
    print("")
    print("Retrieving RGB Pixel Values")
    width, height = img.size
    totalpix = int(width)*int(height)
    print("Size: ", width, "x", height, "( Total Pixels: ", totalpix, ")")

    # using the getdata function the pixel values are split into red, green and blue values
    for pix in rgb:

        # the position of the value is used to determine if it is red, green or blue
        redrow.append(pix[0])
        if len(pix) > 1:  # incase the image is grayscale
            greenrow.append(pix[1])
            if len(pix) > 2:  # incase the image is grayscale
                bluerow.append(pix[2])
        pixnum += 1

        # adds the separated pixel values to a list
        if pixnum % img.size[0] == 0:
            red.append(redrow)
            redrow = []
            if len(pix) > 1:  # incase the image is grayscale
                green.append(greenrow)
                greenrow = []
                if len(pix) > 2:  # incase the image is grayscale
                    blue.append(bluerow)
                    bluerow = []
    print(pixnum, " pixels")

    return red, green, blue


# determines the percent of pixels with embedded information
# based on RS steganalysis project by Brandon Hurst
def analyseimage(img, chosen_mask, discriminator_overlap):

    # initiating the negative mask
    nmask = []

    # cycles through the selected mask, flipping the values to create the negative mask
    for line in chosen_mask:
        nmask.append(list(line))
    for row in range(0, len(nmask)):
        for column in range(0, len(nmask[0])):
            if nmask[row][column] == 1:
                nmask[row][column] = -1
            elif nmask[row][column] == -1:
                nmask[row][column] = 1

    # calls the splitpixels function to calculate and split the values of rgb pixels
    redpix, greenpix, bluepix = splitpixels(img)

    # displays the size of the chosen mask
    print("")
    print("Mask size = ", len(chosen_mask[0]), "x", len(chosen_mask))

    # analyses the red pixels to determine what percent of them may contain embedded content
    print("")
    print("Analysing Red LSBs")
    redpercent = analyseLSBs(redpix, chosen_mask, nmask, discriminator_overlap)

    # analyses the green pixels to determine what percent of them may contain embedded content
    print("")
    print("Analysing Green LSBs")
    greenpercent = analyseLSBs(greenpix, chosen_mask, nmask, discriminator_overlap)

    # analyses the blue pixels to determine what percent of them may contain embedded content
    print("")
    print("Analysing Blue LSBs")
    bluepercent = analyseLSBs(bluepix, chosen_mask, nmask, discriminator_overlap)

    # controls any errors within the calculations
    print("")
    failed = 0
    if redpercent == 0:
        failed += 1
        print("Unable to calculate the percent of red encoded pixels")
    else:
        print("Decimal value of red pixels likely to be encoded : ", redpercent)
    if greenpercent == 0:
        failed += 1
        print("Unable to calculate the percent of green encoded pixels")
    else:
        print("Decimal value of green pixels likely to be encoded : ", greenpercent)
    if bluepercent == 0:
        failed += 1
        print("Unable to calculate the percent of blue encoded pixels")
    else:
        print("Decimal value of blue pixels likely to be encoded : ", bluepercent)
    if failed == 3:
        print("All colours gave a negative root so a prediction is not possible")
        print("")
        encodedpercent = "?"
    else:
        # calculates and displays the total percentage of pixels that are likely to be encoded
        encodedpercent = (redpercent + greenpercent + bluepercent) / (3 - failed)
        print("")
        print("Decimal value probability of pixels likely to be encoded: ", encodedpercent)
        totalpercent = (encodedpercent * 100)
        print("Total Percent of pixels likely to contain embedded data: ", round(totalpercent, 2), "%")
        width, height = img.size
        totalpix = int(width) * int(height)
        # the size of the file is calculated by multiplying the percent of encoded pixels by the total number of pixels
        # then this is multiplied by 3 as each pixel requires 3 bits (rgb)
        data = ((encodedpercent * totalpix) * 3)
        print("Approximately ", round(data, 2), " bits of data (", round((data/8000), 2), "KB)")

    return encodedpercent


# analyse a specific array of pixel values
# based on RS steganalysis project by Brandon Hurst
def analyseLSBs(imagebox, chosen_mask, neg_mask, discriminator_overlap):

    # initialises all the regular and singular groups
    r_p2 = 0
    s_p2 = 0
    r_1p2 = 0
    s_1p2 = 0
    neg_r_p2 = 0
    neg_s_p2 = 0
    neg_r_1p2 = 0
    neg_s_1p2 = 0

    # reassigns the image box and chosen mask into more usable variables
    imrow = len(imagebox)
    imcol = len(imagebox[0])
    maskrow = len(chosen_mask)
    maskcol = len(chosen_mask[0])

    # determine how many groups of pixels will need to be analysed
    if discriminator_overlap:
        # this method is used if discriminator boxes overlap(box shifts by one pixel each time)
        num = float((imrow - maskrow + 1) * (imcol - maskcol + 1))
    else:
        # used of the discriminator boxes are unique
        num = float((imrow - imrow % maskrow) / maskrow * (imcol - imcol % maskcol) / maskcol)

    print("number of groups to check = ", int(num))

    # initiates the counter
    numcount = 0

    # go row by row through pixel array
    for row in range(0, imrow):

        # then go column by column through pixel array
        for column in range(0, imcol):
            # this set of boxes and calculations is used for overlapping groups
            if discriminator_overlap:
                if row <= imrow - maskrow:
                    if column <= imcol - maskcol:
                        # this is the start of a group
                        pos = [row, column]
                        numcount += 1
                        breakimagebox = breakimage(imagebox, chosen_mask, pos)

                        # flips all of the LSBs in the group
                        flip_box = []
                        for line in breakimagebox:
                            flip_box.append(list(line))
                        for fliprow in range(0, len(breakimagebox)):
                            for flipcolumn in range(0, len(breakimagebox[0])):
                                if breakimagebox[fliprow][flipcolumn] % 2 == 0:
                                    flip_box[fliprow][flipcolumn] += 1
                                elif breakimagebox[fliprow][flipcolumn] % 2 == 1:
                                    flip_box[fliprow][flipcolumn] += -1

                        # this calculates the regular and singular groups using the discrimination function
                        discr_breakimagebox = discrimination_function(breakimagebox)
                        discr_mask_breakimagebox = discrimination_function(groupmask(breakimagebox, chosen_mask))
                        discr_neg_mask_breakimagebox = discrimination_function(groupmask(breakimagebox, neg_mask))
                        discr_flip_box = discrimination_function(flip_box)
                        discr_mask_flip_box = discrimination_function(groupmask(flip_box, chosen_mask))
                        discr_neg_mask_flip_box = discrimination_function(groupmask(flip_box, neg_mask))

                        # sorts regular and singular groups
                        if discr_breakimagebox > discr_mask_breakimagebox:
                            s_p2 += 1
                        elif discr_breakimagebox < discr_mask_breakimagebox:
                            r_p2 += 1

                        if discr_breakimagebox > discr_neg_mask_breakimagebox:
                            neg_s_p2 += 1
                        elif discr_breakimagebox < discr_neg_mask_breakimagebox:
                            neg_r_p2 += 1

                        if discr_flip_box > discr_mask_flip_box:
                            s_1p2 += 1
                        elif discr_flip_box < discr_mask_flip_box:
                            r_1p2 += 1

                        if discr_flip_box < discr_neg_mask_flip_box:
                            neg_r_1p2 += 1
                        elif discr_flip_box > discr_neg_mask_flip_box:
                            neg_s_1p2 += 1

                        # prints an updating value of the groups checked so far
                        if numcount % 1000 == 0:
                            sys.stdout.write('\rgroups checked so far = ' + str(numcount))

            # used for non-overlapping groups
            else:
                if (row + 1) % maskrow == 0:
                    if (column + 1) % maskcol == 0:
                        # this is the start of the group
                        pos = [row - maskrow + 1, column - maskcol + 1]
                        numcount += 1
                        breakimagebox = breakimage(imagebox, chosen_mask, pos)

                        # this flips all of the LSBs in the group
                        flip_box = []
                        for line in breakimagebox:
                            flip_box.append(list(line))
                        for fliprow in range(0, len(breakimagebox)):
                            for flipcolumn in range(0, len(breakimagebox[0])):
                                if breakimagebox[fliprow][flipcolumn] % 2 == 0:
                                    flip_box[fliprow][flipcolumn] += 1
                                elif breakimagebox[fliprow][flipcolumn] % 2 == 1:
                                    flip_box[fliprow][flipcolumn] += -1

                        # this is where the singular and regular groups are calculated
                        discr_breakimagebox = discrimination_function(breakimagebox)
                        discr_mask_breakimagebox = discrimination_function(groupmask(breakimagebox, chosen_mask))
                        discr_neg_mask_breakimagebox = discrimination_function(groupmask(breakimagebox, neg_mask))
                        discr_flip_box = discrimination_function(flip_box)
                        discr_mask_flip_box = discrimination_function(groupmask(flip_box, chosen_mask))
                        discr_neg_mask_flip_box = discrimination_function(groupmask(flip_box, neg_mask))

                        # sorts regular and singular groups
                        if discr_breakimagebox > discr_mask_breakimagebox:
                            s_p2 += 1
                        elif discr_breakimagebox < discr_mask_breakimagebox:
                            r_p2 += 1

                        if discr_breakimagebox > discr_neg_mask_breakimagebox:
                            neg_s_p2 += 1
                        elif discr_breakimagebox < discr_neg_mask_breakimagebox:
                            neg_r_p2 += 1

                        if discr_flip_box > discr_mask_flip_box:
                            s_1p2 += 1
                        elif discr_flip_box < discr_mask_flip_box:
                            r_1p2 += 1

                        if discr_flip_box < discr_neg_mask_flip_box:
                            neg_r_1p2 += 1
                        elif discr_flip_box > discr_neg_mask_flip_box:
                            neg_s_1p2 += 1

                        # prints an updating value of the groups checked so far
                        if numcount % 1000 == 0:
                            sys.stdout.write('\rgroups checked so far = ' + str(numcount))

    # overwrite the updating value
    print('\rgroups checked  = ', numcount)

    if num == 0:
        return 0

    # defines variables to be used in the quadratic equations
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


# this function is used to exit the program
def ext():
    print("Goodbye")


# this function controls the menu and the function of each input
def opmenu():
    # calls the main menu
    mainmenu()

    # list of masks used for RS steganalysis
    # to change the mask, replace the mask that is being passed into analyseimage in the rs section below
    m0 = [[0, 1, 0]]
    m1 = [[0, 1, 1, 1, 0]]
    m2 = [[0, 0, 0], [0, 1, 0], [0, 0, 0]]
    m3 = [[0, 1, 0], [1, 1, 1], [0, 1, 0]]
    m4 = [[0, 0, 0, 0], [0, 1, 1, 0], [0, 1, 1, 0], [0, 0, 0, 0]]
    # According to research by the University of Oxford, m2 is the best performing mask

    # the discriminator overlap controls if the mask overlaps with adjacent pixels
    discriminator_overlap = 0

    # controls the menus by taking user input
    option = input("please select an option : ")

    # when a test is chosen the required images are passed into the correct function for the tests to be carried out
    if option == '1':
        print()
        print()
        print("**********Histogram Analysis**********")
        print()
        print("1: Create a Histogram")
        print("2: Return")
        option = input("Please select an option: ")
        if option == '2':
            opmenu()
        elif option == '1':
            filename1 = input("Please enter the name of the suspect image you wish to open: ")
            filename2 = input("Please enter the name of the original image you wish to open: ")
            histogram(filename1, filename2)
    elif option == '2':
        print()
        print()
        print("**********Chi-Square Analysis**********")
        print()
        print("1: Run Analysis")
        print("2: Return")
        option = input("Please select an option: ")
        if option == '2':
            opmenu()
        elif option == '1':
            filename = input("Please enter the name of the file you wish to open: ")
            img = Image.open(filename)
            chi2(img)
    elif option == '3':
        print()
        print()
        print("**********RS Analysis**********")
        print("1: Run Analysis")
        print("2: Return")
        option = input("Please select an option: ")
        if option == '2':
            opmenu()
        elif option == '1':
            filename = input("Please enter the name of the file you wish to open: ")
        image_filename = Image.open(filename)

        analyseimage(image_filename, m2, discriminator_overlap)

    elif option == '4':
        ext()


opmenu()