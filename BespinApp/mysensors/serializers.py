from rest_framework import serializers
from .models import Datum

class DatumSerializer(serializers.ModelSerializer):
    class Meta:
        model = Datum
