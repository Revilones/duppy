# copy&paste into shell started with "python manage.py shell"
# requires a controller named "testcontroller" in database

import datetime

from django.utils import timezone
from random import randrange

from base.models import Controller, Node, Sensor, Data

TIMELENGTH = 48    # number of data points per sensor (1 per hour)
S_ID = 1

controller = Controller.objects.get(name="testcontroller")

# delete previous data
try:
    Node.objects.get(controller=controller).delete()
except:
    pass

node = controller.node_set.create(node_id=1, name="node1")

def create_sensor_data(s_type, start_val, variation, accuracy):
    global S_ID
    sensor = node.sensor_set.create(
        sensor_id = S_ID, 
        name = "sensor_"+s_type[:3]+"_"+str(S_ID), 
        sensor_type = s_type
    )
    S_ID += 1
    payload = start_val
    for i in range(TIMELENGTH):
        last_payload = payload
        payload = last_payload + accuracy * randrange(-variation, variation+1)
        data = sensor.data_set.create(controller=controller, payload=str(payload))
        data.created = timezone.now() - datetime.timedelta(hours=i)
        data.save()

create_sensor_data("co2", 400, 30, 1)
create_sensor_data("co2", 400, 30, 1)
create_sensor_data("co2", 400, 30, 1)
create_sensor_data("humidity", 50, 10, .1)
create_sensor_data("temperature", 25.0, 20, .1)

print("Done.")