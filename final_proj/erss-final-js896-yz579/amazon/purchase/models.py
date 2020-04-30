from django.db import models
from django.utils import timezone
from django.contrib.auth.models import User


class product(models.Model):
    name = models.CharField(max_length=50)
    description = models.CharField(max_length=100)
    def __str__ (self):
        return self.name



class purchase(models.Model):
    count = models.IntegerField()
    x = models.IntegerField(default=0)
    y = models.IntegerField(default=0)
    status = models.CharField(max_length=50)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    product = models.ForeignKey(product, on_delete=models.CASCADE)
    def __str__ (self):
        return str(self.count)

class reviews(models.Model):
    product = models.ForeignKey(product, on_delete=models.CASCADE)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    review = models.TextField()
    def __str__ (self):
        return str(self.review)
