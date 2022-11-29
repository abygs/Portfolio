<?php 

include("../admin_interface/print_table.php");

$dsn = "mysql:host=blitz.cs.niu.edu;dbname=csci467";
$username = "student";
$password = $username;

try {
	$pdo = new PDO($dsn,$username,$password); 
	$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}
catch(PDOException $e) {
	echo "Connection to database failed: ".$e->getMessage();
}

$details = $pdo->query("SELECT * FROM parts");
$rows    = $details->fetchAll(PDO::FETCH_ASSOC);

print_table($rows);
?>
