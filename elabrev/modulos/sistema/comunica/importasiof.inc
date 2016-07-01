<?
include "includes/cabecalho.inc";
$data = getdate();
$datasql = $data['year'].'-'.$data['mon'].'-'.$data['mday'];

function functrim(&$item1, $key) {
   $item1 = trim($item1);
}

//Conexão com o SQL Server
$msconnect=mssql_connect("10.1.3.62:1433","SYSDBSOF_SPO","PX10DZ07AP55");
$msdb=mssql_select_db("dbsof2005",$msconnect);
$msquery = "select distinct convert(varchar(10), dt_referencia, 120) as dt_referencia from VW_SOF_SIOF_SIMEC ";
$msresults = mssql_query($msquery);
$row = mssql_fetch_assoc($msresults);
$datasql = $row['dt_referencia'];


$msquery = "select CO_UNIDADE_ORCAMENTARIA, NO_UNIDADE_ORCAMENTARIA, CO_PROGRAMA, NO_PROGRAMA_TRABALHO, CO_ACAO, NO_PROJETO_ATIVIDADE, CREDITO_AUTORIZADO, CREDITO_CONTIDO_BLOQUEADO, DOTACAO_DISPONIVEL, PROVISAO_RECEBIDA, PROVISAO_CONCEDIDA, DESTAQUE_RECEBIDO, DESTAQUE_CONCEDIDO, PRE_EMPENHO, EMPENHOS_EMITIDOS, EMPENHOS_A_LIQUIDAR, EMPENHOS_LIQUIDADOS, VALOR_PAGO, VALOR_PAGO_RESTO_PAGAR1, VALOR_PAGO_RESTO_PAGAR2, VALOR_RESTO_A_PAGAR, SALDO_UNIDADE, SALDO_SPO, INSCRICAO_RAP, PERCENTUAL_EXECUCAO, convert(varchar(10), DT_REFERENCIA, 120) as DT_REFERENCIA from DBSOF2005.dbo.VW_SOF_SIOF_SIMEC where convert(varchar(10), DT_REFERENCIA, 120) = '".$datasql."';";
$msresults = mssql_query($msquery);
$total_reg =  mssql_num_rows($msresults);
if ($total_reg>0) {
	$sql =  "update siof_simec set sfsstatus='I';";
	$db->executar($sql);
	$sql =  "delete from siof_simec where dt_referencia='".$datasql."';";
	$db->executar($sql);
	
	while ($row = mssql_fetch_assoc($msresults)) {
		$dt_referencia = $row['DT_REFERENCIA'];
		$colunas = array_keys($row);
		$colunas = strtolower(implode(',', $colunas));
		array_walk($row, 'functrim');
		$valores = implode("', '", $row);
		$sql = "INSERT INTO siof_simec ($colunas, sfsstatus) VALUES ('$valores', 'A')";
		$db->executar($sql);
		
		//var_dump($sql);
	    //foreach($RS as $k=>$v) {print $k.', ';${$k}=$v;}
	}
	$sql = 'commit transaction; ';
	$db -> commit();
  	print "<script language=\"JavaScript\">alert('";
	print "Arquivos Atualizados! Dt. Última Importação SIAFI (AAAA/MM/DD): ".$datasql;
	print "');history.back();</script>";
}
else {print "<script language=\"JavaScript\">alert('";
	print "Não há dados disponíveis para importação nesta data!";
	print "');history.back();</script>";}
?>