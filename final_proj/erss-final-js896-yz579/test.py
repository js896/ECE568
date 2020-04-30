import socket
from google.protobuf.internal.decoder import _DecodeVarint32
from google.protobuf.internal.encoder import _EncodeVarint
from send_recv_world import *


def init():
    try:
        world_skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("World_Socket successfully created")
    except socket.error as err:
        print("socket creation failed with error %s" % (err))
    world_address = ('localhost', 23456)
    print('connecting to %s port %s' % world_address)
    world_skt.connect(world_address)
    request = connect_world()
    send_to_world(world_skt, request)
    recv_from_world(world_amazon_pb2.AConnected, world_skt)
