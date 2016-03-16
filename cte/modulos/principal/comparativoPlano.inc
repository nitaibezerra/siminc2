<?php
include_once APPRAIZ.'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '' );
$itrid = cte_pegarItrid( $_SESSION['inuid'] );
$sequencia = 1;
//ARVORE Atual, ARVORE ATUAL COMO ELA ESTÁ (Pepois dos ajustes do municipio).
$dimensoes			= recuperaDimensao($itrid);
$areas				= recuperaArea($itrid);
$indicadores		= recuperaIndicador($itrid);
$acaoEstadual		= recuperaAcao($itrid,'Estado');
$acaoMunicipal		= recuperaAcao($itrid,'Municipio');
$acaoSoOriginalEst	= recuperaAcaoApenasDoOriginal($itrid,$sequencia, 'Estado');
$acaoSoOriginalMun	= recuperaAcaoApenasDoOriginal($itrid,$sequencia, 'Municipio');
$subacoesOriginal 	= recuperaSubacoesOriginal();
$subacoesSoOriginal = recuperaSubacoesSoNaOriginal($sequencia);

//ARVORE HISTÓRICO (Antes de voltar para o Município)
$indicadoresHistorico 		= recuperaIndicadorHistorico($itrid, $sequencia);
$acoesEstadualHistorico		= recuperaAcaoHistorico($itrid, $sequencia, 'Estado');
$acoesMunicipalHistorico	= recuperaAcaoHistorico($itrid, $sequencia, 'Municipio');
$acoesQueEstaoSoNoHis		= recuperaAcaoApenasDoHistorico ($itrid, $sequencia);
$subacoesHistorico			= recuperaSubacoesHistorico($sequencia);
$subQueEstaoSoNoHis			= recuperaSubacoesApenasHistorico($sequencia);

$acoesAtual 	= array();
$acoesHistorico = array();
$totalAcoes 	= array();

// TRATA AÇÕES
foreach( $acaoMunicipal as $acao ){
			$acoesAtual[] =$acao['aciid'];
}
foreach( $acoesMunicipalHistorico as $acao ){
			$acoesHistorico[] =$acao['aciidoriginal'];
}
// Atual - depois
foreach($acoesAtual as $acao){
	if(!in_array($acao,$acoesHistorico)){
		$cont = 0;
	 	foreach($acaoMunicipal as $a){
	 		if($a['aciid'] == $acao){
	 			$a['modificada'] = 1;
	 			$acaoMunicipal[$cont]['atual'] = 1;
	 			$acoesMunicipalHistorico[] = $a;
	 		}
	 		$cont++;
	 	}
	}
}


// Historico - antes
foreach($acoesHistorico as $acao){
	if(!in_array($acao,$acoesAtual)){
		foreach($acoesMunicipalHistorico as $a){
	 		if($a['aciid'] == $acao){
	 			$a['modificada'] = 1;
	 			$acaoMunicipal[] = $a;
	 		}
	 	}
	}
}
// FIM TRATA AÇÕES
// TRATA SUBAÇÕES
foreach( $subacoesOriginal as $subacao ){
			$subacoesAtual[] =$subacao['sbaid'];
}
foreach( $subacoesHistorico as $subacao ){
			$subaidHistorico[] =$subacao['sbaidoriginal'];
}
// Atual - depois

foreach($subacoesAtual as $sbaid){
	if(!in_array($sbaid,$subaidHistorico)){
		$cont = 0;
	 	foreach($subacoesOriginal as $a){
	 		if($a['sbaid'] == $sbaid){
	 			$a['modificada'] 	= 1;
	 			$subacoesOriginal[$cont]['atual'] = 1;
	 			$subacoesHistorico[] = $a;
	 		}
	 		$cont++;
	 	}
	}
}
//dbg($subacoesHistorico,1);
//exit;
/*
// Historico - antes
foreach($acoesHistorico as $acao){
	if(!in_array($acao,$acoesAtual)){
		foreach($acoesMunicipalHistorico as $a){
	 		if($a['aciid'] == $acao){
	 			$a['modificada'] = 1;
	 			//$a['atual'] = 1;
	 			$acaoMunicipal[] = $a;
	 		}
	 	}
	}
}
*/
// FIM TRATA SUBAÇÃOES

//dbg(count($subacoesHistorico),1);

//dbg($totalAcoes,1);
//exit;

// FUNÇÕES
function recuperaAcaoHistorico($itrid, $sequencia, $tipo){
	global $db;
	if($tipo == "Estado"){
		$acilocalizador = 'E';
	}else{
		$acilocalizador = 'M';
	}
	$sqlAcaoHistorico = "SELECT 
							aciidoriginal,ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid 
						FROM 
							cte.indicador i
						INNER JOIN cte.areadimensao a on a.ardid = i.ardid
						INNER JOIN cte.dimensao d on d.dimid = a.dimid
						INNER JOIN cte.pontuacaohistorico p on p.indid = i.indid and p.inuid = ".$_SESSION['inuid']." and p.ptostatus = 'A'
						INNER JOIN cte.acaoindicadorhistorico ai on ai.ptoid = p.ptoid and ai.acilocalizador = '".$acilocalizador."'
						WHERE
							i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = ".$itrid." AND acisequencial = ".$sequencia;
	$acoesHistorico = $db->carregar( $sqlAcaoHistorico );
	if ( !$acoesHistorico ) {
		$acoesHistorico = array();
	}
	return $acoesHistorico;
}

