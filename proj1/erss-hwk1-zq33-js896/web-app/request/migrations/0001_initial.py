# Generated by Django 3.0.2 on 2020-02-07 05:55

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
    ]

    operations = [
        migrations.CreateModel(
            name='Order',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('startpoint', models.CharField(max_length=120)),
                ('destination', models.CharField(max_length=120)),
                ('requiredArrival', models.DateTimeField(null=True, verbose_name='date and time of arrival')),
                ('numPassengers', models.DecimalField(decimal_places=0, max_digits=2, null=True)),
                ('vehicleType', models.CharField(choices=[('Plain', 'Plain'), ('Lux', 'Lux'), ('Super Lux', 'Super Lux')], max_length=200, null=True)),
                ('isShare', models.BooleanField(null=True)),
                ('isShareFound', models.BooleanField(default=False, null=True)),
                ('specialRequest', models.CharField(choices=[('Super Clean', 'Super Clean'), ('Super Fast', 'Super Fast')], max_length=200, null=True)),
                ('status', models.CharField(choices=[('non-confirmed', 'non-confirmed'), ('confirmed', 'confirmed'), ('completed', 'completed')], default='non-confirmed', max_length=120, null=True)),
                ('ownerName', models.CharField(max_length=120, null=True)),
                ('driverName', models.CharField(max_length=120, null=True)),
                ('sharerName', models.CharField(max_length=120, null=True)),
            ],
        ),
        migrations.CreateModel(
            name='driverInfo',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('fullName', models.CharField(max_length=120)),
                ('carNum', models.CharField(max_length=120)),
                ('vehicleType', models.CharField(choices=[('Plain', 'Plain'), ('Lux', 'Lux'), ('Super Lux', 'Super Lux')], max_length=200, null=True)),
                ('vehicleCapacity', models.DecimalField(decimal_places=0, max_digits=2, null=True)),
                ('satisfiedRequest', models.CharField(choices=[('Super Clean', 'Super Clean'), ('Super Fast', 'Super Fast')], max_length=200, null=True)),
                ('user', models.OneToOneField(on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]
