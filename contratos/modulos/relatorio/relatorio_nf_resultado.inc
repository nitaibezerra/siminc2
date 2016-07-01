<?php

function contratos_monta_coluna_relatorio(){

	$coluna = array();

	array_push( $coluna, array("campo" => "ftcdescricao",
							   "label" => "Descrição") );
	
	array_push( $coluna, array("campo" => "dataemissao",
							   "label" => "Data de Emissão da NF") );
	
	array_push( $coluna, array("campo" 	 => "nf_valor",
							   "label" 	 => "Valor da Nota Fiscal",
							   "php"	 => array(
												"expressao" => "'{nivelagrupador}' == 'ftcnumero'",
												"type"		=> "string",
												"var"		=> "html_nf",
// 												"true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaFatura&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar as notas fiscais cadastradas\" class=\"notprint\" style=\"float:left\">
// 														        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">
// 														        </a>",
												"true"		=> "",
												"false"		=> "",
												"html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_nf}&nbsp;&nbsp;{nf_valor}</div>"
											      )
								)
			);
	
	array_push( $coluna, array("campo" => "glosa",
							   "label" => "Glosa") );
	
	array_push( $coluna, array("campo" 	 => "nf_valorglosado",
							   "label" 	 => "Valor da Nota Fiscal<br>(após glosa)",
							   "php"	 => array(
							   						"expressao" => "'{nivelagrupador}' == 'ftcnumero'",
							   						"type"		=> "string",
													"var"		=> "html_nfpaga",
// 													"true"		=> "<a href=\"javascript:janela('?modulo=principal/popUpFatura&acao=A&requisicao=popUpListaFatura&nf=paga&ctrid={ctrid}&entidcontratada={entidcontratada}', 'detalheempenho', 500, 600);\" title=\"Visualizar as notas fiscais pagas\" class=\"notprint\" style=\"float:left\">			        					
// 															        	<img border=\"0\" src=\"/imagens/icone_lupa.png\">					
// 															        </a>",
													"true"		=> "",
													"false"		=> "",
													"html"		=> "<div style=\"white-space: nowrap; margin-right:8px;\">{html_nfpaga}&nbsp;&nbsp;{nf_valorglosado}</div>"
												  )
								) );								
	
	
	return $coluna;
	
}


function contratos_monta_agp_relatorio(){

	$agrupador = $_REQUEST['agrupador'];

	$agp = array(
			"agrupador" => array(),
			"agrupadoColuna" => array(
									  "ctrid",
									  "entidcontratada",
									  "ftcid",
									  "ftcdescricao",
									  "dataemissao",
// 									  "retido",
									  "nf_valor",
									  "glosa",
									  "nf_valorglosado",
					)
	);

	foreach ( $agrupador as $val ){
		switch( $val ){
			case "numcontrato":
				array_push($agp['agrupador'], array(
				"campo" => "numcontrato",
				"label" => "Nº do contrato")
				);
				break;
			case "contratante":
				array_push($agp['agrupador'], array(
				"campo" => "contratante",
				"label" => "Contratante")
				);
				break;
			case "contratado":
				array_push($agp['agrupador'], array(
				"campo" => "contratado",
				"label" => "Contratado")
				);
				break;
// 			case "empenho":
// 				array_push($agp['agrupador'], array(
// 				"campo" => "empenho",
// 				"label" => "Empenho")
// 				);
// 				break;
		}
	}

	array_push($agp['agrupador'], array(
										"campo" => "ftcnumero",
										"label" => "Nota Fiscal (NF)"));
	return $agp;

}

