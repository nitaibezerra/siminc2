<?php

ini_set("memory_limit", "2048M");
// set_time_limit(120);

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$superuser = $db->testa_superuser();
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql($superuser);
$dados = $db->carregar($sql);

if( $_POST['req'] == 'geraxls' ){
	$arCabecalho = array();
	$colXls = array();
	array_push($arCabecalho, 'Esfera');
	array_push($colXls, 'esfera');
	array_push($arCabecalho, 'Estado');
	array_push($colXls, 'estuf');
	array_push($arCabecalho, 'Município');
	array_push($colXls, 'mundescricao');
	array_push($arCabecalho, 'Escola');
	array_push($colXls, 'escola');
	array_push($arCabecalho, 'Turma');
	array_push($colXls, 'turma');
	array_push($arCabecalho, 'Aluno');
	array_push($colXls, 'aluno');
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
	header ( "Content-type: application/xls; name=SIMEC_RelatorioIndicadores".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%',$par2);
	die;
}


?>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
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
	$where = array();
	// estado
	if( $estuf[0] && $estuf_campo_flag ){
		array_push($where, "( mun.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') )");
	}
	
	// municipio
	if( $mundescricao ){
		array_push($where, " mun.mundescricao ilike '%{$mundescricao}%' ");
	}
	
	// Escola
	if( $entid[0] && $entid_campo_flag ){
		array_push($where, " ent.entid " . (!$entid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $entid ) . "') ");
	}
	
	// esfera
	if( $esfera == 'M' ){
		array_push($where, " apc.apcesfera ='M'");
	}
	if( $esfera == 'E' ){
		array_push($where, " apc.apcesfera = 'E'");
	}
	if( $_POST['status'] != '' && $_POST['status'] != ' ' ){
		$where[] = " est.eststatus = '".$_POST['status']."' ";
	}
	if( $_POST['justificativaD'] ){
		$where[] = " est.estmotivoinativacao = 'DESISTENTE' ";
	}
	if( $_POST['justificativaF'] ){
		$where[] = " est.estmotivoinativacao = 'FREQUENCIA INSUFICIENTE' ";
	}
	if( $_POST['justificativaO'] ){
		$where[] = " est.estmotivoinativacao NOT IN ('DESISTENTE','FREQUENCIA INSUFICIENTE') ";
	}
	if(!$superuser) {
		
		$perfis = pegaPerfilGeral();
		
		if(in_array(PFL_SECRETARIO_MUNICIPAL, $perfis)) {
			$inner_municipio = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.muncod = mun.muncod) AND rpustatus='A'";
		}
	
		if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
			$inner_municipio = "AND apc.apcesfera  = 'M'
							    INNER JOIN projovemcampo.secretaria sec ON sec.secaid = apc.secaid AND sec.secoordcpf='".$_SESSION['usucpf']."'";
		}
		
		if(in_array(PFL_SECRETARIO_ESTADUAL, $perfis)) {
			$inner_estado = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.estuf = mun.estuf) AND rpustatus='A'";
		}
		
		if(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
			$inner_estado = "AND apc.apcesfera ='E'
						     INNER JOIN projovemcampo.secretaria sec ON sec.secaid = apc.secaid AND sec.secoordcpf='".$_SESSION['usucpf']."'";
		}
	}
	
	
	// monta o sql 
	$sql = "SELECT DISTINCT 
				mun.estuf as estuf,
				CASE WHEN apcesfera  = 'E'
					THEN 'Estadual'
					ELSE 'Municipal'
				END as esfera,
				mun.mundescricao as mundescricao,
				ent.entnome as escola,
				turdescricao as turma,
				estcpf,
				estnome,
				estcpf||'|'||estnome||' - '||coalesce(mun.mundescricao,'NA')||'/'||mun.estuf||' - '||coalesce(estmotivoinativacao,'NA') as aluno,
				CASE WHEN eststatus = 'A' THEN 1 ELSE 0 END as qtd_ativo,
				CASE WHEN eststatus = 'D' THEN 1 ELSE 0 END as qtd_inativo_des,
				CASE WHEN eststatus = 'I' THEN 1 ELSE 0 END as qtd_inativo_out,
				CASE WHEN eststatus in ('D','I') THEN 1 ELSE 0 END as qtd_inativo_total,
				1 as qtd
			FROM 
				projovemcampo.estudante est
			INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid 
			INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid AND apc.apcstatus ='t'
			INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
			LEFT JOIN  entidade.endereco ende ON ende.entid = ent.entid
			LEFT JOIN  territorios.municipio mun ON mun.muncod = ende.muncod
            ".($inner_estado?$inner_estado:$inner_municipio)."
			WHERE  
				est.eststatus is not null
            ".($where[0] ? ' AND ' . implode(' AND ', $where) : '' )." 
			ORDER BY estuf, mundescricao, turma, estnome
			";
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array("agrupador" => array(),
				 "agrupadoColuna" => array( "aluno",
											"qtd",
											"qtd_ativo",
											"qtd_inativo_out",
											"qtd_inativo_des",
											//"qtd_inativo_fre",
											"qtd_inativo_total" ) );
	
	array_push($agp['agrupador'], array("campo" => "esfera",
								 		"label" => "Esfera") );	
	array_push($agp['agrupador'], array("campo" => "estuf",
								 		"label" => "Estado") );					
	array_push($agp['agrupador'], array("campo" => "mundescricao",
								 		"label" => "Municipio") );					
	array_push($agp['agrupador'], array("campo" => "escola",
										"label" => "Escola") );	
	array_push($agp['agrupador'], array("campo" => "turma",
										"label" => "Turma") );	
	array_push($agp['agrupador'], array("campo" => "aluno",
										"label" => "Aluno") );	
	return $agp;
}

function monta_coluna(){
	
	$coluna = array(	
					array(
						  "campo" => "qtd",
				   		  "label" => "Total",
					   	  "type"  => "numeric"	
					),		
					array(
						  "campo" => "qtd_ativo",
				   		  "label" => "Ativos",
					   	  "type"  => "numeric"	
					),		
					array(
						  "campo" => "qtd_inativo_des",
				   		  "label" => "Inativos por Desistência",
					   	  "type"  => "numeric"	
					),		
//					array(
//						  "campo" => "qtd_inativo_fre",
//				   		  "label" => "Inativos por Frequencia Insuficiente",
//					   	  "type"  => "numeric"	
//					),		
					array(
						  "campo" => "qtd_inativo_out",
				   		  "label" => "Inativos - Outros",
					   	  "type"  => "numeric"	
					),		
					array(
						  "campo" => "qtd_inativo_total",
				   		  "label" => "Total de Inativos",
					   	  "type"  => "numeric"	
					)	
			);
					  	
	return $coluna;			  	
}
?>
</body>
</html>