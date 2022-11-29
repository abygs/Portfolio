<!-- php -->
<?php

// use session to 'remember' what is supposed to be in the cart
session_start();

// move from POST -> SESSION to make cart permanent for current browser session
if(isset($_POST['product_id'], $_POST['quantity']) &&
	is_numeric($_POST['product_id']) && is_numeric($_POST['quantity'])) {

	// make it readable
	$p_id = $_POST['product_id'];
	$qty  = $_POST['quantity'];

	// if the cart is initialized
	if(isset($_SESSION['cart'])) {

		// sanity check
		if($p_id && $qty > 0) {
			
			// see if the key exists, if so update the quantity
			if(array_key_exists($p_id, $_SESSION['cart'])) {				
				$_SESSION['cart'][$p_id] += $qty;
			}

			// otherwise, the item is a new entry in the cart
			else {
				$_SESSION['cart'][$p_id] = $qty;
			}
		}
	}

	// if not set, then initialize the cart
	else {
		// create cart
		$_SESSION['cart'] = array($p_id => $qty);
	}
}

header('location: draw_table.php');
?>
<!-- end php -->

