from django import forms

from .models import Order, driverInfo


class OrderModelForm(forms.ModelForm):

    class Meta:
        model = Order
        fields = [
            'startpoint',
            'destination',
            'requiredArrival',
            'numPassengers',
            'vehicleType',
            'isShare',
            'specialRequest'
        ]


class DriverInfoModelForm(forms.ModelForm):

    class Meta:
        model = driverInfo
        fields = [
            'fullName',
            'carNum',
            'vehicleType',
            'vehicleCapacity',
            'satisfiedRequest'
        ]


class SharerRequirementForm(forms.ModelForm):
    destination = forms.CharField()
    earliestArrival = forms.DateTimeField()
    latestArrival = forms.DateTimeField()
    latestArrival = forms.DecimalField()


class RawOrderForm(forms.Form):
    startpoint = forms.CharField()
    destination = forms.CharField()
    requiredArrival = forms.DateTimeField()
    numPassengers = forms.DecimalField()
    vehicleType = forms.CharField()
    isShare = forms.BooleanField()
    specialRequest = forms.CharField()
