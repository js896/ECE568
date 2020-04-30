from django.urls import reverse
from django.shortcuts import render, get_object_or_404, redirect
from django.core.mail import send_mail
from django.contrib.auth.models import User

from django.contrib.auth.mixins import LoginRequiredMixin, UserPassesTestMixin
from django.views.generic import (
    CreateView,
    DetailView,
    UpdateView,
    ListView,
    DeleteView
)

from .forms import OrderModelForm, DriverInfoModelForm
from .models import Order, driverInfo


class DriverCreateView(LoginRequiredMixin, CreateView):
    template_name = 'order/driverInfo_create.html'
    form_class = DriverInfoModelForm

    def form_valid(self, form):
        form.instance.user = self.request.user
        return super().form_valid(form)


"""
class DriverInfoDetailView(LoginRequiredMixin, DetailView):
    template_name = 'order/driverInfo_detail.html'
    queryset = Order.objects.all()

    def get_object(self):
        return get_object_or_404(driverInfo, user=self.request.user)
"""


def DriverInfoDetailView(request):
    try:
        obj = driverInfo.objects.get(user=request.user)
        context = {
            "object": obj
        }
        return render(request, "order/driverInfo_detail.html", context)
    except driverInfo.DoesNotExist:
        return redirect("orders:order-driver-create")


"""
class DriverInfoUpdateView(LoginRequiredMixin, UserPassesTestMixin, UpdateView):
    template_name = 'order/driverInfo_update.html'
    form_class = DriverInfoModelForm
    queryset = driverInfo.objects.all()

    def form_valid(self, form):
        form.instance.user = self.request.user
        return super().form_valid(form)

    def get_object(self):
        return get_object_or_404(driverInfo, user=self.request.user)

    def test_func(self):
        post = self.get_object()
        if self.request.user == post.user:
            return True
        return False
"""


def DriverInfoUpdateView(request):
    try:
        obj = driverInfo.objects.get(user=request.user)
        form = DriverInfoModelForm(request.POST or None, instance=obj)
        if form.is_valid():
            form.save()
        context = {
            "form": form
        }
        return render(request, "order/driverInfo_update.html", context)
    except:
        return redirect("orders:order-driver-create")


class DriverListView(LoginRequiredMixin, ListView):
    template_name = 'order/order_driver_myOrder.html'

    def get_queryset(self):
        return Order.objects.filter(driverName=self.request.user.username)


def order_driver_lookup_view(request):
    try:
        driver = driverInfo.objects.get(user=request.user)
        querySet = Order.objects.filter(status='non-confirmed').filter(numPassengers__lte=driver.vehicleCapacity)

        context = {
            'objectList': querySet,
            'driver': driver
        }
        return render(request, "order/order_driver_list.html", context)
    except:
        return redirect("orders:order-driver-create")


def order_driver_detail_view(request, my_id):
    obj = get_object_or_404(Order, id=my_id)

    context = {
        'object': obj,
        'orderID': my_id
    }
    return render(request, "order/order_driver_detail.html", context)


def order_driver_confirm_view(request, my_id):
    obj = get_object_or_404(Order, id=my_id)
    if request.method == "POST":
        obj.status = "confirmed"
        obj.driverName = request.user.username
        obj.save()

        to_email = []
        owner = User.objects.get(username=obj.ownerName)
        to_email.append(owner.email)
        sharer = None
        if obj.sharerName:
            sharer = User.objects.get(username=obj.sharerName)
            to_email.append(sharer.email)
        subject = 'Your Order is Confirmed'
        content = 'Your Order is Confirmed, you can check more information'
        from_email = 'duke568_qber@163.com'
        send_mail(subject, content, from_email, to_email)

        return redirect("../allOrders")

    context = {
        'object': obj
    }
    return render(request, "order/order_driver_confirm.html", context)


def order_driver_complete_view(request, my_id):
    obj = get_object_or_404(Order, id=my_id)
    if request.method == "POST":
        obj.status = "completed"
        obj.save()
        return redirect("../allOrders")

    context = {
        'object': obj
    }
    return render(request, "order/order_driver_complete.html", context)
