<?

if($_REQUEST['gerarexcel']) {
	ob_clean();
	$sql = "select * from  par.empenho
			where empprogramafnde = 'CM'  
			order by empvalorempenho desc";
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=listaempfnde_par_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=listaempfnde_par_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$cabecalho = array("empid","empcnpj","empnumerooriginal","empanooriginal","empnumeroprocesso","empcodigoespecie","empcodigopi","empcodigoesfera","empcodigoptres","empfonterecurso","empcodigonatdespesa","empcentrogestaosolic","ampanoconvenio","empnumeroconvenio","empcodigoobs","empcodigotipo","empdescricao","empgestaoeminente","empunidgestoraeminente","empprogramafnde","empnumerosistema","empsituacao","usucpf","empprotocolo","empnumero","empvalorempenho","ds_problema","valor_total_empenhado","valor_saldo_pagamento","empdata");
	$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2,'N',true);
	exit;

}


include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

monta_titulo( 'Lista Empenhos', '&nbsp;' );

$sql = "select * from  par.empenho
		where empprogramafnde = 'CM' and empstatus = 'A'  
		order by empvalorempenho desc";

$cabecalho = array("empid","empcnpj","empnumerooriginal","empanooriginal","empnumeroprocesso","empcodigoespecie","empcodigopi","empcodigoesfera","empcodigoptres","empfonterecurso","empcodigonatdespesa","empcentrogestaosolic","ampanoconvenio","empnumeroconvenio","empcodigoobs","empcodigotipo","empdescricao","empgestaoeminente","empunidgestoraeminente","empprogramafnde","empnumerosistema","empsituacao","usucpf","empprotocolo","empnumero","empvalorempenho","ds_problema","valor_total_empenhado","valor_saldo_pagamento","empdata");

$db->monta_lista_simples($sql,$cabecalho,5000,1,'S','95%','center');

?>
<p align="center"><input type="button" name="excel" value="Gerar excel" onclick="window.location='par.php?modulo=principal/listaEmpenhos_&acao=A&gerarexcel=true';"></p>
