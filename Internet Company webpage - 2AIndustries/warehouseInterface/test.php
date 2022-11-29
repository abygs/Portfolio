<?php

include("../credentials.php");
include("shipping_label.php");

print_label_table(1, $username, $password);

?>
