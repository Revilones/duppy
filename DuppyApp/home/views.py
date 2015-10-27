import datetime

from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.core.urlresolvers import reverse
from django.shortcuts import redirect, render
from django.views.generic import View

from .models import Room

def index(request, *args, **kwargs):
    rooms = Room.objects.filter(enabled=True).order_by("pk")
    if request.user.is_authenticated():
        rooms = rooms.filter(user=request.user)

    context = {"rooms": rooms, "user": request.user}
    return render(request, "home/home.html", context)

class LoginView(View):

    def get(self, request, *args, **kwargs):
        if request.user.is_authenticated():
            return redirect(reverse("home:index"))
        return render(request, "home/login.html")

    def post(self, request, *args, **kwargs):
        invalid = inactive = False
        username = request.POST["username"]
        password = request.POST["password"]
        user = authenticate(username=username, password=password)
        if user is not None:
            if user.is_active:
                login(request, user)
                return redirect(request.GET.get("next", "/"))
            else:
                inactive = True
        else:
            invalid = True
        context = {"error": inactive or invalid}
        return render(request, "home/login.html", context)


class LogoutView(View):

    def get(self, request, *args, **kwargs):
        logout(request)
        return redirect(reverse("home:index"))
