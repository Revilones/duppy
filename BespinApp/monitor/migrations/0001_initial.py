# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('base', '0003_sensor_enable_notification'),
    ]

    operations = [
        migrations.CreateModel(
            name='sensor_monitor',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('name', models.CharField(max_length=250)),
                ('enable_notification', models.BooleanField(default=False)),
                ('notice_range', models.IntegerField(default=10)),
                ('max_value', models.IntegerField(default=5)),
                ('min_value', models.IntegerField(default=10)),
                ('user', models.ForeignKey(to='base.User')),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.AlterUniqueTogether(
            name='sensor_monitor',
            unique_together=set([('user', 'name')]),
        ),
    ]
