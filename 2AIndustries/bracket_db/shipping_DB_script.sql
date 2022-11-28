/* kill the table if its there */
DROP TABLE IF EXISTS Shipping;

/* create the table */
CREATE TABLE Shipping (
	id		INT(4)			NOT NULL AUTO_INCREMENT,
	min_weight	FLOAT(9,2)		NOT NULL,
	max_weight	FLOAT(9,2)		NOT NULL,
	price		FLOAT(9,2)	 	NOT NULL,

	PRIMARY KEY(id)
);

INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('1.00', '5.00', '4.99');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('5.01', '10.00', '6.99');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('10.01', '20.00', '10.99');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('20.01', '25.00', '11.11');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('25.01', '35.00', '12.22');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('35.01', '40.00', '13.33');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('40.01', '50.00', '17.50');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('50.01', '100.00', '15.00');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('100.01', '200.00', '20.00');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('200.01', '500.00', '25.00');
INSERT INTO Shipping (min_weight, max_weight, price)
		VALUES ('500.01', '1000.00', '100.00');

