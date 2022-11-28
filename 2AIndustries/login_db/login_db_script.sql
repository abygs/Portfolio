/* Delete the table if it already exists */
DROP TABLE IF EXISTS Logins;

/* Create the new table */
CREATE TABLE Logins
(
        id              INT(4)      NOT NULL AUTO_INCREMENT,
	username	VARCHAR(25) NOT NULL UNIQUE,
	password	VARCHAR(255) NOT NULL,

        PRIMARY KEY(id)
);

INSERT INTO Logins (username, password)
VALUES ('admin', 'admin');

INSERT INTO Logins (username, password)
VALUES('warehouse', 'warehouse');
