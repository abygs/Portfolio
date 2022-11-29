<?php

include("../credentials.php");
include("print_table.php");
include('adminHeader.html');

?>

            <div class="main">
                <div class="topbar">
                    <div class="toggle" onclick="toggleMenu();"></div>
                    <h2>Orders</h2>
                </div>
                <div class="cardBox">
                    <div class="details">

                <div class="recentOrders">
                        <div class="cardHeader">
                    <h3>Orders</h2>
                </div>

<?php
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

	// change query based on how the admin wants to search
	if(isset($_POST['price'])) { // search by price range

		$stmt = $pdo->prepare("SELECT id, auth, t_stamp, status, price FROM Customer_Order
					WHERE price >= :min
					AND
					      price <= :max;");
	        $stmt->execute(['min' => $_POST['min_price'],
				'max' => $_POST['max_price']]);
	}
	else if(isset($_POST['date'])) { // search by date

		$stmt = $pdo->prepare("SELECT id, auth, t_stamp, status, price FROM Customer_Order
					WHERE t_stamp >= :from
					AND
					      t_stamp <= :to;");
	        $stmt->execute([':from' => strtotime($_POST['from']),
				':to'	=> strtotime($_POST['to'])]);
	}
	else if(isset($_POST['status'])) { // search by status

		$stat = $_POST['stat'];

        	$mysql = "SELECT id, auth, t_stamp, status, price FROM Customer_Order";
		$stmt = $pdo->prepare("SELECT id, auth, t_stamp, status, price FROM Customer_Order
					WHERE status = :stat;");
		$stmt->execute(['stat' => $stat]);
	}
	else { // default

        	$mysql = "SELECT id, auth, t_stamp, status, price FROM Customer_Order";
		$stmt = $pdo->prepare($mysql);
	        $stmt->execute(array());
	}

        //query to retrieve parts
        $rows = $stmt->fetchAll(PDO::FETCH_ASSOC);

        if($rows == false)
        {
                echo "<h2>No orders found.</h2>";
        }
        else
        {
       	
	// price range form
	echo "<form method = 'POST' action = './view_orders.php'>";
	echo "<input type = 'number' name = 'min_price' step = '0.01' min ='0.00'>";
	echo " to ";
	echo "<input type = 'number' name = 'max_price' step = '1.00'>";
	echo "<input type = 'submit' name = 'price' value = 'Search by Price Range'>";
	echo "</form>";

	// date range form
	echo "<form method = 'POST' action = './view_orders.php'>";
	echo "<input type = 'date' name = 'from'>";
	echo " to ";
	echo "<input type = 'date' name = 'to'>";
	echo "<input type = 'submit' name = 'date' value = 'Search by Date Range'>";	
	echo "</form>";

	// shipping status form
	echo "<form method = 'POST' action = './view_orders.php'>";
	echo "<input type = 'radio' name = 'stat' id = 'shipped' value = '1'> Shipped <br>";	
	echo "<input type = 'radio' name = 'stat' id = 'authorized' value = '0'> Authorized<br>";	
	echo "<input type = 'submit' name = 'status' value = 'Search by Status'>";
	echo "</form>";

	echo "<div class='container'>";
                echo"<table>";
                        echo"<thead>";
                                echo "<tr>";
                                        // Headers Are Clickable And Sortable
                                        echo "<th style='cursor: pointer;'><b>Order ID</b></th>";
                                        echo "<th style='cursor: pointer;'><b>Authorization Number</b></th>";
                                        echo "<th style='cursor: pointer;'><b>Time Ordered</b></th>";
                                        echo "<th style='cursor: pointer;'><b>Price</b></th>";
                                        echo "<th style='cursor: pointer;'><b>Shipping Status</b></th>";
					echo "<th><b>Order Detail</b></th>";
                                echo "</tr>";
                        echo "<thead>";
                echo "<tbody>";

 
                        foreach($rows as $row)
                        {
				// readability
				$id = $row['id'];

                                echo "<tr>";
                                        echo "<td><b>".$row["id"]."<input type =
					'hidden' name = 'o_id' value =
					'$id'></b></td>";
                                        echo "<td><b>".$row["auth"]."</b></td>";
                                        echo "<td><b>". date("m-d-Y H:i:s", substr($row['t_stamp'], 0, 10)). "</b></td>";
					echo
					"<td><b>\$".$row["price"]."</b></td>";
                                        //shipped or in process?
                                        echo ($row["status"]) ? "<td><b><span 
					class='status shipped'>Shipped</span></b></td>" : "<td><b><span 
					class='status packing'>Authorized</span></b></td>";
					echo "<td><form method = 'POST' action =
					'./invoice.php'>";
					echo "<input type = 'hidden' name =
					'o_id' value = '".$row['id']."'>";
					echo "<input type = 'submit' value = 'View Order'>";
					echo "</form></td>";
                                echo "</tr>";
                        }
                echo "</tbody></table></div></div></div></div></div>";

        }
        echo "</div>";
?>

<!-- Javascript -->
<script>
	function toggleMenu(){
		let toggle = document.querySelector('.toggle');
		let navigation = document.querySelector('.navigation');
		let main = document.querySelector('.main');
		toggle.classList.toggle('active');
		navigation.classList.toggle('active');
		main.classList.toggle('active');
	}
</script></body></html>
