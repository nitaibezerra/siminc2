<?php
ini_set( "display_error", 1 );
error_reporting( E_ALL ^ E_NOTICE );

//dump( ini_get( "memory_limit" ), true );

ini_set( "memory_limit", "1024M" ); // ...
set_time_limit(0);

$condicaoExterno = array();
$condicaoInterno = array();

// instrumento

array_push( $condicaoInterno, " iu.itrid = " . INSTRUMENTO_DIAGNOSTICO_MUNICIPAL );

// demanda
// sempre é M
// array_push( $condicaoInterno, " ai.acilocalizador = 'M' " );

$tpmid = array();

// grandes cidades e classificação do IDEB
if ( count($_REQUEST['grandescidades']) && $_REQUEST['grandescidades'][0] ) { 	
	array_push($tpmid,$_REQUEST['grandescidades'] );
}
if ( count( $_REQUEST['ideb'] ) && $_REQUEST['ideb'][0] ) {
	for($i=0; $i<count($_REQUEST['ideb']); $i++) {
		array_push( $tpmid, $_REQUEST['ideb'][$i] );
	}	
}

if ( count( $tpmid ) && $tpmid[0] )
{
	$sqlIbge = "
		select
			muncod
		from territorios.muntipomunicipio
		where
			tpmid in ( '" . implode( "','", $tpmid ) . "' )
	";	
	array_push( $condicaoInterno, " iu.muncod in ( " . $sqlIbge . " ) " );
}


// regiao
if ( count( $_REQUEST['regcod'] ) && $_REQUEST['regcod'][0] )
{
	array_push( $condicaoExterno, " regiao.regcod in ( '" . implode( "','", $_REQUEST['regcod'] ) . "' ) " );
}

