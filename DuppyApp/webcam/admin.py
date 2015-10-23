from django.contrib import admin

from .models import Webcam

class WebcamAdmin(admin.ModelAdmin):
    list_display = ("name", "location", "user")
    list_filter = ("user",)

admin.site.register(Webcam, WebcamAdmin)

