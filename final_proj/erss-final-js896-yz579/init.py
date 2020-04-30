"""This is used to connect Amazon to world simulator."""
import socket
from google.protobuf.internal.decoder import _DecodeVarint32
from google.protobuf.internal.encoder import _EncodeVarint
from send_recv_world import *
from communicate_world import *
from communicate_ups import *
from communicate_web import *
import world_amazon_pb2
import db
import threading
import time


WORLD_PORT = 23456
#WORLD_SEND_PORT = 1234
UPS_PORT = 34567
WEB_PORT = 31547
UPS_ADDR = 'vcm-12357.vm.duke.edu'


def init():
    # Drop and recreate tables
    drop_and_recreate()
    # load warehouses
    load_wh()
    # Connect to World listen sopcket
    try:
        world_skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("World_Socket successfully created")
    except socket.error as err:
        print("socket creation failed with error %s" % (err))
    world_address = ('vcm-12371.vm.duke.edu', WORLD_PORT)
    print('connecting to %s port %s' % world_address)
    world_skt.connect(world_address)
    request = connect_world()
    send_to_world(world_skt, request)
    recv_from_world(world_amazon_pb2.AConnected, world_skt)

    # # Connect to UPS listen socket
    # try:
    #     ups_skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #     print("UPS_Socket successfully created")
    # except socket.error as err:
    #     print("socket creation failed with error %s" % (err))
    # ups_address = (UPS_ADDR, UPS_PORT)
    # print('connecting to %s port %s' % ups_address)
    # ups_skt.connect(ups_address)

    # Web listen socket
    try:
        web_skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("UPS_Socket successfully created")
    except socket.error as err:
        print("socket creation failed with error %s" % (err))
    web_address = ('localhost', WEB_PORT)
    web_skt.bind(web_address)
    web_skt.listen(100)

    # entering three threads
    thread_world = threading.Thread(
        target=handle_world_responses, args=(world_skt, web_skt))
    # thread_ups = threading.Thread(
    # target=handle_ups_responses, args=(world_skt, ups_skt))
    thread_web = threading.Thread(
        target=handle_web_responses, args=(world_skt, web_skt))

    threads = [thread_world, thread_web]

    for thread in threads:
        thread.start()

    while True:
        pass


"""
Main function
"""
if __name__ == "__main__":
    init()
