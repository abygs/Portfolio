<?php

function queryLegacyDBs($sql)
{
	//init information
        $servername  = "blitz.cs.niu.edu";
        $user    = "student";
        $pass    = "student";
        $dbname  = "csci467";
        $parts_array = array();

        //Create connection
        $conn = new mysqli($servername, $user, $pass, $dbname);

        //Check connection
        if($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
        }

        //change character set to utf8
        $conn->set_charset("utf8");
        $result = $conn->query($sql);
	$conn->close();

	return $result;
}
?>
