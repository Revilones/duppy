from rest_framework import serializers

from .models import Webcam


class WebcamSerializer(serializers.Serializer):
    name = serializers.CharField()
    location = serializers.CharField()
    latest_image = serializers.CharField(read_only=True)
