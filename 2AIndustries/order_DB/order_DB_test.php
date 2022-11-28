<?php

include("../product_DB/draw_table_generic.php");
include("../credentials.php");

try {
	$dsn= "mysql:host=courses;dbname=$username";
	$pdo = new PDO($dsn, $username, $password);
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

	$describe = $pdo->query("DESCRIBE Customer_Order;");
	$rows 	  = $describe->fetchAll(PDO::FETCH_ASSOC);

	draw_table($rows);
	
	// Note that the timestamps are stored in UNIX time as type BIGINT
	$select = $pdo->query("SELECT * FROM Customer_Order;");
	$rows   = $select->fetchAll(PDO::FETCH_ASSOC);

	draw_table($rows);

	$describe = $pdo->query("DESCRIBE Order_Entry;");
	$rows 	  = $describe->fetchAll(PDO::FETCH_ASSOC);

	draw_table($rows);

	$select = $pdo->query("SELECT * FROM Order_Entry;");
	$rows   = $select->fetchAll(PDO::FETCH_ASSOC);

	draw_table($rows);
}

catch(PDOException $e) {

	echo "Connection to database failed: ".$e->getMessage();
}
?>
