from django.shortcuts import render, redirect
from imageshow.forms import ImageEncryptForm, ImageDecryptForm
from django.http import FileResponse
from PIL import Image

from config import settings
from datetime import datetime

from subprocess import call
import os
import re


def handle_uploaded_file(f):
    name =  str(datetime.now()) + ".png"
    outpath = os.path.join(settings.SHARE_IMAGE_UPLOAD_PATH, name)

    with open(outpath, 'wb+') as destination:
        for chunk in f.chunks():
            destination.write(chunk)

    return name, outpath
 
algo = {
    1: "PVD_greyscale",
    2: "Reversible_DCT",
    3: "Edge_LSB"
}

prefix = {
    1: "ep_",
    2: "er_",
    3: "ee_"
}

def encrypt(request):
    if request.method == 'POST':
        form = ImageEncryptForm(request.POST, request.FILES)

        if form.is_valid():
            filename, filepath = handle_uploaded_file(request.FILES['image'])

            algo_value = int(form['algorithm'].value())
            msg_value = form['message'].value()

            call([os.path.join(settings.BASE_DIR, "main"), "-e", algo[algo_value], msg_value, filepath])
            
            outfilepath = prefix[algo_value] + filename

            return redirect('/output/' + outfilepath + '/' + str(len(msg_value)))
        else:
            form = ImageEncryptForm()
            return render(request, 'imageshow/index.html', {'imageform': form})       
    else:
        form = ImageEncryptForm()
        return render(request, 'imageshow/index.html', {'imageform': form})

def decrypt(request):
    if request.method == 'POST':
        form = ImageDecryptForm(request.POST, request.FILES)

        if form.is_valid():
            filename, filepath = handle_uploaded_file(request.FILES['image'])

            algo_value = int(form['algorithm'].value())
            recovery_key = form['recovery_key'].value()

            outfile = os.path.join(settings.BASE_DIR, "outfile")
            open(outfile, "w").close()
            f = open(outfile, 'w')
            
            call([os.path.join(settings.BASE_DIR, "main"), "-d", algo[algo_value], recovery_key, filepath], stdout = f)
            f.close()

            return redirect('/message')
        else:
            form = ImageDecryptForm()
            return render(request, 'imageshow/decrypt.html', {'imageform': form})       
    else:
        form = ImageDecryptForm()
        return render(request, 'imageshow/decrypt.html', {'imageform': form})    

def download(request, name, num):
    print("dasdasdasdasdas")
    return render(request, 'imageshow/output.html', {'imgname': f'userimages/{name}', 're': num})


def message(request):
    f = open(os.path.join(settings.BASE_DIR, "outfile"), "r")
    message = f.read()
    f.close()
    try:
        m = re.search('(?<=Recovered message: ).*', message).group(0)
    except:
        m = ""
    open(os.path.join(settings.BASE_DIR, "outfile"), "w").close()
    return render(request, 'imageshow/recovery.html', {'message': m})
  