function recuperaAcaoApenasDoHistorico ($itrid, $sequencia){
	global $db;
	$sqlSoNoAcaoHistorico = "SELECT 
								aciidoriginal,ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid 
							FROM 
								cte.indicador i
							INNER JOIN cte.areadimensao a on a.ardid = i.ardid
							INNER JOIN cte.dimensao d on d.dimid = a.dimid
							INNER JOIN cte.pontuacaohistorico p on p.indid = i.indid and p.inuid = ".$_SESSION['inuid']." and p.ptostatus = 'A'
							INNER JOIN cte.acaoindicadorhistorico ai on ai.ptoid = p.ptoid and ai.acilocalizador = 'M'
							WHERE
								i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = ".$itrid." AND acisequencial = ".$sequencia.
							" and aciidoriginal not in (
														SELECT 
															ai.aciid
														FROM 
															cte.indicador i
														INNER JOIN cte.areadimensao a on a.ardid = i.ardid
														INNER JOIN cte.dimensao d on d.dimid = a.dimid
														INNER JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = ".$_SESSION['inuid']." and p.ptostatus = 'A'
														INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid and ai.acilocalizador = 'M'
														WHERE
															i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = ".$itrid.")";

	$acoesApenasNoHistorico = $db->carregar( $sqlSoNoAcaoHistorico );
	if ( !$acoesApenasNoHistorico ) {
		$acoesApenasNoHistorico = array();
	}
	
	return $acoesApenasNoHistorico;
}
		
function recuperaIndicadorHistorico($itrid, $sequencia){
	global $db;
	$sql_indicador ="	SELECT
							i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, length(p.ptodemandaestadual) as demandaestadual, 
							length(p.ptodemandamunicipal) as demandamunicipal, 
							c.ctrpontuacao, aie.aciid as acaoestadual, 
							aim.aciid as acaomunicipal, saie.aciid as subacaoestadual, 
							saim.aciid as subacaomunicipal, 
							usu.usunome, to_char( p.ptodata,'dd/mm/YYYY') as data
						FROM cte.indicador i 
						INNER JOIN cte.areadimensao a on a.ardid = i.ardid 
						INNER JOIN cte.dimensao d on d.dimid = a.dimid 
						LEFT  JOIN cte.pontuacaohistorico p on p.indid = i.indid and p.inuid = ".$_SESSION['inuid']." and p.ptostatus = 'A' 
						LEFT  JOIN cte.criterio c on c.crtid = p.crtid
						LEFT  JOIN cte.acaoindicadorhistorico aie on aie.ptoid = p.ptoid and aie.acilocalizador = 'E' 
						LEFT  JOIN cte.acaoindicadorhistorico aim on aim.ptoid = p.ptoid and aim.acilocalizador = 'M' 
						LEFT  JOIN cte.subacaoindicadorhistorico saie on saie.aciid = aie.aciid 
						LEFT  JOIN cte.subacaoindicadorhistorico saim on saim.aciid = aim.aciid 
						LEFT JOIN seguranca.usuario AS usu ON (p.usucpf = usu.usucpf)
						WHERE
							i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = ".$itrid." AND ptosequencial = $sequencia
						GROUP BY i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, p.ptodemandaestadual, 
							 p.ptodemandamunicipal, c.ctrpontuacao, saie.aciid, saim.aciid, aie.aciid, aim.aciid, usu.usunome, p.ptodata
						ORDER BY
							i.indcod";
		
	$indicadoresHistorico = $db->carregar( $sql_indicador );
	if ( !$indicadoresHistorico ){
		$indicadoresHistorico = array();
	}

	return $indicadoresHistorico;		
}

function recuperaSubacoesHistorico($sequencia){
	global $db;
    $sql = '
    SELECT DISTINCT sa.sbaidoriginal, sa.sbaid, sa.sbadsc, sa.aciid, sa.sbaordem, sa.sbaporescola, sa.frmid,
        usu.usunome, to_char(sa.sbadata,\'dd/mm/YYYY\') as data, spt.ssuid, 
        ss.ssudescricao, fat.foadsc as fatendimento, fex.frmdsc as frmexecucao, p.ptostatus
    FROM cte.subacaoindicadorhistorico sa
	    INNER JOIN cte.acaoindicadorhistorico ai on ai.aciid = sa.aciid 
	    INNER JOIN cte.pontuacaohistorico p on p.ptoid = ai.ptoid and p.inuid = ' . $_SESSION['inuid'] . ' and p.ptostatus = \'A\'
	    LEFT  JOIN cte.subacaoparecertecnicohistorico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)
	    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
	    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
	    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
	    LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
    WHERE sbaidpai IS NULL and sbasequencial = '.$sequencia.'
    AND coalesce( sbasituacaoarvore, 1 ) in ( 1, 3, 5 )
    ORDER BY sa.sbaordem, sa.sbadsc';

    $subacoesHistorico = (array) $db->carregar($sql);
    return $subacoesHistorico;
}

