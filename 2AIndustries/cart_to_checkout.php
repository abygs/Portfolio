<?php

session_start();

foreach($_SESSION['cart'] as $p_id => $key) {
	echo "$p_id => $key<p/>";
}

echo $_SESSION['total'];
echo "<p/>";

?>
