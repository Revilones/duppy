from django.conf.urls import patterns, url
from rest_framework.urlpatterns import format_suffix_patterns
from . import views

urlpatterns = [
    #This is interfering with SensorDataView... Why?
    url(r'controllers/?$',
            views.ControllerSetView.as_view(),
            name="api-controller-set-view"),
    url(r'controllers/(?P<controller_id>[a-zA-Z0-9]+)/?$',
            views.ControllerView.as_view(),
            name="api-controller-view"),

    url(r'controllers/(?P<controller_id>[a-zA-Z0-9]+)/nodes/?$',
            views.NodeSetView.as_view(),
            name="api-node-set-view"),
    url(r'controllers/(?P<controller_id>[a-zA-Z0-9]+)/nodes/(?P<node_id>[0-9]+)/?$',
            views.NodeView.as_view(),
            name="api-node-view"),

    url(r'controllers/(?P<controller_id>[a-zA-Z0-9]+)/nodes/(?P<node_id>[0-9]+)/sensors/?$',
            views.SensorSetView.as_view(),
            name="api-sensor-set-view"),
    url(r'controllers/(?P<controller_id>[a-zA-Z0-9]+)/nodes/(?P<node_id>[0-9]+)/sensors/(?P<sensor_id>[0-9]+)/?$',
            views.SensorView.as_view(),
            name="api-sensor-view"),
    url(r'controllers/(?P<controller_id>[a-zA-Z0-9]+)/sensor-types/?$',
            views.SensorTypeView.as_view(),
            name="api-sensor-type-view"),
    url(r'sensors/(?P<sensor_type>[a-zA-Z0-9]+)?$',
            views.SensorSetTypeView.as_view(),
            name="api-sensor-set-type-view"),
    url(r'controllers/(?P<controller_id>[a-zA-Z0-9]+)/nodes/(?P<node_id>[0-9]+)/sensors/(?P<sensor_id>[0-9]+)/data/?$',
            views.SensorDataView.as_view(),
            name="api-sensor-data-view"),
    url(r'controllers/(?P<controller_id>[a-zA-Z0-9]+)/nodes/(?P<node_id>[0-9]+)/sensors/(?P<sensor_id>[0-9]+)/data/(?P<beginSec>[0-9]+)&(?P<endSec>[0-9]+)/?$',
            views.SensorDataDateView.as_view(),
            name="api-sensor-data-date-view"),

    url(r'data/?$',
            views.DataView.as_view(),
            name="api-data-view"),
]

urlpatterns = format_suffix_patterns(urlpatterns)
