"""
Django settings for amazon project.

Generated by 'django-admin startproject' using Django 3.0.2.

For more information on this file, see
https://docs.djangoproject.com/en/3.0/topics/settings/

For the full list of settings and their values, see
https://docs.djangoproject.com/en/3.0/ref/settings/
"""

import os

# Build paths inside the project like this: os.path.join(BASE_DIR, ...)
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


# Quick-start development settings - unsuitable for production
# See https://docs.djangoproject.com/en/3.0/howto/deployment/checklist/

# SECURITY WARNING: keep the secret key used in production secret!
SECRET_KEY = 'g@4=k0=+l#9psgeatn^c@e!x96f$(xz*^22xr3a$s^y^us8$k8'

# SECURITY WARNING: don't run with debug turned on in production!
DEBUG = True

ALLOWED_HOSTS = ['web', 'vcm-12433.vm.duke.edu', 'vcm-14292.vm.duke.edu']


# Application definition

INSTALLED_APPS = [
    'users.apps.UsersConfig',
    'purchase.apps.PurchaseConfig',
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
]

MIDDLEWARE = [
    'django.middleware.security.SecurityMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',
]

ROOT_URLCONF = 'amazon.urls'

TEMPLATES = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': [],
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                'django.template.context_processors.debug',
                'django.template.context_processors.request',
                'django.contrib.auth.context_processors.auth',
                'django.contrib.messages.context_processors.messages',
            ],
        },
    },
]

WSGI_APPLICATION = 'amazon.wsgi.application'


# Database
# https://docs.djangoproject.com/en/3.0/ref/settings/#databases

DATABASES = {
   #    'default': {
   #     'ENGINE': 'django.db.backends.postgresql',
   #     'NAME': os.path.join(BASE_DIR, 'db.sqlite3'),
   # }
     # 'default': {
     #    'ENGINE': 'django.db.backends.postgresql',
     #    'NAME': 'amazon',
     #    'USER': 'postgres',
     #    'PASSWORD' : 'postgres',
     #    'HOST': '',
     #    'PORT': 5432,
     # }

     'default': {
         'ENGINE': 'django.db.backends.postgresql',
         'NAME': 'miniamazon',
         'USER': 'postgres',
         'PASSWORD' : 'passw0rd',
         'HOST': '',
         'PORT': 5432,
     }
     # 'default': {
     #     'ENGINE': 'django.db.backends.postgresql',
     #     'NAME': 'miniAmazon',
     #     'USER': 'yz579',
     #     'PASSWORD' : '',
     #     'HOST': '',
     #     'PORT': 5432,
     # }

}


# Password validation
# https://docs.djangoproject.com/en/3.0/ref/settings/#auth-password-validators

AUTH_PASSWORD_VALIDATORS = [
    {
        'NAME': 'django.contrib.auth.password_validation.UserAttributeSimilarityValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.MinimumLengthValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.CommonPasswordValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation.NumericPasswordValidator',
    },
]


# Internationalization
# https://docs.djangoproject.com/en/3.0/topics/i18n/

LANGUAGE_CODE = 'en-us'

TIME_ZONE = 'UTC'

USE_I18N = True

USE_L10N = True

USE_TZ = True

# Static files (CSS, JavaScript, Images)
# https://docs.djangoproject.com/en/3.0/howto/static-files/

STATIC_URL = '/static/'

LOGIN_REDIRECT_URL = 'purchase-home'

LOGIN_URL = 'login'