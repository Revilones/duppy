from django.conf.urls import patterns, url
from rest_framework.urlpatterns import format_suffix_patterns
from . import views

urlpatterns = [
    url(r'^(index)?$', views.index, name="index"),
    url(r'^login$', views.LoginView.as_view(), name="login"),
    url(r'^logout$', views.LogoutView.as_view(), name="logout"),
]

urlpatterns = format_suffix_patterns(urlpatterns)
