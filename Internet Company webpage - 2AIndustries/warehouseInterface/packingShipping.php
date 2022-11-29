
<?php
        //includes
        include('../credentials.php');
        include('warehouseHeader.html');
?>
            <div class="main">
                <div class="topbar">
                    <div class="toggle" onclick="toggleMenu();"></div>
                    <h2>Packing/Shipping</h2>
                </div>
                <div class="cardBox">
                    <div class="details">

                <div class="recentOrders">
                        <div class="cardHeader">
                    <h3>Orders To Be Shipped</h2>
                </div>

<?php


	function shippedAlert($message)
	{
    		// Display the alert box
   	 	echo "<script>alert('$message');</script>";
	}

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

	//update parts with checkbox to be shipped
	if(isset($_POST['email']) && isset($_POST['id']))
	{
		$mysql = "UPDATE Customer_Order SET status = 1 WHERE id = ?";
	        $stmt = $pdo->prepare($mysql);
		$stmt->execute(array($_POST['id']));

		shippedAlert("Order shipped and shipment confirmation email sent to " . $_POST['email']);
	}



	//query to retrieve parts
	$mysql = "SELECT * FROM Customer_Order WHERE status = 0";
        $stmt = $pdo->prepare($mysql);
       	$stmt->execute(array());
       	$rows = $stmt->fetchAll(PDO::FETCH_ASSOC);

	if($rows == false)
	{
		echo "<h2>No orders left to ship</h2>";
	}
	else
	{
	echo "<div class='container'>";
		echo"<table>";
	                echo"<thead>";
	                	echo "<tr>";
	                                // Headers Are Clickable And Sortable
		                        echo "<th><b>Ship Order</b></th>";
		                        echo "<th style='cursor: pointer;'><b>Order ID</b></th>";
		                        echo "<th style='cursor: pointer;'><b>Authorization Number</b></th>";
		                        echo "<th style='cursor: pointer;'><b>Time Ordered</b></th>";
		                        echo "<th style='cursor: pointer;'><b>Shipping Status</b></th>";
	                        echo "</tr>";
			echo "<thead>";
		echo "<tbody>";

			foreach($rows as $row)
	                {
		               	echo "<tr>";
					echo "<td><form action='./packingShipping.php' method='POST'>";
					echo "<input type='hidden' name='email' value='".$row["email"]."'/>";
					echo "<input type='hidden' name='id' value='".$row["id"]."'/>";
					echo "<input type='Submit' class='buttons'  value='     Ship     '/> </form></td>";   //Ship selected Orders
	                                echo "<td><b>".$row["id"]."</b></td>";
	                                echo "<td><b>".$row["auth"]."</b></td>";
	                                echo "<td><b>". date("m-d-Y H:i:s", substr($row['t_stamp'], 0, 10)). "</b></td>";

					//shipped or in process?
					echo ($row["status"]) ? "<td><b><span class='status shipped'>Shipped</span></b></td>" : "<td><b><span class='status packing'>Authorized</span></b></td>";

	                                echo "<td><b><a href = './invoice/invoice.php?orderId=".$row["id"] ."'>Invoice</a></b></td>";
	                                echo "<td><b><a href = './shippingLabel/shippingLabel.php?orderId=".$row["id"]."'>Shipping Label</a></b></td>";
	                                echo "<td><b><a href =
					'./packing_list.php?orderId=".$row["id"] ."'>Packing List</a></b></td>";

	                        echo "</tr>";
	                }
              	echo "</tbody>";
            echo "</table>";
      echo "</div>";
    echo "</div>";
   echo "</div>";
 echo "</div>";
echo "</div>";

	}
	echo "</div>";
?>
        <script>
            function toggleMenu(){
                let toggle = document.querySelector('.toggle');
                let navigation = document.querySelector('.navigation');
                let main = document.querySelector('.main');
                toggle.classList.toggle('active');
                navigation.classList.toggle('active');
                main.classList.toggle('active');
            }
        </script>
    </body>
</html>
