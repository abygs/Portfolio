<script>
function myFunction(price,id) {
    alert(price);
    alert(document.getElementById(id).value);
}
</script>
<?php
/// Draw table function
include("header.html");

$servername  = "blitz.cs.niu.edu";
$username    = "student";
$password    = "student";
$dbname      = "csci467";
$parts_array = array();

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

/* change character set to utf8 */
$conn->set_charset("utf8");

// Collects data from "parts" table 
$sql    = "SELECT * FROM parts";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        $parts_array[] = array(
            ucwords($row['description']),
            $row['price'],
            $row['weight'],
            $row['pictureURL'],
            $row['number']
        );
    }

} else {
    Print "0 records found";
}
$conn->close();
echo "<div class='container-md'> <div class='row'>";
for ($i = 0; $i < count($parts_array); $i++) {
    $name = $parts_array[$i][0];
    $url  = $parts_array[$i][3];
    $p_id = $parts_array[$i][4];
    echo "<div class='col-md-4'> <div class='thumbnail'> <a target='_blank'>";
    echo "<img class='d-block m-aut' src='" . $url . "' >";
    echo "<div class='caption'> <br>
        <p style='font-weight: bold;'>" . $name . "<span style='font-weight: bolder;font-size: 24px;'><br> $" . $parts_array[$i][1] . "</span><br>
        <input type='number' id=". $parts_array[$i][4] ." name='quantity' min='1' max='5'style='width: 3em;height: 2.5em;'>
            <input class='btn btn-outline-primary' type='button' value = 'Add to cart'onclick='myFunction(".$parts_array[$i][1]. ",". $parts_array[$i][4].")' value='Add to Cart'>
        </p></div></a></div></div>";
}

include("footer.html");
?>
