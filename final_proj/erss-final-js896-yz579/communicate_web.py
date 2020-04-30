from db import *
from tool import *
from google.protobuf.internal.decoder import _DecodeVarint32
from google.protobuf.internal.encoder import _EncodeVarint
from concurrent.futures import ThreadPoolExecutor
import threading
import psycopg2
import random
import world_amazon_pb2
import web_amazon_pb2
from send_recv_world import *

world_address = ('localhost', 1234)


def recv_from_web(type, socket):
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


def handle_web_responses(world_socket, web_socket):
    conn = connect_db()
    # num_threads = 100
    # excutor = ThreadPoolExecutor(max_workers = num_threads)
    connection, addr = web_socket.accept()
    while(1):
        response = recv_from_web(web_amazon_pb2.Cmd, connection)
        request = world_amazon_pb2.ACommands()
        request.disconnect = False
        for buy in response.buy:
            conn = connect_db()
            cursor = conn.cursor()
            cursor.execute("""
                          INSERT INTO purchase (product_name,product_id,description,user_name,count, shipid, x, y)
                          VALUES(%s,%s,%s,%s,%s,%s,%s,%s);
                          """, [buy.description, buy.productid, buy.description, buy.username, buy.count, buy.packageid, buy.x, buy.y])
            conn.commit()
            cursor.close()
            FIFO.append(buy.packageid)
            FI_FO.append(buy.packageid)
            purchase = world_amazon_pb2.APurchaseMore()
            purchase.seqnum = generate_seqnum_world()
            purchase.whnum = 1
            thing = world_amazon_pb2.AProduct()
            thing.id = buy.productid
            thing.description = buy.description
            thing.count = buy.count
            purchase.things.append(thing)
            request.buy.append(purchase)

        for query in response.queries:
            quer = world_amazon_pb2.AQuery()
            quer.packageid = query.packageid
            quer.seqnum = generate_seqnum_world()
            PURCHASED[quer.seqnum] = quer.packageid
            request.queries.append(quer)

        send_to_world(world_socket, request)
