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
	$cabecalho = array("Código INEP","Escola","CNPJ","E-mail","Telefone","Fax","Razão Social","CEP","Endereço","Complemento","Bairro","Número","Estado","Município","Série","Qtd. Alunos","Esfera");
	if($_POST['tipo_relatorio'] == 'xls'){
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatorioAlunado".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatAlunado".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,100000,5,"N",'100%');
	}elseif($_POST['tipo_relatorio'] == 'visual'){ ?>
		<html>
			<head>
				<script type="text/javascript" src="../includes/funcoes.js"></script>
				<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
				<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
				<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" /> 
			</head>
		<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
		<?php 
		monta_titulo( 'Relatório Alunado por Série', '' );
		header('Content-Type: text/html; charset=iso-8859-1'); 
		$db->monta_lista($sql, $cabecalho, 1000, 5, 'N', '95%');	
	}

function monta_sql(){
	global $filtroSession;
	extract($_POST);
	
	if($estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){		
		$where[0] = " and ende.estuf " . (( $estado_campo_excludente == null || $estado_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	
	if($municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )){
		$where[1] = " and u.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ";		
	}
	
	if($memanoreferencia != ''){
		$where[2] = " AND m.memanoreferencia = $memanoreferencia";		
	} 
	
	/*
	if($memanoreferencia == '2012'){
		$where[2] = " AND m.memanoreferencia = '2012'";
	}elseif($memanoreferencia == '2011'){
		$where[2] = " AND m.memanoreferencia = '2011'";
	}elseif($memanoreferencia == '2010'){
		$where[2] = " AND m.memanoreferencia = '2010'";		
	}elseif($memanoreferencia == '2009'){
		$where[2] = " AND m.memanoreferencia = '2009'";		
	}elseif($memanoreferencia == '2008'){
		$where[2] = " AND m.memanoreferencia = '2008'";				
	}
	*/
	
	if($serie != ''){
		$where[3] = " and p.sceid = $serie";		
	} 
		
	$sql = "SELECT
					e.entcodent AS inep, 
					e.entnome AS escola, 
					e.entnumcpfcnpj AS cnpj, 
					e.entemail AS email,
					e.entnumdddcomercial ||  ' - ' || e.entnumcomercial AS telefone,
			        e.entnumdddfax || ' - ' || e.entnumfax AS fax, 
			        e.entrazaosocial AS razaosocial, 
			        ende.endcep AS cep, 
			        ende.endlog AS endereco, 
			        ende.endcom AS complemento, 
			        ende.endbai AS bairro , 
			        ende.endnum AS numero, 
			        ende.estuf AS estado, 
			        u.mundescricao AS municipio,
			       	p.sceid AS serie, 
			       	p.mapquantidade AS alunos,
			        CASE 
			            WHEN e.tpcid = 1 THEN 'Estadual'
				        WHEN e.tpcid = 2 THEN 'Federal'
			            WHEN e.tpcid = 3 THEN 'Municipal'
			        END AS esfera
			FROM pdeescola.memaiseducacao m
			LEFT JOIN entidade.entidade e USING(entid)
		 	LEFT JOIN entidade.endereco ende USING(entid)
			LEFT JOIN territorios.municipio u USING(muncod)
			LEFT JOIN pdeescola.mealunoparticipante p USING(memid)
			WHERE m.memstatus = 'A' AND e.entstatus = 'A' AND ende.endstatus = 'A'
			--Estado
			".$where[0]."
			--Município.
			".$where[1]."
			--Ano de referência
			".$where[2]."
			--Série.
			".$where[3]."
			ORDER BY ende.estuf, u.mundescricao, e.entcodent, p.sceid";
		//ver($sql,d);
	return $sql;
}
?>