# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('monitor', '0003_sensormonitor_sensor_type'),
    ]

    operations = [
        migrations.AlterField(
            model_name='sensormonitor',
            name='sensor_type',
            field=models.CharField(default=b'co2', max_length=250, choices=[(b'co2', b'Co2'), (b'temperature', b'Temperature'), (b'humidity', b'Humidity')]),
            preserve_default=True,
        ),
    ]
