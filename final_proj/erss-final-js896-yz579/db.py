"""This file is used to communicate with database."""
import psycopg2


def connect_db():
    conn = psycopg2.connect("dbname=products user=js896 password=passw0rd")
    return conn


def connect_django():
    conn = psycopg2.connect("dbname=miniamazon user=postgres password=passw0rd")
    return conn

# def exec_commit(conn, querry):
#     cursor = conn.cursor()
#     cursor.execute(querry)
#     conn.commit()


def drop_and_recreate():
    conn = connect_db()
    drop_tables = """ DROP TABLE IF EXISTS purchase, warehouse, packed, truck_arrive, loaded, delivered, status;"""
    cursor = conn.cursor()
    cursor.execute(drop_tables)
    conn.commit()
    create_tables = """ 
                    CREATE TABLE purchase
        (
            product_name    VARCHAR(50)     NOT NULL,
            product_id      INT          NOT NULL,
            description      VARCHAR(50)     NOT NULL,
            user_name       VARCHAR(50)     NOT NULL,
            count           INT             NOT NULL,
            shipid          INT             NOT NULL,
            x               INT             NOT NULL,
            y               INT             NOT NULL,
            package_id      SERIAL             NOT NULL,
            PRIMARY KEY (package_id)
        );

CREATE TABLE warehouse
        (
            whid    SERIAL     NOT NULL,
            x       INT          NOT NULL,
            y       INT          NOT NULL,
            PRIMARY KEY (whid)
        );

CREATE TABLE packed
        (
            package_id      SERIAL          NOT NULL,
            whid       INT          NOT NULL,
            item_id       INT          NOT NULL,
            seqnum        INT          NOT NULL,
            isRead        INT          NOT NULL,
            PRIMARY KEY (package_id),
            FOREIGN KEY (whid) REFERENCES warehouse (whid),
            FOREIGN KEY (package_id) REFERENCES purchase (package_id)
        );

CREATE TABLE truck_arrive
        (
            package_id      SERIAL          NOT NULL,
            truck_id       INT          NOT NULL,
            seqnum       INT          NOT NULL,
            shipid          INT         NOT NULL,
            PRIMARY KEY (package_id),
            FOREIGN KEY (package_id) REFERENCES packed (package_id)
        );
        """
    cursor.execute(create_tables)
    conn.commit()
    cursor.close()


def load_wh():
    conn = connect_db()
    load_warehouses = """
                      INSERT INTO warehouse (x,y) VALUES (1,1);
                      INSERT INTO warehouse (x,y) VALUES (2,2);
                      INSERT INTO warehouse (x,y) VALUES (3,3);
                      """
    cursor = conn.cursor()
    cursor.execute(load_warehouses)
    conn.commit()
    cursor.close()