// uf
if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] )
{
	array_push( $condicaoInterno, " iu.mun_estuf in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
}

// municipio
if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] )
{
	array_push( $condicaoInterno, " iu.muncod in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
}

// dimensao
if ( count( $_REQUEST['dimid'] ) && $_REQUEST['dimid'][0] )
{
	array_push( $condicaoInterno, " di.dimid in  ( '" . implode( "','", $_REQUEST['dimid'] ) . "' ) " );
}

// area
if ( count( $_REQUEST['ardid'] ) && $_REQUEST['ardid'][0] )
{
	array_push( $condicaoInterno, " ad.ardid in ( '" . implode( "','", $_REQUEST['ardid'] ) . "' ) " );
}

// indicadores
if ( count( $_REQUEST['indid'] ) && $_REQUEST['indid'][0] )
{
	array_push( $condicaoInterno, " id.indid in ( '" . implode( "','", $_REQUEST['indid'] ) . "' ) " );
}

// unidade de medida
if ( count( $_REQUEST['undid'] ) && $_REQUEST['undid'][0] )
{
	array_push( $condicaoInterno, " um.undid in ( '" . implode( "','", $_REQUEST['undid'] ) . "' ) " );
}

// plano interno
if ( count( $_REQUEST['plicod'] ) && $_REQUEST['plicod'][0] )
{
	array_push( $condicaoInterno, " pi.plicod in ( '" . implode( "','", $_REQUEST['plicod'] ) . "' ) " );
}

// programa
if ( count( $_REQUEST['prgid'] ) && $_REQUEST['prgid'][0] )
{
	array_push( $condicaoInterno, " si.prgid in ( '" . implode( "','", $_REQUEST['prgid'] ) . "' ) " );
}

// parecer acao
if ( count( $_REQUEST['aciparecer'] ) && $_REQUEST['aciparecer'][0] )
{
	array_push( $condicaoInterno, " ai.aciparecer in ( '" . implode( "','", $_REQUEST['aciparecer'] ) . "' ) " );
}

// parecer subacao
if ( count( $_REQUEST['psuid'] ) && $_REQUEST['psuid'][0] )
{
	array_push( $condicaoInterno, "  spt.sptid in ( '" . implode( "','", $_REQUEST['psuid'] ) . "' ) " );
}

// status subacao
if ( count( $_REQUEST['ssuid'] ) && $_REQUEST['ssuid'][0] )
{
	array_push( $condicaoInterno, " coalesce(spt.ssuid,0) in ( '" . implode( "','", $_REQUEST['ssuid'] ) . "' ) " );
}

// forma de execução
if ( count( $_REQUEST['frmid'] ) && $_REQUEST['frmid'][0] )
{
	array_push( $condicaoInterno, " fe.frmid in ( '" . implode( "','", $_REQUEST['frmid'] ) . "' ) " );
}

// forma de atendimento
if ( count( $_REQUEST['foaid'] ) && $_REQUEST['foaid'][0] )
{
	array_push( $condicaoInterno, " fa.foaid in ( '" . implode( "','", $_REQUEST['foaid'] ) . "' ) " );
}


$condicaoInterno = count( $condicaoInterno ) ? " where " . implode( " and ", $condicaoInterno ) : "";
$condicaoExterno = count( $condicaoExterno ) ? " where " . implode( " and ", $condicaoExterno ) : "";

$sql = "
	select
		sbaid,
		sbaporescola,
		documento.esdid as estado_documento,
		estado.estuf || ' - ' || municipio.muncod as ibge,
		estado.estuf || ' - ' || municipio.mundescricao || ' - ' || municipio.muncod as municipio,
		estado.estuf || ' - ' || estado.estdescricao as estado,
		regiao.regdescricao as regiao,
		pais.paidescricao as pais,
		relatorio.dimcod || '. ' || relatorio.dimdsc as dimensao,
		relatorio.ardcod || '. ' || relatorio.arddsc as area,
		relatorio.indcod || '. ' || relatorio.inddsc as indicador,
		relatorio.acidsc as acao,
		relatorio.sbadsc as subacao,
		relatorio.unddsc as unidade,
		relatorio.frmdsc as execucao,
		relatorio.prgdsc as programa,
		case 
			when coalesce( relatorio.plicod, '' ) = '' then 'Sem Plano Interno' 
			else relatorio.plicod || '. ' || relatorio.plidsc
		end as planointerno,			
		relatorio.aciparecer as pareceracao,
		coalesce( relatorio.psudescricao, 'Sem Parecer' ) as tipoparecersubacao,
		coalesce( relatorio.gpsdescricao, 'Sem Grupo Parecer' ) as grupoparecersubacao,
		coalesce( relatorio.sptparecer, 'Sem Status' ) as statussubacao,
		relatorio.sptano as ano,
		'' as fis_0_original,
		'' as fis_1_original,
		'' as fis_2_original,
		'' as fis_3_original,
		'' as fis_4_original,
		'' as fis_0_copia,
		'' as fis_1_copia,
		'' as fis_2_copia,
		'' as fis_3_copia,
		'' as fis_4_copia,
		'' as fis_0_original,
		'' as fin_1_original,
		'' as fin_2_original,
		'' as fin_3_original,
		'' as fin_4_original,
		'' as fin_0_copia,
		'' as fin_1_copia,
		'' as fin_2_copia,
		'' as fin_3_copia,
		'' as fin_4_copia

	from (
			select
				-- dados para exibicao
					si.sbaid,
					si.sbaporescola,
					iu.itrid,			-- instrumento
					iu.docid,			-- documento
					iu.muncod,			-- municipio
					di.dimcod,			-- dimensao
					di.dimid,			-- dimensao
					di.dimdsc,			-- dimensao
					ad.ardcod,			-- area
					ad.ardid,			-- area
					ad.arddsc,			-- area
					id.indcod,			-- indicador
					id.indid,			-- indicador
					id.inddsc,			-- indicador
					ai.acidsc,			-- acao
					ai.aciparecer,		-- parecer acao
					si.sbadsc,			-- subacao
					um.undid,			-- unidade de medida
					um.unddsc,			-- unidade de medida
					fe.frmid,			-- forma de execucao
					fe.frmdsc,			-- forma de execucao
					pr.prgdsc,			-- programa
					pi.plicod,			-- plano interno					
					pi.plidsc,			-- plano interno
					ps.psuid,			-- tipo parecer subacao
					ps.psudescricao,	-- tipo parecer subacao
					gp.gpsid,			-- grupo parecer subacao
					gp.gpsdescricao,	-- grupo parecer subacao
					spt.ssuid,			-- status subacao
					spt.sptparecer,	-- status subacao
					spt.sptano
			from cte.instrumentounidade iu
				inner join cte.pontuacao po on po.inuid = iu.inuid and po.ptostatus = 'A'
				inner join cte.indicador id on id.indid = po.indid and id.indstatus = 'A'
				inner join cte.areadimensao ad on ad.ardid = id.ardid and ad.ardstatus = 'A'
				inner join cte.dimensao di on di.dimid = ad.dimid and di.dimstatus = 'A'
				inner join cte.acaoindicador ai on ai.ptoid = po.ptoid
				inner join cte.subacaoindicador si on si.aciid = ai.aciid
				left join cte.unidademedida um on um.undid = si.undid
				left join cte.formaexecucao fe on fe.frmid = si.frmid
				left join cte.programa pr on pr.prgid = si.prgid
				left join financeiro.planointerno pi on pi.plicod = pr.prgplanointerno
				left join cte.parecersubacao ps on ps.psuid = si.psuid
				left join cte.subacaoparecertecnico spt on si.sbaid = spt.sbaid --and spt.sptano = date_part('year', current_date)
				left join cte.grupoparecersubacao gp on gp.gpsid = ps.gpsid
				left join cte.statussubacao ss on ss.ssuid = si.ssuid
				left join cte.formaatendimento fa on fa.foaid = si.foaid
			" . $condicaoInterno. " 
		union all
			select
				-- dados para exibicao
					si.sbaid,
					si.sbaporescola,
					iu.itrid,			-- instrumento
					iu.docid,			-- documento
					iu.muncod,			-- municipio
					di.dimcod,			-- dimensao
					di.dimid,			-- dimensao
					di.dimdsc,			-- dimensao
					ad.ardcod,			-- area
					ad.ardid,			-- area
					ad.arddsc,			-- area
					id.indcod,			-- indicador
					id.indid,			-- indicador
					id.inddsc,			-- indicador
					ai.acidsc,			-- acao
					ai.aciparecer,		-- parecer acao
					si.sbadsc,			-- subacao
					um.undid,			-- unidade de medida
					um.unddsc,			-- unidade de medida
					fe.frmid,			-- forma de execucao
					fe.frmdsc,			-- forma de execucao
					pr.prgdsc,			-- programa
					pi.plicod,			-- plano interno					
					pi.plidsc,			-- plano interno
					ps.psuid,			-- tipo parecer subacao
					ps.psudescricao,	-- tipo parecer subacao
					gp.gpsid,			-- grupo parecer subacao
					gp.gpsdescricao,	-- grupo parecer subacao
					spt.ssuid,			-- status subacao
					spt.sptparecer,	-- status subacao
					spt.sptano
				
			from cte.instrumentounidade iu
				inner join cte.pontuacao po on po.inuid = iu.inuid and po.ptostatus = 'C'
				inner join cte.indicador id on id.indid = po.indid and id.indstatus = 'A'
				inner join cte.areadimensao ad on ad.ardid = id.ardid and ad.ardstatus = 'A'
				inner join cte.dimensao di on di.dimid = ad.dimid and di.dimstatus = 'A'
				inner join cte.acaoindicador ai on ai.ptoid = po.ptoid
				inner join cte.subacaoindicador si on si.aciid = ai.aciid
				left join cte.unidademedida um on um.undid = si.undid
				left join cte.formaexecucao fe on fe.frmid = si.frmid
				left join cte.programa pr on pr.prgid = si.prgid
				left join financeiro.planointerno pi on pi.plicod = pr.prgplanointerno
				left join cte.subacaoparecertecnico spt on si.sbaid = spt.sbaid --and spt.sptano = date_part('year', current_date)
				left join cte.parecersubacao ps on ps.psuid = si.psuid
				left join cte.grupoparecersubacao gp on gp.gpsid = ps.gpsid
				left join cte.statussubacao ss on ss.ssuid = si.ssuid
				left join cte.formaatendimento fa on fa.foaid = si.foaid
			" . $condicaoInterno . "
	) as relatorio
		inner join workflow.documento on
			documento.docid = relatorio.docid
		inner join territorios.municipio on
			municipio.muncod = relatorio.muncod
		inner join territorios.estado on
			estado.estuf = municipio.estuf
		inner join territorios.regiao on
			regiao.regcod = estado.regcod
		inner join territorios.pais on
			pais.paiid = regiao.paiid
	" . $condicaoExterno . "
	group by
		sbaid,
		sbaporescola,
		documento.esdid,			-- estado documento
		municipio.muncod,			-- municipio
		municipio.mundescricao,		-- municipio
		estado.estuf,				-- estado
		estado.estdescricao,		-- estado
		regiao.regdescricao,		-- regiao
		pais.paidescricao,			-- pais
		relatorio.itrid,			-- instrumento
		relatorio.dimcod,			-- dimensao
		relatorio.dimdsc,			-- dimensao
		relatorio.ardcod,			-- area
		relatorio.arddsc,			-- area
		relatorio.indcod,			-- indicador
		relatorio.inddsc,			-- indicador
		relatorio.acidsc,			-- acao
		relatorio.aciparecer,		-- parecer acao
		relatorio.sbadsc,			-- subacao
		relatorio.prgdsc,			-- programa
		relatorio.plicod,			-- plano interno
		relatorio.plidsc,			-- plano interno
		relatorio.unddsc,			-- unidade de medida
		relatorio.frmdsc,			-- forma de execucao
		relatorio.psudescricao,		-- tipo parecer subacao
		relatorio.gpsdescricao,		-- grupo parecer subacao
		relatorio.sptparecer,		-- status subacao
		relatorio.sptano";


//dump( $sql, true);

// organiza dados para exibição
$dados = $db->carregar( $sql );
$dados = $dados ? $dados : array();

$totalLinhas = count($dados);
	for($x =0 ; $x < $totalLinhas; $x++ ){
		$subacao = $dados[$x]['sbaid'];
		$ano = $dados[$x]['ano'] ?  $dados[$x]['ano'] : 0;
		$porescola = $dados[$x]['sbaporescola'];
		$quantidade = recuperaQuantidades($subacao, $porescola, $ano);
		$valor = recuperaValor($subacao, $ano);
		
			$dados[$x]['fis_0_original'] = $quantidade[0]['fis_0_original'];
			$dados[$x]['fis_1_original'] = $quantidade[0]['fis_1_original'];
			$dados[$x]['fis_2_original'] = $quantidade[0]['fis_2_original'];
			$dados[$x]['fis_3_original'] = $quantidade[0]['fis_3_original'];
			$dados[$x]['fis_4_original'] = $quantidade[0]['fis_4_original'];
			
			$dados[$x]['fis_0_copia'] = $quantidade[0]['fis_0_copia'];
			$dados[$x]['fis_1_copia'] = $quantidade[0]['fis_1_copia'];
			$dados[$x]['fis_2_copia'] = $quantidade[0]['fis_2_copia'];
			$dados[$x]['fis_3_copia'] = $quantidade[0]['fis_3_copia'];
			$dados[$x]['fis_4_copia'] = $quantidade[0]['fis_4_copia'];
			
			$dados[$x]['fin_0_original'] = $valor[0]['fin_0_original']; // * $quantidade[0]['fis_0_original'] ;
			$dados[$x]['fin_1_original'] = $valor[0]['fin_1_original']; //* $quantidade[0]['fis_1_original'] ;
			$dados[$x]['fin_2_original'] = $valor[0]['fin_2_original']; //* $quantidade[0]['fis_2_original'] ;
			$dados[$x]['fin_3_original'] = $valor[0]['fin_3_original']; //* $quantidade[0]['fis_3_original'] ;
			$dados[$x]['fin_4_original'] = $valor[0]['fin_4_original']; //* $quantidade[0]['fis_4_original'] ;
			
			$dados[$x]['fin_0_copia'] = $valor[0]['fin_0_copia']; //* $quantidade[0]['fis_0_copia'] ;
			$dados[$x]['fin_1_copia'] = $valor[0]['fin_1_copia']; //* $quantidade[0]['fis_0_copia'] ;
			$dados[$x]['fin_2_copia'] = $valor[0]['fin_2_copia']; //* $quantidade[0]['fis_0_copia'] ;
			$dados[$x]['fin_3_copia'] = $valor[0]['fin_3_copia']; //* $quantidade[0]['fis_0_copia'] ;
			$dados[$x]['fin_4_copia'] = $valor[0]['fin_4_copia']; // * $quantidade[0]['fis_0_copia'] ;

	}
//exit;

//dbg($dados,1);
$agrupadores = array( 'estado', 'dimensao' );
$agrupador = (array) $_REQUEST['agrupador'];
//array_unshift( $agrupador, "pais" );
$dados = cte_agruparDadosRelatorio( $agrupador, $dados );




//dump( memory_get_peak_usage(), true );
/////////// Funcoes //////////////////////////////////////////////////

function recuperaQuantidades($sbaid, $cronograma, $ano){
	global $db;
	$condicao = "";
	if ( count( $_REQUEST['ssuid'] ) && $_REQUEST['ssuid'][0] ){
		$condicao = " and coalesce(spt.ssuid,0) in  ( '" . implode( "','", $_REQUEST['ssuid'] ) . "' )" ;
	}

	if($cronograma == 't'){
		$sqlQuantidade="SELECT
						coalesce(sum(ano2007_original),0) AS fis_0_original, -- QUANTIDADE 2007
						coalesce(sum(ano2008_original),0) AS fis_1_original, -- QUANTIDADE 2008
						coalesce(sum(ano2009_original),0) AS fis_2_original, -- QUANTIDADE 2009 
						coalesce(sum(ano2010_original),0) AS fis_3_original, -- QUANTIDADE 2010
						coalesce(sum(ano2011_original),0) AS fis_4_original,  -- QUANTIDADE 2011 
						
						coalesce(sum(ano2007_copia),0) AS fis_0_copia, -- QUANTIDADE 2007
						coalesce(sum(ano2008_copia),0) AS fis_1_copia, -- QUANTIDADE 2008
						coalesce(sum(ano2009_copia),0) AS fis_2_copia, -- QUANTIDADE 2009 
						coalesce(sum(ano2010_copia),0) AS fis_3_copia, -- QUANTIDADE 2010
						coalesce(sum(ano2011_copia),0) AS fis_4_copia  -- QUANTIDADE 2011 
						FROM (	
							SELECT 
							CASE WHEN qfaano = '2007'  THEN sum(qfaqtd) END AS ano2007_original, -- QUANTIDADE 2007
							CASE WHEN qfaano = '2008'  THEN sum(qfaqtd) END AS ano2008_original, -- QUANTIDADE 2008
							CASE WHEN qfaano = '2009'  THEN sum(qfaqtd) END AS ano2009_original, -- QUANTIDADE 2009 
							CASE WHEN qfaano = '2010'  THEN sum(qfaqtd) END AS ano2010_original, -- QUANTIDADE 2010
							CASE WHEN qfaano = '2011'  THEN sum(qfaqtd) END AS ano2011_original, -- QUANTIDADE 2011
						
							0 AS ano2007_copia, -- QUANTIDADE 2007
							0 AS ano2008_copia, -- QUANTIDADE 2008
							0 AS ano2009_copia, -- QUANTIDADE 2009 
							0 AS ano2010_copia, -- QUANTIDADE 2010
							0 AS ano2011_copia -- QUANTIDADE 2011
							
							FROM  cte.instrumentounidade iu
							inner join cte.pontuacao po on po.inuid = iu.inuid and po.ptostatus = 'A'
							inner join cte.indicador id on id.indid = po.indid and id.indstatus = 'A'
							inner join cte.acaoindicador ai on ai.ptoid = po.ptoid
							inner join cte.subacaoindicador si on si.aciid = ai.aciid
							left join cte.qtdfisicoano q on q.sbaid = si.sbaid
							left join cte.subacaoparecertecnico spt on spt.sbaid = si.sbaid and q.qfaano = spt.sptano
							WHERE q.sbaid = '".$sbaid."' and q.qfaano = ".$ano." ";
		
							if($condicao != ''){
								$sqlQuantidade = $sqlQuantidade . $condicao;		
							};
							
							$sqlQuantidade = $sqlQuantidade . " GROUP BY qfaano
						union all
							SELECT 
							0 AS ano2007_original, -- QUANTIDADE 2007
							0 AS ano2008_original, -- QUANTIDADE 2008
							0 AS ano2009_original, -- QUANTIDADE 2009 
							0 AS ano2010_original, -- QUANTIDADE 2010
							0 AS ano2011_original, -- QUANTIDADE 2011
							
							CASE WHEN qfaano = '2007'  THEN sum(qfaqtd) END AS ano2007_copia, -- QUANTIDADE 2007
							CASE WHEN qfaano = '2008'  THEN sum(qfaqtd) END AS ano2008_copia, -- QUANTIDADE 2008
							CASE WHEN qfaano = '2009'  THEN sum(qfaqtd) END AS ano2009_copia, -- QUANTIDADE 2009 
							CASE WHEN qfaano = '2010'  THEN sum(qfaqtd) END AS ano2010_copia, -- QUANTIDADE 2010
							CASE WHEN qfaano = '2011'  THEN sum(qfaqtd) END AS ano2011_copia -- QUANTIDADE 2011
							FROM  cte.instrumentounidade iu
							inner join cte.pontuacao po on po.inuid = iu.inuid and po.ptostatus = 'C'
							inner join cte.indicador id on id.indid = po.indid and id.indstatus = 'A'
							inner join cte.acaoindicador ai on ai.ptoid = po.ptoid
							inner join cte.subacaoindicador si on si.aciid = ai.aciid
							left join cte.qtdfisicoano q on q.sbaid = si.sbaid
							left join cte.subacaoparecertecnico spt on spt.sbaid = si.sbaid and q.qfaano = spt.sptano
							WHERE q.sbaid = '".$sbaid."' and q.qfaano = ".$ano." ";

							if($condicao != ''){
								$sqlQuantidade = $sqlQuantidade . $condicao;		
							};
							
							$sqlQuantidade = $sqlQuantidade . " GROUP BY qfaano
						) AS quantidade";
//		dbg($sqlQuantidade);
		$quantidade = $db->carregar($sqlQuantidade);
		return $quantidade;
	}else{
		$sqlQuantidade = "
				SELECT
						coalesce(sum(ano2007_original),0) AS fis_0_original, -- QUANTIDADE 2007
						coalesce(sum(ano2008_original),0) AS fis_1_original, -- QUANTIDADE 2008
						coalesce(sum(ano2009_original),0) AS fis_2_original, -- QUANTIDADE 2009 
						coalesce(sum(ano2010_original),0) AS fis_3_original, -- QUANTIDADE 2010
						coalesce(sum(ano2011_original),0) AS fis_4_original,  -- QUANTIDADE 2011 

						coalesce(sum(ano2007_copia),0) AS fis_0_copia, -- QUANTIDADE 2007
						coalesce(sum(ano2008_copia),0) AS fis_1_copia, -- QUANTIDADE 2008
						coalesce(sum(ano2009_copia),0) AS fis_2_copia, -- QUANTIDADE 2009 
						coalesce(sum(ano2010_copia),0) AS fis_3_copia, -- QUANTIDADE 2010
						coalesce(sum(ano2011_copia),0) AS fis_4_copia  -- QUANTIDADE 2011 
						FROM (
								SELECT  
								CASE WHEN sptano = '2007'  THEN sum(sptunt) END AS ano2007_original, 
								CASE WHEN sptano = '2008'  THEN sum(sptunt) END AS ano2008_original, 
								CASE WHEN sptano = '2009'  THEN sum(sptunt) END AS ano2009_original, 
								CASE WHEN sptano = '2010'  THEN sum(sptunt) END AS ano2010_original, 
								CASE WHEN sptano = '2011'  THEN sum(sptunt) END AS ano2011_original,
								
								0 AS ano2007_copia, 
								0 AS ano2008_copia, 
								0 AS ano2009_copia, 
								0 AS ano2010_copia, 
								0 AS ano2011_copia 
								 
								FROM cte.instrumentounidade iu
								inner join cte.pontuacao po on po.inuid = iu.inuid and po.ptostatus = 'A'
								inner join cte.indicador id on id.indid = po.indid and id.indstatus = 'A'
								inner join cte.acaoindicador ai on ai.ptoid = po.ptoid
								inner join cte.subacaoindicador si on si.aciid = ai.aciid
								inner  join cte.subacaoparecertecnico spt on spt.sbaid = si.sbaid 
								where spt.sbaid = '".$sbaid."' and spt.sptano = ".$ano." ";
		
								if($condicao != ''){
									$sqlQuantidade = $sqlQuantidade . $condicao;
								};
		
							$sqlQuantidade = $sqlQuantidade ." GROUP BY sptano
							union all
								SELECT  
								0 AS ano2007_original, 
								0 AS ano2008_original, 
								0 AS ano2009_original, 
								0 AS ano2010_original, 
								0 AS ano2011_original,

								CASE WHEN sptano = '2007'  THEN sum(sptunt) END AS ano2007_copia, 
								CASE WHEN sptano = '2008'  THEN sum(sptunt) END AS ano2008_copia, 
								CASE WHEN sptano = '2009'  THEN sum(sptunt) END AS ano2009_copia, 
								CASE WHEN sptano = '2010'  THEN sum(sptunt) END AS ano2010_copia, 
								CASE WHEN sptano = '2011'  THEN sum(sptunt) END AS ano2011_copia 
								 
								FROM cte.instrumentounidade iu
								inner join cte.pontuacao po on po.inuid = iu.inuid and po.ptostatus = 'C'
								inner join cte.indicador id on id.indid = po.indid and id.indstatus = 'A'
								inner join cte.acaoindicador ai on ai.ptoid = po.ptoid
								inner join cte.subacaoindicador si on si.aciid = ai.aciid
								inner  join cte.subacaoparecertecnico spt on spt.sbaid = si.sbaid 
								where spt.sbaid = '".$sbaid."' and spt.sptano = ".$ano." ";
							
								if($condicao != ''){
									$sqlQuantidade = $sqlQuantidade . $condicao;
								};
								$sqlQuantidade = $sqlQuantidade . "GROUP BY sptano	
						) AS quantidade
		";
//		dbg($sqlQuantidade);
		$quantidade = $db->carregar($sqlQuantidade);
		return $quantidade;	
	}	
}

function recuperaValor($sbaid, $ano){
	global $db;
		
		$sqlValor = "
		SELECT
			sum(ano2007_original) AS fin_0_original, 
			sum(ano2008_original) AS fin_1_original, 
			sum(ano2009_original) AS fin_2_original,
			sum(ano2010_original) AS fin_3_original, 
			sum(ano2011_original) AS fin_4_original,
			
			sum(ano2007_copia) AS fin_0_copia, 
			sum(ano2008_copia) AS fin_1_copia, 
			sum(ano2009_copia) AS fin_2_copia,
			sum(ano2010_copia) AS fin_3_copia, 
			sum(ano2011_copia) AS fin_4_copia   
			FROM (
				SELECT
				CASE WHEN cosano = '2007'  THEN cosqtd * cosvlruni END AS ano2007_original, -- VALOR 2007
				CASE WHEN cosano = '2008'  THEN cosqtd * cosvlruni END AS ano2008_original, -- VALOR 2008
				CASE WHEN cosano = '2009'  THEN cosqtd * cosvlruni END AS ano2009_original, -- VALOR 2009 
				CASE WHEN cosano = '2010'  THEN cosqtd * cosvlruni END AS ano2010_original, -- VALOR 2010
				CASE WHEN cosano = '2011'  THEN cosqtd * cosvlruni END AS ano2011_original, -- VALOR 2011
		
				0 AS ano2007_copia, -- VALOR 2007
				0 AS ano2008_copia, -- VALOR 2008
				0 AS ano2009_copia, -- VALOR 2009 
				0 AS ano2010_copia, -- VALOR 2010
				0 AS ano2011_copia -- VALOR 2011  
			
				FROM cte.instrumentounidade iu
				inner join cte.pontuacao po on po.inuid = iu.inuid and po.ptostatus = 'A'
				inner join cte.indicador id on id.indid = po.indid and id.indstatus = 'A'
				inner join cte.acaoindicador ai on ai.ptoid = po.ptoid
				inner join cte.subacaoindicador si on si.aciid = ai.aciid
				left  join cte.composicaosubacao csi on csi.sbaid = si.sbaid
				--left  join cte.subacaoparecertecnico spt on si.sbaid = spt.sbaid
				where csi.sbaid =   '".$sbaid."' and  csi.cosano = ".$ano."
		
			union all
				SELECT
				0 AS ano2007_original, -- VALOR 2007
				0 AS ano2008_original, -- VALOR 2008
				0 AS ano2009_original, -- VALOR 2009 
				0 AS ano2010_original, -- VALOR 2010
				0 AS ano2011_original, -- VALOR 2011 
		
				CASE WHEN cosano = '2007'  THEN cosqtd * cosvlruni END AS ano2007_copia, -- VALOR 2007
				CASE WHEN cosano = '2008'  THEN cosqtd * cosvlruni END AS ano2008_copia, -- VALOR 2008
				CASE WHEN cosano = '2009'  THEN cosqtd * cosvlruni END AS ano2009_copia, -- VALOR 2009 
				CASE WHEN cosano = '2010'  THEN cosqtd * cosvlruni END AS ano2010_copia, -- VALOR 2010
				CASE WHEN cosano = '2011'  THEN cosqtd * cosvlruni END AS ano2011_copia -- VALOR 2011 
		
				FROM cte.instrumentounidade iu
				inner join cte.pontuacao po on po.inuid = iu.inuid and po.ptostatus = 'C'
				inner join cte.indicador id on id.indid = po.indid and id.indstatus = 'A'
				inner join cte.acaoindicador ai on ai.ptoid = po.ptoid
				inner join cte.subacaoindicador si on si.aciid = ai.aciid
				left  join cte.composicaosubacao csi on csi.sbaid = si.sbaid
				--left  join cte.subacaoparecertecnico spt on si.sbaid = spt.sbaid
				where csi.sbaid =  '".$sbaid."' and  csi.cosano = ".$ano."
				) AS valores";
	
		$valor = $db->carregar($sqlValor);
		return $valor;
}
/////////// Fim Funcoes //////////////////////////////////////////////////
?>
<html>
	<head>
		<title>Relatório Geral CTE</title>
		<script type="text/javascript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<style type="text/css">
			body{ margin: 0; padding: 0; }
		</style>
		<script type="text/javascript">
			top.window.focus();
		</script>
	</head>
	<body>
		<?php if ( count( $dados ) ) : ?>
			<table class="tabela" style="width:100%; background-color: #fefefe;" id= "relatorioPar" border="0" cellpadding="2" cellspacing="1" align="center">
				<tr style="background-color: #d9d9d9;">
					<td width="250" colspan="2">&nbsp;</td>
					<td align="center" width="95">2007</td>
					<td align="center" width="95">2008</td>
					<td align="center" width="95">2009</td>
					<td align="center" width="95">2010</td>
					<td align="center" width="95">2011</td>
					<td align="center" width="105">TOTAL</td>
				</tr>
				<?php
					cte_desenhaRelatorio(
						$dados,
						(boolean) $_REQUEST['solicitado'], // solicitacao
						(boolean) $_REQUEST['atendido'], // atendimento
						(boolean) $_REQUEST['fisico'], // fisico
						(boolean) $_REQUEST['financeiro'] // financeiro
					);
				?>
			</table>
		<?php else : ?>
			<p align="center" style="color: #903030;">
				<br/><br/>
				Nenhum resultado encontrado para o filtro preenchido.
			</p>
		<?php endif; ?>
	</body>
</html>