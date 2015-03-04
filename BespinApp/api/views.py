"""
API Views
"""
from django.shortcuts import get_object_or_404
from django.utils.decorators import method_decorator
from django.views.decorators.csrf import csrf_exempt

from rest_framework.authentication import TokenAuthentication, \
                                          SessionAuthentication
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework.reverse import reverse
from rest_framework.views import APIView

from base import models
from .models import Controller, Data, Node, Sensor
from .serializers import ControllerSerializer, DataSerializer, \
                         NodeSerializer, SensorSerializer

#class DatumList(generics.ListCreateAPIView):
#    authentication_classes = (TokenAuthentication,SessionAuthentication)
#    queryset = Datum.objects.all()
#    serializer_class = DatumSerializer
#
#    @method_decorator(csrf_exempt)
#    def dispatch(self, *args, **kwargs):
#        return super(DatumList, self).dispatch(*args, **kwargs)
#
##@csrf_exempt
#class DatumDetail(generics.RetrieveUpdateDestroyAPIView):
#    authentication_classes = (TokenAuthentication,SessionAuthentication)
#    queryset = Datum.objects.all()
#    serializer_class = DatumSerializer
#
#    @method_decorator(csrf_exempt)
#    def dispatch(self, *args, **kwargs):
#        return super(DatumDetail, self).dispatch(*args, **kwargs)

class ControllerView(APIView):
    """
    API Controller View
    """

    authentication_classes = (TokenAuthentication, SessionAuthentication)

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(ControllerView, self).dispatch(*args, **kwargs)

    def get(self, request, controller_id):
        controller = get_object_or_404(models.Controller,
                user=request.user,
                controller_id=controller_id)

        response = Controller(
                uri=reverse('api-controller-view',
                            args=[controller.controller_id],
                            request=request),
                controller_id=controller.controller_id,
                name=controller.name,
                location=controller.location,
                nodes=reverse('api-node-set-view',
                              args=[controller.controller_id],
                              request=request))

        serializer = ControllerSerializer(response)
        return Response(serializer.data)


class ControllerSetView(APIView):
    """
    API Controller Set View
    """

    authentication_classes = (TokenAuthentication, SessionAuthentication)

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(ControllerSetView, self).dispatch(*args, **kwargs)

    def get(self, request):
        controllers = models.Controller.objects.filter(user=request.user)

        response = [Controller(
                        uri=reverse('api-controller-view',
                                    args=[controller.controller_id],
                                    request=request),
                        controller_id=controller.controller_id,
                        name=controller.name,
                        location=controller.location,
                        nodes=reverse('api-controller-view',
                                      args=[controller.controller_id],
                                      request=request))
                    for controller in controllers]

        serializer = ControllerSerializer(response, many=True)
        return Response(serializer.data)


class DataView(APIView):
    """
    API Data Set View
    """

    authentication_classes = (TokenAuthentication, SessionAuthentication)

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(DataView, self).dispatch(*args, **kwargs)

    def get(self, request, *args, **kwargs):
        response = []
        for data in models.Data.objects.all():
            response.append(Data(
                    controller_id=data.sensor.node.controller.controller_id,
                    node_id=data.sensor.node.node_id,
                    sensor_id=data.sensor.sensor_id,
                    payload=data.payload,
                    created=data.created))
        serializer = DataSerializer(response, many=True)
        return Response(serializer.data)

    def post(self, request, *args, **kwargs):
        serializer = DataSerializer(data=request.data)
        if not serializer.is_valid():
            return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

        obj = serializer.data

        # find the controller
        controller = models.Controller.objects.get(
                user=request.user,
                controller_id=obj.get("controller_id"))

        # find the node
        node = models.Node.objects.get(
                controller=controller,
                node_id=obj.get("node_id"))

        # find the sensor
        sensor = models.Sensor.objects.get(
                node=node,
                sensor_id=obj.get("sensor_id"))

        data = models.Data(
                sensor=sensor,
                payload=obj.get("payload"))
        data.save()

        return Response(status=status.HTTP_201_CREATED)


class NodeView(APIView):
    """
    API Node View
    """

    authentication_classes = (TokenAuthentication, SessionAuthentication)

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(NodeView, self).dispatch(*args, **kwargs)

    def get(self, request, controller_id, node_id):
        controller = get_object_or_404(models.Controller,
                user=request.user,
                controller_id=controller_id)

        node = get_object_or_404(models.Node,
                controller=controller,
                node_id=node_id)

        response = Node(
                controller_id=controller.controller_id,
                node_id=node.node_id,
                name=node.name)

        serializer = NodeSerializer(response)
        return Response(serializer.data)


class NodeSetView(APIView):
    """
    API Node Set View
    """

    authentication_classes = (TokenAuthentication, SessionAuthentication)

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(NodeSetView, self).dispatch(*args, **kwargs)

    def get(self, request, controller_id):
        controller = get_object_or_404(models.Controller,
                user=request.user,
                controller_id=controller_id)

        nodes = models.Node.objects.filter(controller=controller)

        response = [Node(
                        controller_id=controller.controller_id,
                        node_id=node.node_id,
                        name=node.name)
                    for node in nodes]

        serializer = NodeSerializer(response, many=True)
        return Response(serializer.data)


class SensorView(APIView):
    """
    API Sensor View
    """

    authentication_classes = (TokenAuthentication, SessionAuthentication)

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(SensorView, self).dispatch(*args, **kwargs)

    def get(self, request, controller_id, node_id, sensor_id):
        controller = get_object_or_404(models.Controller,
                user=request.user,
                controller_id=controller_id)

        node = get_object_or_404(models.Node,
                controller=controller,
                node_id=node_id)

        sensor = get_object_or_404(models.Sensor,
                node=node,
                sensor_id=sensor_id)

        data = sensor.data_set.latest("created")
        latest_data = data.payload if data else None
        last_update = data.created if data else None



        response = Sensor(
                controller_id=controller.controller_id,
                node_id=node.node_id,
                sensor_id=sensor.sensor_id,
                sensor_type=sensor.sensor_type,
                name=sensor.name,
                latest_data=latest_data,
                last_update=last_update)

        serializer = SensorSerializer(response)
        return Response(serializer.data)


class SensorSetView(APIView):
    """
    API Sensor Set View
    """

    authentication_classes = (TokenAuthentication, SessionAuthentication)

    @method_decorator(csrf_exempt)
    def dispatch(self, *args, **kwargs):
        return super(SensorSetView, self).dispatch(*args, **kwargs)

    def get(self, request, controller_id, node_id):
        controller = get_object_or_404(models.Controller,
                user=request.user,
                controller_id=controller_id)

        node = get_object_or_404(models.Node,
                controller=controller,
                node_id=node_id)

        sensors = models.Sensor.objects.filter(node=node)

        response = [Sensor(
                        controller_id=controller.controller_id,
                        node_id=node.node_id,
                        sensor_id=sensor.sensor_id,
                        sensor_type=sensor.sensor_type,
                        name=sensor.name,
                        latest_data=sensor.data_set.latest("created").payload,
                        last_update=sensor.data_set.latest("created").created)
                    for sensor in sensors]

        serializer = SensorSerializer(response, many=True)
        return Response(serializer.data)
