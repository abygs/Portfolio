<?php
	require('../shipping_label.php');
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

	//query to retrieve order details
        $mysql = "SELECT * FROM Customer_Order WHERE id = ?";
        $stmt = $pdo->prepare($mysql);
        $stmt->execute(array($_GET['orderId']));
        $order = $stmt->fetchAll(PDO::FETCH_ASSOC);

?>
<!DOCTYPE html>
<html lang="en">

<head>
    <link href="https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/4.1.3/css/bootstrap.min.css" rel="stylesheet">
    <link href="./shippingStyle.css" rel="stylesheet">
</head>

    <body>
	<br>
        <main role="main" class="container">
	<a href="../packingShipping.php"><p class="font-weight-regular mb-0 text-left">Back to Warehouse</p></a>
	<div class="row">
                <div class="col-8">
                    <div class="logo">
                        <img src="../../2A_Industries_logo.png"/>
                    </div>
                </div>
                <div class="col-4 border border-dark">
                    <img src="http://www.api2pdf.com/wp-content/uploads/2018/07/download-1.png" />
                    <h4>Order Information</h4>
                    <p class="lead">Order Number: <?php echo ($_GET['orderId']);?></p>
                    <p>Purchase Date: <?php echo date("m/d/Y", substr($order[0]['t_stamp'], 0, 10));?></p>
                    <p>Weight: <?php echo $order[0]['weight'];?> lbs</p>
                    <h4>Ship To</h4>
                    <p><?php echo $order[0]['name'];?><br>
                       <?php echo $order[0]['addr'];?></p>
                </div>
            </div>
            <hr/>

<?php

        //query to retrieve order entry details
        $mysql = "SELECT * FROM Order_Entry WHERE order_id = ?";
        $stmt = $pdo->prepare($mysql);
        $stmt->execute(array($_GET['orderId']));
        $rows = $stmt->fetchAll(PDO::FETCH_ASSOC);

        //build table
        if($rows)
	{
	        //query to retrieve parts
        	$mysql = "SELECT id, qty FROM Inventory WHERE id = ?";
        	$stmt = $pdo->prepare($mysql);
        	$stmt->execute(array($rows[0]['prod_id']));
        	$prods = $stmt->fetchAll(PDO::FETCH_ASSOC);

        	//build table
        	if($prods == false)
        	{
                	echo "<b>No products in inventory</b>";
        	}
        	else
        	{
				// print the table
				print_label_table($_GET['orderId'], $username, $password);

                		echo "</tbody>";
            		echo "</table>";
	      	echo "</div>";
	    echo "</div>";
	   echo "</div>";
	 echo "</div>";
	echo "</div>";

        	}
        	echo "</div>";
	}
	else
	{
		echo "No products";
	}


?>
        </main>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/4.1.3/js/bootstrap.min.js"></script>
    </body>
</html>
