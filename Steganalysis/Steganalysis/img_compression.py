from cv2 import cv2
imgpath = "./edg2_img.png"
img = cv2.imread(imgpath)

img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

def save(path, image, jpg_quality=None, png_compression=None):
  if jpg_quality:
    cv2.imwrite(path, image, [int(cv2.IMWRITE_JPEG_QUALITY), jpg_quality])
  elif png_compression:
    cv2.imwrite(path, image, [int(cv2.IMWRITE_PNG_COMPRESSION), png_compression])
  else:
    cv2.imwrite(path, image)

# save the image in JPEG format with 85% quality
outpath_jpeg = "./compressed_edg2_jpg.jpg"
save(outpath_jpeg,img,jpg_quality=40) #higher the no,better the quality
outpath_png = "./compressed_edg2_png.png"
# save the image in PNG format with 4 Compression
save(outpath_png, img,png_compression=9)#0-9, compression value