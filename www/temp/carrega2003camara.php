<pre>
<?php

require_once "config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
$db = new cls_banco();


$row = 1;
$handle = fopen("../../teste2003.txt", "r");
$query = "UPDATE financeiro.reporcfin SET prgdsc = '%s', acadsc = '%s' WHERE rofid = %d";
pg_query($db->link, "BEGIN");
while (($data = fgetcsv($handle, 1000, ";", "\"")) !== FALSE) {
	$sql = vsprintf($query, array_reverse($data));
	echo $sql . "\n";ob_flush();flush();
	pg_query($db->link, $sql);	
}
pg_query($db->link, "COMMIT");
fclose($handle);


?>
</pre>