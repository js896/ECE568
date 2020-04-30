CREATE TABLE loaded
(
    package_id SERIAL NOT NULL,
    truck_id INT NOT NULL,
    seqnum INT NOT NULL,
    PRIMARY KEY (package_id),
    FOREIGN KEY (package_id) REFERENCES truck_arrive (package_id)
);

CREATE TABLE delivered
(
    package_id SERIAL NOT NULL,
    seqnum INT NOT NULL,
    PRIMARY KEY (package_id),
    FOREIGN KEY (package_id) REFERENCES loaded (package_id)
);

CREATE TABLE status
(
    package_id SERIAL NOT NULL,
    status VARCHAR(50) NOT NULL,
    PRIMARY KEY (package_id),
    FOREIGN KEY (package_id) REFERENCES delivered (package_id)
);