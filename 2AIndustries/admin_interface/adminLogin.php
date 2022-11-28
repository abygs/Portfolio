<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Admin Login</title>
        <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/@fortawesome/fontawesome-free@5.15.4/css/fontawesome.min.css" 
        integrity="sha384-jLKHWM3JRmfMU0A5x5AkjWkw/EYfGUAGagvnfryNV3F9VqM98XiIH7VBGVoxVSc7" crossorigin="anonymous">
        <link href="../loginStyle.css" rel="stylesheet">
        <link href="//maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" rel="stylesheet" id="bootstrap-css">
        <script src="//maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js"></script>
        <script src="//cdnjs.cloudflare.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
        <link href="https://fonts.googleapis.com/css2?family=Roboto&display=swap" rel="stylesheet">
    </head>
    <body>
        <!------ Include the above in your HEAD tag ---------->
        <div class="wrapper fadeInDown">
            <div id="formContent">
            <!-- Tabs Titles -->
                <div class="fadeIn first">
                    <h1>Admin Login</h1>
                </div>
 
                <!-- Login Form -->
                <form action ="./adminLogin.php" method="POST">
                    <input type="text" id="username" class="fadeIn second" name="username" placeholder="username">
                    <input type="password" id="password" class="fadeIn third" name="password" placeholder="password">
                    <input type="submit" class="fadeIn fourth" value="Log In">
                </form>
            </div>
<?php
require('../credentials.php');

if($_SERVER["REQUEST_METHOD"] == "POST")
{
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

        $sql = "SELECT id FROM Logins WHERE username = ? AND password = ?";
        $stmt = $pdo->prepare($sql);
        $stmt->execute(array($_POST['username'], $_POST['password']));


        $rows = $stmt->fetchAll(PDO::FETCH_ASSOC);

        if($rows)
        {
                 header("location: view_orders.php");
        }
        else
                echo "<h3>Invalid Login</h3>";
}
?>
<form action='../draw_table.php'>
<input type='submit' value='Back'>
</form>
        </div>

    </body>

</html>

