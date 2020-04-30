from django.shortcuts import render, get_object_or_404, redirect
from django.contrib.auth.models import User

from .models import Account
from .forms import AccountForm, RawAccountForm

# Create your views here.
def Account_create_view(request):
    if request.method == "POST":
        user_name = request.POST.get('userName')
        user_pw = request.POST.get('userPassword')
        user_fn = request.POST.get('userFirstName')
        user_ln = request.POST.get('userLastName')
        user_em = request.POST.get('userEmail')
        user = User.objects.create_user(user_name, user_em, user_pw)
        user.first_name = user_fn
        user.last_name = user_ln
        user.save()
        return redirect("orders:order-list")

        
    form = AccountForm(request.POST or None)
    if form.is_valid():
        #print(form.userName)
        form = AccountForm()

    context = {
        'form': form
    }
    return render(request, "account/account_create.html", context)
