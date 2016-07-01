<pre><?php
die("<h1>Cuidado ao executar este script</h1>");
exit;

define("TESTE",false);
set_time_limit(0);

$ano = "2005";

$arquivo = "naturezadespesa.txt";



$servidor_bd = '10.210.3.15';
$porta_bd = '5432';
$nome_bd = 'simec';
$usuario_db = 'phpsimec';
$senha_bd = 'pgphpsimecspo';

$pgconn = pg_connect("host=".$servidor_bd." port=".$porta_bd." dbname=".$nome_bd."  user=".$usuario_db." password=".$senha_bd);
pg_set_client_encoding($pgconn,'LATIN5');
pg_query($pgconn, 'begin; ');



$fp = fopen($arquivo, "r") or die("erro ao abrir $arquivo");

$sqlSelNaturezaDespeza = "SELECT * FROM dbemd.naturezadespesa WHERE ndpcod = '%s' AND ndpano = '%s'";
$sqlInsNaturezaDespeza = "INSERT INTO dbemd.naturezadespesa (
						ndpano,
						ndpcod,
						cencod,
						gndcod,
						mapcod,
						edpcod,
						ndpdsc
  					) VALUES (
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s', 
						'%s'
				)";

try {
	while($row = fgetcsv($fp, 1000, "\t", "\"")) {
		list($cencod, $gndcod, $mapcod, $edpcod) = explode(".", $row[0]);
		$ndpcod = $cencod . "." . $gndcod . "." . $mapcod . "." . $edpcod;
		$ndpdsc = $row[1];
		
	
		$query = sprintf($sqlInsNaturezaDespeza, $ano, $ndpcod, $cencod, $gndcod, $mapcod, $edpcod, $ndpdsc);
		if(TESTE)
			echo "$query\n";
	
		if(!pg_query($query)) {
			throw new Exception("ERRO AO INSERIR: (Linha: $linha)\n$query");		
		}
	
		printf("LINHA: %100d OK\n", $linha++);
		if(TESTE)
			echo "\n";
		flush();
	}
}
catch(Exception $e) {
	var_dump($dados);
	var_dump($linha);
	var_dump($e);
}
if(TESTE)
	pg_query($pgconn, 'rollback; ');
else
	pg_query($pgconn, 'commit; ');

pg_close($pgconn);
exit(1);
?>
</pre>