from django.urls import path
from . import views

app_name = 'imageshow'

urlpatterns = [
    path('encrypt', views.encrypt, name = "encrypt_page"),
    path('output/<name>/<int:num>', views.download, name = "download"),
    path('decrypt', views.decrypt, name = "decrypt_page"),
    path('message', views.message, name = "recovered message")
]