function contratos_monta_sql_relatorio(){
	global $arHspidPermitido;
	
	$where = array();

	extract($_REQUEST);
	
	// Interessado
	$whereInteressado = '';
	if( $estid[0] && $estid_campo_flag ){
		$whereInteressado = " cr.estid " . (!$estid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estid ) . "') ";
		$joinInteressado = "JOIN ( SELECT
										DISTINCT
										ctrid
								   FROM contratos.contratosetorresponsavel cr
								   WHERE
										{$whereInteressado}) cr ON cr.ctrid = ctr.ctrid";
		$whereInteressado = " AND " . $whereInteressado;
	}
		
	// Contratante
	if( $hspid[0] && $hspid_campo_flag ){
		if ( count( $arHspidPermitido ) ){
			$hspid = array_intersect($arHspidPermitido, $hspid);
			$hspid = ($hspid ? $hspid : array(0));
		}
		array_push($where, " ctr.hspid " . (!$hspid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $hspid ) . "') ");
	}elseif ( $arHspidPermitido ){
		array_push($where, " ctr.hspid  IN ('" . implode( "','", $arHspidPermitido ) . "') ");
	}
	
	// Contratado
	if( $entid[0] && $entid_campo_flag ){
		array_push($where, " ctr.entidcontratada " . (!$entid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $entid ) . "') ");
	}
	
	// Modalidade
	if( $modid[0] && $modid_campo_flag ){
		array_push($where, " ctr.modid " . (!$modid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $modid ) . "') ");
	}
	
	// Tipo Contrato
	if( $tpcid[0] && $tpcid_campo_flag ){
		array_push($where, " ctr.tpcid " . (!$tpcid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpcid ) . "') ");
	}
	
	// Vigência
	if( $sitid[0] && $sitid_campo_flag ){
		array_push($where, " sit.sitid " . (!$sitid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $sitid ) . "') ");
	}
	

	if ( $filtro_ctrobj ){
		array_push($where, " ctr.ctrobj ILIKE '%" . $filtro_ctrobj . "%'");
	}
	
	if ( $filtro_ctrnum ){
		array_push($where, " ctr.ctrnum = " . $filtro_ctrnum);
	}
	
	if ( $filtro_ctrano ){
		array_push($where, " ctr.ctrano ILIKE '%" . $filtro_ctrano . "%'");
	}
	
	if ( $filtro_ctrnummod ){
		array_push($where, " ctr.ctrnummod ILIKE '%" . $filtro_ctrnummod . "%'");
	}
	
	if ( $filtro_ctrproccontr ){
		array_push($where, " ctr.ctrproccontr ILIKE '%" . $filtro_ctrproccontr . "%'");
	}
	
	if ( $filtro_ctrprocexecctr ){
		array_push($where, " ctr.ctrprocexecctr ILIKE '%" . $filtro_ctrprocexecctr . "%'");
	}
	
	if ( $filtro_ctrprocexecfin ){
		array_push($where, " ctr.ctrprocexecfin ILIKE '%" . $filtro_ctrprocexecfin . "%'");
	}
	
	$dtinicio = formata_data_sql($dtinicio); 
	$dtfim	  = formata_data_sql($dtfim);
	
	if ( $dtinicio && $dtfim ){
		array_push($where, " ctr.ctrdtiniciovig::DATE BETWEEN '{$dtinicio}' AND '{$dtfim}'");
	}elseif ( $dtinicio ){
		array_push($where, " ctr.ctrdtiniciovig::DATE > '{$dtinicio}'");
	}elseif ( $dtfim ){
		array_push($where, " ctr.ctrdtiniciovig::DATE < '{$dtfim}'");
	}

	$dtinicio2 = formata_data_sql($dtinicio2);
	$dtfim2	   = formata_data_sql($dtfim2);
	
	if ( $dtinicio2 && $dtfim2 ){
		array_push($where, " ctr.ctrdtfimvig::DATE BETWEEN '{$dtinicio2}' AND '{$dtfim2}'");
	}elseif ( $dtinicio2 ){
		array_push($where, " ctr.ctrdtfimvig::DATE > '{$dtinicio2}'");
	}elseif ( $dtfim2 ){
		array_push($where, " ctr.ctrdtfimvig::DATE < '{$dtfim2}'");
	}

	// monta o sql
	$sql = "SELECT
				'Nº do contrato: ' || ctr.ctrnum as numcontrato,
				ftc.ftcid,
				'Nº da NF: ' || ftc.ftcnumero as ftcnumero,
				ftc.ftcdescricao,
				CASE 
					WHEN ent.entnumcpfcnpj IS NULL THEN ent.entnome
					ELSE cnpj_formatar( ent.entnumcpfcnpj ) || ' - ' || ent.entnome
				END AS contratado,
				CASE 
					WHEN h.hspcnpj IS NULL THEN h.hspdsc
					ELSE cnpj_formatar( h.hspcnpj ) || ' - ' || h.hspdsc
				END AS contratante,
				TO_CHAR(ftc.ftcdataemissao, 'dd/mm/YYYY') AS dataemissao,
				-- COALESCE(SUM(ftc.retir + ftc.retcsll + ftc.retcofins + ftc.retpasep + ftc.retoutro), 0) AS retido,
				COALESCE(ftc.ftcvalor,0) AS nf_valor,
				COALESCE(ftc.ftcglosa, 0) AS glosa,
				(COALESCE(ftc.ftcvalor,0) - COALESCE(ftc.ftcglosa,0)) AS nf_valorglosado
			FROM
				contratos.ctcontrato ctr	
				JOIN contratos.faturacontrato ftc ON ftc.ftcstatus = 'A' AND ftc.ctrid = ctr.ctrid				
				{$joinInteressado}
				LEFT JOIN entidade.entidade ent ON ent.entid = ctr.entidcontratada
				LEFT JOIN contratos.cttipocontrato tpc ON tpc.tpcid = ctr.tpcid
				LEFT JOIN contratos.ctmodalidadecontrato mod on ctr.modid = mod.modid
				LEFT JOIN contratos.ctsituacaocontrato sit on ctr.sitid = sit.sitid
				LEFT JOIN contratos.hospital h ON h.hspid = ctr.hspid AND 
								  				  h.hspstatus = 'A'	
			WHERE
				ctr.ctrstatus = 'A' " . (($where) ? " AND " . implode(" AND ", $where) : "") . "
				";	
				