function recuperaSubacoesApenasHistorico($sequencia){
	global $db;
	$sql = '
    SELECT DISTINCT sa.sbaid, sa.sbadsc, sa.aciid, sa.sbaordem, sa.sbaporescola, sa.frmid, ai.aciidoriginal,
        usu.usunome, to_char(sa.sbadata,\'dd/mm/YYYY\') as data, spt.ssuid, 
        ss.ssudescricao, fat.foadsc as fatendimento, fex.frmdsc as frmexecucao, p.ptostatus
    FROM cte.subacaoindicadorhistorico sa
	    INNER JOIN cte.acaoindicadorhistorico ai on ai.aciid = sa.aciid 
	    INNER JOIN cte.pontuacaohistorico p on p.ptoid = ai.ptoid and p.inuid = ' . $_SESSION['inuid'] . ' and p.ptostatus = \'A\'
	    LEFT  JOIN cte.subacaoparecertecnicohistorico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)
	    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
	    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
	    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
	    LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
    WHERE sbaidpai IS NULL and sbasequencial = '.$sequencia.' 
    and sbaidoriginal not in (SELECT DISTINCT sa.sbaid
			    FROM cte.subacaoindicador sa
				    INNER JOIN cte.acaoindicador ai on ai.aciid = sa.aciid 
				    INNER JOIN cte.pontuacao p on p.ptoid = ai.ptoid and p.inuid = ' . $_SESSION['inuid'] . ' and p.ptostatus = \'A\'
				    LEFT  JOIN cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)
				    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
				    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
				    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
				    LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
			    WHERE sbaidpai IS NULL
			    AND coalesce( sbasituacaoarvore, 1 ) in ( 1, 3, 5 )
			    )
    AND coalesce( sbasituacaoarvore, 1 ) in ( 1, 3, 5 )
    ORDER BY sa.sbaordem, sa.sbadsc';

    $subacoesSoNaHistorico = (array) $db->carregar($sql);
    return $subacoesSoNaHistorico;
}

function recuperaArea($itrid){
	global $db;
	$sql_area = sprintf(
		"select a.ardid, a.ardcod, a.arddsc, a.dimid
		from cte.areadimensao a
		inner join cte.dimensao d on d.dimid = a.dimid
		where a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d
		order by a.ardcod",
		$itrid
	);
	$areas = $db->carregar( $sql_area );
	if ( !$areas ){
		$areas = array();
	}
	return $areas;
}

function recuperaIndicador($itrid){
	global $db;
	$sql_indicador = sprintf("
		SELECT
			i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, length(p.ptodemandaestadual) as demandaestadual, 
			length(p.ptodemandamunicipal) as demandamunicipal, 
			c.ctrpontuacao, aie.aciid as acaoestadual, 
			aim.aciid as acaomunicipal, saie.aciid as subacaoestadual, 
			saim.aciid as subacaomunicipal, 
			usu.usunome, to_char( p.ptodata,'dd/mm/YYYY') as data
		FROM cte.indicador i 
		INNER JOIN cte.areadimensao a on a.ardid = i.ardid 
		INNER JOIN cte.dimensao d on d.dimid = a.dimid 
		LEFT  JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = %d and p.ptostatus = 'A' 
		LEFT  JOIN cte.criterio c on c.crtid = p.crtid
		LEFT  JOIN cte.acaoindicador aie on aie.ptoid = p.ptoid and aie.acilocalizador = 'E' 
		LEFT  JOIN cte.acaoindicador aim on aim.ptoid = p.ptoid and aim.acilocalizador = 'M' 
		LEFT  JOIN cte.subacaoindicador saie on saie.aciid = aie.aciid 
		LEFT  JOIN cte.subacaoindicador saim on saim.aciid = aim.aciid 
		LEFT JOIN seguranca.usuario AS usu ON (p.usucpf = usu.usucpf)
		WHERE
			i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = %d 
		GROUP BY i.indid, i.inddsc, i.ardid, i.indcod, p.ptoid, p.ptodemandaestadual, 
			 p.ptodemandamunicipal, c.ctrpontuacao, saie.aciid, saim.aciid, aie.aciid, aim.aciid, usu.usunome, p.ptodata
		ORDER BY
			i.indcod",
		$_SESSION['inuid'],
		$itrid
	);
	
	$indicadores = $db->carregar( $sql_indicador );
	if ( !$indicadores ){
		$indicadores = array();
	}
	return $indicadores;
}

function recuperaDimensao($itrid){
	global $db;
	$sql_dimensao = sprintf(
		"select d.dimid, d.dimcod, d.dimdsc
		from cte.dimensao d
		where d.dimstatus = 'A' and d.itrid = %d
		order by d.dimcod",
		$itrid
	);
	$dimensoes = $db->carregar( $sql_dimensao );
	
	if ( !$dimensoes ){
		$dimensoes = array();
	}
	
	return $dimensoes;
}

