from django import forms
from config import settings

algo = [(1, "Pixel Value Difference 2x Substitution"), (2, "Reversible Discrete Cosine Transform"), (3,"Edge Detection Based Steganography")]
class ImageEncryptForm(forms.Form):
    message = forms.CharField(required = True, widget = forms.Textarea)
    algorithm = forms.ChoiceField(choices = algo, required = True)
    image = forms.ImageField(required = True)


class ImageDecryptForm(forms.Form):
    recovery_key = forms.IntegerField(max_value = 100000, min_value = 0, required = True)
    algorithm = forms.ChoiceField(choices = algo, required = True)
    image = forms.ImageField(required = True)
