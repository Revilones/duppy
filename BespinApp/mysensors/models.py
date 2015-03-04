from django.db import models

# Create your models here.

class Datum(models.Model):
    nodeid = models.IntegerField()
    sensorid = models.IntegerField()
    message_type = models.IntegerField()
    sub_type = models.IntegerField()
    payload = models.TextField()
    created = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return "ID: %s - Data: %s;%s;%s;%s;%s - Created: %s" % \
                (self.pk,
                 self.nodeid,
                 self.sensorid,
                 self.message_type,
                 self.sub_type,
                 self.payload,
                 self.created)

    class Meta:
        ordering = ('created',)
        get_latest_by = "created"

