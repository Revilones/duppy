from django.contrib import admin
from .models import Controller, Node, Sensor, Data

class ControllerAdmin(admin.ModelAdmin):
    list_display = ("name", "user", "controller_id", "location")
    exclude = ("controller_id",)

class SensorAdmin(admin.ModelAdmin):
    list_display = ("name", "node", "sensor_id", "sensor_type")

class DataAdmin(admin.ModelAdmin):
    list_display = ("sensor", "payload", "created")
    ordering = ("-created",)

admin.site.register(Controller, ControllerAdmin)
admin.site.register(Node)
admin.site.register(Sensor, SensorAdmin)
admin.site.register(Data, DataAdmin)
