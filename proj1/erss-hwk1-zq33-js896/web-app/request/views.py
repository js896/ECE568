from django.urls import reverse
from django.shortcuts import render, get_object_or_404
from django.contrib.auth.mixins import LoginRequiredMixin, UserPassesTestMixin
from django.contrib.auth.models import User
from django.db.models import Q

from django.views.generic import (
    CreateView,
    DetailView,
    UpdateView,
    ListView,
    DeleteView
)

from .forms import OrderModelForm
from .models import Order, driverInfo



# Create your views here.


class OrderCreateView(LoginRequiredMixin, CreateView):
    template_name = 'order/request_create.html'
    form_class = OrderModelForm
    queryset = Order.objects.all()

    def form_valid(self, form):
        form.instance.ownerName = self.request.user.username
        # form.instance.user.add(self.request.user)
        return super().form_valid(form)


class OrderListView(LoginRequiredMixin, ListView):
    template_name = 'order/request_list.html'

    def get_queryset(self):
        return Order.objects.filter((Q(ownerName=self.request.user.username) | Q(sharerName=self.request.user.username)) & (Q(status='confirmed') | Q(status='non-confirmed')))


def OrderDetailView(request, my_id):
    obj = get_object_or_404(Order, id=my_id)
    driver = None
    sharer = None
    if obj.status != 'non-confirmed':
        driverUser = User.objects.get(username=obj.driverName)
        driver = driverInfo.objects.get(user=driverUser)
    if obj.isShare == True and obj.isShareFound == True:
        sharer = User.objects.get(username=obj.sharerName)

    context = {
        'object': obj,
        'driver': driver,
        'sharer': sharer
    }
    return render(request, "order/request_detail.html", context)


class OrderUpdateView(LoginRequiredMixin, UserPassesTestMixin, UpdateView):
    template_name = 'order/request_update_view.html'
    form_class = OrderModelForm
    queryset = Order.objects.all()

    def get_object(self):
        id_ = self.kwargs.get("id")
        return get_object_or_404(Order, id=id_)

    def form_valid(self, form):
        form.instance.ownerName = self.request.user.username
        return super().form_valid(form)

    def test_func(self):
        post = self.get_object()
        if self.request.user.username == post.ownerName:
            return True
        return False
