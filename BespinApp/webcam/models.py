from django.contrib.auth.models import User
from django.db import models

class Webcam(models.Model):
    user = models.ForeignKey(User)
    name = models.CharField(max_length=250)
    location = models.TextField()

    def latest_image(self):
        pass
