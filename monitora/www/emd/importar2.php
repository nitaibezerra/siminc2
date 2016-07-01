<pre><?php
die("<h1>Cuidado ao executar este script</h1>");
exit;
set_time_limit(0);
define("MAX_CONT", 7);
@require_once "config.php";

$handle = fopen("/home/ptb/lista1000000.lst", "r");
$saida = fopen("/home/ptb/importar03.lst", "w+");
$log = fopen("/home/ptb/importar03.log", "w+");
$i=0;$j=0;$cont=1;
// cabecalho
$linha = trim(fgets($handle, 4096));
fputs($saida, $linha . "\n");
$i++;
while (!feof($handle)) {
	if($cont)
		$linha = trim(fgets($handle, 4096));
	$dados = explode(";", $linha);
	if(@count($dados) > MAX_CONT) {
		$strOut = implode(";", $dados);
		fputs($saida, $strOut . "\n");
		$j++;
		$cont = 1;
	}
	else {
		fputs($log, $linha . "\n");
		$linha .= trim(fgets($handle, 4096));
		fputs($log, $linha . "\n----\n\n");
		$i++;
		$cont = 0;
	}
	$i++;
	if(!($i % 10000))
		printf("%6d, %6d\n", $i,$j);
	flush();
}
fclose($handle);
fclose($saida);

printf("FINAL: %6d\n", $i-$j);
exit;

$arquivo = "/home/ptb/1ptb.lst";
//$tabela = "dbcep.logradouro_ba";

$campos = "";
$fp = fopen($arquivo, "r") or die("erro ao abrir $arquivo");
dbg($fp);
fgets($fp);
dbg(feof($fp));
while(!feof($fp)) {
	$linha = fgets($fp, 4096);
	if(strlen($linha)==80) {
		var_dump($linha);
		exit;
	}
}

fclose($fp);
exit;
$campos = fgetcsv($fp, 1000, "\t", "\"");
$linha = 0;
DB::Conn()->StartTrans();
try {
while($row = fgetcsv($fp, 1000, "\t", "\"")) {
	if(!$row[3])
		$row[3] = null;

	if(!$row[4])
		$row[4] = null;

	if(!$row[7])
		$row[7] = null;

	
	$dados = array();
	foreach($row as $i=>$r) {
		$dados[strtolower($campos[$i])] = $r;
	}
//	var_dump($dados);
//	exit;


	$insertSQL = DB::Conn()->GetInsertSQL($tabela, $dados);
//	var_dump($insertSQL);
//	exit;

//	$dados = implode("', '", $row);

//	$dados = str_replace("'NULL'", "NULL", $dados);

//	$query = sprintf("$sql", $dados);
	DB::Conn()->Execute($insertSQL);
//	echo "$insertSQL\n";
	if(!($linha%100)) {
		printf("LINHA: %100d OK\n", $linha);
		flush();
	}
	$linha++;
}
}
catch(Exception $e) {
	var_dump($dados);
	var_dump($linha);
	dbg($e,1);
}
DB::Conn()->CompleteTrans();
echo "<center><b>acabou!!! $tabela</b></center>";
?>
</pre>