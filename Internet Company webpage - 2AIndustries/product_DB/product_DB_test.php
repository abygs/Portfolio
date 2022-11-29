<?php

include("./draw_table_generic.php");
include("../credentials.php");

try {
	$dsn = "mysql:host=courses;dbname=$username";
	$pdo = new PDO($dsn, $username, $password);
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

	$describe = $pdo->query("DESCRIBE Inventory;");
	$rows	  = $describe->fetchAll(PDO::FETCH_ASSOC);

	draw_table($rows);

	$select = $pdo->query("SELECT * FROM Inventory;");
	$rows   = $select->fetchAll(PDO::FETCH_ASSOC);

	draw_table($rows);
}

catch(PDOException $e) {
	
	echo "Connection to database failed: ".$e->getMessage();
}
?>
