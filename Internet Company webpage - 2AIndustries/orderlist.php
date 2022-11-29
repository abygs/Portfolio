<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">
        <title>2A Industries</title>
        <link rel="shortcut icon" href="https://i.ibb.co/TqJskwJ/image.png" type="image/x-icon">
        <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.5.0/font/bootstrap-icons.css">
        <link href="https://fonts.googleapis.com/css2?family=Roboto&display=swap" rel="stylesheet">
    </head>
    </style>
    <body style="font-family: 'Montserrat', sans-serif;" >
        <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-/bQdsTh/da6pkI1MST/rWKFNjaCP5gBSY4sEBT38Q/9RBh9AH40zEOg7Hlq2THRZ" crossorigin="anonymous"></script>
        <nav class="navbar fixed-top navbar-expand-lg navbar-dark bg-dark ">
            <h1 style="color:white;" class='mx-auto'>2A Industries Order Processing </h1>
        </nav>
        <br><br><br>
<?php 
	include("draw_table.php");
	$username="student";
	$password="student";
	try {
		$dsn="mysql:host=blitz.cs.niu.edu;dbname=csci467";
		$pdo=new PDO($dsn, $username, $password);
		$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

		$detail_table = $pdo->query("SELECT * FROM customers;");
		$rows         = $detail_table->fetchAll(PDO::FETCH_ASSOC);
		draw_dbtable($rows);

	}

	catch(PDOException $e) {
		
		echo "Connection to database failed: ".$e->getMessage();
	}
?>
</div>
</div>
<footer class="bg-light text-center">
    <div class="text-center p-3" style="background-color: rgba(0, 0, 0, 0.2);">
        © 2021 Copyright: 2A Industries
    </div>
    </footer>
</body>
</html>