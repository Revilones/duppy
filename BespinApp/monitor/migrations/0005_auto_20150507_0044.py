# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('monitor', '0004_auto_20150506_2312'),
    ]

    operations = [
        migrations.RenameField(
            model_name='sensormonitor',
            old_name='enable_notification',
            new_name='enabled',
        ),
    ]
