<?php

include("../credentials.php");

session_start();

try {
	$dsn="mysql:host=courses;dbname=$username";
	$pdo = new PDO($dsn, $username, $password);

	// make it readable
	$new_bracket_min   = $_POST['min'];
	$new_bracket_max   = $_POST['max'];
	$new_bracket_price = $_POST['price'];

	// first check that there are no entries that match the min or max
	$check_min = $pdo->prepare("SELECT id FROM Shipping 
					WHERE :min >= min_weight 
					AND   :min <= max_weight;");
	$check_max = $pdo->prepare("SELECT id FROM Shipping 
					WHERE :max >= min_weight 
					AND   :max <= max_weight;");
	$check_min->execute(['min' => $new_bracket_min]);
	$check_max->execute(['max' => $new_bracket_max]);

	// query for empty
	$min_rows = $check_min->fetchAll(PDO::FETCH_ASSOC);
	$max_rows = $check_max->fetchAll(PDO::FETCH_ASSOC);

	// modify case - bracket exits and modify set in POST
	if(isset($_POST['modify'])) {
		
		// readability
		$id = $_POST['id'];

		echo "$id";

		// remove the old entry
		$remove = $pdo->prepare("DELETE FROM Shipping WHERE id = :id;");
		$remove->execute(['id' => $id]); 

		// first check that there are no entries that match the min or max
		$check_min = $pdo->prepare("SELECT id FROM Shipping 
						WHERE :min >= min_weight 
						AND   :min <= max_weight;");
		$check_max = $pdo->prepare("SELECT id FROM Shipping 
						WHERE :max >= min_weight 
						AND   :max <= max_weight;");
		$check_min->execute(['min' => $new_bracket_min]);
		$check_max->execute(['max' => $new_bracket_max]);

		// query for empty
		$min_rows = $check_min->fetchAll(PDO::FETCH_ASSOC);
		$max_rows = $check_max->fetchAll(PDO::FETCH_ASSOC);

		if(empty($min_rows) and empty($max_rows)) {
			
			// update the table
			$insert = $pdo->prepare("INSERT INTO Shipping
							(min_weight, max_weight, price)
						 VALUES
						 	(:min, :max, :price);");
			$insert->execute(['min'   => $new_bracket_min,
					  'max'   => $new_bracket_max,
					  'price' => $new_bracket_price]);
		}
	}
	else if(isset($_POST['remove'])) {
			
		// readability
		$id = $_POST['id'];

		echo "$id";

		// remove the old entry
		$remove = $pdo->prepare("DELETE FROM Shipping WHERE id = :id;");
		$remove->execute(['id' => $id]);
	}

	// error case, bracket already set
	else if((!empty($max_rows) or !empty($max_rows)) and !isset($_POST['modify'])) {
		
		echo "in error case";
		if(!empty($max_rows)) {

			echo "error with max";
			$_SESSION['pricing_error_max'];
		}

		if(!empty($min_rows)) {

			echo "error with min";
			$_SESSION['pricing_error_min'];
		}
	}

	// create new case
	else if((empty($max_rows) and empty($min_rows)) and !isset($_POST['modify'])) { 
		
		echo "in create new";

		$create_bracket = $pdo->prepare("INSERT INTO Shipping
						(min_weight, max_weight, price)
						VALUES
						(:min, :max, :price);");
		$create_bracket->execute(['min'   => $new_bracket_min,
					  'max'   => $new_bracket_max,
					  'price' => $new_bracket_price]);
	}
}
catch(PDOException $e) {
	echo "Connection to database failed: ".$e->getMessage();
}

// redirect to main page
header('location: ./change_pricing.php');
?>
