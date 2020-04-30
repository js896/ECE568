from django.urls import path
from . import views

urlpatterns = [
    path('', views.home, name='purchase-home'),
    path('detail', views.productDetail, name='purchase-proDetail'),
    path('history', views.orderHistory, name='purchase-history'),
    path('detail/<int:pro_id>/', views.productDetail, name = "purchase-proDetail"),
    path('review/<int:pro_id>/', views.reviewHandle, name = "purchase-review"),
    #path(r'^search/$', views.search, name='search'),
]
