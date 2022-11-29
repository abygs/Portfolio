<?php

function print_table($rows) {

	// check that the array is not empty
	if(!empty($rows)) {
		
		// table settings
		echo "<table border=1 cellspacing=1>";

		// print all headings
		foreach($rows[0] as $key => $item) {
			echo "<th>$key</th>";
		}

		// print each row
		foreach($rows as $row) {
			echo "<tr>";
			foreach($row as $item) {
				echo "<td>$item</td>";
			}
			echo "</tr>";
		}
		echo "</table>";
	}
}

?>
