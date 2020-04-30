from django import forms
from django.forms import ModelForm
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm
from .models import product, purchase, reviews


class purchaseForm(forms.ModelForm):
    class Meta:
        model = purchase
        fields = ['count','x','y']
        labels = {
            "count": "Please choose how mant you want to buy",
            "x": "Please choose x coordinate of your address",
            "y": "Please choose y coordinate of your address",
        }
        widgets = {
            "count": forms.widgets.NumberInput(attrs={"class": "form-control"}),
            "x": forms.widgets.NumberInput(attrs={"class": "form-control"}),
            "y": forms.widgets.NumberInput(attrs={"class": "form-control"}),
        }

class reviewForm(forms.ModelForm):
    class Meta:
        model = reviews
        fields = ['review']
        widgets = {
            "count": forms.widgets.TextInput(attrs={"class": "form-control"}),
        }
