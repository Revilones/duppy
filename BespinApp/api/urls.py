from django.conf.urls import patterns, url
from rest_framework.urlpatterns import format_suffix_patterns
from . import views

urlpatterns = [
    url(r'data/?$', views.DataView.as_view()),

    url(r'controllers/?$',
            views.ControllerSetView.as_view()),
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
            views.SensorSetView.as_view()),
    url(r'controllers/(?P<controller_id>[a-zA-Z0-9]+)/nodes/(?P<node_id>[0-9]+)/sensors/(?P<sensor_id>[0-9]+)/?$',
            views.SensorView.as_view()),
    #url(r'data/(?P<pk>[0-9]+)$', views.DatumDetail.as_view())
]

urlpatterns = format_suffix_patterns(urlpatterns)
