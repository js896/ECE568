from django import forms

from .models import Account

class AccountForm(forms.ModelForm):

    class Meta:
        model = Account
        fields = [
           'userName',
            'userPassword',
            'userFirstName',
            'userLastName',
            'userEmail'
        ]


class RawAccountForm(forms.Form):
        userName = forms.CharField()
        userPassword = forms.CharField()
        userFirstName = forms.CharField()
        userEmail = forms.CharField()
