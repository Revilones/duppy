"""
Webcam Views
"""
from django.shortcuts import get_object_or_404
from django.utils.decorators import method_decorator
from django.views.decorators.csrf import csrf_exempt

from rest_framework import status
from rest_framework.authentication import TokenAuthentication, \
                                          SessionAuthentication
from rest_framework.response import Response
from rest_framework.views import APIView

from .models import Webcam
from .serializers import WebcamSerializer


class WebcamSetView(APIView):
    """
    Webcam Set View
    """

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(WebcamSetView, self).dispatch(*args, **kwargs)

    def get(self, request):
        webcams = Webcam.objects.filter(user=request.user)
        serializer = WebcamSerializer(webcams, many=True)

        return Response(serializer.data)

class WebcamView(APIView):
    """
    Webcam View
    """

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(WebcamView, self).dispatch(*args, **kwargs)

    def get(self, request, webcam_id):
        webcam = get_object_or_404(Webcam,
                user=request.user,
                pk=webcam_id)
        serializer = WebcamSerializer(webcam)

        return Response(serializer.data)
