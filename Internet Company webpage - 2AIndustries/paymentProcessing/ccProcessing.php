<!--
	Anushka Verma, Aby Shulgan, Vansh Rai, Addison Alvey-Blanco
	CSCI 467: Group 2A Final Project
	Credit Card Processing System
	12/3/2021
-->

<?php
	include('../credentials.php');

	session_start();
	if(isset($_POST['cart']))
	{
		$_POST['cart'] = unserialize($_POST['cart']);
	}

	//update Order database
	$dsn = "mysql:host=courses;dbname=".$username;
        $pdo = new PDO($dsn, $username, $password);
        $pdo->setAttribute(PDO::ATTR_ERRMODE,PDO::ERRMODE_EXCEPTION);


	$address = $_POST['street'] . ', ' . $_POST['city'] . ', ' . $_POST['state'];
	try
	{
		//insert order into order db
		$mysql = "INSERT INTO Customer_Order(t_stamp, auth, status, name, email, addr, weight, price, shipping) VALUES(?,?,?,?,?,?,?,?,?)";
		$rspre = $pdo->prepare($mysql);
		$rspre->execute(array(time(), $_POST['authorization'], false, $_POST['name'], $_POST['email'], $address, $_POST['weight'], $_POST['total'], $_POST['shipping']));
		$id = $pdo->lastInsertId();
	}
	catch(PDOexception $e)
	{
		echo "Failed to insert Order into database: " . $e->getMessage();
	}

	if(isset($_POST['cart']))
	{
		//insert order_entry db
		foreach($_POST['cart'] as $productId => $qty)
		{
			try
			{
				//insert order into order db
				$mysql = "INSERT INTO Order_Entry(prod_id, order_id, qty) VALUES(?,?,?)";
				$rspre = $pdo->prepare($mysql);
				$rspre->execute(array($productId, $id ,$qty));
			}
			catch(PDOexception $e)
			{
				echo "Failed to insert Order into database: " . $e->getMessage();
			}
		}
	}


	//update inventory to show products purchased
	if(isset($_POST['cart']))
	{
		foreach($_POST['cart'] as $productId => $qty)
		{
			try
			{
				//insert order into order db
				$mysql = "UPDATE Inventory SET qty = (qty - ?) WHERE id = ?";
				$rspre = $pdo->prepare($mysql);
				$rspre->execute(array($qty, $productId));
			}
			catch(PDOexception $e)
			{
				echo "Failed to update inventory database: " . $e->getMessage();
			}
		}
	}

	//Email order confirmation

	$to      = $_POST['email'];
	$subject = 'Thank You for your Order from 2A Industries';
	$message = 'Hi, '.$_POST['name'].'!\nYour authorization number is: '. $_POST['authorization'];
	$headers = 'From: z1893909@student.niu.edu'       . "\r\n" .
               	 'Reply-To: z1893909@student.niu.edu' . "\r\n" .
              	 'X-Mailer: PHP/' . phpversion();

	mail($to, $subject, $message, $headers);


	session_destroy();
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">
        <link href="../style.css" rel="stylesheet">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta charset="utf-8">
        <title>2A Industries</title>         <!--Title Of The Current Tab-->
    </head>
    <body>
        <div class="container mt-5 mb-5">
            <div class="row d-flex justify-content-center">
                <div class="col-md-8">
                    <div class="card">
                        <div class="text-left logo p-2 px-5"> <img src="../2A_Industries_logo.png" width = "50"></div>
                        <div class="invoice p-5">
                            <h5>Your order is Confirmed!</h5> <span class="font-weight-bold d-block mt-4">Hello <?php echo($_POST['name'])?>,</span>
                            <span>A confirmation email has been sent to <?php echo $_POST['email']; ?>  and your order will be shipped within the next two days!</span>
                            <div class="payment border-top mt-3 mb-3 border-bottom table-responsive">
                                <table class="table table-borderless">
                                    <tbody>
                                        <tr>
                                            <td>
                                                <div class="py-2"> <span class="d-block text-muted">Order Date</span>
                                                <span><?php echo(date("m/d/y")); ?></span>
                                            </div>
                                            </td>
                                            <td>
                                                <div class="py-2"> <span class="d-block text-muted">Authorization Number</span>
                                                <span><?php echo($_POST["authorization"])?></span> </div>
                                            </td>
                                            <td>
                                                <div class="py-2"> <span class="d-block text-muted">Price</span>
                                                <span> $<?php echo($_POST['total'] + $_POST['shipping'])?></span> </div>
                                            </td>
                                            <td>
                                                <div class="py-2"> <span class="d-block text-muted">Name</span>
                                                <span><?php echo($_POST["name"])?></span> </div>
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
                            </div>
                            <p class="font-weight-regular mb-0 text-center">Thank You For Shopping With Us!</p>
							<a href="../draw_table.php"><p class="font-weight-regular mb-0 text-center">Go To Home</p></a>
                        </div>
                        <div class="d-flex justify-content-between footer p-3"> <span><text>&copy; 2A Industries</text></span>
						<span><?php echo date("m.d.y");?></span> </div>
                    </div>
                </div>
            </div>
        </div>
    </body>
</html>
