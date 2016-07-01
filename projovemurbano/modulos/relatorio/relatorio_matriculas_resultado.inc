<?php
checkano();
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
	array_push($arCabecalho, 'Polo');
	array_push($colXls, 'polo');
	array_push($arCabecalho, 'Nucleo');
	array_push($colXls, 'nucleo');
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
		array_push($where, "( pju.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') OR mun.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') )");
	}
	
	// municipio
	if( $mundescricao ){
		array_push($where, " mun.mundescricao ilike '%{$mundescricao}%' ");
	}
	
	// Polo
	if( $polid[0] && $polid_campo_flag ){
		array_push($where, " cae.polid " . (!$polid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $polid ) . "') ");
	}
	
	// Nucleo
	if( $nucid[0] && $nucid_campo_flag ){
		array_push($where, " cae.nucid " . (!$nucid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $nucid ) . "') ");
	}
	
	// esfera
	if( $esfera == 'M' ){
		array_push($where, " mun.mundescricao is not null");
	}
	if( $esfera == 'E' ){
		array_push($where, " mun.mundescricao is null");
	}
	if( $_POST['status'] != '' && $_POST['status'] != ' ' ){
		$where[] = " cae.caestatus = '".$_POST['status']."' ";
	}
	if( $_POST['justificativaD'] ){
		$where[] = " cae.caejustificativainativacao = 'DESISTENTE' ";
	}
	if( $_POST['justificativaF'] ){
		$where[] = " cae.caejustificativainativacao = 'FREQUENCIA INSUFICIENTE' ";
	}
	if( $_POST['justificativaO'] ){
		$where[] = " cae.caejustificativainativacao NOT IN ('DESISTENTE','FREQUENCIA INSUFICIENTE') ";
	}
	if( $_SESSION['projovemurbano']['ppuid'] != 1 ){
		$where[] = " cae.ppuid = ".$_SESSION['projovemurbano']['ppuid']."";
		$where[] = " nuc.tprid IN (".(is_array($_POST['tprid'])?implode(',',$_POST['tprid']):'666').") ";
	}
	if(!$superuser) {
		
		$perfis = pegaPerfilGeral();
		
		if(in_array(PFL_SECRETARIO_MUNICIPAL, $perfis)) {
			$inner_municipio = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.muncod = pju.muncod) AND rpustatus='A'";
		}
	
		if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
			$inner_municipio = " AND pju.muncod IS NOT NULL
							    INNER JOIN projovemurbano.coordenadorresponsavel cr ON pju.pjuid=cr.pjuid AND cr.corcpf='".$_SESSION['usucpf']."'";
		}
		
		if(in_array(PFL_SECRETARIO_ESTADUAL, $perfis)) {
			$inner_estado = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.estuf = pju.estuf)";
		}
		
		if(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)) {
			$inner_estado = "AND pju.estuf IS NOT NULL
						     INNER JOIN projovemurbano.coordenadorresponsavel cr ON pju.pjuid=cr.pjuid AND cr.corcpf='".$_SESSION['usucpf']."'";
		}
	}
	
	if( $_SESSION['projovemurbano']['ppuid'] != 1){
		$inner2013 = "LEFT JOIN projovemurbano.tipoprograma tpr ON nuc.tprid = tpr.tprid";
		$campo2013 = "tprdesc,";
	}
	
	// monta o sql 
	$sql = "SELECT DISTINCT COALESCE(pju.estuf,mun.estuf) as estuf,
                        CASE WHEN pju.muncod IS NULL
                                THEN 'Estadual'
                                ELSE 'Municipal'
                        END as esfera,
                        $campo2013
                        coalesce(mun.mundescricao,'Esfera Estadual') as mundescricao,
                        COALESCE('POLO '||cae.polid,'Não Possui') as polo,
                        'NÚCLEO '||nuc.nucid as nucleo,
                        CASE WHEN nes.nuetipo = 'S' THEN 'SEDE : ' ELSE 'ANEXO : ' END|| entnome as escola,
                        turdesc as turma,
                        caecpf,
                        caenome,
                        caecpf||'|'||caenome||' - '||coalesce(mun2.mundescricao,'NA')||'/'||cae.estuf||' - '||coalesce(caejustificativainativacao,'NA') as aluno,
                        CASE WHEN caestatus = 'A' THEN 1 ELSE 0 END as qtd_ativo,
                        CASE WHEN caestatus = 'D' THEN 1 ELSE 0 END as qtd_inativo_des,
                        CASE WHEN caestatus = 'I' THEN 1 ELSE 0 END as qtd_inativo_out,
                        CASE WHEN caestatus in ('D','I') THEN 1 ELSE 0 END as qtd_inativo_total,
                        --CASE WHEN caestatus = 'I' AND caejustificativainativacao = 'DESISTENTE' THEN 1 ELSE 0 END as qtd_inativo_des,
                        --CASE WHEN caestatus = 'I' AND caejustificativainativacao = 'FREQUENCIA INSUFICIENTE' THEN 1 ELSE 0 END as qtd_inativo_fre,
                        --CASE WHEN caestatus = 'I' AND caejustificativainativacao NOT IN ('DESISTENTE','FREQUENCIA INSUFICIENTE') THEN 1 ELSE 0 END as qtd_inativo_out,
                        --CASE WHEN caestatus = 'I' THEN 1 ELSE 0 END as qtd_inativo_total,
                        1 as qtd
            FROM projovemurbano.cadastroestudante cae

            LEFT JOIN projovemurbano.turma tur ON tur.turid = cae.turid 

            LEFT JOIN projovemurbano.nucleo nuc ON nuc.nucid = tur.nucid --AND nuc.nucstatus = 'A'
            $inner2013 
            LEFT JOIN projovemurbano.nucleoescola nes ON nes.entid = tur.entid AND nes.nucid = nuc.nucid --AND nuestatus = 'A'
            LEFT JOIN entidade.entidade ent ON ent.entid = tur.entid 

            LEFT JOIN projovemurbano.municipio muni ON muni.munid = nuc.munid

            INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = cae.pjuid AND pjustatus = 'A'

            LEFT JOIN territorios.municipio mun ON mun.muncod = pju.muncod 
            LEFT JOIN territorios.municipio mun2 ON mun2.muncod = cae.muncod

            ".($inner_estado?$inner_estado:$inner_municipio)."
            WHERE  
            cae.caestatus is not null
            AND cae.ppuid = {$_SESSION['projovemurbano']['ppuid']}
            ".($where[0] ? ' AND' . implode(' AND ', $where) : '' )." 

            ORDER BY estuf, mundescricao, polo, nucleo, turma, caenome";
//             ver($sql,d);
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
	if( $_SESSION['projovemurbano']['ppuid'] != 1){
		$inner2013 = "LEFT JOIN projovemurbano.tipoprograma tpr ON nuc.tprid = tpr.tprid";
		$campo2013 = "tprdesc,";
		array_push($agp['agrupador'], array("campo" => "tprdesc",
								 		"label" => "Tipo do programa") );
	}				
	array_push($agp['agrupador'], array("campo" => "polo",
										"label" => "Polo") );		
	array_push($agp['agrupador'], array("campo" => "nucleo",
										"label" => "Nucleo") );
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