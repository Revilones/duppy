from django.utils.decorators import method_decorator
from django.views.decorators.csrf import csrf_exempt
from rest_framework.authentication import TokenAuthentication, SessionAuthentication
from rest_framework import generics
from .models import Datum
from .serializers import DatumSerializer


#@csrf_exempt
class DatumList(generics.ListCreateAPIView):
    authentication_classes = (TokenAuthentication,SessionAuthentication)
    queryset = Datum.objects.all()
    serializer_class = DatumSerializer

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(DatumList, self).dispatch(*args, **kwargs)

#@csrf_exempt
class DatumDetail(generics.RetrieveUpdateDestroyAPIView):
    queryset = Datum.objects.all()
    serializer_class = DatumSerializer

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(DatumDetail, self).dispatch(*args, **kwargs)
