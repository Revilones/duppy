from django.db import models

from base.models import Controller, User


class Orginization(models.Model):
    """
    Orginization represents a company
    """
    name = models.CharField(max_length=250)
    active = models.BooleanField(default=False)
    created = models.DateTimeField(auto_now_add=True)
    users = models.ManyToMany(User)
    controllers = models.ManyToMany(Controller)

