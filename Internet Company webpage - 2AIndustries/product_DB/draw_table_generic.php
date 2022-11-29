<?php

function draw_table($rows) {
	
	if(!empty($rows)) {

		echo "<table border=1 cellspacing=1>";

		foreach($rows[0] as $key => $item) {
			echo "<th>$key</th>";
		}


		foreach($rows as $row) {

			echo "<tr>";
			foreach($row as $item) {
				echo "<td>$item</td>";
			}
			echo "</tr>";
		}

		echo "</table>";
		
		return true;
	}

	else {
		return false;
	}
}
?>
