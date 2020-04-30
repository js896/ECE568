from django.db import models
from django.urls import reverse

# Create your models here.

class Account(models.Model):
    userName = models.CharField(max_length = 120)
    userPassword = models.CharField(max_length = 120)
    userFirstName = models.CharField(max_length = 120)
    userLastName = models.CharField(max_length = 120)
    userEmail = models.CharField(max_length = 120)