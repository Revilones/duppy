from django import template
from django.core.urlresolvers import reverse

register = template.Library()

@register.simple_tag
def navactive(request, urls):
    if request.path.strip('/') in ( url for url in urls.split() ):
        return "active"
    return ""
