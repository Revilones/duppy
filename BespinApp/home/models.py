from django.template.loader import render_to_string
from django.db import models

from base.models import Sensor as BaseSensor, User
from monitor.models import SensorMonitor

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
    monitors = models.ManyToManyField(SensorMonitor, through="RoomMonitorRelationship")
    name = models.CharField(max_length=250)
    enabled = models.BooleanField(default=False)
    
    def __str__(self):
        return self.name

    def list_sensors(self):
        return self.sensors.all().order_by("roomsensorsrelationship__order")

    def filter_by_sensortype(self, sensor_type):
        return self.sensors.all().filter(sensor_type=sensor_type)

    def list_monitors(self):
        return self.monitors.all()


class RoomSensorsRelationship(models.Model):
    sensor = models.ForeignKey(Sensor)
    room = models.ForeignKey(Room)
    order = models.IntegerField(default=0)

class RoomMonitorRelationship(models.Model):
    monitor = models.ForeignKey(SensorMonitor)
    room = models.ForeignKey(Room)
