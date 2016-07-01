<?php
ini_set("memory_limit", "2048M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$superuser = $db->testa_superuser();
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql($superuser);
$dados = $db->carregar($sql);

if( $_POST['tipoPesquisa'] == 'xls' ){
	$arCabecalho = array();
	$colXls = array();
	array_push($arCabecalho, 'Código');
	array_push($colXls, 'sbacod');
	array_push($arCabecalho, 'Título');
	array_push($colXls, 'titulo');
	array_push($arCabecalho, 'Descrição');
	array_push($colXls, 'sbadsc');
	array_push($arCabecalho, 'Situação');
	array_push($colXls, 'situacao');
	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
		array_push($colXls, $cabecalho['campo']);
	}
	
	$arDados = Array();
	if( is_array( $dados ) ){
		foreach( $dados as $k => $registro ){
			foreach( $colXls as $campo ){
				$arDados[$k][$campo] = $registro[$campo];
			}
		}
	}
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatorioEscolasContempladasPROEMI_2012".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatorioEscolasContempladasPROEMI_2012".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%',$par2);
	die;
}
?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setTotNivel(true);
$r->setMonstrarTolizadorNivel(false);
$r->setTotalizador(false);
$r->setBrasao(true);
$r->setEspandir( $_REQUEST['expandir']);
echo $r->getRelatorio();

function monta_sql( $superuser ){
	
	global $db;
	
	extract($_REQUEST);
	$where = array("emi.emistatus        = 'A'");
	
	// estado
	if( $estuf[0] && $estuf_campo_flag ){
		array_push($where, "ede.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ");
	}
	
	// monta o sql 
	$sql = "SELECT distinct
							 sa.sbacod  as sbacod,
							CASE WHEN trim(sa.sbatitulo) is not null THEN 
								sa.sbatitulo
							ELSE 
								'Não Preenchido' 
							END as titulo,
							sa.sbadsc,
							CASE WHEN sa.sbasituacao = 'P' THEN ' Pendente de Aprovação '
								 WHEN sa.sbasituacao = 'A' THEN ' Aprovado '
								 WHEN sa.sbasituacao = 'R' THEN ' Revisado '
								 WHEN sa.sbasituacao = 'E' THEN ' Enviado para Revisão ' 
							END as situacao
						FROM
							monitora.pi_subacao sa
						INNER JOIN monitora.pi_subacaounidade suu ON suu.sbaid = sa.sbaid 
						INNER JOIN public.unidade u ON suu.unicod = u.unicod 
						WHERE sa.sbastatus = 'A' " . ( !empty($arFiltro) ? " AND " . implode(" AND ", $arFiltro) : '' ) . " AND suu.unicod IS NOT NULL
						AND u.orgcod = '26000' $stWhere
						GROUP BY sa.sbaid, sa.sbacod, sa.sbatitulo, sa.sbadsc, sa.sbasituacao, sa.sbastatus
						--ORDER BY sa.sbatitulo";
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array("agrupador" => array(),
				 "agrupadoColuna" => array( "sbacod",
											"sbatitulo",
											"sbadsc",
											"sbasituacao" ) );
	
	array_push($agp['agrupador'], array("campo" => "sbacod"));
	array_push($agp['agrupador'], array("campo" => "sbatitulo",
										"label" => "sbadsc") );					
	array_push($agp['agrupador'], array("campo" => "sbasituacao"));
	return $agp;
}

function monta_coluna(){
	
	$coluna = array(	
					array(
						  "campo" => "sbacod",
				   		  "label" => "",
					   	  "type"  => "string"	
					),		
					array(
						  "campo" => "sbatitulo",
				   		  "label" => "",
					   	  "type"  => "string"	
					),		
					array(
						  "campo" => "sbadsc",
				   		  "label" => "",
					   	  "type"  => "string"	
					),		
					array(
						  "campo" => "sbasituacao",
				   		  "label" => "",
					   	  "type"  => "string"	
					)
			);
					  	
	return $coluna;			  	
}
?>
</body>
</html>