from django.template.loader import render_to_string
from django.db import models

from base.models import Sensor as BaseSensor, User

class Sensor(BaseSensor):

    class Meta:
        proxy = True

    def as_html(self):
        context = {}
        context = {"sensor": self}
        return render_to_string("home/%s.html" % self.sensor_type, context)

    def last_seen(self):
        return self.data_set.latest("created")

class Room(models.Model):
    user = models.ForeignKey(User)
    sensors = models.ManyToManyField(Sensor, through="RoomSensorsRelationship")
    name = models.CharField(max_length=250)
    enabled = models.BooleanField(default=False)

    def __str__(self):
        return self.name

    def list_sensors(self):
        return self.sensors.all().order_by("roomsensorsrelationship__order")

class RoomSensorsRelationship(models.Model):
    sensor = models.ForeignKey(Sensor)
    room = models.ForeignKey(Room)
    order = models.IntegerField(default=0)
