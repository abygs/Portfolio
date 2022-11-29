<?php

include("./header.html");

session_start();

if(isset($_POST['new'])) {
	// create a new weight bracket
	echo "<h3>Create a new weight bracket</h3>";
	echo "<p/>";
	echo "<form method = 'POST' action = './change_pricing_new.php'>";
	echo "<input type = 'number' step = '0.01' min = '0' max = '999' value = '0'
	name = 'min'> Enter a minimum weight for the new bracket<p/>";
	echo "<input type = 'number' step = '0.01' min = '0' max = '999' value = '0'
	name = 'max'> Enter a maximum weight for the new bracket<p/>";
	echo "<input type = 'number' step = '0.01' min = '0' max = '999' value = '0'
	name = 'price'> Enter a price for the new bracket<p/>";
	echo "<input type = 'submit' value = 'Create new bracket'>";
	echo "</form>";
}
else if(isset($_POST['modify'])) {

	$id    = $_POST['id'];
	$min   = $_POST['min'];
	$max   = $_POST['max'];
	$price = $_POST['price'];

	// modify a weight bracket
	echo "<h3>Modify an existing weight bracket</h3>";
	echo "<p/>";
	echo "<form method = 'POST' action = './change_pricing_new.php'>";
	echo "<input type = 'number' step = '0.01' min = '0' max = '999' value = 
		'$min' name = 'min'> Enter a new minimum weight for the bracket<p/>";
	echo "<input type = 'number' step = '0.01' min = '0' max = '999' value =
		'$max' name = 'max'> Enter a new maximum weight for the bracket<p/>";
	echo "<input type = 'number' step = '0.01' min = '0' max = '999' value =
		'$price' name = 'price'> Enter a new price for the bracket<p/>";
	echo "<input type = 'hidden' name = 'id' value = '$id'>";
	echo "<input type = 'submit' value = 'Update bracket' name = 'modify'>";
	echo "</form>";
}
else if(isset($_POST['remove'])) {

	$id    = $_POST['id'];
	$min   = $_POST['min'];
	$max   = $_POST['max'];
	$price = $_POST['price'];

	echo "<h3>Confirm that you want to remove the following bracket</h3>";
	echo "<table border = 1 cellspacing = 1>";
	echo "<th>Bracket ID</th><th>Minimum Weight</th><th>Maximum
	Weight</th><th>Price</th>";
	echo "<tr><td>$id</td><td>$min</td><td>$max</td><td>$price</td>";
	echo "<td>";
	echo "<form method = 'POST' action = './change_pricing_new.php'>";
	echo "<input type = 'hidden' name = 'id' value = '$id'>";
	echo "<input type = 'hidden' name = 'min' value = '$min'>";
	echo "<input type = 'hidden' name = 'max' value = '$max'>";
	echo "<input type = 'hidden' name = 'price' value = '$price'>";
	echo "<input type = 'submit' name = 'remove' value = 'Confirm'>";
	echo "</form></td>";
	echo "</table>";

}

echo "<a href = './change_pricing.php'>Back to main pricing page</a>";