// 	// monta o sql
// 	$sql = "SELECT
// 				es.nu_empenho,
// 				CASE 
// 					WHEN ent.entnumcpfcnpj IS NULL THEN ent.entnome
// 					ELSE cnpj_formatar( ent.entnumcpfcnpj ) || ' - ' || ent.entnome
// 				END AS contratante,
// 				CASE 
// 					WHEN h.hspcnpj IS NULL THEN h.hspdsc
// 					ELSE cnpj_formatar( h.hspcnpj ) || ' - ' || h.hspdsc
// 				END AS contratado,
// 				TO_CHAR(es.dataempenho, 'DD-MM-YYYY') AS dataempenho,
// 				es.observacao,
// 				'Nº do contrato: ' || ctr.ctrnum as numcontrato,
// 				es.valor AS empenho_valor
// 			FROM 
// 				contratos.ctcontrato ctr	
// 			{$joinInteressado}
// 			JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = ctr.ctrid	
// 			JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid
// 			LEFT JOIN entidade.entidade ent ON ent.entid = ctr.entidcontratada
// 			LEFT JOIN contratos.cttipocontrato tpc ON tpc.tpcid = ctr.tpcid
// 			LEFT JOIN contratos.ctmodalidadecontrato mod on ctr.modid = mod.modid
// 			LEFT JOIN contratos.ctsituacaocontrato sit on ctr.sitid = sit.sitid
// 			LEFT JOIN contratos.hospital h ON h.hspid = ctr.hspid AND 
// 							  				  h.hspstatus = 'A'	
// 			WHERE
// 				ctr.ctrstatus = 'A' " . (($where) ? " AND " . implode(" AND ", $where) : "") . "
// 			LIMIT
// 				25";
// 	dbg($sql);		
	return $sql;
}


set_time_limit(0);
ini_set("memory_limit", "1024M");

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
	$existe_rel = 0;
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_REQUEST['titulo']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_REQUEST['titulo'],
			addslashes( addslashes( serialize( $_REQUEST ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	?>
	<script type="text/javascript">
		alert('Operação realizada com sucesso!');
		location.href = 'contratos.php?modulo=relatorio/relatorio_geral&acao=A';
	</script>
	<?
	die;
}

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = contratos_monta_sql_relatorio();
$agrupador = contratos_monta_agp_relatorio();
$coluna    = contratos_monta_coluna_relatorio();
$dados 	   = $db->carregar($sql);


$rel->setEspandir(true);	
$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTotNivel(true);
$rel->setTotalizador(true);
$rel->setTolizadorLinha(true);
$rel->setMonstrarTolizadorNivel(true);

//dbg($rel->getAgrupar(), 1);

// Gera o XLS do relatório
if ( $_REQUEST['tipoRelatorio'] == 'xls' ){
	ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die;
}

?>
<html>
	<head>
		<title>SIG - Sistema de Informações Gerenciais da EBSERH</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
		<!--  Monta o Relatório -->
		<? echo $rel->getRelatorio(); ?>
		
	</body>
</html>