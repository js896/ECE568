"""This file handles communication with the world simulator"""
from google.protobuf.internal.decoder import _DecodeVarint32
from google.protobuf.internal.encoder import _EncodeVarint
import world_amazon_pb2


def connect_world():
    """
    Wrapp up init info to world simulator
    """
    cmd = world_amazon_pb2.AConnect()
    wh = world_amazon_pb2.AInitWarehouse()
    wh.id = 1
    wh.x = 1
    wh.y = 1
    cmd.isAmazon = True
    cmd.worldid = 1
    cmd.initwh.append(wh)
    return cmd


def send_ACommand():
    cmd = world_amazon_pb2.ACommands()
    cmd.disconnect = False
    cmd.acks.append(0)
    purchase_more = world_amazon_pb2.APurchaseMore()
    product = world_amazon_pb2.AProduct()
    product.id = 1
    product.description = "Ipad"
    product.count = 1
    purchase_more.whnum = 1
    purchase_more.things.append(product)
    purchase_more.seqnum = 0
    cmd.buy.append(purchase_more)
    return cmd


def test():
    cmd = world_amazon_pb2.ACommands()
    cmd.disconnect = False
    cmd.acks.append(0)
    purchase_more = world_amazon_pb2.APurchaseMore()
    product = world_amazon_pb2.AProduct()
    product.id = 1
    product.description = "Ipad"
    product.count = 100
    purchase_more.whnum = 1
    purchase_more.things.append(product)
    purchase_more.seqnum = 2
    cmd.buy.append(purchase_more)
    return cmd


def send_to_world(skt, request):
    """This function sends request to world simulator"""
    serialized_request = request.SerializeToString()
    _EncodeVarint(skt.send, len(serialized_request), None)
    skt.send(serialized_request)


def recv_from_world(type, skt):
    """Receive message from world simulator."""
    size_variant = b''
    while True:
        size_variant += skt.recv(1)
        try:
            size = _DecodeVarint32(size_variant, 0)[0]
        except IndexError:
            #print("error\n")
            continue  # if decode failed, read one more byte from stream
        break  # else if decode succeeded, break. Size is available
    data = skt.recv(size)  # data in string format
    response = type()
    response.ParseFromString(data)
    print(response)
    return response
    # var_int_buff = []
    # while True:
    #     buf = skt.recv(1)
    #     var_int_buff += buf
    #     msg_len, new_pos = _DecodeVarint32(var_int_buff, 0)
    #     if new_pos != 0:
    #         break
    # whole_message = skt.recv(msg_len)
    # response = type()
    # response.ParseFromString(whole_message)
    # print(response)
    # return response
