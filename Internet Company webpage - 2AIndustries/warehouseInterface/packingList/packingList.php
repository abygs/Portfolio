<!--
	Anushka Verma, Aby Shulgan, Vansh Rai, Addison Alvey-Blanco
	CSCI 467: Group 2A Final Project
	Credit Card Processing System
	12/3/2021
-->

<?php
	require('../../credentials.php');
	require('../../queryHelpers.php');

	try
        {
                //Connection to database
                $dsn = "mysql:host=courses;dbname=".$username;
                $pdo = new PDO($dsn, $username, $password);
                $pdo->setAttribute(PDO::ATTR_ERRMODE,PDO::ERRMODE_EXCEPTION);

        }
        catch(PDOexception $e)
        {
                echo "Unable to access database: " . $e->getMessage();
        }

	//query to retrieve order info
        $mysql = "SELECT * FROM Order_Entry WHERE order_id = ?";
        $stmt = $pdo->prepare($mysql);
        $stmt->execute(array($_GET['orderId']));
        $prods = $stmt->fetchAll(PDO::FETCH_ASSOC);

        if($prods)
        {
		print_r($prods);
        }

?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">
        <link href="./packingStyle.css" rel="stylesheet">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta charset="utf-8">
        <title>2A Industries</title>         <!--Title Of The Current Tab-->
    </head>
    <body>
        <div class="container mt-5 mb-5">
            <div class="row d-flex justify-content-center">
                <div class="col-md-8">
                    <div class="card">
                        <div class="text-left logo p-2 px-5"> <img src="../../2A_Industries_logo.png" width = "50"></div>
                        <div class="invoice p-5">
                            <h2>Packing List</h2> <span class="font-weight-bold d-block mt-4"></span>
                            <div class="payment border-top mt-3 mb-3 border-bottom table-responsive">
                            </div>
			    <div class="product border-bottom table-responsive">
				<table class="table table-borderless">
					<tbody>
						<th width="20%"> <span class="font-weight-bold">Product ID</span> </th>
						<th width="60%"> <span class="font-weight-bold">Product Name</span> </th>
						<th width="20%"> <span class="font-weight-bold">Quantity</span> </th>
						<tr>
							<td width="20%"> <span class="font-weight-bold"><?php echo 'id';?></span>
							</td>
							<td width="60%"> <span class="font-weight-bold"><?php echo 'Product Name';?></span>
							</td>
							<td width="20%">
							<div class="text-right"> <span class="font-weight-bold"><?php echo 'x3';?></span> </div>
							</td>
				 		</tr>
					</tbody>
				</table>
			</div>
			<div class="row d-flex justify-content-end">
				<div class="col-md-5">
					<table class="table table-borderless">
						</table>
					</div>
				</div>
				<a href="../packingShipping.php"><p class="font-weight-regular mb-0 text-left">Back to Warehouse</p></a>
			</div>
			<div class="d-flex justify-content-between footer p-3"> <span><text>&copy; 2A Industries</text></span>
				<span><?php echo date("m.d.y");?></span> </div>
			</div>
			</div>
		</div>
	</div>
</body>
</html>
