from django.urls import path

from .views import(
home_view,
role_select_view
)

app_name = 'welcome'

urlpatterns = [
    path('', home_view, name='order-list'),
    path('/selection', role_select_view, name='order-detail'),
]