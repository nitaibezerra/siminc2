<?
$data = getdate();
$datasql = $data['year'].'-'.$data['mon'].'-'.$data['mday'];
//print $datasql;
//exit();
//Parâmetros de conexão com o PG
global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $email_sistema;
//Desenv.
$servidor_bd = '10.210.3.17';
$porta_bd = '5432';
$nome_bd = 'simec';
$usuario_db = 'phpsimec';
$senha_bd = 'pgphpsimecspo';

function functrim(&$item1, $key) {
   $item1 = trim($item1);
}
print date('d/m/Y H:i:s').'|';
flush();

//Conexão com o SQL Server
$msconnect=mssql_connect("MECSRV62:1433","SYSDBSOF_SPO","PX10DZ07AP55");
$msdb=mssql_select_db("dbsof2005",$msconnect);
$msquery = "select CO_UNIDADE_ORCAMENTARIA, NO_UNIDADE_ORCAMENTARIA, CO_PROGRAMA, NO_PROGRAMA_TRABALHO, CO_ACAO, NO_PROJETO_ATIVIDADE, CREDITO_AUTORIZADO, CREDITO_CONTIDO_BLOQUEADO, DOTACAO_DISPONIVEL, PROVISAO_RECEBIDA, PROVISAO_CONCEDIDA, DESTAQUE_RECEBIDO, DESTAQUE_CONCEDIDO, PRE_EMPENHO, EMPENHOS_EMITIDOS, EMPENHOS_A_LIQUIDAR, EMPENHOS_LIQUIDADOS, VALOR_PAGO, VALOR_PAGO_RESTO_PAGAR1, VALOR_PAGO_RESTO_PAGAR2, VALOR_RESTO_A_PAGAR, SALDO_UNIDADE, SALDO_SPO, INSCRICAO_RAP, PERCENTUAL_EXECUCAO, convert(varchar(10), DT_REFERENCIA, 120) as DT_REFERENCIA from DBSOF2005.dbo.VW_SOF_SIOF_SIMEC where convert(varchar(10), DT_REFERENCIA, 120) = '".$datasql."';";
$msresults= mssql_query($msquery);

$pgconnect = pg_connect("host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=".$GLOBALS['nome_bd']."  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."");
pg_set_client_encoding($pgconnect,'LATIN5');
pg_query($pgconnect, 'begin transaction; ');
pg_query($pgconnect, "update siof_simec set sfsstatus='I';");
pg_query($pgconnect, "delete from siof_simec where dt_referencia='".$datasql."';");
while ($row = mssql_fetch_assoc($msresults)) {
	$dt_referencia = $row['DT_REFERENCIA'];
	$colunas = array_keys($row);
	$colunas = strtolower(implode(',', $colunas));
	array_walk($row, 'functrim');
	$valores = implode("', '", $row);
	$sql = "INSERT INTO siof_simec ($colunas, sfsstatus) VALUES ('$valores', 'A')";
	pg_query($pgconnect, $sql);
	
	//var_dump($sql);
    //foreach($RS as $k=>$v) {print $k.', ';${$k}=$v;}
}
pg_query($pgconnect, 'commit transaction; ');

pg_close($pgconnect);

print $dt_referencia . '|OK!';
?>