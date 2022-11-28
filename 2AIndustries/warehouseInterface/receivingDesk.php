<?php
        //includes
        require('../credentials.php');
	require('../queryHelpers.php');
	include('warehouseHeader.html');
?>
            <div class="main">
                <div class="topbar">
                    <div class="toggle" onclick="toggleMenu();"></div>
                    <h2>Receiving Desk</h2>
                </div>
                <div class="cardBox">
                    <div class="details">

                <div class="recentOrders">
                        <div class="cardHeader">
                    <h3>Update Inventory</h2>
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


	//update inventory if value is entered
	if(isset($_POST['update']))
	{
		foreach($_POST['update'] as $id => $qty)
		{
			if($qty > 0)
	               	{
				$mysql = "UPDATE Inventory SET qty = (qty + ?) WHERE id = ?";
        	       		$stmt = $pdo->prepare($mysql);
				$stmt->execute(array($qty, $id));
			}
		}
	}


	//query to retrieve parts
        $mysql = "SELECT id, qty FROM Inventory";
        $stmt = $pdo->prepare($mysql);
        $stmt->execute(array());
        $rows = $stmt->fetchAll(PDO::FETCH_ASSOC);


	//build table
        if($rows == false)
        {
                echo "<b>No products in inventory</b>";
        }
        else
        {
        echo "<div class='container'>";
                echo"<table>";
                        echo"<thead>";
                                echo "<tr>";
                                	// Headers Are Clickable And Sortable
                                        echo "<th><b>Enter Quantity Received</b></th>";
                                        echo "<th style='cursor: pointer;'><b>Product ID</b></th>";
                                        echo "<th style='cursor: pointer;'><b>Description</b></th>";
                                        echo "<th style='cursor: pointer;'><b>Quantity On Hand</b></th>";
                                echo "</tr>";
                        echo "<thead>";
                echo "<tbody>";

			$sql = "SELECT number, description FROM parts";
			$results = queryLegacyDBs($sql);
			$descriptions = array();
			if($results->num_rows > 0)
                	{
                        	while($row = $results->fetch_assoc())
					$descriptions[$row['number']] = $row['description'];
                	}


                        foreach($rows as $row)
                        {
                                echo "<tr>";
					echo "<form action='./receivingDesk.php' method='POST'>";
					echo "<td><input type='number' name='update[".$row["id"]."]' id='updateItem' min=0 placeholder=0 '/>    ";
					echo "<input type='submit' value='Update' /></td>";
                                        echo "<td><b>".$row["id"]."</b></td>";
                                        echo "<td><b>".$descriptions[$row["id"]]."</b></td>";
					echo "<td><b>".$row["qty"]."</b></td>";
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

