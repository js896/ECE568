from django.db import models
from django.urls import reverse

#import datetime

from django.contrib.auth.models import User

# Create your models here.


class Order(models.Model):

    orderStatus = (
        ('non-confirmed', 'non-confirmed'),
        ('confirmed', 'confirmed'),
        ('completed', 'completed'),
    )

    vehicleTypes = (
        ('Plain', 'Plain'),
        ('Lux', 'Lux'),
        ('Super Lux', 'Super Lux'),
    )

    specialRequests = (
        ('Super Clean', 'Super Clean'),
        ('Super Fast', 'Super Fast'),
    )
    
    startpoint = models.CharField(max_length = 120)
    destination = models.CharField(max_length = 120)
    # requiredArrival = models.DateTimeField('date and time of arrival', null = True, default=datetime.datetime.now())
    requiredArrival = models.DateTimeField('date and time of arrival', null = True)
    numPassengers = models.DecimalField(max_digits = 2, decimal_places = 0, null = True)
    vehicleType = models.CharField(max_length = 200, null = True, choices=vehicleTypes)
    isShare = models.BooleanField(null = True)
    isShareFound = models.BooleanField(default=False, null=True)
    specialRequest = models.CharField(max_length = 200, null = True, choices=specialRequests)
    status = models.CharField(null = True, max_length = 120, choices=orderStatus, default='non-confirmed')
    #user = models.ManyToManyField(User, null=True)
    ownerName = models.CharField(max_length = 120, null=True)
    driverName = models.CharField(max_length = 120, null=True)
    sharerName = models.CharField(max_length = 120, null=True)

    def get_absolute_url(self):
        return reverse("orders:order-detail", kwargs = {"my_id":self.id})

    def get_driver_absolute_url(self):
        return reverse("orders:order-driver-orderDetail", kwargs = {"my_id":self.id})

    def get_sharer_absolute_url(self):
        return reverse("orders:order-sharer-detail", kwargs = {"my_id":self.id})


class driverInfo(models.Model):
    vehicleTypes = (
        ('Plain', 'Plain'),
        ('Lux', 'Lux'),
        ('Super Lux', 'Super Lux'),
    )

    #vehicleCapacities = (
        #('4 passengers', '4 passengers'),
        #('7 passengers', '7 passengers'),
        #('10 passengers', '10 passengers'),
    #)

    specialRequests = (
        ('Super Clean', 'Super Clean'),
        ('Super Fast', 'Super Fast'),
    )

    fullName = models.CharField(max_length=120)
    carNum = models.CharField(max_length=120)
    vehicleType = models.CharField(max_length=200, null=True, choices=vehicleTypes)
    vehicleCapacity = models.DecimalField(max_digits=2, decimal_places=0, null=True)
    #vehicleCapacity = models.CharField(max_length=200, null=True, choices=vehicleCapacities)
    satisfiedRequest = models.CharField(max_length = 200, null = True, choices=specialRequests)
    user = models.OneToOneField(User, on_delete=models.CASCADE)

    def get_absolute_url(self):
        return reverse("orders:order-driver-detail")

'''
class SharerRequirement(models.Model):
    destination = models.CharField(max_length=120)
    earliestArrival = models.DateTimeField('earliest arrival time', null = True)
    latestArrival = models.DateTimeField('latest arrival time', null = True)
    numPassengers = models.DecimalField(max_digits=2, decimal_places=0, null=True)
'''