function recuperaAcao($itrid, $tipo){
	global $db;
	if($tipo == "Estado"){
		$acilocalizador = 'E';
	}else{
		$acilocalizador = 'M';
	}
	$sql_acao_municipal = 
		"SELECT 
			ai.aciid as aciidoriginal, ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid
		FROM 
			cte.indicador i
		INNER JOIN cte.areadimensao a on a.ardid = i.ardid
		INNER JOIN cte.dimensao d on d.dimid = a.dimid
		INNER JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = ".$_SESSION['inuid']." and p.ptostatus = 'A'
		INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid and ai.acilocalizador = '".$acilocalizador."'
		WHERE
			i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = ".$itrid;

	$acoesOriginal = $db->carregar( $sql_acao_municipal );
	if ( !$acoesOriginal ) {
		$acoesOriginal = array();
	}
	return $acoesOriginal;
}

function recuperaAcaoApenasDoOriginal($itrid, $sequencia, $tipo){
	global $db;
	if($tipo == "Estado"){
		$acilocalizador = 'E';
	}else{
		$acilocalizador = 'M';
	}
	
	$sql_acao_municipal = 
		"SELECT 
			ai.aciid as aciidoriginal, ai.aciid, ai.ptoid, ai.acidsc, ai.acilocalizador, i.indid
		FROM 
			cte.indicador i
		INNER JOIN cte.areadimensao a on a.ardid = i.ardid
		INNER JOIN cte.dimensao d on d.dimid = a.dimid
		INNER JOIN cte.pontuacao p on p.indid = i.indid and p.inuid = ".$_SESSION['inuid']." and p.ptostatus = 'A'
		INNER JOIN cte.acaoindicador ai on ai.ptoid = p.ptoid and ai.acilocalizador = '".$acilocalizador."'
		WHERE
			i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = ".$itrid." 
			and ai.aciid not in(SELECT 
							aciidoriginal
						FROM 
							cte.indicador i
						INNER JOIN cte.areadimensao a on a.ardid = i.ardid
						INNER JOIN cte.dimensao d on d.dimid = a.dimid
						INNER JOIN cte.pontuacaohistorico p on p.indid = i.indid and p.inuid = ".$_SESSION['inuid']." and p.ptostatus = 'A'
						INNER JOIN cte.acaoindicadorhistorico ai on ai.ptoid = p.ptoid and ai.acilocalizador = '".$acilocalizador."'
						WHERE
							i.indstatus = 'A' and a.ardstatus = 'A' and d.dimstatus = 'A' and d.itrid = ".$itrid." AND acisequencial = ".$sequencia.") ";
	
	$acoesApenasNaOriginal = $db->carregar( $sql_acao_municipal );
	if ( !$acoesApenasNaOriginal ) {
		$acoesApenasNaOriginal = array();
	}
	return $acoesApenasNaOriginal;
}

function recuperaSubacoesOriginal(){
	global $db;
	$sql = '
    SELECT DISTINCT sa.sbaid as sbaidoriginal, sa.sbaid, sa.sbadsc, sa.aciid, sa.sbaordem, sa.sbaporescola, sa.frmid,
        usu.usunome, to_char(sa.sbadata,\'dd/mm/YYYY\') as data, spt.ssuid, 
        ss.ssudescricao, fat.foadsc as fatendimento, fex.frmdsc as frmexecucao, p.ptostatus
    FROM cte.subacaoindicador sa
	    INNER JOIN cte.acaoindicador ai on ai.aciid = sa.aciid 
	    INNER JOIN cte.pontuacao p on p.ptoid = ai.ptoid and p.inuid = ' . $_SESSION['inuid'] . ' and p.ptostatus = \'A\'
	    LEFT  JOIN cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)
	    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
	    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
	    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
	    LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
    WHERE sbaidpai IS NULL
    AND coalesce( sbasituacaoarvore, 1 ) in ( 1, 3, 5 )
    ORDER BY sa.sbaordem, sa.sbadsc';

    $subacoes = (array) $db->carregar($sql);
    return $subacoes;
}

