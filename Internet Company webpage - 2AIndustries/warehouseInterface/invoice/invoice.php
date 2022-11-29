<?php

include("../../credentials.php");
include("../header.html");

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

	// get the customers name, address, other relevant info
	$customer_details = $pdo_new->prepare("SELECT * FROM Customer_Order
						WHERE id = :id;");
	$customer_details->execute(['id' => $order_id]);
	$rows_customer = $customer_details->fetchAll(PDO::FETCH_ASSOC);

	echo $rows_customer[0]['name'];
	echo "<br>";
	echo $rows_customer[0]['addr'];
	echo "<br>";
	echo $rows_customer[0]['email'];
	echo "<br>";

	// get weight
	$weight = $rows_customer[0]['weight'];

	// query the order_entry database to get part number and quantities
	$id_qty = $pdo_new->prepare("SELECT prod_id, qty FROM Order_Entry
				WHERE order_id = :o_id;");
	$id_qty->execute(['o_id' => $order_id]);

	$rows = $id_qty->fetchAll(PDO::FETCH_ASSOC);

	// useful variables
	$t_price = 0;		// total price   (sum(product_price))
	$p_price = array();	// product price (price * qty)
	// we now want to query the legacy db to get product description
	echo "<table border = 1 cellspacing=1>";
	echo "<th>Product</th><th>Quantity</th><th>Product
	Name</th><th>Total Weight</th><th>Price
	per Part</th><th>Total price</th>";
	foreach($rows as $row) {
	
		// readable
		$qty = $row['qty'];
		$id  = $row['prod_id'];

		// table entries begin
		echo "<tr>";

		// query for description, price
		$p_details = $pdo_legacy->prepare("SELECT description, price, weight FROM parts 
						   WHERE number = :id;");
		$p_details->execute(['id' => $row['prod_id']]);
		$rows_legacy = $p_details->fetchAll(PDO::FETCH_ASSOC);
		
		// table entries for current row
		echo "<td>$id</td>";
		echo "<td>$qty</td>";
		
		// legacy query stuff
		foreach($rows_legacy as $row_legacy) {
		
			// readability
			$price = $row_legacy['price'];
			$name  = $row_legacy['description'];
			$prod_weight = $row_legacy['weight'];

			// table data 
			echo "<td>$name</td>";
			echo "<td>".$prod_weight*$qty." lbs.</td>";
			echo "<td>\$".number_format($price,2,'.','')."</td>";	
			echo "<td>\$".number_format($price*$qty,2,'.','')."</td>";		
		}

		echo "</tr>";
	}
	echo "</table>";

	// readable
	$price = $rows_customer[0]['price'];
	$shipping = $rows_customer[0]['shipping'];
	$absolute_total = $price + $shipping;

	// print
	echo "Total Weight of Order: $weight lbs.<br>";
	echo "Subtotal: \$".number_format($price,2,'.','')."<br>";
	echo "Shipping: \$".number_format($shipping, 2, '.', '')."<br>";
	echo "Total: \$".number_format($absolute_total, 2, '.', '')."<br>";
}
catch(PDOException $e) {
	echo "Connection to database failed: ".$e->getMessage();
}

echo "<form action = '../packingShipping.php'>";
echo "<input type = 'submit' value = 'Return to warehouse interface'>";
echo "</form>";
?>
