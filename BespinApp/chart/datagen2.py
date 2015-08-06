# run script from "python manage.py shell"

import datetime

from django.utils import timezone
from random import randrange

from base.models import Controller, Node, Sensor, Data

controller = Controller.objects.get(name="chartcontroller")

Node.objects.get(controller=controller).delete()

node = controller.node_set.create(node_id=1, name="node1")

# sensor = node.sensor_set.create(sensor_id=1, name="sensor1")

# payload = 25.0

# for i in range(60):
#     last_payload = payload
#     payload = last_payload + .1 * randrange(-20,21)
#     data = sensor.data_set.create(payload = str(payload))
#     data.created = timezone.now() - datetime.timedelta(days=i*.25)
#     data.save()

def create_sensor_data(sid, name, type, base, variation, accuracy):
	sensor = node.sensor_set.create(sensor_id=sid, name=name)

	payload = base

	for i in range(60):
	    last_payload = payload
	    payload = last_payload + accuracy * randrange(-variation, variation+1)
	    data = sensor.data_set.create(payload = str(payload))
	    data.created = timezone.now() - datetime.timedelta(days=i*.25)
	    data.save()

# check spelling on sensor types, test
create_sensor_data(1, "tempsensor", "TEMP", 25.0, 20, .1)
create_sensor_data(2, "co2sensor", "CO2", 400, 30, 1)
create_sensor_data(3, "humsensor", "HUMIDITY", 50, 10, .1)