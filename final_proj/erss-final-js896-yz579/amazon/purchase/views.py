from django.shortcuts import render, redirect, reverse
from django.http import HttpResponse
from .models import product, reviews, purchase
from django.contrib.auth.decorators import login_required
from django.shortcuts import get_object_or_404
from .forms import purchaseForm, reviewForm

import socket
from google.protobuf.internal.decoder import _DecodeVarint32
from google.protobuf.internal.encoder import _EncodeVarint
from google.protobuf.internal.encoder import _VarintBytes
from . import web_amazon_pb2 as amazon
import time

HOST = "localhost"
PORT = 31547

#set up socket
web_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_ip = socket.gethostbyname(HOST)
web_socket.connect((server_ip, PORT))

def recv_from_back(type, socket):
    size_variant = b''
    while True:
        size_variant += socket.recv(1)
        try:
            size = _DecodeVarint32(size_variant, 0)[0]
        except IndexError:
            #print("error\n")
            continue  # if decode failed, read one more byte from stream
        break  # else if decode succeeded, break. Size is available
    data = socket.recv(size)  # data in string format
    response = type()
    response.ParseFromString(data)
    print(response)
    return response

# while(1):
#     response = recv_from_back(amazon.Resp, web_socket)
#     buy = get_object_or_404(purchase, pk=response.pkgstatus.packageid)
#     buy.status = response.pkgstatus.status
#     buy.save()


def home(request):
    context = {
        'products': product.objects.all()
    }
    return render(request, 'purchase/home.html', context)

def productDetail(request,pro_id):
    command = amazon.Cmd()#create command
    element = get_object_or_404(product, pk=pro_id)
    form = purchaseForm()
    if request.method == 'POST':
        form = purchaseForm(request.POST)
        if form.is_valid:
            buy = form.save(commit=False)
            buy.status = 'purchased'
            buy.user = request.user
            buy.product = element
            buy.save()
            purchase = amazon.Purchase()
            purchase.productid = pro_id
            purchase.description = element.name
            purchase.count = buy.count
            purchase.username = request.user.username
            purchase.packageid = buy.id
            purchase.x = buy.x
            purchase.y = buy.y
            command.buy.append(purchase)
            # send_to_back(web_socket,command)
            serialized_request = command.SerializeToString()
            size = command.ByteSize()
            web_socket.sendall(_VarintBytes(size) + serialized_request)
            return redirect('purchase-home')
        else:
            form = purchaseForm()
            messages.error(request, 'Cannot buy!')

    commands = reviews.objects.filter(product=element)
    context = {
        'product': element,
        'commands': commands,
        'form': form
    }
    return render(request, 'purchase/productDetail.html', context)




@login_required
def orderHistory(request):
    orders = purchase.objects.filter(user=request.user.id)
    for order in orders:
        command = amazon.Cmd()
        query_status = amazon.Query()
        query_status.packageid = order.id
        command.queries.append(query_status)
        serialized_request = command.SerializeToString()
        size = command.ByteSize()
        web_socket.sendall(_VarintBytes(size) + serialized_request)
        # response = recv_from_back(amazon.Resp, web_socket)
        # order.status = response.pkgstatus.status
        # order.save()
    time.sleep(4)
    ordersAll = purchase.objects.filter(user=request.user.id)
    context = {
        'historis': ordersAll
    }
    return render(request, 'purchase/orderHistory.html', context)


def reviewHandle(request,pro_id):
    element = get_object_or_404(product, pk=pro_id)
    form = reviewForm()
    if request.method == 'POST':
        form = reviewForm(request.POST)
        if form.is_valid:
            oneCommand = form.save(commit=False)
            oneCommand.product = element
            oneCommand.user = request.user
            oneCommand.save()
            return redirect('purchase-home')
        else:
            form = reviewForm()
            messages.error(request, 'Cannot write!')
    context = {
        'product': element,
        'form': form,
    }
    return render(request, 'purchase/review.html', context)

def search(request):
    #q = request.GET.get('q','')
    q = request.POST.get('scontent','')
    if not q:
        context = {
            'products': product.objects.all()
        }
        return render(request, 'purchase/home.html', context)
    productsList = product.objects.filter(name__icontains=q)
    return render(request, 'purchase/home.html', { 'products': productsList})
