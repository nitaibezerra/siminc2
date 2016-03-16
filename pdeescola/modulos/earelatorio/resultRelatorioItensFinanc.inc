<?php
	if ($_REQUEST['filtrosession']){
		$filtroSession = $_REQUEST['filtrosession'];
	}

	if($_POST['tipo_relatorio']){
		header('Content-Type: text/html; charset=iso-8859-1'); 
	}
	ini_set("memory_limit","500M");
	set_time_limit(0);

        require_once APPRAIZ . "includes/classes/MontaListaAjax.class.inc";
	global $db;
	$sql = monta_sql();
	$cabecalho = array("UF","Município","INEP","Descrição","Quantidade","Valor","Classificação","Ação");
        
	if($_POST['tipo_relatorio'] == 'xls'){
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatorioItensFinanc".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatorioItensFinanc".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,100000,5,"N",'100%');
                
	}elseif($_POST['tipo_relatorio'] == 'visual'){ 
  ?>
		<html>
			<head>
				<script type="text/javascript" src="../includes/funcoes.js"></script>
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
				<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
				<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" /> 
			</head>
		
                        <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
		<?php 
		monta_titulo( 'Relatório Itens Financiados', '' );
		//header('Content-Type: text/html; charset=iso-8859-1'); 
		$db->monta_lista($sql, $cabecalho, 1000, 5, 'N', '95%');
                }

function monta_sql(){
	global $filtroSession;
	extract($_POST);
	
	if($estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){		
		$where[0] = " AND edc.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	
	if($memanoreferencia[1] != ''){
		$where[1] = " AND eac.eacanoreferencia = $memanoreferencia";		
	} 
		
	$sql = "SELECT edc.estuf, RTRIM(LTRIM(mun.mundescricao)), eac.entcodent, aca.eaadescricao, aca.eaaqtd, 
                      CASE WHEN aca.eaavlrcusteio IS NULL THEN eaavlrcapital
                      ELSE aca.eaavlrcusteio 
                      END as valor, 
                      tif.etidescricao, tac.etadescricao
                FROM  pdeescola.eacescolaacessivel eac
                INNER JOIN entidade.entidade ent USING (entid)
                INNER JOIN entidade.endereco edc  USING (entid)
                INNER JOIN territorios.municipio mun USING (muncod)
                INNER JOIN pdeescola.eacacaoacessibilidade aca USING (eacid)
                INNER JOIN pdeescola.eactipoitemfinanciavel tif USING (etiid)
                INNER JOIN pdeescola.eactipoacaoacessibilidade tac USING (etaid)
                WHERE eac.eacstatus = 'A'
			--Estado
			".$where[0]."
			--Ano de referência
			".$where[1]."
			ORDER BY edc.estuf, mun.mundescricao, eac.entcodent, aca.eaadescricao";
		//ver($sql,d);
	return $sql;
}
?>