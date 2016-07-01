<?
for($i=0;$i<2;$i++) {
	$x = time();
	exec("nohup /usr/bin/php -f teste1.php > /dev/null 2>&1 &");
	echo (time()-$x)."<br/>"; 
}
echo "fim";
?>