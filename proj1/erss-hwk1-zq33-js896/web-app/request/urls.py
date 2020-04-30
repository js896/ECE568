from django.urls import path
from .views import(
    OrderUpdateView,
    OrderCreateView,
    OrderDetailView,
    OrderListView
)

from .viewsForDriver import (
    order_driver_lookup_view,
    order_driver_detail_view,
    order_driver_confirm_view,
    order_driver_complete_view,
    DriverCreateView,
    DriverInfoDetailView,
    DriverInfoUpdateView,
    DriverListView
)

from .viewsForSharer import(
    order_sharer_lookup_view,
    order_sharer_join_view,
    order_sharer_detail_view
)

app_name = 'orders'
urlpatterns = [
    path('', OrderListView.as_view(), name='order-list'),
    path('<int:my_id>/', OrderDetailView, name='order-detail'),
    path('create/', OrderCreateView.as_view(), name='order-create'),
    path('<int:id>/update/', OrderUpdateView.as_view(), name='order-update'),


    path('driver/allOrders', order_driver_lookup_view, name='order-driver-allOrders'),
    path('driver/<int:my_id>/orderDetail', order_driver_detail_view, name='order-driver-orderDetail'),
    path('driver/<int:my_id>/orderConfirm', order_driver_confirm_view, name='order-driver-orderConfirm'),
    path('driver/<int:my_id>/orderComplete', order_driver_complete_view, name='order-driver-orderComplete'),
    path('driver/register/', DriverCreateView.as_view(), name='order-driver-create'),
    path('driver/profile/', DriverInfoDetailView, name='order-driver-detail'),
    path('driver/profile/update', DriverInfoUpdateView, name='order-driver-update'),
    path('driver/myOrders', DriverListView.as_view(), name='order-driver-myOrders'),


    path('sharer/lookup', order_sharer_lookup_view, name='order-sharer-lookup'),
    path('<int:my_id>/join', order_sharer_join_view, name='order-sharer-join'),
    path('sharer/<int:my_id>/detail', order_sharer_detail_view, name='order-sharer-detail'),
]
