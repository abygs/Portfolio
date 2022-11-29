<?php 

include("./header.html");

// includes
include("../credentials.php");

// start session in case we need it
session_start();

// back to admin interface page
echo "<a href = './view_orders.php'>Back to interface home</a>";
echo "<p />";

if(isset($_SESSION['pricing_error_min'])) {
	echo "Error: minimum bracket for weight already exists. Try again.<p/>";
	unset($_SESSION['pricing_error_min']);
}

if(isset($_SESSION['pricing_error_max'])) {
	echo "Error: maximum bracket for weight already exists. Try again.";
	unset($_SESSION['pricing_error_max']);
}

// need connection to legacy database and new database
$dsn = "mysql:host=courses;dbname=$username";

// connect to db
try { 

	$pdo = new PDO($dsn, $username, $password);
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}
catch(PDOException $e) {
	echo "Connection to legacy database failed: ".$e->getMessage();
}

// query the shipping price database to see if there are any brackets set
$check_prices = $pdo->query("SELECT * FROM Shipping;");
$rows	      = $check_prices->fetchAll(PDO::FETCH_ASSOC);

echo "<form method = 'POST' action = './create_new_modify_existing.php'>";
echo "<input type = 'submit' name = 'new' value = 'Create a new bracket'>";
echo "</form>";

// check if the query is empty
if(empty($rows)) {
	echo "There are no price brackets set.<p/>";
}
else { 	// create a form where all current brackets are displayed with options to
	// change the details of the brackets
	
	// create an array that holds all current brackets in the db
	$brackets = array();

	// there are n brackets store in brackets with 4 entries in each bracket
	// 	[0]. id	[1]. min_weight	[2]. max_weight	[3]. price
	foreach($rows as $row) {
		
		$brackets[] = array(
			$row['id'],
			$row['min_weight'],
			$row['max_weight'],
			$row['price']
		);
	}

	// create a form for each bracket with a button for modify and a button
	// for remove		
	echo "<table border = 1 cellspacing = 1>";
	echo "<tr>";
	echo "<th>Bracket ID</th>";
	echo "<th>Minimum Weight</th>";
	echo "<th>Maximum Weight</th>";
	echo "<th>Price</th>";
	echo "<th>Options</th>";
	echo "</tr>";

	for($i = 0; $i < count($brackets); $i++) {
		
		// make it readable
		$id = $brackets[$i][0];
		$min = $brackets[$i][1];
		$max = $brackets[$i][2];
		$price = $brackets[$i][3];
		
		echo "<tr>";
		echo "<form method = 'POST' action = 'create_new_modify_existing.php'>";
		
		echo "<td>$id</td><td>$min</td><td>$max</td><td>$price</td>";
	
		echo "<input type = 'hidden' name = 'id' value = '$id'>";
		echo "<input type = 'hidden' name = 'min' value = '$min'>";
		echo "<input type = 'hidden' name = 'max' value = '$max'>";
		echo "<input type = 'hidden' name = 'price' value = '$price'>";
		echo "<td><input type = 'submit' name = 'modify' value =
			'Modify'>";
		echo "<input type = 'submit' name = 'remove' value =
			'Remove'></td>";

		echo "</form>";
		echo "</tr>";
	}
	echo "</table>";
}
?>
