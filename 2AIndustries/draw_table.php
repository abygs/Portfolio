<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>

<!-- php -->
<?php

// includes
include("header.html");
include("credentials.php");

$parts_array = array();

if(!empty($_POST['search'])) {
	
	try {
		$dsn_legacy="mysql:host=blitz.cs.niu.edu;dbname=csci467";
		$pdo = new PDO($dsn_legacy, "student", "student");
		$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	}
	catch(PDOException $e) {
		echo "Connection to database failed: ".$e->getMessage();
	}

	// search the database
	$name = $_POST['search_for'];
	$parts = $pdo->prepare("SELECT * FROM parts
				WHERE description LIKE :desc;");
	$parts->execute(['desc' => "%".$name."%"]);

	// check that the row count is > 0
	if($parts->rowCount() > 0) {
		
		// fill the parts array
		$list = $parts->fetchAll(PDO::FETCH_ASSOC);
		foreach($list as $row) {
			$parts_array[] = array(
			ucwords($row['description']),
			$row['price'],
			$row['weight'],
			$row['pictureURL'],
			$row['number']
			);
		}
	}
	else { // no matches
		echo "No products found.";
	}
}
else { // default case: no search
	// init information
	$servername  = "blitz.cs.niu.edu";
	$user    = "student";
	$pass    = "student";
	$dbname  = "csci467";

	// Create connection
	$conn = new mysqli($servername, $user, $pass, $dbname);

	// Check connection
	if ($conn->connect_error) {
	    die("Connection failed: " . $conn->connect_error);
	}

	/* change character set to utf8 */
	$conn->set_charset("utf8");

	$sql    = "SELECT * FROM parts";
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
	    while ($row = $result->fetch_assoc()) {
		$parts_array[] = array(
		    ucwords($row['description']),
		    $row['price'],
		    $row['weight'],
		    $row['pictureURL'],
		    $row['number']
		);
	    }
	    
	} else {
	    Print "0 records found";
	}

	$conn->close();
}

// get availabe quantities for each product
try {

	$dsn="mysql:host=courses;dbname=$username";
	$pdo=new PDO($dsn, $username, $password);
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}
catch(PDOException $e) {
	echo "Connection to database failed: ".$e->getMessage();
}

// search bar
echo "<form method = 'POST' action = './draw_table.php'>";
echo "<input type = 'text' placeholder = 'Enter product name' name = 'search_for'>";
echo "<input type = 'submit' value = 'Search' name = 'search'>";
echo "</form>";

// list products
echo "<center><div class='container-md'> <div class='row'>";

// start session
session_start();

// loop to create the page
for($i = 0; $i < count($parts_array); $i++) {

	// make stuff readable
	$name   = $parts_array[$i][0];
	$price  = $parts_array[$i][1];
	$weight = $parts_array[$i][2];
	$url    = $parts_array[$i][3];
	$p_id   = $parts_array[$i][4];

	// query the database to get the quantity on hand
	$query_qty = $pdo->prepare("SELECT qty FROM Inventory WHERE id =
	:id;");
	$query_qty->execute(['id' => $p_id]);
	$qty = $query_qty->fetchAll(PDO::FETCH_ASSOC);
	$on_hand = $qty[0]['qty'];

	// create an array with name, url so we don't have to query db again
	$prod_info = array($name => $url);
	
	// store info in session
	if(!isset($_SESSION['info'])) {	// if info not set, then set it
		$_SESSION['info'] = array($p_id => $prod_info);
	}
	else { // otherwise update for current product in loop
		if(array_key_exists($p_id, $_SESSION['info'])) { } // do nothing
		else { // update info for current product
			$_SESSION['info'][$p_id] = $prod_info;
		}
	}

	// form for adding to cart
	// product picture
	echo "<div class='col-md-4'><div class='thumbnail'><a target =
	'_blank'>";	
	echo "<img class='d-block m-aut' src='".$url."'>";
	
	// product name
	echo "<div class='caption'><br>";
	echo "<p style='font-weight:bold;'>".$name." - $weight lbs. <span style=
		'font-weight:bolder;font-size:24px;'><br>$".$price."";

	// begin form
	echo "<form action='update_cart.php' method='POST'>";
	echo "<input type='number' name='quantity' value='0'
		min='1' max='$on_hand'
		style='width:3em;height:2.5em;'></span> $on_hand available<br>";
	echo "<input type='hidden' name='product_id' value=".$p_id.">";
	echo "<input type='submit' value='Add To Cart'>";
	echo "</form>"; // end form
	
	echo "</p></div></a></div></div></form>";	// lots of closing tags
}

// wow
echo "</div></center>";

// clean up
$pdo->connection = null;

include("footer.html");
?>
<!-- end php -->
