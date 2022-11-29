<?php

include("../credentials.php");
include("../product_DB/draw_table_generic.php");

try {
	// connect to legacy and new product database
	$dsn_legacy="mysql:host=blitz.cs.niu.edu;dbname=csci467";
	$dsn_new="mysql:host=courses;dbname=$username";

	$pdo_legacy = new PDO($dsn_legacy, "student", "student");
	$pdo_new = new PDO($dsn_new, $username, $password);

	$pdo_legacy->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$pdo_new->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	// end conn

	// readability
	$order_id = $_GET['orderId'];

	// query the order_entry database to get part number and quantities
	$id_qty = $pdo_new->prepare("SELECT prod_id, qty FROM Order_Entry
				WHERE order_id = :o_id;");
	$id_qty->execute(['o_id' => $order_id]);

	$rows = $id_qty->fetchAll(PDO::FETCH_ASSOC);

	// header for the packing slip
	echo "<h3>PACKING SLIP FOR ORDER NO. $order_id</h3>";

	// useful variables
	$t_price = 0;		// total price   (sum(product_price))
	$t_weight = 0;
	$p_price = array();	// product price (price * qty)
	// we now want to query the legacy db to get product description
	echo "<table border = 1 cellspacing=1>";
	echo "<th>Product ID</th><th>Quantity</th><th>Product
	Name</th>";
	foreach($rows as $row) {
	
		// readable
		$qty = $row['qty'];
		$id  = $row['prod_id'];

		// table entries begin
		echo "<tr>";

		// query for description, price
		$p_details = $pdo_legacy->prepare("SELECT description, price 
		FROM parts WHERE number = :id;");
		$p_details->execute(['id' => $row['prod_id']]);
		$rows_legacy = $p_details->fetchAll(PDO::FETCH_ASSOC);
		
		// table entries for current row
		echo "<td>$id</td>";
		echo "<td>$qty</td>";
		
		// legacy query stuff
		foreach($rows_legacy as $row_legacy) {
		
			// readability
			$name  = $row_legacy['description'];

			// table data 
			echo "<td>$name</td>";
		}

		echo "</tr>";
	}
	echo "</table>";
}
catch(PDOException $e) {
	echo "Connection to database failed: ".$e->getMessage();
}

echo "<form action = '../warehouseInterface/packingShipping.php'>";
echo "<input type = 'submit' value = 'Return to warehouse interface'>";
echo "</form>";
?>