function recuperaSubacoesSoNaOriginal($sequencia){
	global $db;
	$sql = '
    SELECT DISTINCT sa.sbaid, sa.sbadsc, sa.aciid, sa.sbaordem, sa.sbaporescola, sa.frmid,
        usu.usunome, to_char(sa.sbadata,\'dd/mm/YYYY\') as data, spt.ssuid, 
        ss.ssudescricao, fat.foadsc as fatendimento, fex.frmdsc as frmexecucao, p.ptostatus
    FROM cte.subacaoindicador sa
	    INNER JOIN cte.acaoindicador ai on ai.aciid = sa.aciid 
	    INNER JOIN cte.pontuacao p on p.ptoid = ai.ptoid and p.inuid = ' . $_SESSION['inuid'] . ' and p.ptostatus = \'A\'
	    LEFT  JOIN cte.subacaoparecertecnico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)
	    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
	    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
	    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
	    LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
    WHERE sbaidpai IS NULL
    AND coalesce( sbasituacaoarvore, 1 ) in ( 1, 3, 5 )
    and sa.sbaid not in (SELECT DISTINCT sa.sbaidoriginal
			    FROM cte.subacaoindicadorhistorico sa
				    INNER JOIN cte.acaoindicadorhistorico ai on ai.aciid = sa.aciid 
				    INNER JOIN cte.pontuacaohistorico p on p.ptoid = ai.ptoid and p.inuid = ' . $_SESSION['inuid'] . ' and p.ptostatus = \'A\'
				    LEFT  JOIN cte.subacaoparecertecnicohistorico spt on sa.sbaid = spt.sbaid AND sptano = date_part(\'year\', current_date)
				    LEFT  JOIN cte.statussubacao ss on ss.ssuid = spt.ssuid
				    LEFT  JOIN cte.formaatendimento fat on fat.foaid = sa.foaid
				    LEFT  JOIN cte.formaexecucao fex on fex.frmid = sa.frmid
				    LEFT  JOIN seguranca.usuario AS usu ON (sa.usucpf = usu.usucpf)
			    WHERE sbaidpai IS NULL and sbasequencial = '.$sequencia.'
			    AND coalesce( sbasituacaoarvore, 1 ) in ( 1, 3, 5 )
			    )
    ORDER BY sa.sbaordem, sa.sbadsc';

    $subacoesSoNaOriginal = (array) $db->carregar($sql);
    return $subacoesSoNaOriginal;
}

function delimitador($texto){
	if(strlen($texto) > 70){
		$texto = substr($texto,0,70).'...';
	}
	return $texto;
}

function arvoreAtual(){
	global $db, $dimensoes, $areas, $indicadores, $acaoEstadual, $acaoMunicipal, $subacoesOriginal;

	$script	="
		<script type=\"text/javascript\">
		var u='/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A&titleFor=';
		arvore 						= new dTree( \"arvore\" );
		arvore.config.folderLinks 	= true;
		arvore.config.useIcons 		= true;
		arvore.config.useCookies 	= true; 
		arvore.add( 0, -1, 'instrumento - Arvore 1' );";
		// CARREGA DIMENSÃO
		foreach( $dimensoes as $dimensao ){
			$texto = '<span style="color: #000000;">'. $dimensao['dimcod'] .'. '. $dimensao['dimdsc'] .'</span>'; 
			$script .= " arvore.add( 'd_".$dimensao['dimid']."', 0, '".$texto."' );";
		}
		// CARREGA ÁREA
		foreach( $areas as $area ){
			$texto 	 = '<span style="color: #000000;">'. $area['ardcod'] .'. '. delimitador($area['arddsc']) .'</span>';
			$script .= " arvore.add( 'a_".$area['ardid']."', 'd_".$area['dimid']."', '".$texto."' );";
		}
		// CARREGA INDICADORES
		foreach( $indicadores as $indicador ){
			$texto = '<span style="color: '. $cor .';" title="teste">'. $indicador['indcod'] .'. '. delimitador($indicador['inddsc']) .'</span>';
			$icone = '../imagens/check_p.gif';
			$script .= " arvore.add( 'i_".$indicador['indid']."', 'a_".$indicador['ardid']."', '".$texto."', 'javascript: avaliar(".$indicador['indid'].");', '', '', '".$icone."', '".$icone."' );";
			$script .= " arvore.add( 'ipe_".$indicador['indid']."', 'i_".$indicador['indid']."', 'Plano de Ação da Rede Estadual', '', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );";
		}
		//CARREGA AÇÕES historico
		foreach( $acaoMunicipal as $chave=>$acao ){
			if($acao['atual']){
				$cor1 = '#3784A8;';
			}else{
				$cor1 = '';
				$textoN = "";
			}
			if($acao['modificada']){
				$cor2 = 'red;';
			}else{
				$cor2 = '';
			}
			if($cor2 != '' ){
				$cor = $cor2;
			}else if($cor1 != '' ){
				$cor = $cor1;
				$textoN = "(Ação Inserida pelo Nunicípio)";
			}else{
				$cor = '';
				$textoN = '';
			}
			
			$texto = '<a style="color: '. $cor .';" href="javascript:alterarAcao('. $acao['aciid'] .');"  title="'. $title .'">'. str_replace( "'", "", str_replace( array( "\n", "\r", chr(10), chr(13) ), " ", trim( delimitador($acao['acidsc']) ) ) ) .'</a>';
			$script .= " arvore.add( 'ai_".$acao['aciid']."', 'ipe_".$acao['indid']."', '".$texto.$textoN."', 'javascript:void(0);', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );";
			$script .= " arvore.add( 'ais_".$acao['aciid']."', 'ai_".$acao['aciid']."', '<img align=\"absmiddle\" src=\"/imagens/gif_inclui.gif\" title=\"cadastrar subação\"/> Subações (".$acao['subacoes'].")', 'javascript:cadastrarSubacao(".$acao['aciid'].");', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );";
		}
		//CARREGA SUBAÇÃOES
		//dbg($subacoesOriginal,1);
		$cor1 = '';
		$cor2 = '';
		$textoN = '';
		foreach( $subacoesOriginal as $subacao ){
			if($subacao['atual']){
				$cor1 = '#3784A8;';
			}else{
				$cor1 = '';
				$textoN = "";
			}
			if($subacao['modificada']){
				$cor2 = 'red;';
			}else{
				$cor2 = '';
			}
			if($cor2 != '' ){
				$cor = $cor2;
			}else if($cor1 != '' ){
				$cor = $cor1;
				$textoN = "(Subação Inserida pelo Nunicípio)";
			}else{
				$cor = '';
				$textoN = '';
			}
			$texto = '<a style="color:'.$cor.'" onmouseover="SuperTitleAjax(u+' . $subacao['sbaid'] . ',this)" onmouseout="SuperTitleOff(this);" href="javascript:alterarSubacao('. $subacao['sbaid'] .');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",delimitador($subacao['sbadsc'])).'</a>';
			$script .= " arvore.add( 'sa_".$subacao['sbaid']."', 'ais_".$subacao['aciid']."', '".$prefixo." ".$texto.$textoN." ', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );";
		}
		$script .=" elemento = document.getElementById( '_arvore' );
					elemento.innerHTML = arvore;
				</script>";
		echo $script;
}

function arvoreHistorico(){
	global $db, $dimensoes, $areas, $indicadoresHistorico, $acoesEstadualHistorico, $acoesMunicipalHistorico, $subacoesHistorico;

	$script	="
		<script type=\"text/javascript\">
		var u='/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A&titleFor=';
		arvoreH 						= new dTree( \"arvoreH\" );
		arvoreH.config.folderLinks 	= true;
		arvoreH.config.useIcons 	= true;
		arvoreH.config.useCookies 	= true; 
		arvoreH.add( 0, -1, 'instrumento - Arvore 2' );";
		// CARREGA DIMENSÃO
		foreach( $dimensoes as $dimensao ){
			$texto = '<span style="color: #000000;">'. $dimensao['dimcod'] .'. '. $dimensao['dimdsc'] .'</span>'; 
			$script .= " arvoreH.add( 'di_".$dimensao['dimid']."', 0, '".$texto."','','','','','' );";
		}
		// CARREGA ÁREA
		foreach( $areas as $area ){
			$texto 	 = '<span style="color: #000000;">'. $area['ardcod'] .'. '. delimitador($area['arddsc']) .'</span>';
			$script .= " arvoreH.add( 'ar_".$area['ardid']."', 'di_".$area['dimid']."', '".$texto."' );";
		}
		// CARREGA INDICADORES
		foreach( $indicadoresHistorico as $indicador ){
			$texto = '<span style="color: '. $cor .';" title="teste">'. $indicador['indcod'] .'. '. delimitador($indicador['inddsc']) .'</span>';
			$icone = '../imagens/check_p.gif';
			$script .= " arvoreH.add( 'in_".$indicador['indid']."', 'ar_".$indicador['ardid']."', '".$texto."', 'javascript: avaliar(".$indicador['indid'].");', '', '', '".$icone."', '".$icone."' );";
			$script .= " arvoreH.add( 'ipea_".$indicador['indid']."', 'in_".$indicador['indid']."', 'Plano de Ação da Rede Estadual', '', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );";
		}
		//CARREGA AÇÕES
		//dbg($acoesMunicipalHistorico,1);
		foreach( $acoesMunicipalHistorico as $acao ){
			if($acao['atual']){
				$cor = 'blue;';
			}else{
				$cor = '';
			}
			if($acao['modificada']){
				$cor = 'red;';
			}else{
				$cor = '';
			}
			$texto = '<a style="color:'.$cor.'" href="javascript:alterarAcao('. $acao['aciid'] .');"  title="'. $title .'">'. str_replace( "'", "", str_replace( array( "\n", "\r", chr(10), chr(13) ), " ", trim( delimitador($acao['acidsc']) ) ) ) .'</a>';
			$script .= " arvoreH.add( 'aic_".$acao['aciid']."', 'ipea_".$acao['indid']."', '".$texto."', 'javascript:void(0);', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );";
			$script .= " arvoreH.add( 'aisi_".$acao['aciid']."', 'aic_".$acao['aciid']."', '<img align=\"absmiddle\" src=\"/imagens/gif_inclui.gif\" title=\"cadastrar subação\"/> Subações (".$acao['subacoes'].")', 'javascript:cadastrarSubacao(".$acao['aciid'].");', '', '', '../includes/dtree/img/folder.gif', '../includes/dtree/img/folderopen.gif' );";
		}
		//CARREGA SUBAÇÃOES
		$cor = '';
		foreach( $subacoesHistorico as $subacao ){
			if($subacao['atual']){
				$cor1 = '#3784A8;';
			}else{
				$cor1 = '';
			}
			if($subacao['modificada']){
				$cor2 = 'red;';
			}else{
				$cor2 = '';
			}
			if($cor2 != '' ){
				$cor = $cor2;
			}else if($cor1 != '' ){
				$cor = $cor1;
			}else{
				$cor = '';
			}
			$texto = '<a style="color:'.$cor.'" onmouseover="SuperTitleAjax(u+' . $subacao['sbaid'] . ',this)" onmouseout="SuperTitleOff(this);" href="javascript:alterarSubacao('. $subacao['sbaid'] .');">'. str_replace( array( "\n", "\r", chr(10), chr(13),"'",";"), " ",delimitador($subacao['sbadsc'])).'</a>';
			$script .= " arvoreH.add( 'sab_".$subacao['sbaid']."', 'aisi_".$subacao['aciid']."', '".$prefixo." ".$texto." ', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif' );";
		}
		$script .=" elemento = document.getElementById( '_arvore2' );
					elemento.innerHTML = arvoreH;
				</script>";
		echo $script;
}	
?>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script type="text/javascript" src="/includes/JQuery/interface.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.3.2.min.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-ui-1.7.2.custom.min.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery.cookie.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery.treeview.js"></script>
<link rel="stylesheet" href="/includes/JQuery/jquery.treeview.css" />

<style type="text/css">
	#lixeira { 
		width: 41px;
		height: 41px;
		float: right;
	} 
	li{
		z-index: 1;
	}

	.conteudoLixeira{
		bottom:30px;
		position: fixed;
		left:300px;
		border: 2px solid #5C5D5E; 
		width: 800px; 
		height: 500px;
		background-color: white;
	}
	
	#divFecharLixeira{
		text-align:right;
	}
	
	#titulo{
	text-transform: uppercase;
	font-family: serif;
	font-size: 14px;
	font-weight: bold;
	background-color: #DCDCDC;
	}
	
	#btnDeletar{
		width: 12px; 
		height: 12px; 
		border: none;
	}
	
	.DisabilitaBtnDeletar{
		display: none;
	}
	
	.restaurar{
		width: 10px; 
		height: 12px; 
		border: none;
	}
	
	.teste{
		color:green;
	}
	
	.pontilhado{
		width: 14px; 
		height: 10px; 
		border: none; 
		margin-right: 2px;
	}
