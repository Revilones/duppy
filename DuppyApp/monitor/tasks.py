from datetime import timedelta
from celery import task
from django.db import models
from django.db.models import Count, Avg
from base.models import *
from home.models import *
from twilio.rest import TwilioRestClient 

ACCOUNT_SID = "AC62208c6ddd097cf5a50cfb7c96153eb0" 
AUTH_TOKEN = "8d84e62455cc88522face7c68eeacf38" 
NUMBER = "+13607194051"

def send_message(monitor, message):
    client = TwilioRestClient(ACCOUNT_SID, AUTH_TOKEN) 
 
    client.messages.create(
	    to=monitor.phone_number, 
	    from_=NUMBER, 
	    body=message,  
)

def check_tolerance(room, sensors, monitor):

    failed = False 
    message = "\nRoom: %s\n" % (room.name)

    for sensor in sensors:
        end = sensor.data_set.latest().created
        begin  = end - timedelta(minutes=monitor.notice_range)
        data = sensor.data_set.filter(created__range=[begin,end])
        average = data.aggregate(Avg('payload'))['payload__avg']

        if (average > monitor.max_value or average < monitor.min_value):
            failed = True
            message += "Sensor: %s=%d\n"\
                        % (sensor.name, average)

    if failed:
        message += "Out of Range\n"
        send_message(monitor, message)

    return message

@task()
def room_monitor():
    results = []

    rooms = Room.objects.all()
    for room in rooms:
        monitors = room.list_monitors()
        for monitor in monitors:
            if monitor.enabled == True:
                sensor_type = monitor.sensor_type
                sensors = room.filter_by_sensortype(sensor_type)
                results.append(check_tolerance(room, sensors, monitor))
        
    return results
