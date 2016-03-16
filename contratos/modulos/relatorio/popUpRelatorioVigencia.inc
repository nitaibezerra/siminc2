<?php 
//Teste XLS
if($_POST['limpaSession']){
	unset($_SESSION['evento']['post']);
	$_POST['limpaSession'] = "";
}

if( empty($_SESSION['evento']['post']) ){
	$_SESSION['evento']['post'] = $_POST;
}


//Rececendo os valores dos filtros
extract($_SESSION['evento']['post']);

//Campo de Data referente ao Vencimento dos Contratos
if( $dtinicio[0] && ( $dtinicio_campo_flag || $dtinicio_campo_flag == '1' )){
	$where[0] = " AND contrato.ctrdtfimvig " . (( $dtinicio_campo_excludente == null || $dtinicio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $dtinicio ) . "') ";		
}
if( $dtfim[0] && ($dtfim_campo_flag || $dtfim_campo_flag == '1' )){
	$where[1] = " AND contrato.ctrdtfimvig " . (( $dtfim_campo_excludente == null || $dtfim_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $dtfim ) . "') ";
}
//zerando o valor da Variável $where 
$where = "";
//Revertendo o formato da Data de dd/mm/aaaa, para aaaa/mm/dd 
if( $_POST['dtinicio'] && $_POST['dtfim'] ){
	$novadtinicio = substr($_POST['dtinicio'],6,4).'/'.substr($_POST['dtinicio'],3,2).'/'.substr($_POST['dtinicio'],0,2);
	$novadtfim = substr($_POST['dtfim'],6,4).'/'.substr($_POST['dtfim'],3,2).'/'.substr($_POST['dtfim'],0,2);
//Condição que é utilizada, quando as datas de inicio e fim forem selecionadas. 	
	$where = "WHERE contrato.ctrdtfimvig BETWEEN '{$novadtinicio}'	AND	'{$novadtfim}'";
}
/*Consulta no Banco de Dados 
Tabelas:
- evento.ctcontrato;
- demandas.unidadeatendimento;
- evento.ctsituacaocontrato.
Campos:
- ctrdtfimvig da Tabela evento.ctcontrato;   
- ctrnomecontratada da Tabela evento.ctcontrato; 
- ctrobj da Tabela evento.ctcontrato; 
- ctrproccontr da Tabela evento.ctcontrato;  
- b.unasigla da Tabela demandas.unidadeatendimento;
- c.sitdsc da Tabela evento.ctsituacaocontrato.
*/
$sql  = "	SELECT 
				to_char(contrato.ctrdtfimvig, 'DD/MM/YYYY'), 
				entidadeContratada.entnome,
				contrato.ctrobj, 
				contrato.ctrproccontr,
				contrato.ctrobs,  
				unidadeAtendimento.unasigla,
				situacaoContrato.sitdsc
			FROM 
				evento.ctcontrato 
			AS 
				contrato
			INNER JOIN
				demandas.unidadeatendimento 
			AS 
				unidadeAtendimento
			ON 
				contrato.unaid = unidadeAtendimento.unaid
			INNER JOIN
				evento.ctsituacaocontrato
			AS
				situacaoContrato
			ON
				contrato.sitid = situacaoContrato.sitid
			LEFT JOIN 
				entidade.entidade 
			AS 
				entidadeContratada 
			ON 
				contrato.entidcontratada = entidadeContratada.entid
			{$where}
			ORDER BY 
				contrato.ctrdtfimvig 
			ASC	";
$cabecalho = array("Vencimento", "Contratada", "Objeto", "Processo", "Observação" , "Área Responsável", "Situação" );
if($_REQUEST['consulta'] == 'xls'){
	header('content-type: text/html; charset=ISO-8859-1');
	$db->sql_to_excel($sql, 'RelatorioVigencias', $cabecalho);
	exit;
}
//Preenchendo a Variável $arDados, com Dados do Banco de Dados			
$arDados = $db->carregar( $sql );
//zerando o valor da Variável $n_proceso 
$n_proceso = '';
//Preenchendo a Variável $unidades, com um Array
$unidades = array();
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
<?php 
//Tabela de Apresentção
echo "<br/>";
//Cabeçalho
$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
				"<td colspan=\"100\">" .			
					monta_cabecalho_relatorio('100') .
				"</td>" .
			  "</tr>
			  </table>";

echo $cabecalhoBrasao;
print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
//Título da Tabela
monta_titulo( 'Relatório de Vigência', '<br>');
//Dados da Tabela 
$db->monta_lista_array($arDados, $cabecalho, 100000, 1, 'N', 'center', '');													

?>
</body>
</html>