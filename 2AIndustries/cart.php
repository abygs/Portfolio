<?php
include("header.html");
require("credentials.php");

// use session to grab the current cart state 
session_start();

// check for clear cart option to be set
if(isset($_POST['clear'])) {
	if(!isset($_SESSION['cart'])) {} // cart is empty case: do nothing
	else { // all other cases
		$_SESSION = array(); // completely reset SESSION
	}
}
// check for remove to be set
else if(isset($_GET['remove'])) {

	// throw this away
	unset($_GET['remove']);

	// remove the item from the cart
	foreach($_GET as $p_id => $x) {

		// get the name for message
		$name = key($_SESSION['info'][$p_id]);

		// remove from cart
		unset($_SESSION['cart'][$p_id]);
		echo "Removed $name from cart.";
	}
}
if(!isset($_SESSION['cart'])) {
	echo "<p>Your cart is empty</p>";
}

// check for an update to quantities to be set
else if(isset($_POST['update_qty'])) {

	// throw this away
	unset($_POST['update_qty']);

	// update all quantities for each product
	foreach($_POST as $p_id => $new_qty) {

		$name = key($_SESSION['info'][$p_id]);

		// get the old quantity for message purposes
		$old_qty = $_SESSION['cart'][$p_id];

		// check for new_qty = 0 or new_qty != 0
		if($new_qty == 0) { // quantity = 0
			unset($_SESSION['cart'][$p_id]);
			echo "<p>Removed $name from cart.</p>"; // remove
		}
		else { // all other cases
			$_SESSION['cart'][$p_id] = $new_qty;
			echo "<p>Updated $name to $new_qty.</p>"; // update
		}
	}
}
// connect to DB so that we can perform queries to get prices
$user= "student";
$pass= "student";

try {
	$dsn_parts="mysql:host=blitz.cs.niu.edu;dbname=csci467";
	$parts=new PDO($dsn_parts,$user,$pass);
	$parts->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}

catch(PDOException $e_parts) {
	echo "Connection to database failed: ".$e_parts->getMessage();
}

try {
	$dsn_inv="mysql:host=courses;dbname=$username";
	$inv = new PDO($dsn_inv, $username, $password);
	$inv->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}

catch(PDOException $e_inv) {
	echo "Connection to database failed: ".$e_inv->getMessage();
}

$t_price = 0;

echo "<table border = 1 cellspacing = 1>";
// check if cart is not empty
if(!empty($_SESSION['cart'])) { // if not empty, then print all contents of cart
		foreach($_SESSION['cart'] as $p_id => $qty) {

			$query_qty = $inv->prepare("SELECT qty FROM Inventory
			WHERE id = :id;");
			$query_qty->execute(['id' => $p_id]);
			$on_hand = $query_qty->fetchAll(PDO::FETCH_ASSOC);
			$max = $on_hand[0]['qty'];

			// make it readable
			if($qty > $max) { // check that ordered <= on hand
				$ordered = $max;
			}
			else {
				$ordered = $qty;
			}
			$min = 0;

			// update the quantity in the session array
			$_SESSION['cart'][$p_id] = $ordered;

			// set name, url
			$name = key($_SESSION['info'][$p_id]); // key of 'info'
			$url  = $_SESSION['info'][$p_id][$name]; // value of 'info'
			$id   = key($_SESSION['info']);

			// query db to get price for product
			$get_price = $parts->prepare("SELECT price FROM parts 
					    	      WHERE number = :id;");
			$get_price->execute(['id' => $p_id]);
			$price_details = $get_price->fetchAll(PDO::FETCH_ASSOC);

			// update price of quantity(product) and total price
			$price = $price_details[0]['price'];
			$q_price = $price * $ordered;
			$t_price += $q_price;

			// form created for each product in cart to update quantity or remove
			// the item
			echo "<tr>";
			echo "<td>$name </td>";
			echo "<td><div class='col-md-4'><div class='thumbnail'><a
			target='_blank'></td>";
			echo "<td><img class='d-block m-aut' src=$url></td>";
			echo "<td><form action='./cart.php' method='POST'>"; // begin form
			echo "<input type='number' name=".$p_id." min=".$min."
				max=".$max." value=".$ordered.">";
			echo "<input type='submit' name='update_qty'
			value='Update cart'>";
			echo "</form></td>";

			// remove from cart form
			echo "<td>";
			echo "<form action='./cart.php' method='GET'>";
			echo "<input type='hidden' name=$p_id>";
			echo "<input type='submit' name='remove'
			id=$p_id value='Remove from cart'>";
			echo "</form></td>"; // end form
			echo "<td>\$".number_format($q_price, 2, '.', '')."</td>";
		}
}
echo "</table>";

// form to clear/remove item/update cart quantities
echo "<p/>";
echo "<form action='./cart.php' method='POST'>";
echo "<input type='submit' name='clear' value='Clear cart'>";
echo "</form>";
echo "<p/>";

echo "<p/>";
if($t_price != 0) {
	echo "\$".number_format($t_price, 2, '.', '')."";
}

// if the cart is not empty, give an option to go to checkout
// sends $_SESSION['cart'] and $_SESSION['total'] to next page
if(isset($_SESSION['cart'])) {

	// set the total
	$_SESSION['total'] = $t_price;

	// form to go to checkout
	echo "<p/>";
	echo "<form action='./paymentProcessing/payment.php' method='POST'>";
	echo "<input type='hidden' name='total' value='".$t_price."'>";
	echo "<input type='hidden' name='cart' value='".serialize($_SESSION['cart'])."'>";
	echo "<input type='submit' class='btn btn-success'  name='checkout' value='Checkout'>";
	echo "</form>";
}

// clean up
$parts->connection = null;
$inv->connection = null;

// return to homepage button
echo "<form action='./draw_table.php'>";
echo "<input type='submit' class='button' value='Continue Shopping'>";
echo "</form>";

?>
