/*
 * Addison Alvey-Blanco	z1875558
 * CSCI467		
 * Group Project	11/13/2021
 * Setup Script for Order DB
 * 	> Creates a DB of orders 
 *********************************/

/* This script will delete everything in the DB and reset the DB.
   Do not run this script after the DB is up and running unless the
   purpose is to reset the DB.
*/

/* Delete the table if it already exists */
DROP TABLE IF EXISTS Customer_Order;
DROP TABLE IF EXISTS Order_Entry;

/* Create the new table */
CREATE TABLE Customer_Order (
	id		INT(4)	    NOT NULL AUTO_INCREMENT, /* order ID */
	t_stamp		BIGINT(11)  NOT NULL, /* timestamp (epoch format) */
	auth		INT(5)      NOT NULL, /* authorization # */
	status		BOOL 	    NOT NULL, /* status (shipped, etc) */
	name		VARCHAR(50) NOT NULL, /* name of the customer */
	email 		VARCHAR(50) NOT NULL, /* email address of the customer */
	addr		VARCHAR(50) NOT NULL, /* address of the customer */
	weight		FLOAT	    NOT NULL, /* total weight of order*/
	price		DECIMAL(9,2)  NOT NULL, /*final selling price of total order*/
	shipping	DECIMAL(9,2)  NOT NULL, /*this order's shipping price upon order placed*/

	PRIMARY KEY(id) /* order ID is the primary key */
);

/* This table represents a line on the invoice of an order */
CREATE TABLE Order_Entry (
	order_id	INT(5)	NOT NULL, /* order_id that this line belongs to */
	prod_id		INT(5) 	NOT NULL, /* prod_id on this line */
	qty		INT(5)	NOT NULL, /* quantity of prod */

	PRIMARY KEY(prod_id, order_id) /* pk is fk into prod, order tables */
);
