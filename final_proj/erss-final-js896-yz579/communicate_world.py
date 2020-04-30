from google.protobuf.internal.decoder import _DecodeVarint32
from google.protobuf.internal.encoder import _EncodeVarint
from google.protobuf.internal.encoder import _VarintBytes
import world_amazon_pb2
import web_amazon_pb2
import threading
#from concurrent.futures import ThreadPoolExecutor
from send_recv_world import *
from tool import *
from communicate_ups import *


WAITING_ACKS_FROM_WORLD = set()


def handle_world_responses(world_socket, web_socket):
    """Handle world responses."""
    # num_threads = 100
    # with ThreadPoolExecutor(max_workers = num_threads) as executor:
    while (1):
        #threading.Lock()
        response = recv_from_world(world_amazon_pb2.AResponses, world_socket)
        for error in response.error:
            print("Error message: ", error.err)
            if error.err == 'error: warehouse id/product_id/package id/truck id in certain world and warehouse(if product_id)' :
                send_purchased_to_web(web_socket, error.originseqnum)
        for ack in response.acks:
            print("Received ACK:", ack)
        for bought in response.arrived:
            print("Arrived:", bought.seqnum)
            handle_bought(world_socket, bought)
        for packed in response.ready:
            print("Packed: ", packed)
            handle_packed(world_socket, packed)
        for loaded in response.loaded:
            print("Loaded: ", loaded)
            handle_loaded(world_socket, loaded)
        for pkgstatus in response.packagestatus:
            print("PackageId: ", pkgstatus.packageid)
            print("PackageStatus: ", pkgstatus.status)
            handle_status(web_socket, pkgstatus)
        if response.finished == True:
            print("Closing connection to simulator")
            world_socket.close()
            break


def handle_bought(world_socket, bought):
    ack_to_world(world_socket, bought)
    pack_product(world_socket, bought)


def handle_packed(world_socket, packed):
    ack_to_world(world_socket, packed)
    send_package_ready_wait_for_truck(packed)
    send_to_world_load(world_socket, packed)


def handle_loaded(world_socket, loaded):
    ack_to_world(world_socket, loaded)
    send_loaded_ups(world_socket, loaded)  # read from truck arrive


def handle_status(web_socket, pkg):
    packageid = pkg.packageid
    status = pkg.status
    conn = connect_django()
    cursor = conn.cursor()
    cursor.execute("""UPDATE purchase_purchase SET status = %s WHERE id = %s""", [status, packageid])
    conn.commit()
    #cursor.execute("""SELECT * FROM purchase_purchase WHERE id = %s"""", [packageid])
    cursor.close()

def send_purchased_to_web(web_socket, seqnum):
    packageid = PURCHASED[seqnum]
    status = 'Purchased'
    conn = connect_django()
    cursor = conn.cursor()
    cursor.execute("""UPDATE purchase_purchase SET status = %s WHERE id = %s""", [status, packageid])
    conn.commit()
    cursor.close()