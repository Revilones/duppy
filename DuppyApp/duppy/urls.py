from django.conf.urls import patterns, include, url
from django.contrib import admin
from django.views.generic import RedirectView

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'bespin.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    #(r'^grappelli/', include('grappelli.urls')), # grappelli URLS
    url(r'^admin/', include(admin.site.urls)), # admin site
    url(r'^api/', include('api.urls')),
    url(r'', include('webcam.urls')),
    url(r'^api-auth/', include('rest_framework.urls',
            namespace='rest_framework')),
    url(r'^home/', include('home.urls', namespace="home")),
    url('^$', RedirectView.as_view(pattern_name="home:index",
                                 permanent=False)),
    url(r'^chart/', include('chart.urls', namespace="chart")),
)
