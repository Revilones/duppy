import datetime

from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.core.urlresolvers import reverse
from django.shortcuts import redirect, render
from django.views.generic import View

from .models import Room

@login_required(login_url="/login")
def index(request, *args, **kwargs):
    rooms = Room.objects.filter(user=request.user)

    context = {"rooms": rooms}
    return render(request, "dashboard/index.html", context)

class LoginView(View):

    def get(self, request, *args, **kwargs):
        if request.user.is_authenticated():
            return redirect(reverse("dashboard:index"))
        return render(request, "dashboard/login.html")

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
        return render(request, "dashboard/login.html", context)


class LogoutView(View):

    def get(self, request, *args, **kwargs):
        logout(request)
        return redirect(reverse("dashboard:index"))
