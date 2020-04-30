from django.urls import reverse
from django.shortcuts import render, get_object_or_404, redirect
from django.contrib.auth.mixins import LoginRequiredMixin, UserPassesTestMixin
from django.db.models import Q

from django.views.generic import (
    CreateView,
    DetailView,
    UpdateView,
    ListView,
    DeleteView
)

from .forms import OrderModelForm, DriverInfoModelForm, SharerRequirementForm
from .models import Order, driverInfo


def order_sharer_lookup_view(request):
    destination = request.GET.get('destination')
    earliestArrival = request.GET.get('earliestArrival')
    latestArrival = request.GET.get('latestArrival')
    post_list = Order.objects.filter(Q(destination=destination) & Q(requiredArrival__range=(earliestArrival, latestArrival)) & Q(isShare=True) & Q(isShareFound=False) & Q(status='non-confirmed'))

    context = {
        'post_list': post_list
    }
    return render(request, "order/order_sharer_list.html", context)


def order_sharer_join_view(request, my_id):
    obj = get_object_or_404(Order, id=my_id)
    if request.method == "POST":
        obj.isShareFound = True
        obj.sharerName = request.user.username
        obj.save()
        return redirect("../")

    context = {
        'object': obj
    }
    return render(request, "order/order_sharer_join.html", context)


def order_sharer_detail_view(request, my_id):
    obj = get_object_or_404(Order, id=my_id)

    context = {
        'object': obj,
        'orderID': my_id
    }
    return render(request, "order/order_sharer_detail.html", context)


