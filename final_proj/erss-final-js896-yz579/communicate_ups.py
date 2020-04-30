import socket
import threading
from concurrent.futures import ThreadPoolExecutor
from db import *
from tool import *

UPS_PORT = 34567
UPS_ADDR = 'vcm-12371.vm.duke.edu'
ups_address = (UPS_ADDR, UPS_PORT)


def serialize_package_ready(whID, xDest, yDest, customerName, itemID, itemName, itemDescription, pkgID, seqNum):
    res = ''
    res += 'PACKAGE_READY\r\n\r\n'
    res += ('whid:' + str(whID) + '\r\n')
    res += ('x_dest:' + str(xDest) + '\r\n')
    res += ('y_dest:' + str(yDest) + '\r\n')
    res += ('customer_name:' + customerName + '\r\n')
    res += ('item_id:' + str(itemID) + '\r\n')
    res += ('item_name:' + itemName + '\r\n')
    res += ('item_description:' + itemDescription + '\r\n')
    res += ('package_id:' + str(pkgID) + '\r\n')
    res += ('seqnum:' + str(seqNum) + '\r\n')
    res += '\r\n'
    return res


def parse_truck_arrive(msg):
    pkgID = msg.decode().split('\r\n')[2].split(':')[1]
    truckID = msg.decode().split('\r\n')[3].split(':')[1]
    seqNum = msg.decode().split('\r\n')[4].split(':')[1]
    res = {'packageid': pkgID, 'truckid': truckID, 'seqnum': seqNum}
    # res = {}
    # info = msg.decode()
    # mark = info.find('\r\n\r\n')
    # start = mark + 4
    # last = info.find('\r\n\r\n', start) - start
    # while (start < last):
    #     delim = info.find(':', start) - start 
    #     end = info.find('\r\n', start) - start
    #     key = info[start, start + delim - start]
    #     value = info[delim + 1, end]
    #     res[key] = value
    #     start = end + 2
    return res


def serialize_loaded(pkgID, truckID, seqNum):
    res = ''
    res += 'LOADED\r\n\r\n'
    res += ('package_id:' + str(pkgID) + '\r\n')
    res += ('truck_id:' + str(truckID) + '\r\n')
    res += ('seqnum:' + str(seqNum) + '\r\n')
    res += '\r\n'
    return res


def parse_delivered(msg):
    pkgID = msg.decode().split('\r\n')[2].split(':')[1]
    seqNum = msg.decode().split('\r\n')[3].split(':')[1]
    res = {'packageid': pkgID, 'seqnum': seqNum}
    # res = {}
    # info = msg.decode()
    # mark = info.find('\r\n\r\n')
    # start = mark + 4
    # last = info.find('\r\n\r\n', start) - start
    # while (start < last):
    #     delim = info.find(':', start) - start 
    #     end = info.find('\r\n', start) - start
    #     key = info[start, start + delim - start]
    #     value = info[delim + 1, end]
    #     res[key] = value
    #     start = end + 2
    return res


def serialize_ack(seqNum):
    res = ''
    res += 'ACK\r\n\r\n'
    res += ('seqnum:' + str(seqNum) + '\r\n')
    res += '\r\n'
    return res


def parse_ack(msg):
    print(msg)
    seqNum = msg.decode().split('\r\n')[2].split(':')[1]
    # res = {}
    # info = msg.decode()
    # mark = info.find('\r\n\r\n')
    # start = mark + 4
    # last = info.find('\r\n\r\n', start) - start
    # while (start < last):
    #     delim = info.find(':', start) - start 
    #     end = info.find('\r\n', start) - start
    #     key = info[start, start + delim - start]
    #     value = info[delim + 1, end]
    #     res[key] = value
    #     start = end + 2
    return seqNum


def parse_type(msg):
    type = msg.decode().split('\r\n')[0]
    # res = {}
    # info = msg.decode()
    # mark = info.find('\r\n\r\n')
    # start = mark + 4
    # last = info.find('\r\n\r\n', start) - start
    # while (start < last):
    #     delim = info.find(':', start) - start 
    #     end = info.find('\r\n', start) - start
    #     key = info[start, start + delim - start]
    #     value = info[delim + 1, end]
    #     res[key] = value
    #     start = end + 2
    return type


