from django.db import models
from django.db.models import Count, Avg
from base.models import *

# Create your models here.

class SensorMonitor(models.Model):
    user = models.ForeignKey(User)
    sensor_type = models.CharField(max_length=250,
                                   choices=SENSOR_TYPES,
                                   default=CO2)
    name = models.CharField(max_length=250)
    enabled = models.BooleanField(default = False)
    notice_range = models.IntegerField(default = 10)
    max_value = models.IntegerField(default = 5)
    min_value = models.IntegerField(default = 10)
    phone_number = models.CharField(max_length=250, default="")

    def __str__(self):
        return "%s (%s)" % (self.name, self.user)

    class Meta:
        unique_together = (("user", "name"))

#class twilioAccount(models.Model):
#    account_sid = models.CharField(max_length=250, default="")
#    auth_token = models.CharField(max_length=250, default="")
#    phone_number = models.CharField(max_length=250, default="")