</style>
<script type="text/javascript">

	
	$(function() {
		// MOVER DE UMA LIXEIRA PARA OUTRA
			$('span.pasta').droppable({
				tollerance		: 'pointer',
				activeclass		: 'teste',
				drop: function(ev, ui) {
					var local = $('ul', this.parentNode.parentNode);
					if (local.size() == 0) {
						$(this).after('<ul></ul>');
						local = $('ul', this.parentNode.parentNode);
					}

					aceitaItem(ui.draggable, local[0]);
				}
			});
			
		function aceitaItem($item, local){
			$item.fadeOut(function() {
				$item.find('li').end().appendTo(local).fadeIn();
				//subbranch.eq(0).append(dropped);
			});
		}

	
		// CRIAÇÃO DE OBJETOS (ARVORES - LIXEIRA - ÁREA DA LIXEIRA)

		var $arvore1  		= $('#arvore1'), 
			$arvore2  		= $('#arvore2'), // Duplicar
			$arvore3  		= $('#arvore3'), // Duplicar
			$lixeira    	= $('#lixeira'), 
			$arealixeira 	= $('#arealixeira') ;

		// DEIXA OS ITENS DA ARVORE ARRASTAVEIS
		//$('li',$arvore1 | $arvore2 | $arvore3).draggable({ 					// duplicar conteudo com | 
		$('li').draggable({ 	
			cancel: 'a.podeMover',									// Se clicar no item ele não se move so se segurar o mouse
			revert: 'invalid', 										// Quando o item não for para o lugar certo ele volta para posição inicial
			containment: $('#area').length ? '#area' : 'document', 	// Se tiver dentro deste frame o objeto se mexe so nele. opcional
			helper: 'clone',
			cursor: 'move'
		});

		// DEIXA A LIXEIRA ACEITAR OS ITENS DA ARVORE
		$lixeira.droppable({
			accept: '#arvore1 > li, #arvore2 > li, #arvore3 > li', // Duplicar
			//activeClass: 'ui-state-highlight',
			drop: function(ev, ui) {
				deletarItens(ui.draggable);
			}
		});

		// FUNÇÃO DE DELETAR OS ITENS DA ARVORE
		var recycle_icon = '<img title="Restaurar item" class="podeMover restaurar"  src="../../imagens/restaurar.gif" />';
		function deletarItens($item) {
			$item.fadeOut(function() {
				var $list = $('ul',$arealixeira).length ? $('ul',$arealixeira) : $('<ul class="arvore"/>').appendTo($arealixeira);
				//$item.find('img.deletar').remove();
				$item.append(recycle_icon).appendTo($list).fadeIn(function() {
					$item.animate({ width: '300px' }).find('li').animate({ height: '36px' });
					document.getElementById('iconeLixeira').src="/imagens/trashfull.jpg";
				});
			});
			$item.find('img.deletar').css("display", "none");
		}

		// FUNCTION PARA RETORNAR O ITEM PARA ARVORE (BTN DA LIXEIRA)
		//var lixeira_icon = '<img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" />';
		function restaurarItem($item, localItem) {
			if(localItem == 1){
				local = $arvore1;
			}else if(localItem == 2){
				local = $arvore2;
			}else if(localItem == 3){
				local = $arvore3;
			}
			
			$item.fadeOut(function() {
				$item.find('img.restaurar').remove();
				$item.css('width','50%').find('li').end().appendTo(local).fadeIn();
				//$item.css('width','50%').append(lixeira_icon).find('li').end().appendTo(local).fadeIn();
			});
			$item.find('img.deletar').css("display", "");
			var totalNalixeira = $('li',$arealixeira).length;
			if(totalNalixeira == 1){
				document.getElementById('iconeLixeira').src="/imagens/trash.jpg";
			}
		}

		// DELEGAÇÃO DE EVENTOS PARA OS BOTÕES DE DELETAR E RESTAURAR
		$('ul > li').click(function(ev) {
			document.getElementById('lixeira').style.display = '';
			var local = $(this).attr("value");
			var $item = $(this);
			var $target = $(ev.target);
			if ($target.is('img.deletar')) {
				deletarItens($item);
			} else if ($target.is('img.restaurar')) {
				restaurarItem($item, local);
			} 
			return false;
		});
	});

	// VISUALIZAR A LIXEIRA
	function visualizarLixeira(){
		document.getElementById('arealixeira').style.display = '';
	}
	
	// FECHAR A LIXEIRA
	function fecharLixeira(){
		document.getElementById('arealixeira').style.display = 'none';
	}

	// FUNCTION PARA CRIAR A ARVORE
	$(function() {
		$("#tree").treeview({
			collapsed: true,
			animated: "medium",
			control:"#sidetreecontrol",
			persist: "location"
		});
	})
	
	// FUNCTION PARA CRIAR A ARVORE 2
	$(function() {
		$("#tree2").treeview({
			collapsed: true,
			animated: "medium",
			control:"#sidetreecontrol2",
			persist: "location"
		});
	})


	

