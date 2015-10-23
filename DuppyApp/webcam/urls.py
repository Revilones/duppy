from django.conf.urls import patterns, url

from rest_framework.urlpatterns import format_suffix_patterns

from . import views

urlpatterns = [
    url(r'api/webcams/?$',
            views.WebcamSetView.as_view(),
            name="api-webcam-set-view"),
    url(r'api/webcams/(?P<webcam_id>[0-9]+)/?$',
            views.WebcamView.as_view(),
            name="api-webcam-view"),
]

urlpatterns = format_suffix_patterns(urlpatterns)

