
<?php
	require('../credentials.php');
	require('../queryHelpers.php');

	//array to hold all products weights
	$weights = array();

	if(isset($_POST['cart']))
	{
		$_POST['cart'] = unserialize($_POST['cart']);
	}

        foreach($_POST['cart'] as $productId => $qty)
        {
		$sql    = "SELECT weight FROM parts WHERE number = '". $productId ."'";
		$result = queryLegacyDBs($sql);

		if($result->num_rows > 0)
		{
			while($row = $result->fetch_assoc())
				array_push($weights, $row['weight'] * $qty);
		}
		else
		{
			echo "0 records found";
		}
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

        //get shipping price determined by total weight shipped
	$totalWeight = array_sum($weights);
	$shippingPrice = 0.00;

     	$mysql = "SELECT id, min_weight, max_weight, price FROM Shipping WHERE ? >= min_weight AND ? <= max_weight";
	$stmt = $pdo->prepare($mysql);
	$stmt->execute(array($totalWeight, $totalWeight));
	$rows = $stmt->fetchAll(PDO::FETCH_ASSOC);

       	if($rows)
	{
		$shippingPrice = $rows['0']['price'];
	}

	if(isset($_POST['cart']))
        {
                $_POST['cart'] = serialize($_POST['cart']);
	}

?>

<!DOCTYPE html>
<html lang="en">
    <head>
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
   	<script src="./formValidation.js"></script>

        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">
        <title>2A Industries</title>
        <link rel="shortcut icon" href="https://i.ibb.co/TqJskwJ/image.png" type="image/x-icon">
        <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.5.0/font/bootstrap-icons.css">
        <link href="https://fonts.googleapis.com/css2?family=Roboto&display=swap" rel="stylesheet">
    </head>
    <style>
        img {
            width: 150px;
            height: auto;
        }
    </style>
    <script></script>
    <body style="font-family: 'Montserrat', sans-serif;">
        <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-/bQdsTh/da6pkI1MST/rWKFNjaCP5gBSY4sEBT38Q/9RBh9AH40zEOg7Hlq2THRZ" crossorigin="anonymous"></script>
        <nav class="navbar fixed-top navbar-expand-lg navbar-dark bg-dark">
            <div class="container-md">
                <img src="https://i.ibb.co/TqJskwJ/image.png" class="navbar-brand" style="width:60px;height:50px;" alt="...">
                <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNavDropdown" aria-controls="navbarNavDropdown" aria-expanded="false" aria-label="Toggle navigation">
                </button>
                <div class="collapse navbar-collapse" id="navbarNavDropdown">
                    <ul class="navbar-nav">
                        <li class="nav-item">
                            <a class="nav-link" aria-current="page"
                            style="font-size: larger;" href="../draw_table.php">Home</a>
                        </li>
                        <li class="nav-item">
                         <a class="nav-link" aria-current="page" style="font-size: larger;" href="../warehouseInterface/warehouseLogin.php">Warehouse</a>
                        </li>
                        <li class="nav-item">
                        <a class="nav-link" style="font-size: larger;" href="../admin_interface/adminLogin.php">Admin</a>
                        </li>
                </ul>
                </div>
                <button class="btn btn-primary btn-dark" type="submit"
                onclick="document.location.href='cart.php'"><i class="bi bi-bag" style="font-size:25px;color: white;"></i></button>
            </div>
        </nav>
        <br><br><br><br>
        <div class="container">
            <section>
                <div class="row">
                  <div class="col-md-8 mb-4">
                    <div class="card mb-4">
                      <div class="card-header py-3">
                        <h5 class="mb-0">Billing details</h5>
                      </div>
                      <div class="card-body">
			<form action="./ccProcessing.php" id="ccProcessing" method="POST">
                          <!-- 2 column grid layout with text inputs for the name -->
                          <div class="row mb-4">
                            <div class="col">
                              <div class="form-outline">
                                <input name="name" id="name" type="text" class="form-control" required/>
                                <label class="form-label" for="name">Name</label>
                              </div>
                            </div>
                          </div>
              
                          <!-- Text input -->
                          <div class="form-outline mb-4">
                            <input type="text" name="street" id="street" class="form-control" required//>
                            <label class="form-label" for="street">Street Address</label>
			 </div>
                          <div class="row mb-4">
                            <div class="col">
                              <div class="form-outline">
			  <input type="text" name="city" id="city" class="form-control" required//>
                            <label class="form-label" for="city">City</label>
  			      </div>
			    </div>
                            <div class="col">
                              <div class="form-outline">
				<label for="state">State</label>
				<select name="state" id="state" required>
					<option value="" disabled selected hidden>Choose State...</option>
					<option value="AL">Alabama</option>
					<option value="AK">Alaska</option>
					<option value="AZ">Arizona</option>
					<option value="AR">Arkansas</option>
					<option value="CA">California</option>
					<option value="CO">Colorado</option>
					<option value="CT">Connecticut</option>
					<option value="DE">Delaware</option>
					<option value="FL">Florida</option>
					<option value="GA">Georgia</option>
					<option value="HI">Hawaii</option>
					<option value="ID">Idaho</option>
					<option value="IL">Illinois</option>
					<option value="IN">Indiana</option>
					<option value="IA">Iowa</option>
					<option value="KS">Kansas</option>
					<option value="KY">Kentucky</option>
					<option value="LA">Louisiana</option>
					<option value="ME">Maine</option>
					<option value="MD">Maryland</option>
					<option value="MA">Massachusetts</option>
					<option value="MI">Michigan</option>
					<option value="MN">Minnesota</option>
					<option value="MS">Mississippi</option>
					<option value="MO">Missouri</option>
					<option value="MT">Montana</option>
					<option value="NE">Nebraska</option>
					<option value="NV">Nevada</option>
					<option value="NH">New Hampshire</option>
					<option value="NJ">New Jersey</option>
					<option value="NM">New Mexico</option>
					<option value="NY">New York</option>
					<option value="NC">North Carolina</option>
					<option value="ND">North Dakota</option>
					<option value="OH">Ohio</option>
					<option value="OK">Oklahoma</option>
					<option value="OR">Oregon</option>
					<option value="PA">Pennsylvania</option>
					<option value="RI">Rhode Island</option>
					<option value="SC">South Carolina</option>
					<option value="SD">South Dakota</option>
					<option value="TN">Tennessee</option>
					<option value="TX">Texas</option>
					<option value="UT">Utah</option>
					<option value="VT">Vermont</option>
					<option value="VA">Virginia</option>
					<option value="WA">Washington</option>
					<option value="WV">West Virginia</option>
					<option value="WI">Wisconsin</option>
					<option value="WY">Wyoming</option>
				</select>
                              </div>
                            </div>
                          </div>

                          <!-- Email input -->
                          <div class="form-outline mb-4">
                            <input type="email" name="email" id="email" class="form-control" required//>
                            <label class="form-label" for="email">Email</label>
                          </div>
              
                          <hr class="my-4" />
              
                          <h5 class="mb-4">Credit Card</h5>
              
                          <div class="row mb-4">
                            <div class="col">
                              <div class="form-outline">
                                <input type="text" name="nameOnCard" id="nameOnCard" class="form-control" required//>
                                <label class="form-label" for="nameOnCard">Name on card</label>
                              </div>
                            </div>
                            <div class="col">
                              <div class="form-outline">
                                <input type="integer" name="cardNumber" id="cardNumber" class="form-control" minlength="3" maxlength="16" required//>
                                <label class="form-label" for="cardNumber" >Credit card number</label>
                              </div>
                            </div>
                          </div>
              
                          <div class="row mb-4">
                            <div class="col-3">
                              <div class="form-outline">
                                <input type="integer" name="expiration" id="expiration" class="form-control" placeholder="mm/yyyy" minlength="7" maxlength="7" required//>
                                <label class="form-label" for="expiration">Expiration Date</label>
                              </div>
                            </div>
                            <div class="col-3">
                              <div class="form-outline">
                                <input type="integer" name="CVV" id="CVV" class="form-control" minlength="3" maxlength="3" required//>
                                <label class="form-label" for="CVV">CVV</label>
                              </div>
                            </div>
                          </div>

              		  <input type='hidden' name='total' id='total' value='<?php echo $_POST['total']; ?>'>
		          <input type='hidden' name='cart' id='cart' value='<?php echo $_POST['cart']; ?>'>
		          <input type='hidden' name='weight' id='weight' value='<?php echo $totalWeight; ?>'>
		          <input type='hidden' name='shipping' id='shipping' value='<?php echo $shippingPrice; ?>'>

 			  <button class="btn btn-primary btn-lg btn-block" type="submit" >
                            Place Your Order
                          </button>
                        </form>
                      </div>
                    </div>
                  </div>
              
                  <div class="col-md-4 mb-4">
                    <div class="card mb-4">
                      <div class="card-header py-3">
                        <h5 class="mb-0">Summary</h5>
                      </div>
                      <div class="card-body">
                        <ul class="list-group list-group-flush">
                          <li
                            class="list-group-item d-flex justify-content-between align-items-center border-0 px-0 pb-0"
                          >
                            Products
                            <span>$<?php echo $_POST['total']; ?></span>
                          </li>
                          <li
                            class="list-group-item d-flex justify-content-between align-items-center px-0"
                          >
                            Shipping
                            <span>$<?php echo $shippingPrice; ?></span>
                          </li>
                          <li
                            class="list-group-item d-flex justify-content-between align-items-center border-0 px-0 mb-3"
                          >
                            <div>
                              <strong>Total amount</strong>
                            </div>
                            <span><strong>$<?php echo $_POST['total'] + $shippingPrice; ?></strong></span>
                          </li>
                        </ul>
                      </div>
                    </div>
                  </div>
                </div>
              </section>
              <!-- Credit card form -->
        </div>        
    <!-- Credit card form -->
</body>
</html>

