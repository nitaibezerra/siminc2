<?php

ini_set("memory_limit", "1024M");

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

/*
 * Testando se existe o agrupador ESCOLA
 */
$agpEscola=false;
if($_REQUEST['agrupador']) {
	foreach($_REQUEST['agrupador'] as $agp) {
		if($agp == "entnome" || 
		   $agp == "esddsc") {
			$agpEscola=true;	
		}
	}
}


// monta o sql, agrupador e coluna do relatório
switch($_REQUEST['tiporel']) {
	case 'relatorioescolas':
		$sql       = recuperarSql($agpEscola);
		$coluna    = recuperarArColunas($agpEscola);
		break;
	case 'relatoriodistribuicaomaterial':
		$sql       = recuperarSqlDistribuicaoMaterial();
		$coluna    = recuperarArColunasDistribuicaoMaterial();
		break;
}

$agrupador = recuperarArAgrupador();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador( $agrupador, $dados ); 
$rel->setColuna( $coluna );
$rel->setTotNivel( true );
$rel->setMonstrarTolizadorNivel( TIPO_TOTALIZADOR_SUB_ITEM );


/*if($_REQUEST["tipovisualizacao"] == 'xls' && $_REQUEST['tipoSaida'] == 'listagem'){

	$arCabecalho = array();
	foreach($coluna as $cabecalho){
		if($cabecalho['label'] == 'Escola'){
			array_push($arCabecalho, 'Código INEP');			
		}
		array_push($arCabecalho, $cabecalho['label']);
	}
	
	//ver($arCabecalho,d);
	
	if($agpEscola) {
		$arWhere = $_REQUEST["demanda"] == 'M' ? array( " m.muncod is not null " ) : array( " m.muncod is null " );
	}
	else{
		$arWhere = $_REQUEST["demanda"] == 'M' ? array( " muncod is not null " ) : array( " muncod is null " );
	}
	
	// Região
	if ( count( $_REQUEST['regcod'] ) && $_REQUEST['regcod'][0] ){
		array_push( $arWhere, " regcod in ( '" . implode( "','", $_REQUEST['regcod'] ) . "' ) " );
	}
	// UF
	if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] ){
		array_push( $arWhere, " estado in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
	}
	// Município
	if($agpEscola) {
		if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] && $_REQUEST["demanda"] == 'M' ){
			array_push( $arWhere, " m.muncod in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
		}
	}		
	else{
		if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] && $_REQUEST["demanda"] == 'M' ){
			array_push( $arWhere, " muncod in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
		}
	}	
	//Situação
	if ( count( $_REQUEST['esdid'] ) && $_REQUEST['esdid'][0] ){
		array_push( $arWhere, " esdid in ( '" . implode( "','", $_REQUEST['esdid'] ) . "' ) " );
	}	
	//Ano
	if(!$agpEscola) {
		if ( count( $_REQUEST['ano'] ) && $_REQUEST['ano'][0] ){
			array_push( $arWhere, " esaano in ( '" . implode( "','", $_REQUEST['ano'] ) . "' ) " );
		}	
	}else{
		if ( count( $_REQUEST['ano'] ) && $_REQUEST['ano'][0] ){
			array_push( $arWhere, " m.esaano in ( '" . implode( "','", $_REQUEST['ano'] ) . "' ) " );
		}	
	}
	
	// Capitais
	if ( $_REQUEST['capitais'] ) {
		$arInner[] = " inner join territorios.estado estado on estado.estuf = m.estuf and estado.muncodcapital = m.muncod ";
	}
	if ( $_REQUEST['grandesCidades'] ) {
		if($agpEscola) {
			$arInner[] = " inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = estuf and mtm.tpmid = 1 ";
		}else{
			$arInner[] = " inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid = 1 ";
		}
	}
	if ( $_REQUEST['indigena'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mti on mti.muncod = m.muncod and mti.estuf = m.estuf and mti.tpmid = 16 ";
	}
	if ( $_REQUEST['quilombola'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mtq on mtq.muncod = m.muncod and mtq.estuf = m.estuf and mtq.tpmid = 17 ";
	}
	if ( $_REQUEST['cidadania'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mtq2 on mtq2.muncod = m.muncod and mtq2.estuf = m.estuf and mtq2.tpmid = 139 ";
	}	
	
	$arWhere = count( $arWhere ) ? " where ".implode( "and ", $arWhere ) : "";
	$stInner = count( $arInner ) ? implode( " ", $arInner ) : "";
	
	$arOrder = array();
	if(in_array('estuf',$_REQUEST['agrupador'])){
		$select .= 'estado as estuf,';
		array_push($arOrder, "estuf");
	}
	if(in_array('mundescricao',$_REQUEST['agrupador'])){
		$select .= 'mundescricao,';
		array_push($arOrder, "mundescricao");
	}
	if(in_array('entnome',$_REQUEST['agrupador'])){
		$select .= "COALESCE(entcodent, 'Não informado') as entcodent, ";
		$select .= "entnome, ";
		//array_push($arOrder, "entcodent");
		array_push($arOrder, "entnome");
	}
	if(in_array('regdescricao',$_REQUEST['agrupador'])){
		$select .= 'regdescricao,';
	}
	if(in_array('entnomesec',$_REQUEST['agrupador'])){
		$select .= 'entnomesec,';
	}
	if(in_array('esddsc',$_REQUEST['agrupador'])){
		$select .= 'esddsc,';
	}
	
	if($agpEscola) {
		// escola
		$sql = "select 
						$select
						totalalunos1, 
						totalalunos2, 
						totalalunos3, 
						totalalunos4, 
						totalalunos5, 
						totalalunos 
				from cte.rel_escolaativasituacao m
				". str_replace( ' estuf', ' estado', $stInner ) ."
				$arWhere
				" . ( is_array($arOrder) && count($arOrder) ? ' order by ' . implode(', ', $arOrder) : '' ) ."
				";
				
	} else {
		$sql = "SELECT	
						$select
						totalalunos1, 
						totalalunos2, 
						totalalunos3, 
						totalalunos4, 
						totalalunos5, 
						totalalunos
						
				FROM (SELECT CASE WHEN iu.muncod IS NULL THEN iu.estuf ELSE iu.mun_estuf END AS estado,
					   ed.esdid, 
				       iu.muncod, 
				       m.mundescricao, 
				       r.regdescricao, 
				       r.regcod, 
				       COUNT(e.entid) AS totalescolas, 
				       COALESCE(sum(eae.eaeqtdpredios), 0::bigint) AS totalpredios, 
				       COALESCE(sum(eae.eaeqtdturmas), 0::bigint) AS totalturmas, 
				       COALESCE(sum(eae.eaeqtdalunos1), 0::bigint) AS totalalunos1, 
				       COALESCE(sum(eae.eaeqtdalunos2), 0::bigint) AS totalalunos2, 
				       COALESCE(sum(eae.eaeqtdalunos3), 0::bigint) AS totalalunos3, 
				       COALESCE(sum(eae.eaeqtdalunos4), 0::bigint) AS totalalunos4, 
				       COALESCE(sum(eae.eaeqtdalunos5), 0::bigint) AS totalalunos5, 
				       COALESCE(sum(eae.eaeqtdalunos1 + eae.eaeqtdalunos2 + eae.eaeqtdalunos3 + eae.eaeqtdalunos4 + eae.eaeqtdalunos5), 0::bigint) AS totalalunos, 
				       COALESCE(ea.esaqtdprofessores::bigint, 0::bigint) AS nprofessores, 
				       COALESCE(ea.esaqtdtecnicos::bigint, 0::bigint) AS ntecnicos,
				       ea.esaano
				   FROM cte.escolaativa ea
				   LEFT JOIN cte.escolaativaescolas eae ON eae.esaid = ea.esaid
				   LEFT JOIN entidade.entidade e ON e.entid = eae.entid
				   JOIN cte.instrumentounidade iu ON iu.inuid = ea.inuid
				   LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
				   JOIN territorios.estado est ON est.estuf = iu.mun_estuf OR est.estuf = iu.estuf
				   JOIN territorios.regiao r ON r.regcod = est.regcod
				   LEFT JOIN workflow.documento d ON d.docid = ea.docid
				   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				   $stInner
				  GROUP BY  CASE 
				  	WHEN iu.muncod IS NULL THEN iu.estuf ELSE iu.mun_estuf END, 
				  	ed.esdid,
				  	iu.muncod, 
				  	m.mundescricao, 
				  	r.regdescricao, 
				  	r.regcod,
				  	ea.esaqtdprofessores,
				  	ea.esaqtdtecnicos, ea.esaano ) as t
			 $arWhere 
				" . ( is_array($arOrder) && count($arOrder) ? ' order by ' . implode(', ', $arOrder) : '' ) ."
				";
	}
	
	//ver($sql,d);
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	
	$nomeDoArquivoXls = "SIMEC_REL_PAR_ESCOLA_ATIVA_".date("YmdHis");
	$db->sql_to_excel($arDados, $nomeDoArquivoXls, $arCabecalho);
	
} else */if($_REQUEST["tipovisualizacao"] == 'xls'){
	$nomeDoArquivoXls = "SIMEC_REL_PAR_ESCOLA_ATIVA_".date("YmdHis");
	echo $rel->getRelatorioXls();
	echo "<script>self.close();</script>";	
	exit;
}