</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="background-color: white;" >
	<tr>
		<td>
			<div>
				<div id="sidetreecontrol"><a href="?#">Fechar Todos </a> | <a href="?#">Abrir Todos</a></div>
				<ul id="tree">
					<li ><a href="a"><span class="pasta"><img class="pontilhado"  src="../../imagens/pontilhado.jpg" />Indicador 1 </span></a>
						<ul id="arvore1">
							<li  value="1"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="z">Ação 1 </a></li>
							<li  value="1"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="z">Ação 2</a></li>
							<li  value="1"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="z">Ação 3</a>
								<ul id="arvore2" class="arvorec">
									<li  value="2"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="z">Subação 1</a></li>
									<li  value="2"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="z">Subação 2</a></li>
								</ul>
							</li>
						</ul>
					</li>
					<li id="linhas" ><a href="a"><span class="pasta"><img class="pontilhado"  src="../../imagens/pontilhado.jpg" />Indicador 2</span></a>
						<ul id="arvore3">
							<li value="3"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="c">Ação 1 </a> </li>
							<li value="3"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="b">Ação 2 </a> </li>
							<li value="3"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="A">Ação 3</a> </li>
						</ul>
					</li>
				</ul>
			</div>
		</td>
		<td>
			<div>
				<div id="sidetreecontrol2"><a href="?#">Fechar Todos </a> | <a href="?#">Abrir Todos</a></div>
				<ul id="tree2">
					<li id="linhas" ><a href="a"><span class="pasta"><img class="pontilhado"  src="../../imagens/pontilhado.jpg" />Indicador 1</span></a>
						<ul id="arvore4">
							<li  value="1"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="z">Ação 1 </a></li>
							<li  value="1"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="z">Ação 2</a></li>
							<li  value="1"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><span class="pasta"><a href="z"><img class="pontilhado"  src="../../imagens/pontilhado.jpg" />Ação 3</a></span>
								<ul id="arvore5">
									<li  value="3"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="z">Subação 1</a></li>
									<li  value="3"><img title="Deletar item" id="btnDeletar" class="podeMover deletar" src="../../imagens/excluir_2.gif" /><a href="z">Subação 2</a></li>
								</ul>
							</li>
						</ul>
					</li>
					<li id="linhas"><a href="a"><span class="pasta"><img class="pontilhado"  src="../../imagens/pontilhado.jpg" />Indicador 2</span></a>
						
					</li>
				</ul>
			</div>
		</td>
	</tr>
</table>



		
		
<table style="background-color: #BEBEBE; right: 0px; position: fixed; bottom:25px;" width="100%;">
  <tr>
    <td style="padding-right: 50px;">
		<div id="lixeira" onclick="visualizarLixeira();" ><img id="iconeLixeira" src="/imagens/trash.jpg" /></div>
	</td>
  </tr>
</table>

<div id="arealixeira" class="conteudoLixeira">
	<div id="titulo">
		<div style="position:absolute; float:left; padding-left: 10px;">Lixeira </div>	
		<div style="" id="divFecharLixeira"><img onclick="fecharLixeira();" src="../../imagens/excluir_2.gif"></div>
	</div>
</div>


<script type="text/javascript">
	document.getElementById('arealixeira').style.display = 'none';
</script>
