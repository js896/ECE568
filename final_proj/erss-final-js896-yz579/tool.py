from db import *
import world_amazon_pb2
from send_recv_world import *
import queue

SEQNUM_WORLD = 0
SEQNUM_UPS = 0
SHIPID = 0
WAITING_ACKS_FROM_WORLD = set()
FIFO = list()
FI_FO = list()
PURCHASED = {}

def generate_seqnum_world():
    global SEQNUM_WORLD
    seqnum = SEQNUM_WORLD
    SEQNUM_WORLD += 1
    WAITING_ACKS_FROM_WORLD.add(seqnum)
    return seqnum


def generate_seqnum_ups():
    global SEQNUM_UPS
    seqnum = SEQNUM_UPS
    SEQNUM_UPS += 1
    return seqnum


def generate_shipid():
    global SHIPID
    shipid = SHIPID
    SHIPID += 1
    return shipid


def get_num_wh():
    conn = connect_db()
    query = "SELECT COUNT(*) FROM warehouse"
    cursor = conn.cursor()
    cursor.execute(query)
    num_wh = cursor.fetchall()
    return num_wh


def ack_to_world(world_socket, response):
    reply = world_amazon_pb2.ACommands()
    reply.acks.append(response.seqnum)
    send_to_world(world_socket, reply)


def pack_product(world_socket, bought):
    cmd = world_amazon_pb2.ACommands()
    pack = world_amazon_pb2.APack()
    shipid = FIFO.pop(0)
    seqnum = bought.seqnum + 1
    whnum = bought.whnum
    product = world_amazon_pb2.AProduct()
    for thing in bought.things:
        product.id = thing.id
        product.description = thing.description
        product.count = thing.count
    pack.whnum = whnum
    pack.things.append(product)
    pack.shipid = shipid
    pack.seqnum = seqnum
    cmd.disconnect = False
    cmd.topack.append(pack)
    send_to_world(world_socket, cmd)
    conn = connect_db()
    cursor = conn.cursor()
    for thing in bought.things:
        #cursor.execute("""SET IDENTITY_INSERT packed ON""")
        cursor.execute("""
                       Insert INTO packed (whid, item_id, seqnum, isRead)
                       VALUES (%s,%s,%s,%s)                 
                       """, [whnum, product.id, generate_seqnum_ups(), 0])
        conn.commit()
    cursor.close()


def send_to_world_load(world_socket, packed):
    cmd = world_amazon_pb2.ACommands()
    load = world_amazon_pb2.APutOnTruck()
    conn = connect_db()
    cursor = conn.cursor()
    cursor.execute("""
                   SELECT shipid, truck_id FROM truck_arrive
                   """)
    tuple = cursor.fetchone()
    print ("shipid: ")
    print(tuple[0])
    print("truck_id: ")
    print(tuple[1])
    # cursor.execute("""
    #                SELECT whid From packed WHERE shipid = %s  
    #                """, [tuple[0]])
    
    seqnum = packed.seqnum
    load.whnum = 1
    load.truckid = tuple[1]
    load.shipid = tuple[0]
    load.seqnum = seqnum + 1
    cmd.load.append(load)
    cmd.disconnect = False
    send_to_world(world_socket, cmd)
