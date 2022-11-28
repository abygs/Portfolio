/*
 * Addison Alvey-Blanco	z1875558
 * CSCI467		
 * Group Project	11/13/2021
 * Setup Script for Product DB
 * 	> Uses legacy DB to setup
 *	  a new DB for product in-
 *	  ventory.
 *********************************/

/* This script will delete everything in the DB and reset the DB.
   Do not run this script after the DB is up and running unless the
   purpose is to reset the DB.
*/

/* Delete the old table */
DROP TABLE IF EXISTS Inventory;

/* Create the new table */
CREATE TABLE Inventory (
	id	INT(5)	     NOT NULL, /* product id*/
	qty	INT(5)	     NOT NULL, /* quantity on hand */

	PRIMARY KEY(id) /* product id is the primary key */
);