function recuperarSqlDistribuicaoMaterial(){
	
	$arInner = array();
	$arWhere = $_REQUEST["demanda"] == 'M' ? array( " muncod is not null " ) : array( " muncod is null " );
	$arWhere = $_REQUEST["demanda"] == 'M' ? array( " fen.funid=7 " ) : array( " fen.funid=6 " );
	
	// Região
	if ( count( $_REQUEST['regcod'] ) && $_REQUEST['regcod'][0] ){
		array_push( $arWhere, " r.regcod in ( '" . implode( "','", $_REQUEST['regcod'] ) . "' ) " );
	}
	// UF
	if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] ){
		array_push( $arWhere, " CASE WHEN iu.muncod IS NULL THEN iu.estuf in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) ELSE iu.mun_estuf in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) END  " );
	}
	// Município
	if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] && $_REQUEST["demanda"] == 'M' ){
		array_push( $arWhere, " muncod in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
	}	
	//Situação
	if ( count( $_REQUEST['esdid'] ) && $_REQUEST['esdid'][0] ){
		array_push( $arWhere, " ed.esdid in ( '" . implode( "','", $_REQUEST['esdid'] ) . "' ) " );
	}
	//Ano
	if ( count( $_REQUEST['ano'] ) && $_REQUEST['ano'][0] ){
		array_push( $arWhere, " ea.esaano in ( '" . implode( "','", $_REQUEST['ano'] ) . "' ) " );
	}	
	
	// Capitais
	if ( $_REQUEST['capitais'] ) {
		$arInner[] = " inner join territorios.estado estado on estado.estuf = m.estuf and estado.muncodcapital = m.muncod ";
	}
	if ( $_REQUEST['grandesCidades'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid = 1 ";
	}
	if ( $_REQUEST['indigena'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mti on mti.muncod = m.muncod and mti.estuf = m.estuf and mti.tpmid = 16 ";
	}
	if ( $_REQUEST['quilombola'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mtq on mtq.muncod = m.muncod and mtq.estuf = m.estuf and mtq.tpmid = 17 ";
	}
	if ( $_REQUEST['cidadania'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mtq2 on mtq2.muncod = m.muncod and mtq2.estuf = m.estuf and mtq2.tpmid = 139 ";
	}	
	
	$arWhere = count( $arWhere ) ? " where ".implode( "and ", $arWhere ) : "";
	$stInner = count( $arInner ) ? implode( " ", $arInner ) : "";
	
	//where fen2.funid=".(($_REQUEST['demanda']=="M")?"7":"6")
	
	$sql = "SELECT ea.esaid, 
				   ea.inuid, 
			       CASE WHEN iu.muncod IS NULL THEN iu.estuf ELSE iu.mun_estuf END AS estuf, 
				   iu.muncod, 
				   COALESCE(m.mundescricao, 'Não existe') as mundescricao, 
				   e.entid, 
				   COALESCE(e.entnome, 'Não Informado'::character varying) AS entnomesec, 
				   COALESCE(ed.esdid, 0) AS esdid, 
				   COALESCE(ed.esddsc, 'Não Iniciado'::character varying) AS esddsc, 
				   r.regdescricao, 
				   r.regcod, 
				   '<font size=1>CEP: ' || COALESCE(ende.endcep, 'Não informado') || ', Logradouro: ' || COALESCE(trim(ende.endlog), 'Não informado') || ', Complemento: ' || COALESCE(ende.endcom, 'Não informado') || ', Número: ' || COALESCE(ende.endnum, 'Não Informado') || ', Bairro: ' || COALESCE(ende.endbai, 'Não informado') || ', ' || CASE WHEN iu.muncod IS NULL THEN iu.estuf ELSE iu.mun_estuf END || CASE WHEN iu.muncod IS NULL THEN '' ELSE ', ' || m.mundescricao END || '</font>' AS enderecosec,
				   CASE WHEN e.entnome IS NULL THEN 0 ELSE 1 END AS totalescolas, 
				   COALESCE(sum(eae.eaeqtdpredios), 0::bigint) AS totalpredios, 
				   COALESCE(sum(eae.eaeqtdturmas), 0::bigint) AS totalturmas, 
				   COALESCE(sum(eae.eaeqtdalunos1), 0::bigint) AS totalalunos1, 
				   COALESCE(sum(eae.eaeqtdalunos2), 0::bigint) AS totalalunos2, 
				   COALESCE(sum(eae.eaeqtdalunos3), 0::bigint) AS totalalunos3, 
				   COALESCE(sum(eae.eaeqtdalunos4), 0::bigint) AS totalalunos4, 
				   COALESCE(sum(eae.eaeqtdalunos5), 0::bigint) AS totalalunos5, 
				   COALESCE(sum(eae.eaeqtdalunos1 + eae.eaeqtdalunos2 + eae.eaeqtdalunos3 + eae.eaeqtdalunos4 + eae.eaeqtdalunos5), 0::bigint) AS totalalunos, 
				   COALESCE(ea.esaqtdprofessores::bigint, 0::bigint) AS nprofessores, 
				   COALESCE(ea.esaqtdtecnicos::bigint, 0::bigint) AS ntecnicos
			FROM cte.escolaativa ea
			LEFT JOIN cte.escolaativaescolas eae ON eae.esaid = ea.esaid
			JOIN cte.instrumentounidade iu ON iu.inuid = ea.inuid 
			LEFT JOIN entidade.endereco ende ON ".(($_REQUEST['demanda']=="M")?"ende.muncod=iu.muncod":"ende.estuf=iu.estuf")."
			LEFT JOIN entidade.entidade e ON e.entid = ende.entid
			LEFT JOIN entidade.funcaoentidade fen ON fen.entid = e.entid
			LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
			JOIN territorios.estado est ON est.estuf = iu.mun_estuf OR est.estuf = iu.estuf
			JOIN territorios.regiao r ON r.regcod = est.regcod
			LEFT JOIN workflow.documento d ON d.docid = ea.docid
			LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
			$stInner
			".$arWhere."
			GROUP BY ea.esaid,
					 ende.endcep, 
					 ende.endlog,
					 ende.endcom,
					 ende.endnum,
					 ende.endbai,
					 ea.inuid, 
					 CASE WHEN iu.muncod IS NULL THEN iu.estuf ELSE iu.mun_estuf END, 
					 iu.muncod, 
					 m.mundescricao, 
					 e.entid, 
					 e.entnome, 
					 ed.esddsc, 
					 r.regdescricao, 
					 ed.esdid, 
					 r.regcod, 
					 ea.esaqtdprofessores, 
					 ea.esaqtdtecnicos";
	return $sql;
	
}

function recuperarSql($agpEscola){
	
	if($agpEscola) {
		$arWhere = $_REQUEST["demanda"] == 'M' ? array( " m.muncod is not null " ) : array( " m.muncod is null " );
	}
	else{
		$arWhere = $_REQUEST["demanda"] == 'M' ? array( " muncod is not null " ) : array( " muncod is null " );
	}
	
	// Região
	if ( count( $_REQUEST['regcod'] ) && $_REQUEST['regcod'][0] ){
		array_push( $arWhere, " regcod in ( '" . implode( "','", $_REQUEST['regcod'] ) . "' ) " );
	}
	// UF
	if ( count( $_REQUEST['estuf'] ) && $_REQUEST['estuf'][0] ){
		array_push( $arWhere, " estado in ( '" . implode( "','", $_REQUEST['estuf'] ) . "' ) " );
	}
	// Município
	if($agpEscola) {
		if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] && $_REQUEST["demanda"] == 'M' ){
			array_push( $arWhere, " m.muncod in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
		}
	}		
	else{
		if ( count( $_REQUEST['muncod'] ) && $_REQUEST['muncod'][0] && $_REQUEST["demanda"] == 'M' ){
			array_push( $arWhere, " muncod in ( '" . implode( "','", $_REQUEST['muncod'] ) . "' ) " );
		}
	}	
	//Situação
	if ( count( $_REQUEST['esdid'] ) && $_REQUEST['esdid'][0] ){
		array_push( $arWhere, " esdid in ( '" . implode( "','", $_REQUEST['esdid'] ) . "' ) " );
	}	
	//Ano
	if(!$agpEscola) {
		if ( count( $_REQUEST['ano'] ) && $_REQUEST['ano'][0] ){
			array_push( $arWhere, " esaano in ( '" . implode( "','", $_REQUEST['ano'] ) . "' ) " );
		}	
	}else{
		if ( count( $_REQUEST['ano'] ) && $_REQUEST['ano'][0] ){
			array_push( $arWhere, " m.esaano in ( '" . implode( "','", $_REQUEST['ano'] ) . "' ) " );
		}	
	}
	
	//dbg($arWhere,1);
		
	// Capitais
	if ( $_REQUEST['capitais'] ) {
		$arInner[] = " inner join territorios.estado estado on estado.estuf = m.estuf and estado.muncodcapital = m.muncod ";
	}
	if ( $_REQUEST['grandesCidades'] ) {
		if($agpEscola) {
			$arInner[] = " inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = estuf and mtm.tpmid = 1 ";
		}else{
			$arInner[] = " inner join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf and mtm.tpmid = 1 ";
		}
	}
	if ( $_REQUEST['indigena'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mti on mti.muncod = m.muncod and mti.estuf = m.estuf and mti.tpmid = 16 ";
	}
	if ( $_REQUEST['quilombola'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mtq on mtq.muncod = m.muncod and mtq.estuf = m.estuf and mtq.tpmid = 17 ";
	}
	if ( $_REQUEST['cidadania'] ) {
		$arInner[] = " inner join territorios.muntipomunicipio mtq2 on mtq2.muncod = m.muncod and mtq2.estuf = m.estuf and mtq2.tpmid = 139 ";
	}	
	
	$arWhere = count( $arWhere ) ? " where ".implode( "and ", $arWhere ) : "";
	$stInner = count( $arInner ) ? implode( " ", $arInner ) : "";
	
	if($agpEscola) {
		// escola
		$sql = "select 
						esaid, 
						eaeid, 
						inuid, 
						estado as estuf,
						m.muncodendereco, 
						COALESCE((coalesce( m.mundescricaoendereco, '' )), 'Não informado') as mundescricao, 
						entid, 
						entnome  AS entnome, 
						entcodent, 
						esddsc, 
						regdescricao,
						totalescolas, 
						totalpredios, 
						totalturmas,
						totalalunos1, 
						totalalunos2, 
						totalalunos3, 
						totalalunos4, 
						totalalunos5, 
						totalalunos, 
						nprofessores, 
						ntecnicos 
				from cte.rel_escolaativasituacao m
				". str_replace( ' estuf', ' estado', $stInner ) ."
				$arWhere
				order by estuf, mundescricao, entnome";
				
	} else {
		$sql = "SELECT	estado as estuf,
						muncod ,
						(coalesce( mundescricao, '' )) as mundescricao, 
						regdescricao,
						totalescolas, 
						totalpredios, 
						totalturmas,
						totalalunos1, 
						totalalunos2, 
						totalalunos3, 
						totalalunos4, 
						totalalunos5, 
						totalalunos, 
						nprofessores, 
						ntecnicos
						
				FROM (SELECT CASE WHEN iu.muncod IS NULL THEN iu.estuf ELSE iu.mun_estuf END AS estado,
					   ed.esdid, 
				       iu.muncod, 
				       m.mundescricao, 
				       r.regdescricao, 
				       r.regcod, 
				       COUNT(e.entid) AS totalescolas, 
				       COALESCE(sum(eae.eaeqtdpredios), 0::bigint) AS totalpredios, 
				       COALESCE(sum(eae.eaeqtdturmas), 0::bigint) AS totalturmas, 
				       COALESCE(sum(eae.eaeqtdalunos1), 0::bigint) AS totalalunos1, 
				       COALESCE(sum(eae.eaeqtdalunos2), 0::bigint) AS totalalunos2, 
				       COALESCE(sum(eae.eaeqtdalunos3), 0::bigint) AS totalalunos3, 
				       COALESCE(sum(eae.eaeqtdalunos4), 0::bigint) AS totalalunos4, 
				       COALESCE(sum(eae.eaeqtdalunos5), 0::bigint) AS totalalunos5, 
				       COALESCE(sum(eae.eaeqtdalunos1 + eae.eaeqtdalunos2 + eae.eaeqtdalunos3 + eae.eaeqtdalunos4 + eae.eaeqtdalunos5), 0::bigint) AS totalalunos, 
				       COALESCE(ea.esaqtdprofessores::bigint, 0::bigint) AS nprofessores, 
				       COALESCE(ea.esaqtdtecnicos::bigint, 0::bigint) AS ntecnicos,
				       ea.esaano
				   FROM cte.escolaativa ea
				   LEFT JOIN cte.escolaativaescolas eae ON eae.esaid = ea.esaid
				   LEFT JOIN entidade.entidade e ON e.entid = eae.entid
				   JOIN cte.instrumentounidade iu ON iu.inuid = ea.inuid
				   LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
				   JOIN territorios.estado est ON est.estuf = iu.mun_estuf OR est.estuf = iu.estuf
				   JOIN territorios.regiao r ON r.regcod = est.regcod
				   LEFT JOIN workflow.documento d ON d.docid = ea.docid
				   LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				   $stInner
				  GROUP BY  CASE 
				  	WHEN iu.muncod IS NULL THEN iu.estuf ELSE iu.mun_estuf END, 
				  	ed.esdid,
				  	iu.muncod, 
				  	m.mundescricao, 
				  	r.regdescricao, 
				  	r.regcod,
				  	ea.esaqtdprofessores,
				  	ea.esaqtdtecnicos, ea.esaano ) as t
			 $arWhere 
				order by estuf, mundescricao";
	}
	//ver ( $sql);
	return $sql;
	
}

function recuperarArAgrupador(){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
										  "estuf",
										  "mundescricao",
										  "muncodendereco",
										  "entcodent",
										  "enderecosec",
										  "totalpredios",
										  "totalturmas",
										  "totalescolas",
										  "nprofessores",
										  "ntecnicos",
										  "totalalunos1",
										  "totalalunos2",
										  "totalalunos3",
										  "totalalunos4",
										  "totalalunos5",
										  "totalalunos" )	  
				);
	foreach ( $agrupador as $val ){
		
		switch( $val ){
			case "estuf":
				array_push($agp['agrupador'], array(
													"campo" => "estuf",
											  		"label" => "UF"									
									   				) );
			break;
			case "regdescricao":
				array_push($agp['agrupador'], array(
													"campo" => "regdescricao",
											  		"label" => "Região")										
									   				);
			break;
			case "mundescricao":
				array_push($agp['agrupador'], array(
													"campo" => "mundescricao",
											  		"label" => "Município")										
									   				);
			break;
			case "entnome":
				array_push($agp['agrupador'], array(
													"campo" => "entnome",
											  		"label" => "Escola")										
									   				);
			break;
			case "entnomesec":
				array_push($agp['agrupador'], array(
													"campo" => "entnomesec",
											  		"label" => "Secretaria")										
									   				);
			break;
			
			case "esddsc":
				array_push($agp['agrupador'], array(
													"campo" => "esddsc",
											  		"label" => "Situação")										
									   				);
			break;
		}
	}
	
	return $agp;
	
}

function recuperarArColunasDistribuicaoMaterial(){
	
	$coluna[] = array( "campo" 	  => "estuf",
					   "type" 	  => "string",
			   		   "label" 	  => "Estado" );
	
	$coluna[] = array( "campo" 	  => "mundescricao",
					   "type" 	  => "string",
			   		   "label" 	  => "Municipio" );
	
	$coluna[] = array( "campo" 	  => "muncodendereco",
					   "type" 	  => "string",
			   		   "label" 	  => "Codigo do IBGE" );
	
	$coluna[] = array( "campo" 	  => "entcodent",
					   "type" 	  => "string",
			   		   "label" 	  => "Codigo do INEP" );
	
	$coluna[] = array( "campo" 	  => "enderecosec",
			   		   "label" 	  => "Endereço da Secretaria" );
	
	$coluna[] = array( "campo" 	  => "totalescolas",
					   "type" 	  => "numeric",
					   "blockAgp" => "entnome",
			   		   "label" 	  => "Total de Escolas" );
	
	$coluna[] = array( "campo" 	  => "totalpredios",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Total de Prédios" );
	
	$coluna[] = array( "campo" 	  => "totalturmas",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Total de Turmas" );
	
	$coluna[] = array( "campo" 	  => "nprofessores",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Número de Professores" );
	
	$coluna[] = array( "campo" 	  => "ntecnicos",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Números de Técnicos" );
	
	$coluna[] = array( "campo" 	  => "totalalunos1",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 1º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos2",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 2º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos3",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 3º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos4",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 4º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos5",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 5º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Total de Alunos" );

	return $coluna;
	
}


function recuperarArColunas($agpEscola){
	
	if($_REQUEST["tipovisualizacao"] == 'xls' && $_REQUEST['tipoSaida'] == 'listagem'){
		$agrupador = $_POST['agrupador'];
		$coluna = array();
		foreach ( $agrupador as $val ){
		
			switch( $val ){
				case "estuf":
					array_push($coluna, array(
														"campo" => "estuf",
												  		"label" => "UF"									
										   				) );
				break;
				case "regdescricao":
					array_push($coluna, array(
														"campo" => "regdescricao",
												  		"label" => "Região")										
										   				);
				break;
//				case "mundescricao":
//					array_push($coluna, array(
//														"campo" => "mundescricao",
//												  		"label" => "Município")										
//										   				);
//				break;
//				case "entnome":
//					array_push($coluna, array(
//														"campo" => "entnome",
//												  		"label" => "Escola")										
//										   				);
//				break;
				case "entnomesec":
					array_push($coluna, array(
														"campo" => "entnomesec",
												  		"label" => "Secretaria")										
										   				);
				break;
				
				case "esddsc":
					array_push($coluna, array(
														"campo" => "esddsc",
												  		"label" => "Situação")										
										   				);
				break;
			}
		
		}
	}
	
	$coluna[] = array( "campo" 	  => "estuf",
					   "type" 	  => "string",
			   		   "label" 	  => "Estado" );
	
	$coluna[] = array( "campo" 	  => "mundescricao",
					   "type" 	  => "string",
			   		   "label" 	  => "Municipio" );
	
	$coluna[] = array( "campo" 	  => "muncodendereco",
					   "type" 	  => "string",
			   		   "label" 	  => "Codigo do IBGE" );
	
	$coluna[] = array( "campo" 	  => "entcodent",
					   "type" 	  => "string",
			   		   "label" 	  => "Codigo do INEP" );
	
	if($_REQUEST['tipoSaida'] != 'listagem'){
		$coluna[] = array( "campo" 	  => "totalescolas",
						   "type" 	  => "numeric",
						   "blockAgp" => "entnome",
				   		   "label" 	  => "Total de Escolas" );
		
		$coluna[] = array( "campo" 	  => "totalpredios",
						   "type" 	  => "numeric",
				   		   "label" 	  => "Total de Prédios" );
		
		$coluna[] = array( "campo" 	  => "totalturmas",
						   "type" 	  => "numeric",
				   		   "label" 	  => "Total de Turmas" );
	}
	
	if(!$agpEscola) {
		$coluna[] = array( "campo" 	  => "nprofessores",
						   "blockAgp" => "entnome",
						   "type" 	  => "numeric",
				   		   "label" 	  => "Número de Professores" );
		
		$coluna[] = array( "campo" 	  => "ntecnicos",
						   "type" 	  => "numeric",
				   		   "label" 	  => "Números de Técnicos" );
	}
	
	$coluna[] = array( "campo" 	  => "totalalunos1",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 1º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos2",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 2º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos3",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 3º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos4",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 4º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos5",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Alunos do 5º ano" );
	
	$coluna[] = array( "campo" 	  => "totalalunos",
					   "type" 	  => "numeric",
			   		   "label" 	  => "Total de Alunos" );
	
	return $coluna;
	
}

?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
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