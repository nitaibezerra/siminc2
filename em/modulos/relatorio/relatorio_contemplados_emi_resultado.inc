<?php
ini_set("memory_limit", "2048M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$superuser = $db->testa_superuser();
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql($superuser);
$dados = $db->carregar($sql);

if( $_POST['req'] == 'geraxls' ){
	$arCabecalho = array();
	$colXls = array();
	array_push($arCabecalho, 'UF');
	array_push($colXls, 'estuf');
	array_push($arCabecalho, 'Município');
	array_push($colXls, 'municipio');
	array_push($arCabecalho, 'Nome da Escola');
	array_push($colXls, 'escola');
	array_push($arCabecalho, 'Código INEP');
	array_push($colXls, 'inep');
	array_push($arCabecalho, 'N° Alunos Beneficiados');
	array_push($colXls, 'alunos');
	array_push($arCabecalho, 'Valor Repasse (R$)');
	array_push($colXls, 'valor_total');
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
	$sql = "SELECT DISTINCT
				ede.estuf as estuf,
				mun.mundescricao as municipio,
				emi.entcodent as inep,
				ent.entnome as escola,
				cen2.qtd_alunos as alunos,
				sum(mdovalorunitario*mdoqtd) as valor_total,
				tjo.etjdescricao 
			FROM 
			    em.emiensinomedioinovador emi 
			LEFT  JOIN em.emigap ga1 ON ga1.emiid = emi.emiid 
			LEFT  JOIN em.emimatrizdistribuicaoorcamentargap gt1 ON ga1.papid = gt1.papid
			INNER JOIN entidade.entidade ent ON ent.entid = emi.entid
			LEFT  JOIN (SELECT DISTINCT
					coalesce(sum(emcquantidadealunos),0) as qtd_alunos, 
					entid 
				    FROM em.emicenso 
				    GROUP BY entid) as cen2 ON cen2.entid::integer = ent.entid
					    
			INNER JOIN em.emitipojornada     tjo ON tjo.etjid = emi.etjid
			INNER JOIN entidade.endereco     ede ON ede.entid = ent.entid
			INNER JOIN territorios.estado    est ON est.estuf = ede.estuf
			LEFT  JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			WHERE
				".implode(' AND ', $where)."
			GROUP BY
				ent.entnome, emi.entcodent, ede.estuf, mun.mundescricao, ent, cen2.qtd_alunos, tjo.etjjornada,  ent.entid, tjo.etjdescricao        
			ORDER BY
			    estuf, municipio, escola";
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array("agrupador" => array(),
				 "agrupadoColuna" => array( "inep",
											"etjdescricao",
											"alunos",
											"valor_total" ) );
	
	array_push($agp['agrupador'], array("campo" => "estuf",
								 		"label" => "UF") );					
	array_push($agp['agrupador'], array("campo" => "municipio",
										"label" => "Município") );					
	array_push($agp['agrupador'], array("campo" => "escola",
										"label" => "Nome da Escola") );	
	return $agp;
}

function monta_coluna(){
	
	$coluna = array(	
					array(
						  "campo" => "inep",
				   		  "label" => "Código <br> INEP",
					   	  "type"  => "string"	
					),		
					array(
						  "campo" => "etjdescricao",
				   		  "label" => "Jornada Escolar",
					   	  "type"  => "string"	
					),		
					array(
						  "campo" => "alunos",
				   		  "label" => "N° Anulos <br> Beneficiados",
					   	  "type"  => "numeric"	
					),		
					array(
						  "campo" => "valor_total",
				   		  "label" => "Valor <br> Repasse (R$)",
					   	  "type"  => "numeric"	
					)
			);
					  	
	return $coluna;			  	
}
?>
</body>
</html>