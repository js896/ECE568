from django.shortcuts import render

# Create your views here.
from django.shortcuts import render

# Create your views here.
def home_view(request, *args, **kwargs):
    return render(request, "home.html", {})

def role_selection_view(request, *args, **kwargs):
    return render(request, "role.html", {})