def build_ups_socket():
    try:
        ups_skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("UPS_Socket successfully created")
    except socket.error as err:
        print("socket creation failed with error %s" % (err))
    print('connecting to %s port %s' % ups_address)
    return ups_skt


def wait_until_recv_ack(ups_socket):
    type = ''
    seqNum = -1
    while type != 'ACK':
        msg = ups_socket.recv(1024)
        type = parse_type(msg)
        seqNum = parse_ack(msg)


def wait_until_recv_truck(ups_socket):
    type = ''
    seqNum = -1
    while type != 'TRUCK_ARRIVE':
        msg = ups_socket.recv(1024)
        type = parse_type(msg)
    return msg


def wait_until_recv_delivered(ups_socket):
    type = ''
    seqNum = -1
    while type != 'DELIVERED':
        msg = ups_socket.recv(1024)
        type = parse_type(msg)
        #seqNum = parse_ack(msg)
    print(msg)

def ack_to_ups(ups_socket, seqnum):
    res = serialize_ack(seqnum)
    print('ack_to_ups: ')
    print(res.encode())
    try:
        ups_socket.send(res.encode())
    except StopIteration:
        print('send error')
    print('after send')


def send_package_ready_wait_for_truck(packed):
    global ups_address
    select = """ 
             SELECT packed.whid, purchase.x, purchase.y, user_name, product_id, product_name, description, seqnum FROM packed, purchase WHERE packed.package_id = purchase.package_id AND isRead = 0
             """
    conn = connect_db()
    cursor = conn.cursor()
    cursor.execute(select)
    tuples = cursor.fetchall()
    ups_socket = build_ups_socket()
    for tuple in tuples:
        msg = serialize_package_ready(
            tuple[0], tuple[1], tuple[2], tuple[3], tuple[4], tuple[5], tuple[6], FI_FO.pop(0), tuple[7])
    ups_socket.connect(ups_address)
    ups_socket.send(msg.encode())
    wait_until_recv_ack(ups_socket)
    print('recv ack')
    msg = wait_until_recv_truck(ups_socket)
    print('recv truck arrive')
    print(msg)
    res = parse_truck_arrive(msg)
    print('truckid:')
    print(res['truckid'])
    cursor.execute("""
                   INSERT INTO truck_arrive (truck_id,seqnum,shipid)
                   VALUES (%s,%s,%s);
                   """, [res['truckid'], res['seqnum'], packed.shipid])
    conn.commit()
    print(res['seqnum'])
    ack_to_ups(ups_socket, res['seqnum'])
    ups_socket.close()
    set_read = """
               UPDATE packed SET isRead = 1 WHERE isRead = 0
               """
    cursor.execute(set_read)
    conn.commit()
    cursor.close()


def send_ack(ups_socket, seqnum):
    msg = serialize_ack(seqnum)
    ups_socket.send(msg.encode())


def ack_to_ups_last(world_socket , seqnum):
    ack_to_ups(world_socket, seqnum)

def send_loaded_ups(world_socket, loaded):
    ups_socket = build_ups_socket()
    ups_socket.connect(ups_address)
    packageid = loaded.shipid
    conn = connect_db()
    cursor = conn.cursor()
    cursor.execute("""
                   SELECT truck_id FROM truck_arrive WHERE shipid = %s
                   """, [packageid])
    truckid = cursor.fetchone()[0]
    print (truckid)
    seqnum = generate_seqnum_ups()
    res = serialize_loaded(packageid, truckid, seqnum)
    print ('loaded to ups:')
    print(res)
    ups_socket.send(res.encode())
    wait_until_recv_ack(ups_socket)
    wait_until_recv_delivered(ups_socket)
    ack_to_ups_last(world_socket, 1)
    ups_socket.close()
    cursor.close()
