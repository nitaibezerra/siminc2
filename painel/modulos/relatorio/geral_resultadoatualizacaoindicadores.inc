<?php

set_time_limit(0);
ini_set("memory_limit", "1024M");

function atualizacaoindicadores_monta_sql_relatorio(){
	
	$wh  = array();
	$wh1 = array();
	
	extract($_REQUEST);
	
	if($situacaoindicador=='1') $wh1[] = "i.indpublicado=true";
	if($situacaoindicador=='2') $wh1[] = "i.indpublicado=false";
	
	if($situacaoatualizacao=='1') $wh[] = "case when dpediasverificacao=1 then ultatualizacao+interval '10 days'
			     when dpediasverificacao=7 then ultatualizacao+interval '17 days'
			     when dpediasverificacao=30 then ultatualizacao+interval '40 days'
			     when dpediasverificacao=60 then ultatualizacao+interval '70 days'
			     when dpediasverificacao=180 then ultatualizacao+interval '190 days'
			     when dpediasverificacao=365 then ultatualizacao+interval '375 days'
			     when dpediasverificacao=730 then ultatualizacao+interval '740 days'
			end>now() and foo.ultatualizacao is not null";
	
	if($situacaoatualizacao=='2') $wh[] = "case when dpediasverificacao=1 then ultatualizacao+interval '10 days'
			     when dpediasverificacao=7 then ultatualizacao+interval '17 days'
			     when dpediasverificacao=30 then ultatualizacao+interval '40 days'
			     when dpediasverificacao=60 then ultatualizacao+interval '70 days'
			     when dpediasverificacao=180 then ultatualizacao+interval '190 days'
			     when dpediasverificacao=365 then ultatualizacao+interval '375 days'
			     when dpediasverificacao=730 then ultatualizacao+interval '740 days'
			end<now() or foo.ultatualizacao is null";
	
	
	// secretaria
	if( $secid[0] && $secid_campo_flag ){
		array_push($wh1, " i.secid " . (!$secid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $secid ) . "') ");
	}
	
	if ($tipoRelatorio == 'xls') {
		$condicao_em_dia   = "'Atualização em dia'";
		$condicao_atrasada = "'Atualização atrasada'";
	} else {
		$condicao_em_dia   = "'<span style=background:#c6e2ff;>Atualização em dia</span>'";
		$condicao_atrasada = "'<span style=background:#f0cdd1;>Atualização atrasada</span>'";
	}
	
	
	
	/*
	// mantenedor
	if( $idmantenedor[0] && $idmantenedor_campo_flag ){
		array_push($where, " co.idmantenedor " . (!$idmantenedor_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $idmantenedor ) . "') ");
	}
	
	
	// UF
	if( $uf[0] && $uf_campo_flag ){
		array_push($where, " es.estuf " . (!$uf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $uf ) . "') ");
	}
	
	// municipio
	if( $municipio[0]  && $municipio_campo_flag ){
		array_push($where, " mu.muncod " . (!$municipio_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $municipio ) . "') ");
	}
	
	// ies
	if( $iesid[0]  && $iesid_campo_flag ){
		array_push($where, " ie.iesid " . (!$iesid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $iesid ) . "') ");
	}
	
	// nome curso
	if( $idcurso[0]  && $iesid_campo_flag ){
		array_push($where, " co.idcurso " . (!$idcurso_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $idcurso ) . "') ");
	}
	
	// tipo curso
	if( $tipocurso[0]  && $tipocurso_campo_flag ){
		array_push($where, " co.tipocurso " . (!$tipocurso_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tipocurso ) . "') ");
	}
	
	// polo
	if( $polid[0]  && $polid_campo_flag ){
		array_push($where, " pl.polid " . (!$polid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $polid ) . "') ");
	}
	*/
	
	// monta o sql 
	$sql = "select 
			foo.usucpf||' - '||foo.usunome as responsavel,
			foo.usufoneddd||' '||foo.usufonenum as telefoneresponsavel,
			foo.indnome,
			foo.secdsc,
			foo.perdsc,
			case when foo.indpublicado=true then 'Publicado' else 'Não publicado' end as publicado,
			coalesce(to_char(foo.ultatualizacao,'dd/mm/YYYY HH24:MI'),'Sem atualização') as ultatualizacao,
			case when 
			(((case when dpediasverificacao=1 then ultatualizacao+interval '10 days'
			     when dpediasverificacao=7 then ultatualizacao+interval '17 days'
			     when dpediasverificacao=30 then ultatualizacao+interval '40 days'
			     when dpediasverificacao=60 then ultatualizacao+interval '70 days'
			     when dpediasverificacao=180 then ultatualizacao+interval '190 days'
			     when dpediasverificacao=365 then ultatualizacao+interval '375 days'
			     when dpediasverificacao=730 then ultatualizacao+interval '740 days'
			end)>now()) and foo.ultatualizacao is not null) then {$condicao_em_dia} else $condicao_atrasada end as atualizacao
			
			from (
			select u.usucpf, 
			       u.usunome, 
				   s.secdsc,
			       i.indid||' - '||i.indnome as indnome, 
			       p.perdsc, 
			       p.dpediasverificacao,
				   i.indpublicado,
				   u.usufoneddd,
				   u.usufonenum,
			       (select max(agddata) from painel.agendamentocarga where indid=i.indid and agdprocessado=true) as ultatualizacao 
			from painel.indicador i 
			inner join painel.periodicidade p on p.perid = i.peridatual 
			left join seguranca.usuario u on u.usucpf = i.usucpf 
			left join painel.secretaria s on s.secid = i.secid 
			where indencerrado=false AND indstatus='A' ".(($wh1)?"AND ".implode(" AND ", $wh1):"")."
			) foo ".(($wh)?"WHERE ".implode(" AND ", $wh):"");

	return $sql;
	
}

$sql = atualizacaoindicadores_monta_sql_relatorio();

$cabecalho = array("Responsável","Telefone responsável","Indicador","Secretaria","Periodicidade de atualização","Situação do indicador", "Data da última atualização","Situação da atualização");

// Gera o XLS do relatório
if ($_REQUEST['tipoRelatorio'] == 'xls'){
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=rel_contatos_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=rel_atualizacaoindicadores_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,1000000000,5,'N','100%','');
	exit;
	
}

?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
	<!--  Monta o Relatório -->
	<? 
	
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','S');
	
	?>
	</body>
</html>