CREATE TABLE purchase
        (
            product_name    VARCHAR(50)     NOT NULL,
            product_id      SERIAL          NOT NULL,
            desciption      VARCHAR(50)     NOT NULL,
            user_name       VARCHAR(50)     NOT NULL,
            PRIMARY KEY (product_id)
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
            PRIMARY KEY (package_id),
            FOREIGN KEY (whid) REFERENCES warehouse (whid),
            FOREIGN KEY (item_id) REFERENCES purchase (product_id)
        );

CREATE TABLE truck_arrive
        (
            package_id      SERIAL          NOT NULL,
            truck_id       INT          NOT NULL,
            seqnum       INT          NOT NULL,
            PRIMARY KEY (package_id),
            FOREIGN KEY (package_id) REFERENCES packed (package_id)
        );

CREATE TABLE loaded 
        (
            package_id      SERIAL          NOT NULL,
            truck_id       INT          NOT NULL,
            seqnum        INT          NOT NULL,
            PRIMARY KEY (package_id),
            FOREIGN KEY (package_id) REFERENCES truck_arrive (package_id)
        );

CREATE TABLE delivered 
        (
            package_id      SERIAL          NOT NULL,
            seqnum       INT          NOT NULL,
            PRIMARY KEY (package_id),
            FOREIGN KEY (package_id) REFERENCES loaded (package_id)
        );

CREATE TABLE status
        (
            package_id      SERIAL          NOT NULL,
            status          VARCHAR(50)     NOT NULL,
            PRIMARY KEY (package_id),
            FOREIGN KEY (package_id) REFERENCES delivered (package_id)
        );