from django.urls import path

from accounts.views import(
    Account_create_view
)

app_name = 'account'

urlpatterns = [
    path('create/', Account_create_view, name = 'account-create')
]
