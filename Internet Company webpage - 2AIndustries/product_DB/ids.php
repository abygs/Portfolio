<?php

try {

	$dsn = "mysql:host=blitz.cs.niu.edu;dbname=csci467";
	$pdo = new PDO($dsn, "student", "student");
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

	// query for all ids, put them in output file done
	$id_query = $pdo->query("SELECT * FROM parts;");
	$id_rows  = $id_query->fetchAll(PDO::FETCH_ASSOC);

	foreach($id_rows as $row) {
		echo $row['number'];
		echo "<br>";
	}
}
catch(PDOException $e) {
	echo "Connection to database failed: ".$e->getMessage();
}

?>
