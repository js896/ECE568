from django.contrib import admin
# Register your models here.
from .models import product
from .models import purchase
from .models import reviews

admin.site.register(product)
admin.site.register(purchase)
admin.site.register(reviews)
