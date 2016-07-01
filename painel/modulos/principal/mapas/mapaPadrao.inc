<?php
ini_set( "memory_limit", "2048M" );
set_time_limit(0);

if (!defined('LINK_PADRAO')) { define('LINK_PADRAO', 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A'); }

function instituicoesResidenciaMedica() {
	global $db;
	if($_REQUEST['muncod']) {
		$arrMuncod6[] = substr($_REQUEST['muncod'],0,6);
	}
	if($_REQUEST['tpmid']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct substr(muncod,1,6) as muncod from territoriosgeo.muntipomunicipio where tpmid = {$_REQUEST['tpmid']}";
		$arrMuncod6 = $db->carregarColuna($sql);
	}
	if($_REQUEST['estuf']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct substr(muncod,1,6) as muncod from territoriosgeo.municipio where estuf = '{$_REQUEST['estuf']}'";
		$arrMuncod6 = $db->carregarColuna($sql);
	}
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
		$sql = "select     \"Municipio\" as aa,\"UF\" as ab,\"Instituicao\" as ac,
			    coalesce(sum(\"Vagas_acesso_cirurgia_geral\"::numeric),0) as b,
			    coalesce(sum(\"Vagas_acesso_clinica_medica\"::numeric),0) as c,
			    coalesce(sum(\"Vagas_acesso_gineco_obstetricia\"::numeric),0) as d,
			    coalesce(sum(\"Vagas_acesso_pediatria\"::numeric),0) as e,
			    coalesce(sum(\"Vagas_acesso_med_familia_comunidade\"::numeric),0) as f,
			    coalesce(sum(\"Vagas_acesso_cirurgia_geral\"::numeric),0) + coalesce(sum(\"Vagas_acesso_clinica_medica\"::numeric),0) + coalesce(sum(\"Vagas_acesso_gineco_obstetricia\"::numeric),0) +
			    coalesce(sum(\"Vagas_acesso_pediatria\"::numeric),0) + coalesce(sum(\"Vagas_acesso_med_familia_comunidade\"::numeric),0) as total
				from carga.medicos_residencia_medica_municipios where \"Cod_IBGE\" in('".implode("','",$arrMuncod6)."')
				group by \"Instituicao\",\"Municipio\",\"UF\"
				order by \"Instituicao\"";
		$db->monta_lista_simples($sql,array("Município","UF","Instituição","Cirurgia Geral","Clínica Médica","Ginecologia e Obstetrícia","Pediatria","Família Comunidade","Total"),10000,5,'S','100%',$par2);
	
}


function unidadesMaisCinquentaLeitos() {
	global $db;
	if($_REQUEST['muncod']) {
		$arrMuncod6[] = substr($_REQUEST['muncod'],0,6);
	}
	if($_REQUEST['tpmid']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct substr(muncod,1,6) as muncod from territoriosgeo.muntipomunicipio where tpmid = {$_REQUEST['tpmid']}";
		$arrMuncod6 = $db->carregarColuna($sql);
	}
	if($_REQUEST['estuf']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct substr(muncod,1,6) as muncod from territoriosgeo.municipio where estuf = '{$_REQUEST['estuf']}'";
		$arrMuncod6 = $db->carregarColuna($sql);
	}
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	$sql = "select 
				\"MUNICIPIO\",
				\"UF\",
				\"ESTABELECIMENTO\",
				\"ESFERA_ADMINISTRATIVA_ESTABELECIMENTO\",
				\"LEITO_SUS_-_CLASSIFICAÇÃO\",
				\"Cirúrgico_SUS\",
				\"Clínico_SUS\",
				\"Obstétrico_SUS\",
				\"Pediátrico_SUS\",
				\"Leitos_UTI_SUS\",
				\"Outros_SUS\",
				\"LEITOS_SUS_TOTAIS\"
			from 
				carga.leitos_sus_painel
			WHERE
				\"IBGE\" in('".implode("','",$arrMuncod6)."') ";
	$arrCab = array("Município","UF","Estabelecimento","Esfera","Classificação SUS","Curúrgicos SUS","Clínico SUS","Obstétrico SUS","Pediátrico SUS","UTI SUS","Outros SUS","Total SUS");
	$db->monta_lista_simples($sql,$arrCab,10000,10000,'S','', "N", true );
	

}

function instituicoesRedePublica() {
	global $db;
	
	if($_REQUEST['muncod']) {
		$arrMuncod[] = $_REQUEST['muncod'];
	}
	if($_REQUEST['tpmid']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct muncod as muncod from territoriosgeo.muntipomunicipio where tpmid = {$_REQUEST['tpmid']}";
		$arrMuncod = $db->carregarColuna($sql);
		if($_GET['exportar']){
			$coluna_municipio_estado = "\"Município\" as mun,a.\"UF\" as uf,";
		}else{
			$coluna_municipio_estado = "\"Município\" || ' / ' || a.\"UF\" as mun,";
		}
	}
	if($_REQUEST['estuf']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct muncod as muncod from territoriosgeo.municipio where estuf = '{$_REQUEST['estuf']}'";
		$arrMuncod = $db->carregarColuna($sql);
		if($_GET['exportar']){
			$coluna_municipio_estado = "\"Município\" as mun,a.\"UF\" as uf,";
		}else{
			$coluna_municipio_estado = "\"Município\" || ' / ' || a.\"UF\" as mun,";
		}
	}
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "
	<script>
		function exportarExcel()
		{
			window.location = window.location + '&exportar=1';
		}
		function alterarExpansao(expansao)
		{
			if(expansao == 'todas'){
				window.location = 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=instituicoesRede&muncod={$_REQUEST['muncod']}&estuf={$_REQUEST['estuf']}&tpmid={$_REQUEST['tpmid']}';
			}if(expansao == 'privada'){
				window.location = 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=instituicoesRedePrivada&muncod={$_REQUEST['muncod']}&estuf={$_REQUEST['estuf']}&tpmid={$_REQUEST['tpmid']}';
			}
		}
	</script>
	
	";
	
	monta_titulo("Expansão <select onchange='alterarExpansao(this.value)' name='expansao' ><option value='todas' >Todas</option><option selected='selected' value='publica' >Pública</option><option value='privada' >Privada</option></select>","<a href='javascript:exportarExcel()'>Exportar para planilha.</a>");
	
	$sql = "select  $coluna_municipio_estado  \"Instituição\",
			    \"\"\"Campo\"\" de prática IFES\",
			    \"VAGAS EXISTENTES\",
			    \"Autorizadas (públicas)\",
			    \"Ampliação Públicas (novas)\",
			    \"Categoria Administrativa\",
			    \"Esfera\"
			from carga.medicos_planilha_base a
			inner join carga.medicos_vagas_existentes_publica b ON b.muncod = a.muncod
			where a.muncod in('".implode("','",$arrMuncod)."') and \"Categoria Administrativa\" = 'PÚBLICA'
			".($coluna_municipio_estado ? "order by a.\"UF\",\"Município\" " : "");
	if($coluna_municipio_estado){
		if($_GET['exportar']){
			$cabecalho = array("Município","UF","Instituição","Campo de prática","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera");
		}else{
			$cabecalho = array("Município / UF","Instituição","Campo de prática","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera");
		}
	}else{
		$cabecalho = array("Instituição","Campo de prática","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera");
	}
	
	if($_GET['exportar']){
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=planilha_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,10000,5,100,'S');
		exit;
	}else{
		$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
	}
	
}


function instituicoesRede() {
	global $db;
	
	if($_REQUEST['muncod']) {
		$arrMuncod[] = $_REQUEST['muncod'];
	}
	if($_REQUEST['tpmid']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct muncod as muncod from territoriosgeo.muntipomunicipio where tpmid = {$_REQUEST['tpmid']}";
		$arrMuncod = $db->carregarColuna($sql);
		if($_GET['exportar']){
			$coluna_municipio_estado = "\"Município\" as mun,a.\"UF\" as uf,";
		}else{
			$coluna_municipio_estado = "\"Município\" || ' / ' || a.\"UF\" as mun,";
		}
	}
	if($_REQUEST['estuf']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct muncod as muncod from territoriosgeo.municipio where estuf = '{$_REQUEST['estuf']}'";
		$arrMuncod = $db->carregarColuna($sql);
		if($_GET['exportar']){
			$coluna_municipio_estado = "\"Município\" as mun,a.\"UF\" as uf,";
		}else{
			$coluna_municipio_estado = "\"Município\" || ' / ' || a.\"UF\" as mun,";
		}
	}
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "
	<script>
		function exportarExcel()
		{
			window.location = window.location + '&exportar=1';
		}
		function alterarExpansao(expansao)
		{
			if(expansao == 'publica'){
				window.location = 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=instituicoesRedePublica&muncod={$_REQUEST['muncod']}&estuf={$_REQUEST['estuf']}&tpmid={$_REQUEST['tpmid']}';
			}if(expansao == 'privada'){
				window.location = 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=instituicoesRedePrivada&muncod={$_REQUEST['muncod']}&estuf={$_REQUEST['estuf']}&tpmid={$_REQUEST['tpmid']}';
			}
		}
	</script>
	
	";
	
	monta_titulo("Expansão <select onchange='alterarExpansao(this.value)' name='expansao' ><option selected='selected' value='todas' >Todas</option><option value='publica' >Pública</option><option value='privada' >Privada</option></select>","<a href='javascript:exportarExcel()'>Exportar para planilha.</a>");
	
	$sql = "select  $coluna_municipio_estado  \"Instituição\",
			    \"\"\"Campo\"\" de prática IFES\",
			    \"VAGAS EXISTENTES\",
			    \"Autorizadas (públicas)\",
			    \"Ampliação Públicas (novas)\",
			    \"Categoria Administrativa\",
			    \"Esfera\"
			from carga.medicos_planilha_base a
			inner join carga.medicos_vagas_existentes_publica b ON b.muncod = a.muncod
			where a.muncod in('".implode("','",$arrMuncod)."')
			".($coluna_municipio_estado ? "order by a.\"UF\",\"Município\" " : "");
	if($coluna_municipio_estado){
		if($_GET['exportar']){
			$cabecalho = array("Município","UF","Instituição","Campo de prática","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera");
		}else{
			$cabecalho = array("Município / UF","Instituição","Campo de prática","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera");
		}
	}else{
		$cabecalho = array("Instituição","Campo de prática","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera");
	}
	
	if($_GET['exportar']){
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=planilha_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,10000,5,100,'S');
		exit;
	}else{
		$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
	}
	
}

function exibeLeitosObservacao()
{
	global $db;
	
	if($_REQUEST['muncod']) {
		$arrMuncod[] = $_REQUEST['muncod'];
	}
	
	if($_REQUEST['tpmid']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct muncod as muncod from territoriosgeo.muntipomunicipio where tpmid = {$_REQUEST['tpmid']}";
		$arrMuncod = $db->carregarColuna($sql);
		if($_GET['exportar']){
			$coluna_municipio_estado = "\"Município\" as mun,a.\"UF\" as uf,";
		}else{
			$coluna_municipio_estado = "\"Município\" || ' / ' || a.\"UF\" as mun,";
		}
	}
	
	if($_REQUEST['estuf']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct muncod as muncod from territoriosgeo.municipio where estuf = '{$_REQUEST['estuf']}'";
		$arrMuncod = $db->carregarColuna($sql);
		if($_GET['exportar']){
			$coluna_municipio_estado = "\"Município\" as mun,a.\"UF\" as uf,";
		}else{
			$coluna_municipio_estado = "\"Município\" || ' / ' || a.\"UF\" as mun,";
		}
	}
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "
	<script>
		function exportarExcel()
		{
			window.location = window.location + '&exportar=1';
		}
	</script>
	
	";
	
	monta_titulo("Leitos SUS","");
	
	$sql = "select   $coluna_municipio_estado  \"Hospital\",
				    \"Localidade_Municipio\",
				    \"Localidade_DISTANCIA KM\",
				    \"Leitos_SUS _CNES\" as cnes,
				    \"Leitos_2013\",
				    \"Leitos_Construção\" as autorizadas_privadas,
				    \"Leitos_TOTAL\" as ampliacao_privadas
				from carga.medicos_leitos_investimento a
				where a.muncod in('".implode("','",$arrMuncod)."')
				".($coluna_municipio_estado ? "order by a.\"UF\",\"Município\" " : "order by \"Hospital\"");
	if($coluna_municipio_estado){
		if($_GET['exportar']){
			$cabecalho = array("Município","UF","Instituição","Campo de prática","Critério de seleção","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera","Justificativa");
		}else{
			$arrCabecalho[0] = "Hospital";
			$arrCabecalho[1]["label"] = "Localidade";
			$arrCabecalho[1]["colunas"] = array("Município","Distância em Km");
			$arrCabecalho[2]["label"] = "Leitos";
			$arrCabecalho[2]["colunas"] = array("CNES","2013","Construção","Total");
		}
	}else{
		$arrCabecalho[0] = "Hospital";
		$arrCabecalho[1]["label"] = "Localidade";
		$arrCabecalho[1]["colunas"] = array("Município","Distância em Km");
		$arrCabecalho[2]["label"] = "Leitos";
		$arrCabecalho[2]["colunas"] = array("CNES","2013","Construção","Total");
	}

	if($_GET['exportar']){
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=planilha_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,10000,5,100,'S');
		exit;
	}else{
		$db->monta_lista($sql,$arrCabecalho,10000000,1000,"S","95%","S");
	}
	
}


function instituicoesRedePrivada() {
	global $db;
	
	if($_REQUEST['muncod']) {
		$arrMuncod[] = $_REQUEST['muncod'];
	}
	
	if($_REQUEST['tpmid']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct muncod as muncod from territoriosgeo.muntipomunicipio where tpmid = {$_REQUEST['tpmid']}";
		$arrMuncod = $db->carregarColuna($sql);
		if($_GET['exportar']){
			$coluna_municipio_estado = "\"Município\" as mun,a.\"UF\" as uf,";
		}else{
			$coluna_municipio_estado = "\"Município\" || ' / ' || a.\"UF\" as mun,";
		}
	}
	
	if($_REQUEST['estuf']) {
		//Pegando os Municípios do grupo
		$sql = "select distinct muncod as muncod from territoriosgeo.municipio where estuf = '{$_REQUEST['estuf']}'";
		$arrMuncod = $db->carregarColuna($sql);
		if($_GET['exportar']){
			$coluna_municipio_estado = "\"Município\" as mun,a.\"UF\" as uf,";
		}else{
			$coluna_municipio_estado = "\"Município\" || ' / ' || a.\"UF\" as mun,";
		}
	}
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	echo "
	<script>
		function exportarExcel()
		{
			window.location = window.location + '&exportar=1';
		}
		function alterarExpansao(expansao)
		{
			if(expansao == 'publica'){
				window.location = 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=instituicoesRedePublica&muncod={$_REQUEST['muncod']}&estuf={$_REQUEST['estuf']}&tpmid={$_REQUEST['tpmid']}';
			}if(expansao == 'todas'){
				window.location = 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=instituicoesRede&muncod={$_REQUEST['muncod']}&estuf={$_REQUEST['estuf']}&tpmid={$_REQUEST['tpmid']}';
			}
		}
	</script>
	
	";
	
	monta_titulo("Expansão <select onchange='alterarExpansao(this.value)' name='expansao' ><option value='todas' >Todas</option><option value='publica' >Pública</option><option selected='selected' value='privada' >Privada</option></select>","<a href='javascript:exportarExcel()'>Exportar para planilha.</a>");
	
	
	$sql = "select   $coluna_municipio_estado  \"Instituição\",
				    \"Campo de Prática\",
				    \"Critério de Seleção\",
				    \"VAGAS EXISTENTES\",
				    coalesce(\"Autorizadas (privadas)\"::numeric,0) as autorizadas_privadas,
				    coalesce(\"Ampliação Privadas (novas)\"::numeric,0) as ampliacao_privadas,
				    \"Categoria Administrativa\",
				    \"Esfera\",
				    \"Ano\"
				from carga.medicos_planilha_base a
				inner join carga.medicos_vagas_existentes_privada b ON b.muncod = a.muncod
				where a.muncod in('".implode("','",$arrMuncod)."') and \"Categoria Administrativa\" = 'PRIVADA'
				".($coluna_municipio_estado ? "order by a.\"UF\",\"Município\" " : "");
	if($coluna_municipio_estado){
		if($_GET['exportar']){
			$cabecalho = array("Município","UF","Instituição","Campo de prática","Critério de seleção","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera","Justificativa");
		}else{
			$cabecalho = array("Município / UF","Instituição","Campo de prática","Critério de seleção","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera","Justificativa");
		}
	}else{
		$cabecalho = array("Instituição","Campo de prática","Critério de seleção","Vaga existentes","Vagas autorizadas","Vagas novas","Categoria","Esfera","Justificativa");
	}
	
	if($_GET['exportar']){
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=planilha_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,10000,5,100,'S');
		exit;
	}else{
		$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
	}
	
}


if($_REQUEST['requisicao'])
{
	ob_clean();
	$_REQUEST['requisicao']();
	die;
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

if(!$_REQUEST['mapid']){
	$arrMapas = pegaMapasCadastrados();
}else{
	$sql = "select mapid from mapa.mapa where mapid = {$_REQUEST['mapid']} and mapstatus = 'A'";
	$mapid = $db->pegaUm($sql);
	if(!$mapid){
		$arrMapas = pegaMapasCadastrados();
	}else{
		$_SESSION['painel_vars']['mapid'] = $mapid;
		pegaMapasCadastrados(null,false);
	}
}

function alteraTipoFiltroAvancado()
{
	global $db;
	$tmaid = $_POST['tmaid'];
	$sql = "select tpdid from mapa.tema where tmaid = $tmaid";
	$tpdid = $db->pegaUm($sql);
	$nome = str_replace("span_tema_","",$_POST['campo']);
	$arrD = explode("_",$nome);
	if($tpdid == 3){
		$arrOptions = array(
							0=> array("codigo" => "true", "descricao" => "Sim"),
							1=> array("codigo" => "false", "descricao" => "Não")
							);
		$db->monta_combo("cmb_filtro[{$arrD[0]}][{$arrD[1]}]",$arrOptions,"S","","","","","","","","");
	}else{
		comboTipoFiltro("cmb_filtro[{$arrD[0]}][{$arrD[1]}]");
	}
}

function carregaCorFiltroAvancado()
{
	global $db;
	$fusid = $_POST['fusid'];
	$sql = "select fuspublico,fustitulo,fusfiltros from mapa.filtrousuario where fusid = $fusid";
	$fus = $db->pegaLinha($sql);
	$arrFiltros = unserialize($fus['fusfiltros']);
	$arrDados['cor'] 	   = $arrFiltros['cor'];
	$arrDados['fusid'] 	   = $fusid;
	$arrDados['fuspublico']= $fus['fuspublico'];
	$arrDados['contexto']  = $arrFiltros['contexto_filtro_avancado'];
	$arrDados['titulo']    = $fus['fustitulo'];
	echo simec_json_encode(codificaUTF8($arrDados));
	die;
}

function codificaUTF8($array = array())
{
	foreach($array as $key => $texto){
		$arrUTF8[$key] = ICONV( "ISO-8859-1", "UTF-8", $texto );
	}
	return $arrUTF8;
}

function salvarFiltrosAvancados()
{
	global $db;
		
	$fustitulo = utf8_decode($_POST['fustitulo']);
	$fuspublico = $_POST['fuspublico'] == "t" ? "true" : "false";
	$mapid = $_POST['mapid'];
	$fusid = $_POST['fusid'];
	$usucpf = $_SESSION['usucpf'];
	$fusfiltros = serialize($_POST);
	
	if($fusid != ""){
		$sql = "select usucpf from mapa.filtrousuario where fusid = $fusid";
		$usucpfBD = $db->pegaUm($sql);
		if($usucpfBD != $usucpf){
			$fusid = "";
		}
	}
	
	
	if($fusid != ""){
		$sql = "update 
					mapa.filtrousuario
				set
					fustitulo = '$fustitulo',
					fuspublico = $fuspublico,
					fusfiltros = '$fusfiltros'
				where
					fusid = $fusid;";
	}else{
		$sql = "insert into 
				mapa.filtrousuario 
			(fustitulo,fuspublico,mapid,usucpf,fusfiltros,fusstatus) 
				values 
			('$fustitulo',$fuspublico,$mapid,'$usucpf','$fusfiltros','A')";	
	}
	$db->executar($sql);
	$db->commit();
	listaFiltrosAvancados($mapid);
}

function salvarFiltrosNormais()
{
	global $db;
		
	$fustitulo = utf8_decode($_POST['fustitulo']);
	$fuspublico = $_POST['fuspublico'] == "t" ? "true" : "false";
	$mapid = $_POST['mapid'];
	$fusid = $_POST['fusid'];
	$usucpf = $_SESSION['usucpf'];
	$fusfiltros = serialize($_POST);
	
	if($fusid != ""){
		$sql = "select usucpf from mapa.filtrousuario where fusid = $fusid";
		$usucpfBD = $db->pegaUm($sql);
		if($usucpfBD != $usucpf){
			$fusid = "";
		}
	}
	
	
	if($fusid != ""){
		$sql = "update 
					mapa.filtrousuario
				set
					fustitulo = '$fustitulo',
					fuspublico = $fuspublico,
					fusfiltros = '$fusfiltros'
				where
					fusid = $fusid;";
	}else{
		$sql = "insert into 
				mapa.filtrousuario 
			(fustitulo,fuspublico,mapid,usucpf,fusfiltros,fusstatus,fustipo) 
				values 
			('$fustitulo',$fuspublico,$mapid,'$usucpf','$fusfiltros','A','N')";	
	}
	$db->executar($sql);
	$db->commit();
	listaFiltrosNormais($mapid);
}

function exibeComboBuscaAvancada()
{
	global $db;
	
	global $db;

	extract($_GET);
	if($camada != "" && $camada != "municipio"){
		$arrWhere[] = "tma.tpdid != 4";
	}
	
	$sql = "select
				tma.tmaid as codigo,
				tma.tmadsc as descricao
			from
				mapa.mapatema mpt
			inner join
				mapa.mapa map ON map.mapid = mpt.mapid
			inner join
				mapa.tema tma ON tma.tmaid = mpt.tmaid
			where
				map.mapid = $mapid
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			order by
				tma.tmadsc";

	$db->monta_combo("tmaid",$sql,"S","Selecione...","","","","200px","");
	
}

function exibeMunicipiosFiltroAvancado()
{
	global $db;

	//$muncod = "1200013,1200351,1200427,1200807,1200252,1200336,1200708,1200385,1200450,1200104,1200302,1200609,1200500";
	//$combo_parametro = "1";
	extract($_POST);
	if($muncod){
		$arrMuncod = explode(",",$muncod);
		if($arrMuncod){
			foreach($arrMuncod as $mun){
				if($mun){
					$arrMun[] = $mun;
				}
			}
		}
	}
	$arrMuncod = false;
	$muncod = false;
	
	$camada = !$camada ? "municipio" : $camada;
	
	$parametro = !$parametro ? "N/A" : utf8_decode($parametro);
	monta_titulo($parametro,"<a class='link' onclick='exportarMunicipiosFiltrosAvancado()' >Exportar para Excel</a>");
	?>
	<form name="form_excel" id="form_excel" method="post" >
		<?php if($arrMun): ?>
			<input type="hidden" name="hdn_excel_municipios" value="'<?php echo implode("','",$arrMun) ?>'"  />
			<input type="hidden" name="camada" value="<?php echo $camada ?>"  />
			<input type="hidden" name="parametro" value="<?php echo $parametro ?>"  />
			<input type="hidden" name="requisicao" value="exportarMunicipiosFiltrosAvancado"  />
			<?php if($hdn_tmaid): ?>
				<?php foreach($hdn_tmaid as $tmaidExcel): ?>
				<input type="hidden" name="hdn_tmaid[]" value="<?php echo $tmaidExcel ?>"  />
				<?php endforeach; ?>
			<?php endif; ?>
		<?php endif; ?>
	</form>
	<?php
	if(!$arrMun){
		echo "Não existem dados.";
		die;
	}else{
		if($camada != "municipio" && strstr($camada,"tipmunici_")){ //Tipo de Município
			$camada = str_replace("tipmunici_","",$camada);
			$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
			$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
			$arrWhere[] = "tpm.gtmid = $camada";
			$arrWhere[] = "tpm.tpmid in ('".implode("','",$arrMun)."') ";
			//$campo = "tpm.tpmid";
			$campo = "'<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"jQuery(\'#municipiosLegenda\').hide();localizaTipoMunicipio(\'' || tpm.tpmid|| '\',' || ST_X(ST_Centroid(tpmpoligono)) || ',' || ST_Y(ST_Centroid(tpmpoligono)) || ')\" />'";
			$descricao = "tpmdsc";
			$chave = "tpm.tpmid";
			$arrGroupBy[] = "tpm.tpmid";
			$arrGroupBy[] = "tpmdsc";
			$arrGroupBy[] = "tpmpoligono";
		}else{ //Município
			$arrWhere[] = "mun.muncod in ('".implode("','",$arrMun)."') ";
			$campo = "'<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"jQuery(\'#municipiosLegenda\').hide();localizaMapa2(\'' || mun.muncod || '\',' || ST_X(munlatlong) || ',' || ST_Y(munlatlong) || ')\" />'";
			$descricao = "mundescricao,mun.estuf";
			$chave = "mun.muncod";
		}
		$hdn_tmaid = array_unique($hdn_tmaid);
		if($hdn_tmaid){
			foreach($hdn_tmaid as $tmaid){
				if($tmaid != 1){
					$arrInnerJoin[] = "mapa.temadado  tem_{$tmaid} ON tem_{$tmaid}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$tmaid}.tmaid = $tmaid");
					$sql = "select 
								tmadsc,
								tpddsc,
								tpdcampotema 
							from 
								mapa.tipodado tpd
							inner join
								mapa.tema tma ON tma.tpdid = tpd.tpdid
							where
								tma.tmaid = $tmaid";
					$tdm = $db->pegaLinha($sql);				
					if($camada != "municipio"){
						if($tdm['tpddsc'] == "Boleano"){
							$arrCampos[] = "(CASE WHEN tem_{$tmaid}.{$tdm['tpdcampotema']} is true THEN 'Sim' else 'Não' END) as capmpo{$tmaid}";
							$arrGroupBy[] = "tem_{$tmaid}.{$tdm['tpdcampotema']}";
						}else{
							$arrCampos[] = "sum(tem_{$tmaid}.{$tdm['tpdcampotema']})".($tdm['tpddsc'] == "Quantitativo" ? "::integer" : "")." as capmpo{$tmaid}";
						}
					}else{
						if($tdm['tpddsc'] == "Boleano"){
							$arrCampos[] = "(CASE WHEN tem_{$tmaid}.{$tdm['tpdcampotema']} is true THEN 'Sim' else 'Não' END) as capmpo{$tmaid}";
						}else{
							$arrCampos[] = "tem_{$tmaid}.".$tdm['tpdcampotema'].($tdm['tpddsc'] == "Quantitativo" ? "::integer" : "")."  as capmpo{$tmaid}";
						}
					}
					$arrColunas[] = $tdm['tmadsc'];
				}else{
					if($camada != "municipio"){
						$arrCampos[] = "sum(mun.munpopulacao) as campo{$tmaid}";
					}else{
						$arrCampos[] = "mun.munpopulacao as campo{$tmaid}";	
					}
					$arrColunas[] = "População";
				}
			}
		}
				
		$sql = "select distinct
					$campo as acao,
					$chave as muncod,
					$descricao as descri
					".($arrCampos ? " , ".implode(" , ",$arrCampos) : "")."
				from 
					territoriosgeo.municipio mun
				".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")."
				inner join
					mapa.temadado pro ON mun.muncod = pro.muncod 
				where
					1=1
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")."
				order by
					$descricao";
		//dbg($sql,1);
		if($camada == "municipio"){
			$arrCabecalho = array("Ação","Município","UF");
		}else{
			$arrCabecalho = array("Ação","Descrição");	
		}
		

		if($arrColunas){
			foreach($arrColunas as $coluna){
				array_push($arrCabecalho,$coluna);
			}
		}
		
		$dados = $db->carregar($sql);
	
		if($dados){
		
			foreach($dados as $d){
					$arrMuncod[] = $d['muncod'];
			}
			$arrMunRep = array();
			foreach(array_count_values($arrMuncod) as $mun => $rep){
				if($rep > 1){
					$arrMunRep[] = $mun;
				}
			}
			
			foreach($dados as $d){
				if(in_array($d['muncod'],$arrMunRep) && ( ($tpdcampotema == "tmdboleano" && ($d['parametro'] == "f" || $d['parametro'] == "Não")) || ($tpdcampotema != "tmdboleano" && $d['parametro'] <= 0) ) ){
					
				}else{
					unset($d['muncod']);
					$arrDados[] = $d;
				}
			}
			
		}
		
		require_once(APPRAIZ."includes/classes/MontaListaAjax.class.inc");
		$ajax = new MontaListaAjax($db);
		$ajax->montaLista($arrDados,$arrCabecalho,50,5,"N","center",100);
		
	}
	
}

function exportarMunicipiosFiltrosAvancado()
{
	global $db;
	
	//$muncod = "1200013,1200351,1200427,1200807,1200252,1200336,1200708,1200385,1200450,1200104,1200302,1200609,1200500";
	//$combo_parametro = "1";
	
	extract($_REQUEST);
	
	$muncod = str_replace(array("\\","'"),array("",""),$hdn_excel_municipios);
		
	if($muncod){
		$arrMuncod = explode(",",$muncod);
		if($arrMuncod){
			foreach($arrMuncod as $mun){
				if($mun){
					$arrMun[] = $mun;
				}
			}
		}
	}
	$arrMuncod = false;
	$muncod = false;
	
	$camada = !$camada ? "municipio" : $camada;
	
	$parametro = !$parametro ? "N/A" : utf8_decode($parametro);
	
	if(!$arrMun){
		echo "Não existem dados.";
		die;
	}else{
		if($camada != "municipio" && strstr($camada,"tipmunici_")){ //Tipo de Município
			$camada = str_replace("tipmunici_","",$camada);
			$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
			$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
			$arrWhere[] = "tpm.gtmid = $camada";
			$arrWhere[] = "tpm.tpmid in ('".implode("','",$arrMun)."') ";
			//$campo = "tpm.tpmid";
			//$campo = "'<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"jQuery(\'#municipiosLegenda\').hide();localizaTipoMunicipio(\'' || tpm.tpmid|| '\',' || ST_X(ST_Centroid(tpmpoligono)) || ',' || ST_Y(ST_Centroid(tpmpoligono)) || ')\" />'";
			$descricao = "tpmdsc";
			$chave = "tpm.tpmid";
			$arrGroupBy[] = "tpm.tpmid";
			$arrGroupBy[] = "tpmdsc";
			//$arrGroupBy[] = "tpmpoligono";
		}else{ //Município
			$arrWhere[] = "mun.muncod in ('".implode("','",$arrMun)."') ";
			//$campo = "'<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"jQuery(\'#municipiosLegenda\').hide();localizaMapa2(\'' || mun.muncod || '\',' || ST_X(munlatlong) || ',' || ST_Y(munlatlong) || ')\" />'";
			$descricao = " mun.muncod || '' , mundescricao , mun.estuf";
			$chave = "mun.muncod";
		}
		$hdn_tmaid = array_unique($hdn_tmaid);
		if($hdn_tmaid){
			foreach($hdn_tmaid as $tmaid){
				if($tmaid != 1){
					$arrInnerJoin[] = "mapa.temadado  tem_{$tmaid} ON tem_{$tmaid}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$tmaid}.tmaid = $tmaid");
					$sql = "select 
								tmadsc,
								tpddsc,
								tpdcampotema 
							from 
								mapa.tipodado tpd
							inner join
								mapa.tema tma ON tma.tpdid = tpd.tpdid
							where
								tma.tmaid = $tmaid";
					$tdm = $db->pegaLinha($sql);				
					if($camada != "municipio"){
						if($tdm['tpddsc'] == "Boleano"){
							$arrCampos[] = "(CASE WHEN tem_{$tmaid}.{$tdm['tpdcampotema']} is true THEN 'Sim' else 'Não' END) as capmpo{$tmaid}";
							$arrGroupBy[] = "tem_{$tmaid}.{$tdm['tpdcampotema']}";
						}else{
							$arrCampos[] = "sum(tem_{$tmaid}.{$tdm['tpdcampotema']})".($tdm['tpddsc'] == "Quantitativo" ? "::integer" : "")." as capmpo{$tmaid}";
						}
					}else{
						if($tdm['tpddsc'] == "Boleano"){
							$arrCampos[] = "(CASE WHEN tem_{$tmaid}.{$tdm['tpdcampotema']} is true THEN 'Sim' else 'Não' END) as capmpo{$tmaid}";
						}else{
							$arrCampos[] = "tem_{$tmaid}.".$tdm['tpdcampotema'].($tdm['tpddsc'] == "Quantitativo" ? "::integer" : "")."  as capmpo{$tmaid}";
						}
					}
					$arrColunas[] = $tdm['tmadsc'];
				}else{
					if($camada != "municipio"){
						$arrCampos[] = "sum(mun.munpopulacao) as campo{$tmaid}";
					}else{
						$arrCampos[] = "mun.munpopulacao as campo{$tmaid}";	
					}
					$arrColunas[] = "População";
				}
			}
		}
				
		$sql = "select distinct
					$chave as muncod,
					$descricao as descri
					".($arrCampos ? " , ".implode(" , ",$arrCampos) : "")."
				from 
					territoriosgeo.municipio mun
				".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")."
				inner join
					mapa.temadado pro ON mun.muncod = pro.muncod 
				where
					1=1
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")."
				order by
					$descricao";
		//dbg($sql,1);
		if($camada == "municipio"){
			$arrCabecalho = array("IBGE","Município","UF");
		}else{
			$arrCabecalho = array("Descrição");
		}

		if($arrColunas){
			foreach($arrColunas as $coluna){
				array_push($arrCabecalho,$coluna);
			}
		}
		
		$dados = $db->carregar($sql);
	
		if($dados){
		
			foreach($dados as $d){
					$arrMuncod[] = $d['muncod'];
			}
			$arrMunRep = array();
			foreach(array_count_values($arrMuncod) as $mun => $rep){
				if($rep > 1){
					$arrMunRep[] = $mun;
				}
			}
			
			foreach($dados as $d){
				if(in_array($d['muncod'],$arrMunRep) && ( ($tpdcampotema == "tmdboleano" && ($d['parametro'] == "f" || $d['parametro'] == "Não")) || ($tpdcampotema != "tmdboleano" && $d['parametro'] <= 0) ) ){
					
				}else{
					unset($d['muncod']);
					$arrDados[] = $d;
				}
			}
			
		}
		
		ob_clean();
		header( 'Content-type: application/xls' );
		header( 'Content-Disposition: attachment; filename="planilha_simec.xls"' );
		$db->monta_lista_tabulado($arrDados,$arrCabecalho,10000,100,"N","100%","N");
		exit();
	}
	
}




function visualizarFiltrosAvancados()
{
	global $db;
	
	$camada = $_POST['camada'] ? $_POST['camada'] : "municipio";
	$mapid = $_POST['mapid'];
		
	if($_POST['estuf']){
		$arrEstados = explode(",",str_replace("estuf","",$_POST['estuf']));
		$arrWhereUF = " and mun.estuf in ('".implode("','",$arrEstados)."')";
	}
	$n = 0;
	foreach($_POST['hdn_tmaid'] as $tmaid){
		$arrInnerTema[] = "mapa.temadado  tem_{$tmaid} ON tem_{$tmaid}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$tmaid}.tmaid = $tmaid")." $arrWhereUF";
		$where = "";
		$where = " ".$_POST['contexto_filtro_avancado']." ";
		$arrRange = retornaRangeTema($tmaid,$mapid,$camada);
		if($_POST['range'][$tmaid]){
			$range = $arrRange[$_POST['range'][$tmaid]];
			$range = str_replace(array(".",",","%"),array("",".",""),$range);
			if(strstr($range,"-")){
				$range = str_replace("-"," and ", $range);
				$where.= " ".($tmaid == "1" ? "mun.munpopulacao" : "tem_{$tmaid}.tmdvalor")." between $range";
			}elseif(strstr("Não",$range) || strstr("Sim",$range)){
				$range = str_replace(array("Sim","Não"),array("true","false"),$range);
				$where.= " tem_{$tmaid}.tmdboleano is $range";
			}else{
				$where.= " ".($tmaid == "1" ? "mun.munpopulacao" : "tem_{$tmaid}.tmdvalor")." = '$range'";
			}
			$arrWhereTema[] = $where;
		}
		if($_POST['condicaoavancada'][$tmaid]){
			if($_POST['condicaoavancada'][$tmaid] == "entre"){
				$arrWhereTema[] = " ".($n > 0 ? $_POST['contexto_filtro_avancado'] : " and ")." ".($tmaid == "1" ? "mun.munpopulacao" : "tem_{$tmaid}.tmdvalor")." between ".str_replace(array(".",",","%"),array("",".",""),$_POST['qtdcondicaoavancadainicio'][$tmaid])." and ".str_replace(array(".",",","%"),array("",".",""),$_POST['qtdcondicaoavancadafim'][$tmaid]);
			}else{
				$arrWhereTema[] = " ".($n > 0 ? $_POST['contexto_filtro_avancado'] : " and ")." ".($tmaid == "1" ? "mun.munpopulacao" : "tem_{$tmaid}.tmdvalor")." ".$_POST['condicaoavancada'][$tmaid]." '".str_replace(array(".",",","%"),array("",".",""),$_POST['qtdcondicaoavancada'][$tmaid])."'";
			}
		}
		$n++;
		
	}
	
	if($camada != "municipio" && strstr($camada,"tipmunici_")){ //Tipo de Município
		$camada = str_replace("tipmunici_","",$camada);
		$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
		$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
		$arrWhere[] = "tpm.gtmid = $camada";
		$campo = "distinct tpm.tpmid";
	}else{ //Município
		$campo = "distinct mun.muncod";
	}
	
	$sql = "select 
				$campo as muncod
			from 
				territoriosgeo.municipio mun
			".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")." 
			".($arrInnerTema ? " inner join ".implode(" inner join ",$arrInnerTema) : "")."
			where
				1=1
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")." 
			".($arrWhereTema ? implode(" ",$arrWhereTema) : "")."
			".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")."
			";
	//dbg($sql,1);
	
	$dados = $db->carregar($sql);
	if($dados){	
		echo JSON_encode($dados);
	}
	
}

function limparMapaCompleto()
{
	global $db;
	$camada = $_POST['camada'];
	if($camada == "municipio" || !$camada){
		$sql = "select 
					muncod
				from 
					territoriosgeo.municipio";
	}else{
		$gtmid = str_replace("tipmunici_","",$camada);
		$sql = "select tpmid as muncod from territoriosgeo.tipomunicipio where gtmid = $gtmid";
	}
	$dados = $db->carregar($sql);
	echo JSON_encode($dados);
}

function visualizarFiltrosAvancadosMapa()
{
	global $db;
	
	extract($_POST);
	
	$camada = 'municipio';
	$_POST['agrupador'] = $camada_filtros_avancados ? $camada_filtros_avancados : "municipio";
	$_POST['agrupador'] = str_replace("tipmunici_","",$_POST['agrupador']);

	$estuf = str_replace("estuf","",$estuf);
	if($estuf){
		$arrUF = explode(",",$estuf);
		$arrWhereUF = "and mun.estuf in ('".implode("','",$arrUF)."')";
	}
	
	$n=1;
	foreach($cmb_tema as $grupo => $arrGrupo){
		foreach($arrGrupo as $tipoFiltro => $tmaid){
			$condicaoDentroGrupo = $cmb_grupo[$grupo][$tipoFiltro];
			$condicao = $cmb_filtro[$grupo][$tipoFiltro];
			$valorCondicaoInicio = str_replace(array(".",",","%"),array("",".",""),$cmb_filtro_inicio[$grupo][$tipoFiltro]);
			$valorCondicaoFim = str_replace(array(".",",","%"),array("",".",""),$cmb_filtro_fim[$grupo][$tipoFiltro]);
			
			$sql1 = "select tmaformula,tpdid from mapa.tema where tmaid = ".$cmb_tema[$grupo][$tipoFiltro];
			$arrTma = $db->pegaLinha($sql1);
			$tpdid = $arrTma['tpdid'];
			$tmaformula = $arrTma['tmaformula'];
			if($tpdid == 6){
				include_once APPRAIZ . 'www/painel/_funcoes_formula_tema.php';
				if($_POST['agrupador'] != "municipio"){
					if($_POST['agrupador'] == "regiao"){
						$formula_coluna = retornaFormulaPopUp($tmaformula,"regiao");
					}elseif($_POST['agrupador'] == "estado"){
						$formula_coluna = retornaFormulaPopUp($tmaformula,"estado");
					}else{
						$formula_coluna = retornaFormulaPopUp($tmaformula,"tpm");
					}
					$arrHaving[] = $formula_coluna['coluna'];
					$arrCampos[] = $formula_coluna['coluna']."  as coluna{$n} ";
					if($formula_coluna['leftjoin']){
						foreach($formula_coluna['leftjoin'] as $left){
							$arrInnerTema[] = $left;
						}
					}
					$arrWhereFormula[$n] = true;
				}
				if($_POST['agrupador'] == "municipio"){
					$tpdid = atualizaDadosTemaFormula($cmb_tema[$grupo][$tipoFiltro],$_POST['agrupador']);
				}
			}
			
			$arrCondicaoDentroGrupoHaving[] = $condicaoDentroGrupo;
			$arrCondicaoHaving[] = $condicao;
			$arrValorInicialHaving[] = $valorCondicaoInicio;
			$arrValorFinalHaving[] = $valorCondicaoFim;
			
			if($tpdid == 3){
				$arrInnerTema[] = "mapa.temadado  tem_{$n} ON tem_{$n}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$n}.tmaid = $tmaid")." $arrWhereUF";
				$arrCampos[] = "(CASE WHEN tem_{$n}.tmdboleano is true THEN 'Sim' ELSE 'Não' END) as coluna{$n}";
				$arrWhere[$grupo][] = "$condicaoDentroGrupo COALESCE(tem_{$n}.tmdboleano,false) is $condicao";
				$tmdsc = $db->pegaUm("select tmadsc from mapa.tema where tmaid = $tmaid");
				if($condicao == "true"){
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> $tmdsc ";
					$arrCabecalho[] = $tmdsc;
				}else{
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> Não $tmdsc ";
					$arrCabecalho[] = $tmdsc;	
				}
				if($tipoFiltro == 1){
					$condicaoGrupo = str_replace(array("and","or"),array("e ","ou "),$cmb_entregrupo[$grupo]);
					$arrCabecalho[] = $tmdsc;
				}
			}else{
				if($tmaid != "1"){
					$arrInnerTema[] = "mapa.temadado  tem_{$n} ON tem_{$n}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$n}.tmaid = $tmaid")." $arrWhereUF";
					if($_POST['agrupador'] != "municipio"){
						if($tpdid == 2 || $tpdid == 1){
							$arrCampos[] = "sum(COALESCE(tem_{$n}.tmdvalor,0))::integer as coluna{$n}";
							$arrHaving[] = "sum(COALESCE(tem_{$n}.tmdvalor,0))::integer";
						}else{
							if(!$formula_coluna){
								$arrCampos[] = "'<center>-</center>' as coluna{$n}";
							}else{
								$formula_coluna = false;
							}
						}
					}else{
						if($tpdid == 4){
							$arrCampos[] = "COALESCE(tem_{$n}.tmdvalor,0) as coluna{$n}";
						}else{
							$arrCampos[] = "COALESCE(tem_{$n}.tmdvalor,0)::integer as coluna{$n}";
						}	
					}
					if($condicao == "entre"){
						$arrWhere[$grupo][] = "$condicaoDentroGrupo COALESCE(tem_{$n}.tmdvalor,0) between $valorCondicaoInicio and $valorCondicaoFim";
					}else{
						$arrWhere[$grupo][] = "$condicaoDentroGrupo COALESCE(tem_{$n}.tmdvalor,0) $condicao $valorCondicaoInicio";
					}
				}else{
					$arrCampos[] = "mun.munpopulacao as coluna{$n}";
					if($condicao == "entre"){
						$arrWhere[$grupo][] = "$condicaoDentroGrupo mun.munpopulacao between $valorCondicaoInicio and $valorCondicaoFim";
					}else{
						$arrWhere[$grupo][] = "$condicaoDentroGrupo mun.munpopulacao $condicao $valorCondicaoInicio";
					}
				}	
				
				$tmdsc = $db->pegaUm("select tmadsc from mapa.tema where tmaid = $tmaid");
				
				if($condicao == "entre"){
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> $tmdsc entre $valorCondicaoInicio e $valorCondicaoFim";
				}else{
					switch($condicaoDentroGrupo){
						case "and":
						$condicao_extenso = "e";
						break;
						case "or":
						$condicao_extenso = "ou";
						break;
						default:
						$condicao_extenso = "";
						break;
					}
					$arrFormula[$grupo][] = "<b>$condicao_extenso</b> $tmdsc $condicao $valorCondicaoInicio";
				}
	
				$arrCabecalho[] = $tmdsc;
				if($tipoFiltro == 1){
					$arrCabecalho[1+(int)$grupo] = array();
					$condicaoGrupo = str_replace(array("and","or"),array("e ","ou "),$cmb_entregrupo[$grupo]);
					$arrCabecalho[1+(int)$grupo]["label"] = "$condicaoGrupo Grupo $grupo";
				}
			}
			$n++;
		}
	}
	
	foreach($arrWhere as $grupo => $arrGrupo){
		if($cmb_entregrupo[$grupo]){
			$sql.= " ) ".$cmb_entregrupo[$grupo]." (";
			switch($cmb_entregrupo[$grupo]){
				case "and":
				$condicao_extenso = "e";
				break;
				case "or":
				$condicao_extenso = "ou";
				break;
			}
			$formula.= " ) ".$condicao_extenso." (";
		}
		foreach($arrWhere[$grupo] as $where){
			$sql.= " ".$where." ";
		}
		foreach($arrFormula[$grupo] as $form){
			$formula.= " ".$form." ";
		}
	}
	$sql = "( $sql )";
	
	$sql = str_replace(array("and (  )","(  )","( )"),array("","(1=1)","(1=1)"),$sql);
	$sqlExterno = "( $sqlExterno )";
	$sqlExterno = str_replace(array("(  )","( )"),array("(1=1)","(1=1)"),$sqlExterno);

	switch($_POST['agrupador']){
		
		case "municipio":
			$sqlNovo = "select * from ( select distinct mun.muncod
			from 
				territoriosgeo.municipio mun
			".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
			where 1=1 ".($arrWhereUF ? $arrWhereUF :"") ." and  
			$sql";
			
			if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
				//$sqlNovo .= " order by mundesc";
			}
			$sqlNovo .= ") as foo where $sqlExterno";
		break;
		default:
			if($camada != $_POST['agrupador']){
				if($arrCampos){
					$n=1;
					if($arrWhere){
						foreach($arrWhere as $grupo => $arrW){
							foreach($arrW as $where){
								if($arrCondicaoHaving[$n-1] == "entre"){
									$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." ".$arrHaving[$n-1]." between ".$arrValorInicialHaving[$n-1]." and ".$arrValorFinalHaving[$n-1];
								}else{
									$arrHavingFinal[] = $arrCondicaoDentroGrupoHaving[$n-1]." ".$arrHaving[$n-1]." ".$arrCondicaoHaving[$n-1]." ".$arrValorInicialHaving[$n-1];
								}
								$n++;
							}
							$having_final.= " (".implode(" ",$arrHavingFinal).") ".$cmb_entregrupo[$grupo+1];
							unset($arrHavingFinal);
						}
					}
				}
				
				$arrInnerTema = array_unique($arrInnerTema);
				$sqlNovo = "
							select 
								tpm.tpmid as muncod
							from 
							territoriosgeo.municipio mun
							inner join
							territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod
							inner join
							territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
							".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
							where
							tpm.gtmid = {$_POST['agrupador']} and tpm.tpmstatus = 'A' ".($arrWhereUF ? $arrWhereUF : "")."
							group by tpm.tpmid
							".($having_final ? "having ".$having_final."" : "").";";
			}else{
				$sqlNovo = "select * from ( select distinct tpm.tpmdsc
				from 
					territoriosgeo.municipio mun
				inner join
					territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod
				inner join
					territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid 
				".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
				where
					tpm.gtmid = {$_POST['agrupador']} and tpm.tpmstatus = 'A' and ";
				
				$sqlNovo = str_replace( array( "mun.munpopulacao","COALESCE(" , ",0)" ) , array( "sum(mun.munpopulacao)","sum(COALESCE(" , ",0))" ) , $sqlNovo);
				
				$sqlNovo .= $sql . " group by tpm.tpmdsc,tpm.tpmid";
				
				if(!strstr(strtoupper($sqlNovo),"ORDER BY")){
					//$sqlNovo .= " order by tipo";
				}
				$sqlNovo .= ") as foo where $sqlExterno";
			}
	}
	//dbg($_POST);
	//dbg($sqlNovo,1);
	/*$sqlNovo = "select 
				mun.muncod
			from 
				territoriosgeo.municipio mun
			".($arrInnerTema ? " left join ".implode(" left join ",$arrInnerTema) : "")."
			where
			$sql";*/
	$dados = $db->carregar($sqlNovo);
	echo JSON_encode($dados);
}

function excluirFiltroAvancado()
{
	global $db;
	$fusid = $_POST['fusid'];
	$mapid = $_POST['mapid'];
	if($fusid && $mapid){
		$sql = "update 
					mapa.filtrousuario
				set
					fusstatus = 'I'
				where
					fusid = $fusid
				and
					mapid = $mapid;";
		$db->executar($sql);
		$db->commit();
		listaFiltrosAvancados($mapid);
	}
}
function excluirFiltroNormal()
{
	global $db;
	$fusid = $_POST['fusid'];
	$mapid = $_POST['mapid'];
	if($fusid && $mapid){
		$sql = "update 
					mapa.filtrousuario
				set
					fusstatus = 'I'
				where
					fusid = $fusid
				and
					mapid = $mapid;";
		$db->executar($sql);
		$db->commit();
		listaFiltrosNormais($mapid);
	}
}


function exportarMunicipiosLegendaExcel()
{
	global $db;
	
	//$muncod = "1200013,1200351,1200427,1200807,1200252,1200336,1200708,1200385,1200450,1200104,1200302,1200609,1200500";
	//$combo_parametro = "1";
	
	extract($_REQUEST);
		
	$camada = !$camada ? "municipio" : $camada;
	
	$parametro = !$parametro ? "N/A" : utf8_decode($parametro);
	
	$sql = "select
				tpdcampotema
			from
				 mapa.tema tma
			inner join
				mapa.tipodado tpd ON tpd.tpdid = tma.tpdid
			where
				tmaid = $combo_parametro";
	$tpdcampotema = $db->pegaUm($sql);
	
	if($combo_parametro == 1){
		$tpdcampotema = "munpopulacao";
	}
	
	switch($tpdcampotema)
	{
		case "tmdboleano":
			$coalesce = "false";
			$whereCampo = "";
		break;
		case "tmdtexto":
			$coalesce = "'N/A'";
			$whereCampo = "";
		break;
		default:
			$coalesce = "0";
			$whereCampo = " and $tpdcampotema >= 0 ";
	}
	
	//percent
	$sql = "select tpdid from mapa.tema where tmaid = $combo_parametro ";
	$tpdid = $db->pegaUm($sql);
	if($tpdid == 4){
		$percent = true;
	}else{
		$percent = false;
	}
		
	if(!$muncod){
		echo "Não existem dados.";
		die;
	}else{
		if($camada != "municipio" && strstr($camada,"tipmunici_")){ //Tipo de Município
			$camada = str_replace("tipmunici_","",$camada);
			$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
			$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
			$arrWhere[] = "tpm.gtmid = $camada";
			$arrWhere[] = "tpm.tpmid in ($muncod) ";
			//$campo = "'<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"jQuery(\'#municipiosLegenda\').hide();localizaTipoMunicipio(\'' || tpm.tpmid|| '\',' || ST_X(ST_Centroid(tpmpoligono)) || ',' || ST_Y(ST_Centroid(tpmpoligono)) || ')\" />'";
			$descricao = "tpmdsc";
			if($tpdcampotema != "tmdboleano"){
				$campoQtde = "sum($tpdcampotema)";
				$arrGroupBy[] = "tpm.tpmid";
				$arrGroupBy[] = "tpm.tpmdsc";
			}else{
				$campoQtde = "$tpdcampotema";
			}
			$chave = "tpm.tpmid";
		}else{ //Município
			$arrWhere[] = "mun.muncod in ('".str_replace(",","','",$muncod)."') ";
			//$campo = "'<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"jQuery(\'#municipiosLegenda\').hide();localizaMapa2(\'' || mun.muncod || '\',' || ST_X(munlatlong) || ',' || ST_Y(munlatlong) || ')\" />'";
			$descricao = "mun.muncod as ibge,mundescricao,mun.estuf";
			$campoQtde = "$tpdcampotema";
			$chave = "mun.muncod";
		}
		
		
		switch($tpdcampotema)
		{
			case "tmdboleano":
				$campoParametro = "COALESCE($campoQtde,$coalesce)";
			break;
			case "tmdtexto":
				if($percent){
					//$campoParametro = "to_char(COALESCE($campoQtde,$coalesce), '999g999g999g999d99')";
					$campoParametro = "COALESCE($campoQtde,$coalesce)::decimal(12,2)";
				}else{
					//$campoParametro = "replace(to_char(COALESCE($campoQtde,$coalesce), '999g999g999g999'),',','.') || ' '";
					$campoParametro = "COALESCE($campoQtde,$coalesce)::integer";
				}
			break;
			default:
				if($percent){
					//$campoParametro = "to_char(COALESCE($campoQtde,$coalesce), '999g999g999g999d99')";
					$campoParametro = "COALESCE($campoQtde,$coalesce)::decimal(12,2)";
				}else{
					//$campoParametro = "replace(to_char(COALESCE($campoQtde,$coalesce), '999g999g999g999'),',','.') || ' '";
					$campoParametro = "COALESCE($campoQtde,$coalesce)::integer";
				}
		}
		
		
		$sql = "select
					$chave as muncod,
					$descricao as descri,
					$campoParametro as parametro
					--'<span style=\"display:none\" >' || COALESCE($campoQtde,$coalesce) || '</span><div style=\"color:blue;width:100%;text-align:right\" >' || $campoParametro || '</div>' as parametro
				from 
					territoriosgeo.municipio mun
				".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")."
				left join
					mapa.temadado pro ON mun.muncod = pro.muncod and pro.tmaid = $combo_parametro 
				where
					1=1
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")."";
		//dbg($sql,1);	
		if($camada == "municipio"){
			$arrCabecalho = array("IBGE","Município","UF",$parametro);
		}else{
			$arrCabecalho = array("Descrição",$parametro);
		}

		$dados = $db->carregar($sql);
	
		if($dados){
		
			foreach($dados as $d){
					$arrMuncod[] = $d['muncod'];
			}
			$arrMunRep = array();
			foreach(array_count_values($arrMuncod) as $mun => $rep){
				if($rep > 1){
					$arrMunRep[] = $mun;
				}
			}
			
			foreach($dados as $d){
				if(in_array($d['muncod'],$arrMunRep) && ( ($tpdcampotema == "tmdboleano" && $d['parametro'] == "f") || ($tpdcampotema != "tmdboleano" && $d['parametro'] <= 0) ) ){
					
				}else{
					unset($d['muncod']);
					if($d['parametro'] == "f"){
						 $d['parametro'] = "Não";
					}
					if($d['parametro'] == "t"){
						 $d['parametro'] = "Sim";
					}
					$arrDados[] = $d;
				}
			}
			
		}
		ob_clean();
		header( 'Content-type: application/xls' );
		header( 'Content-Disposition: attachment; filename="planilha_simec.xls"' );
		$db->monta_lista_tabulado($arrDados,$arrCabecalho,10000,100,"N","100%","N");
		exit();
	}

}

function exibeMunicipiosLegenda()
{
	global $db;
	
	//$muncod = "1200013,1200351,1200427,1200807,1200252,1200336,1200708,1200385,1200450,1200104,1200302,1200609,1200500";
	//$combo_parametro = "1";
	extract($_POST);
		
	$camada = !$camada ? "municipio" : $camada;
	
	$parametro = !$parametro ? "N/A" : utf8_decode($parametro);
	monta_titulo($parametro,"<a class='link' onclick='exportarMunicipiosLegendaExcel($posicao)' >Exportar para Excel</a>");
	?>
	<form name="form_excel_municipios" id="form_excel_municipios" method="post" >
		<input type="hidden" name="requisicao" value="exportarMunicipiosLegendaExcel"  />
		<input type="hidden" name="muncod" value="<?php echo $muncod ?>"  />
		<input type="hidden" name="camada" value="<?php echo $camada ?>"  />
		<input type="hidden" name="parametro" value="<?php echo $parametro ?>"  />
		<input type="hidden" name="combo_parametro" value="<?php echo $combo_parametro ?>"  />
		<input type="hidden" name="posicao" value="<?php echo $posicao ?>"  />
	</form>
	<?php
	
	$sql = "select
				tpdcampotema
			from
				 mapa.tema tma
			inner join
				mapa.tipodado tpd ON tpd.tpdid = tma.tpdid
			where
				tmaid = $combo_parametro";
	$tpdcampotema = $db->pegaUm($sql);
	
	if($combo_parametro == 1){
		$tpdcampotema = "munpopulacao";
	}
	
	switch($tpdcampotema)
	{
		case "tmdboleano":
			$coalesce = "false";
			$whereCampo = "";
		break;
		case "tmdtexto":
			$coalesce = "'N/A'";
			$whereCampo = "";
		break;
		default:
			$coalesce = "0";
			$whereCampo = " and $tpdcampotema >= 0 ";
	}
	
	//percent
	$sql = "select tpdid from mapa.tema where tmaid = $combo_parametro ";
	$tpdid = $db->pegaUm($sql);
	if($tpdid == 4){
		$percent = true;
	}else{
		$percent = false;
	}
		
	if(!$muncod){
		echo "Não existem dados.";
		die;
	}else{
		if($camada != "municipio" && strstr($camada,"tipmunici_")){ //Tipo de Município
			$camada = str_replace("tipmunici_","",$camada);
			$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
			$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
			$arrWhere[] = "tpm.gtmid = $camada";
			$arrWhere[] = "tpm.tpmid in ($muncod) ";
			$campo = "'<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"jQuery(\'#municipiosLegenda\').hide();localizaTipoMunicipio(\'' || tpm.tpmid|| '\',' || ST_X(ST_Centroid(tpmpoligono)) || ',' || ST_Y(ST_Centroid(tpmpoligono)) || ')\" />'";
			$descricao = "tpmdsc";
			if($tpdcampotema != "tmdboleano"){
				$campoQtde = "sum($tpdcampotema)";
				$arrGroupBy[] = "tpm.tpmid";
				$arrGroupBy[] = "tpm.tpmdsc";
				$arrGroupBy[] = "tpm.tpmpoligono";
			}else{
				$campoQtde = "$tpdcampotema";
			}
			$chave = "tpm.tpmid";
		}else{ //Município
			$arrWhere[] = "mun.muncod in ('".str_replace(",","','",$muncod)."') ";
			$campo = "'<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer\" onclick=\"jQuery(\'#municipiosLegenda\').hide();localizaMapa2(\'' || mun.muncod || '\',' || ST_X(munlatlong) || ',' || ST_Y(munlatlong) || ')\" />'";
			$descricao = "mundescricao , mun.estuf";
			$campoQtde = "$tpdcampotema";
			$chave = "mun.muncod";
		}
		
		
		switch($tpdcampotema)
		{
			case "tmdboleano":
				$campoParametro = "COALESCE($campoQtde,$coalesce)";
			break;
			case "tmdtexto":
				if($percent){
					//$campoParametro = "to_char(COALESCE($campoQtde,$coalesce), '999g999g999g999d99')";
					$campoParametro = "COALESCE($campoQtde,$coalesce)::decimal(12,2)";
				}else{
					//$campoParametro = "replace(to_char(COALESCE($campoQtde,$coalesce), '999g999g999g999'),',','.') || ' '";
					$campoParametro = "COALESCE($campoQtde,$coalesce)::integer";
				}
			break;
			default:
				if($percent){
					//$campoParametro = "to_char(COALESCE($campoQtde,$coalesce), '999g999g999g999d99')";
					$campoParametro = "COALESCE($campoQtde,$coalesce)::decimal(12,2)";
				}else{
					//$campoParametro = "replace(to_char(COALESCE($campoQtde,$coalesce), '999g999g999g999'),',','.') || ' '";
					$campoParametro = "COALESCE($campoQtde,$coalesce)::integer";
				}
		}
		
		
		$sql = "select 
					$campo as acao,
					$chave as muncod,
					$descricao as descri,
					$campoParametro as parametro
					--'<span style=\"display:none\" >' || COALESCE($campoQtde,$coalesce) || '</span><div style=\"color:blue;width:100%;text-align:right\" >' || $campoParametro || '</div>' as parametro
				from 
					territoriosgeo.municipio mun
				".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")."
				left join
					mapa.temadado pro ON mun.muncod = pro.muncod and pro.tmaid = $combo_parametro 
				where
					1=1
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")."";
		//dbg($sql,1);

		if($camada == "municipio"){
			$arrCabecalho = array("Ação","Município","UF",$parametro);
		}else{
			$arrCabecalho = array("Ação","Descrição",$parametro);	
		}
		
		$dados = $db->carregar($sql);
	
		if($dados){
		
			foreach($dados as $d){
					$arrMuncod[] = $d['muncod'];
			}
			$arrMunRep = array();
			foreach(array_count_values($arrMuncod) as $mun => $rep){
				if($rep > 1){
					$arrMunRep[] = $mun;
				}
			}
			
			foreach($dados as $d){
				if(in_array($d['muncod'],$arrMunRep) && ( ($tpdcampotema == "tmdboleano" && $d['parametro'] == "f") || ($tpdcampotema != "tmdboleano" && $d['parametro'] <= 0) ) ){
					
				}else{
					unset($d['muncod']);
					if($d['parametro'] == "f"){
						 $d['parametro'] = "Não";
					}
					if($d['parametro'] == "t"){
						 $d['parametro'] = "Sim";
					}
					$arrDados[] = $d;
				}
			}
			
		}
		
		require_once(APPRAIZ."includes/classes/MontaListaAjax.class.inc");
		$ajax = new MontaListaAjax($db);
		$ajax->montaLista($arrDados,$arrCabecalho,50,5,"N","center",100);
		
	}
	
}

function pegaMapasCadastrados($usucpf = null,$die = true)
{
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	$pflcod = PAINEL_PERFIL_MAPAS;
	
	if($db->testa_superuser()) {
		
		$sql = "SELECT DISTINCT 
					m.mapid AS codigo, 
					m.mapdsc AS descricao
				FROM 
					mapa.mapa m  
				WHERE 
					m.mapstatus = 'A' ".((defined('MAP_FIXED'))?"AND m.mapid='".MAP_FIXED."'":"")."
				order by
					m.mapdsc";
		
	} else {
	
		$sql = "SELECT DISTINCT 
					m.mapid AS codigo, 
					m.mapdsc AS descricao
				FROM 
					painel.usuarioresponsabilidade ur 
				INNER JOIN 
					mapa.mapa m ON ur.mapid = m.mapid 
				WHERE 
					ur.rpustatus = 'A' 
				AND 
					ur.usucpf = '$usucpf' 
				AND 
					ur.pflcod = $pflcod 
				AND 
					m.mapstatus = 'A' ".((defined('MAP_FIXED'))?"AND m.mapid='".MAP_FIXED."'":"")."
				order by
					m.mapdsc";
	
	}
	
	$arrMapas = $db->carregar($sql);

	if(!$arrMapas){ ?>
		<table class="tabela" style="margin-bottom:20px" align="center" cellpadding="3" cellspacing="1" width="100%" >
			<tr>
				<td style="font-weight:bold;color:#000000;text-align:center" >Não existem mapas atribuídos ao seu perfil.</td>
			</tr>
		</table>
		<script>document.getElementById('aguarde').style.display='none'</script>
	<?php
	exit;
	}
	if(count($arrMapas) > 1){?>
		<table class="tabela" style="margin-bottom:20px" align="center" cellpadding="3" cellspacing="1" width="100%" >
			<tr>
				<td width="30%" class="SubtituloDireita" >Selecione o Mapa:</td>
				<td><?php $db->monta_combo("mapid",$arrMapas,"S","Selecione..","selecionaMapaUR","","","","","","",$_SESSION['painel_vars']['mapid']) ?></td>
			</tr>
			<?php if($_GET['mapid'] ||  $_SESSION['painel_vars']['mapid']): ?>
				<tr>
					<td class="SubTituloDireita">Camadas:</td>
					<td ><?
						$_GET['mapid'] = $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid'];
						$grupotm = $db->carregar("	SELECT 
														'tipmunici_'||gtm.gtmid as codigo,
														gtmdsc as descricao 
													FROM 
														territoriosgeo.grupotipomunicipio gtm
													INNER JOIN
														mapa.mapacamada mpc ON mpc.gtmid = gtm.gtmid 
													WHERE 
														mpc.mapid = {$_GET['mapid']}
													AND
														gtmstatus='A' 
													ORDER BY 
														gtmdsc");
						if(!$grupotm){
							$grupotm = $db->carregar("	SELECT 
														'tipmunici_'||gtm.gtmid as codigo,
														gtmdsc as descricao 
													FROM 
														territoriosgeo.grupotipomunicipio gtm
													WHERE 
														gtmstatus='A' 
													ORDER BY 
														gtmdsc");
						}
						$combo_camada[] = array('codigo'=>'municipio','descricao'=>'Municípios'); 
						if($grupotm[0]) {
							foreach($grupotm as $gtm) {
								$combo_camada[] = $gtm;
							}
						}
						$db->monta_combo('camada', $combo_camada, 'S', 'Selecione', '', '', '', '', 'N', 'camada');  ?>
						<input type="button" name="btn_carregar_camada" value="Carregar Camada"   onclick="carregarCamadaMapa()"  />
					</td>
				</tr>
			<?php endif; ?>
		</table>
		<script>
			document.getElementById('aguarde').style.display='none';
			function selecionaMapaUR(mapid)
			{
				if(!mapid)
				{
					alert('Favor selecionar o Mapa!');
					return false;
				}else{
					var url = '<?=LINK_PADRAO ?>&mapid=' + mapid;
					window.location.href=url;
				}
			}
		</script>
	<?php
	if($die){
		die;
	}
	}else{
		$_SESSION['painel_vars']['mapid'] = $arrMapas[0]['codigo'];
		?>
		<input type="hidden" name="camada">
		<table class="tabela" style="margin-bottom:20px" align="center" cellpadding="3" cellspacing="1" width="100%" >
			<tr>
				<td style="font-weight:bold;color:#000000;text-align:center" colspan="2" ><?php echo $arrMapas[0]['descricao'] ?></td>
			</tr>
			<?php if($_GET['mapid'] ||  $_SESSION['painel_vars']['mapid']): ?>
				<tr>
					<td class="SubTituloDireita">Camadas:</td>
					<td ><?
						$_GET['mapid'] = $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid'];
						$grupotm = $db->carregar("	SELECT 
														'tipmunici_'||gtm.gtmid as codigo,
														gtmdsc as descricao 
													FROM 
														territoriosgeo.grupotipomunicipio gtm
													INNER JOIN
														mapa.mapacamada mpc ON mpc.gtmid = gtm.gtmid 
													WHERE 
														mpc.mapid = {$_GET['mapid']}
													AND
														gtmstatus='A' 
													ORDER BY 
														gtmdsc");
						if(!$grupotm){
							$grupotm = $db->carregar("	SELECT 
														'tipmunici_'||gtm.gtmid as codigo,
														gtmdsc as descricao 
													FROM 
														territoriosgeo.grupotipomunicipio gtm
													WHERE 
														gtmstatus='A' 
													ORDER BY 
														gtmdsc");
						}
						$combo_camada[] = array('codigo'=>'municipio','descricao'=>'Municípios'); 
						if($grupotm[0]) {
							foreach($grupotm as $gtm) {
								$combo_camada[] = $gtm;
							}
						}
						$db->monta_combo('camada', $combo_camada, 'S', 'Selecione', '', '', '', '', 'N', 'camada');  ?>
						<input type="button" name="btn_carregar_camada" value="Carregar Camada"   onclick="carregarCamadaMapa()"  />
					</td>
				</tr>
			<?php endif; ?>

		</table>
		<?php
	}
	
}

function exibeComboTema()
{
	global $db;

	extract($_GET);
	if($camada != "" && $camada != "municipio"){
		$arrWhere[] = "tma.tpdid != 4";
	}
	
	$sql = "select
				tma.tmaid as codigo,
				tma.tmadsc as descricao
			from
				mapa.mapatema mpt
			inner join
				mapa.mapa map ON map.mapid = mpt.mapid
			inner join
				mapa.tema tma ON tma.tmaid = mpt.tmaid
			where
				map.mapid = $mapid
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			order by
				tma.tmadsc";
	
	 $db->monta_combo("combo_parametro",$sql,"S","Selecione...","carregaParametroCombo","","","200px","");
}

function gerarXml() {
	global $db;
	
	ini_set("memory_limit", "2048M");
	set_time_limit(0);
	
	$_arrSQL['xml']=true;
	
	$coluna = array();
	
	if($_REQUEST['icone']) {
		
		foreach($_REQUEST['icone'] as $ic) {
			
			$sql = "SELECT tpd.tpdtipo, tpd.tpdcampo, dli.dtiid, dli.dtidsc FROM mapa.detalheindicador dli 
					INNER JOIN mapa.tipodado tpd ON tpd.tpdid = dli.tpdid 
					INNER JOIN mapa.indicador ind ON ind.indid = dli.indid
					WHERE ind.indid='".$ic."' ORDER BY ind.inddsc, dli.dtiordem";
			
			$tipos = $db->carregar($sql);
			
			if($tipos[0]) {
				
				foreach($tipos as $tp) {
					
					$nomecoluna = $tp['tpdcampo']."_".$tp['dtiid'];
					
					$_arrSQL['d1'][] = "CASE WHEN di.dtiid=".$tp['dtiid']." THEN COALESCE({$tp['tpdcampo']},0) ELSE 0 END as {$nomecoluna}";
					$_arrSQL['d2'][] = "SUM({$nomecoluna}) as {$nomecoluna}";
					$_arrSQL['d3'][] = $nomecoluna;
					$_agp_relatorio[] = $nomecoluna;
					array_push( $coluna, array("campo" 	  => $nomecoluna,
			   								   "label" 	  => $tp['dtidsc'],
											   "type"	  => $tp['tpdtipo']
			   		   						   ) );
				}
			}
		}
	}
	
	$sql = montaSQL($_arrSQL);
	
	$dados = $db->carregar($sql);
	
	if($dados):
		
		$conteudo .= "<markers> "; // inicia o XML
		
		foreach($dados as $d):
										
			$ajuste = (($_position_ex[$d['lat']][$d['lng']])?($_position_ex[$d['lat']][$d['lng']]*0.0056):0);
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "info=\"". $d['info'] ."\" ";
			$conteudo .= "lat=\"". $d['lat'] ."\" ";
			$conteudo .= "lng=\"". ($d['lng']+$ajuste) ."\" ";
			$conteudo .= "icon=\"". $d['icon'] ."\" ";
			$conteudo .= "/> ";
			
			$_position_ex[$d['lat']][$d['lng']]++;
		
		endforeach;
		
		$conteudo .= "</markers> ";
		header ("Content-Type:text/xml");
		print $conteudo;
		unset($conteudo);
		
	endif;
}

//if(!function_exists("montaBalaoTipoMunicipio")) {

	function montaBalaoTipoMunicipio()
	{
		global $db;
		
		ini_set("memory_limit", "2048M");
		set_time_limit(0);
		//error_reporting(-1);
		
		echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
		echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
		
		extract($_REQUEST);
		
		
		if($_GET['mapid'] == 32){
			
			if($tpmid) {
			
			//Pegando o tipo (Estado, Região, etc.)
			$sql = "select * from territoriosgeo.grupotipomunicipio where gtmid = $gtmid";
			
			//Pegando a camada
			$sql = "select * from territoriosgeo.tipomunicipio where tpmid = $tpmid";
			$arrTpm = $db->pegaLinha($sql);
			
			//Pegando os Municípios do grupo
			$sql = "select distinct muncod from territoriosgeo.muntipomunicipio where tpmid = $tpmid";
			$arrMuncod = $db->carregarColuna($sql);
			
			//Pegando os Municípios do grupo
			$sql = "select distinct substr(muncod,1,6) as muncod from territoriosgeo.muntipomunicipio where tpmid = $tpmid";
			$arrMuncod6 = $db->carregarColuna($sql);
			
			}
			
			if($estuf) {

			//Pegando os Municípios do grupo
			$sql = "select distinct muncod from territoriosgeo.municipio where estuf = '$estuf'";
			$arrMuncod = $db->carregarColuna($sql);
			
			//Pegando os Municípios do grupo
			$sql = "select distinct substr(muncod,1,6) as muncod from territoriosgeo.municipio where estuf = '$estuf'";
			$arrMuncod6 = $db->carregarColuna($sql);
			
			$descricaoestado = $db->pegaUm("select estuf || ' / ' || estdescricao FROM territorios.estado where estuf = '$estuf'");
				
				
			}
			
			
			$sql = "select muncod, mundescricao from territoriosgeo.municipio where muncod in ('".($arrMuncod ? implode("','",$arrMuncod) : "000000")."') ";
			$arrMunicipios = $db->carregar($sql);
			
			foreach($arrMunicipios as $mun){
				//http://simec-local/painel/painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=montaBalao&muncod=3109303&mapid=32
				$url = "painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=montaBalao&muncod={$mun['muncod']}&mapid=32";
				$arrLinkMun[] = "<a href='$url' >{$mun['mundescricao']}</a>";
			}
			if($_REQUEST['muncodorigem']) $mundescricao = $db->pegaUm("select mundescricao from territorios.municipio where muncod='".$_REQUEST['muncodorigem']."'");
			
			if($gtmid) {
				monta_titulo($arrTpm['tpmdsc'],"<a href='painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=montaBalao&muncod={$_REQUEST['muncodorigem']}&mapid=32' >Voltar para ".$mundescricao."</a>");
				/*
				if($gtmid==1) {
					monta_titulo($arrTpm['tpmdsc'],"Municípios da Região: ".implode(", ",$arrLinkMun));
				} else {
					monta_titulo($arrTpm['tpmdsc'],"");
				}
				*/
			}
			
			if($estuf) {
				monta_titulo($descricaoestado,"<a href='painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=montaBalao&muncod={$_REQUEST['muncodorigem']}&mapid=32' >Voltar para ".$mundescricao."</a>");
				//monta_titulo($descricaoestado,"Municípios do Estado: ".implode(", ",$arrLinkMun));
			}
						
		
				?>
				<br/>
		
		<script>
function unidadesExpansaoPublica() {
	window.open('<?=LINK_PADRAO."&requisicao=instituicoesRedePublica".(($tpmid)?"&tpmid=".$tpmid:"").(($estuf)?"&estuf=".$estuf:"") ?>','instituicoespublicas','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
function unidadesExpansaoPrivada() {
	window.open('<?=LINK_PADRAO."&requisicao=instituicoesRedePrivada".(($tpmid)?"&tpmid=".$tpmid:"").(($estuf)?"&estuf=".$estuf:"") ?>','instituicoesprivadas','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
function unidadesMaisCinquentaLeitos() {
	window.open('<?=LINK_PADRAO."&requisicao=unidadesMaisCinquentaLeitos".(($tpmid)?"&tpmid=".$tpmid:"").(($estuf)?"&estuf=".$estuf:"") ?>','unidadescinquentaleitos','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
function  instituicoesresidenciamedica() {
	window.open('<?=LINK_PADRAO."&requisicao=instituicoesResidenciaMedica".(($tpmid)?"&tpmid=".$tpmid:"").(($estuf)?"&estuf=".$estuf:"") ?>','instituicoesresidenciamedica','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
</script>
		
		<br/>
		<table class="tabela" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
		<td align="center" colspan="2" valign="top">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr><td colspan="2" class="SubtituloCentro" >Médicos</td></tr>
			<?php 
					$sql = "select 
								tmadsc,
								sum(tmdvalor) as tmdvalor,
								tma.tmaid 
							from 
								mapa.temadado tm
							inner join
								mapa.tema tma ON tma.tmaid = tm.tmaid
							where
								tma.tmaid = 1
							and
								muncod in('".implode("','",$arrMuncod)."') 
								group by tmadsc, tma.tmaid
							";
					
						$arrDados = $db->carregar($sql);
						
						if($arrDados[0]){
							foreach($arrDados as $tema){
								$valor = $tema['tmdvalor'];
							}
						}
							
						echo "<tr><td width='70%' class='SubtituloDireita'>População</td><td align='right' >".number_format($valor,0,',','.')."</td></tr>";
						
						// gato para o caso de visualização por estado
						if($estuf) {
							
							$sql = "select 	replace(\"Nº de médicos por 1.000 hab. (2013)\",',','.') as medicos,
											replace(\"Nº de vagas por 10.000 hab. (2013)\",',','.') as vagas,
											replace(\"Relação de vagas por 10.000 pós Expansão - habitantes (2013\",',','.') as expansao
									from carga.medicos_relacao_vagas
									where trim(\"Região\") = '".$estuf."'";
					
							$tema = $db->pegaLinha($sql);
	
							if($tema){
								$medicos = $tema['medicos'];
								$vagas = $tema['vagas'];
								$expansao = $tema['expansao'];
							} else {
								$medicos = 0;
								$vagas = 0;
								$expansao = 0;
							}
							
							echo "<tr><td width='70%' class='SubtituloDireita'>Relação médico/1.000 habitantes </td><td align='right' >".number_format($medicos,2,',','.')."</td></tr>";
							echo "<tr><td width='70%' class='SubtituloDireita'>Relação vaga de graduação em medicina/10.000 habitantes</td><td align='right' >".number_format($vagas,2,',','.')."</td></tr>";
							echo "<tr><td width='70%' class='SubtituloDireita'>Relação vaga de graduação em medicina/10.000 após expansão de vagas</td><td align='right' >".number_format($expansao,2,',','.')."</td></tr>";
							
							$sql = "select sum(tmdvalor) as valor from mapa.temadado td
							inner join territorios.municipio m ON m.muncod = td.muncod
							where tmaid=656 and m.muncod in(select distinct muncod from territoriosgeo.municipio where estuf = '".$estuf."')";					
							$medicos_distribuicao = $db->pegaUm($sql);
							
							echo "<tr><td width='70%' class='SubtituloDireita'>Médicos - Distribuição dos 1.000 primeiros</td><td align='right' >".number_format($medicos_distribuicao,0,',','.')."</td></tr>";
							
						} else {
							
							$sql = "select sum(tmdvalor) as valor from mapa.temadado td
							inner join territorios.municipio m ON m.muncod = td.muncod
							where tmaid = 615 and m.muncod in('".implode("','",$arrMuncod)."')";					
							$vl_medicos = $db->pegaUm($sql);	
							
							echo "<tr><td width='70%' class='SubtituloDireita'>Médicos total</td><td align='right' >".number_format($vl_medicos,2,',','.')."</td></tr>";
	
							$sql = "select sum(tmdvalor) as valor from mapa.temadado td
							inner join territorios.municipio m ON m.muncod = td.muncod
							where tmaid=580 and m.muncod in('".implode("','",$arrMuncod)."')";					
							$vl_vagas_exis = $db->pegaUm($sql);
	
							$sql = "select sum(tmdvalor) as valor from mapa.temadado td
							inner join territorios.municipio m ON m.muncod = td.muncod
							where tmaid=624 and m.muncod in('".implode("','",$arrMuncod)."')";					
							$vl_vagas_exispos = $db->pegaUm($sql);	
							
							$sql = "select sum(tmdvalor) as valor from mapa.temadado td
							inner join territorios.municipio m ON m.muncod = td.muncod
							where tmaid=656 and m.muncod in('".implode("','",$arrMuncod)."')";					
							$medicos_distribuicao = $db->pegaUm($sql);

							$sql = "select sum(tmdvalor) as valor from mapa.temadado td
							inner join territorios.municipio m ON m.muncod = td.muncod
							where tmaid=615 and m.muncod in('".implode("','",$arrMuncod)."')";
							$tema_615 = $db->pegaUm($sql);
							
							$sql = "select sum(tmdvalor) as valor from mapa.temadado td
							inner join territorios.municipio m ON m.muncod = td.muncod
							where tmaid=655 and m.muncod in('".implode("','",$arrMuncod)."')";
							$tema_655 = $db->pegaUm($sql);
							if($tema_615 && $tema_655){
								$relacao_medicos_hab = ($tema_615/$tema_655)*1000;
							}else{
								$relacao_medicos_hab = 0;
							}
														
							echo "<tr><td width='70%' class='SubtituloDireita'>Relação médico/1.000 habitantes </td><td align='right' >".number_format($relacao_medicos_hab,2,',','.')."</td></tr>";
							echo "<tr><td width='70%' class='SubtituloDireita'>Relação vaga de graduação em medicina/10.000 habitantes</td><td align='right' >".number_format((($vl_vagas_exis*10000)/$valor),2,',','.')."</td></tr>";
							echo "<tr><td width='70%' class='SubtituloDireita'>Relação vaga de graduação em medicina/10.000 após expansão de vagas</td><td align='right' >".number_format((($vl_vagas_exispos*10000)/$valor),2,',','.')."</td></tr>";
							echo "<tr><td width='70%' class='SubtituloDireita'>Médicos - Distribuição dos 1.000 primeiros</td><td align='right' >".number_format($medicos_distribuicao,0,',','.')."</td></tr>";
							
						}
						
			?>
			</table>
		</td>
	</tr>
	<tr>
		<td align="center" width="50%" valign="top">
			<p><img src="../imagens/consultar.gif" style="cursor:pointer;" align="absmiddle" onclick="unidadesExpansaoPublica();"> <b>Expansão Pública</b></p>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr><td class="SubtituloCentro" >Criação</td></tr>
				<tr><td>
				<?php 
					$sql = "select tmadsc, coalesce(sum(tmdvalor),0) from mapa.tema t
								inner join mapa.temadado td ON td.tmaid = t.tmaid
							where t.tmaid in ( 573, 574, 578, 579 ) and muncod in('".implode("','",$arrMuncod)."')
							group by tmadsc";
					$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
				?>
				</td></tr>
				<tr><td class="SubtituloCentro" >Aumento</td></tr>
				<tr><td>
				<?php 
					$sql = "select tmadsc, coalesce(sum(tmdvalor),0) from mapa.tema t
							inner join mapa.temadado td ON td.tmaid = t.tmaid
							where t.tmaid in ( 571, 572, 575, 576, 577 ) and muncod in('".implode("','",$arrMuncod)."') 
							group by tmadsc;";
					$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
				?>
				</td></tr>
			</table>
		</td>
		<td align="center" width="50%" valign="top">
			<p><img src="../imagens/consultar.gif" style="cursor:pointer;" align="absmiddle" onclick="unidadesExpansaoPrivada();"> <b>Expansão Privada</b></p>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr><td class="SubtituloCentro" >Criação</td></tr>
				<tr><td>
				<?php 
					$sql = "select tmadsc, coalesce(sum(tmdvalor),0) from mapa.tema t
								inner join mapa.temadado td ON td.tmaid = t.tmaid
							where t.tmaid in ( 583, 584, 585, 588 ) and muncod in('".implode("','",$arrMuncod)."')
							group by tmadsc";
					$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
				?>
				</td></tr>
				<tr><td class="SubtituloCentro" >Aumento</td></tr>
				<tr><td>
				<?php 
					$sql = "select tmadsc, coalesce(sum(tmdvalor),0) from mapa.tema t
							inner join mapa.temadado td ON td.tmaid = t.tmaid
							where t.tmaid in ( 582, 586, 587 ) and muncod in('".implode("','",$arrMuncod)."') 
							group by tmadsc;";
					$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
				?>
				</td></tr>
			</table>
		</td>
	
	</tr>

	<tr>
		<td colspan=2 class="SubtituloCentro">Leitos SUS</td>
	</tr>
	<tr>
		<td colspan=2>
		<?
		$sql = "select tmadsc, round(coalesce(sum(tmdvalor),0),0) from mapa.tema t
				inner join mapa.temadado td ON td.tmaid = t.tmaid
				where t.tmaid in ( 593, 594, 595, 596, 597, 598, 599, 600 , 657) and muncod in('".implode("','",$arrMuncod)."')
				group by tmadsc, t.tmaid order by t.tmaid;";
		$arrDados = $db->carregar($sql);
		if($arrDados){
			foreach($arrDados as $dado){
				if(strstr($dado['tmadsc'],"Leitos Necessários (para expansão das vagas)")){
					$valor_necessario = $dado['valor'];
				}
				if(strstr($dado['tmadsc'],"Leitos Disponíveis - Necessários (Saldo)")){
					$valor_saldo = $dado['valor'];
				}
			}
		}
		if(strstr($valor_saldo,"-")){
			$valor_saldo = str_replace("-","",$valor_saldo);
			if((int)$valor_saldo > (int)$valor_necessario){
				if($arrDados){
					$n=0;
					foreach($arrDados as $dado){
						if(strstr($dado['tmadsc'],"Leitos Disponíveis - Necessários (Saldo)")){
							$arrDados[$n]['valor'] = $valor_necessario;
						}
						$n++;
					}
				}
			}
		}
		$arrDados = $arrDados ? $arrDados : array();
		$db->monta_lista_simples($arrDados,array(),10000,5,'N','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro">Equipes de Atenção Básica</td>
	</tr>
	<tr>
		<td colspan=2>
		<?
		$sql = "select tmadsc, round(coalesce(sum(tmdvalor),0),0) as valor from mapa.tema t
				inner join mapa.temadado td ON td.tmaid = t.tmaid
				where t.tmaid in ( 601, 602, 603, 604, 605 ) and muncod in('".implode("','",$arrMuncod)."')
				group by tmadsc;";
		$arrDados = $db->carregar($sql);
		if($arrDados){
			foreach($arrDados as $dado){
				if(strstr($dado['tmadsc'],"Equipes de Atenção Básica - Necessárias")){
					$valor_necessario = $dado['valor'];
				}
				if(strstr($dado['tmadsc'],"Equipes de Atenção Básica - Saldo")){
					$valor_saldo = $dado['valor'];
				}
			}
		}
		if(strstr($valor_saldo,"-")){
			$valor_saldo = str_replace("-","",$valor_saldo);
			if(1==1){
				if($arrDados){
					$n=0;
					foreach($arrDados as $dado){
						if(strstr($dado['tmadsc'],"Equipes de Atenção Básica - Saldo")){
							$arrDados[$n]['valor'] = $valor_necessario;
						}
						$n++;
					}
				}
			}
		}
		$arrDados = $arrDados ? $arrDados : array();
		$db->monta_lista_simples($arrDados,array(),10000,5,'N','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro">Vagas de Residência</td>
	</tr>
	<tr>
		<td colspan=2>
		<?
		$sql = "select \"Existentes_Cirurgia Geral\"::numeric as v ,
			  \"Existentes_Clínica Médica\"::numeric as x ,
			  \"Existentes_Ginecologia e Obstetrícia\"::numeric as o,
			  \"Existentes_Pediatria\"::numeric as p,
			  \"Existentes_Residência em Atenção Básica\"::numeric as r,
			  \"Existentes_Cirurgia Geral\"::numeric + \"Existentes_Clínica Médica\"::numeric + \"Existentes_Ginecologia e Obstetrícia\"::numeric + \"Existentes_Pediatria\"::numeric + \"Existentes_Residência em Atenção Básica\"::numeric as existentes_total,
				\"Ampliacao_Cirurgia Geral\"::numeric as a,
				  \"Ampliacao_Clínica Médica\"::numeric as b,
				  \"Ampliacao_Ginecologia e Obstetrícia\"::numeric as c,
				  \"Ampliacao_Pediatria\"::numeric as d,
				  \"Ampliacao_Residência em Atenção Básica \"::numeric as e,
				  \"Ampliacao_Cirurgia Geral\"::numeric + \"Ampliacao_Clínica Médica\"::numeric + \"Ampliacao_Ginecologia e Obstetrícia\"::numeric + \"Ampliacao_Pediatria\"::numeric + \"Ampliacao_Residência em Atenção Básica \"::numeric as ampliacao_total
				from carga.medicos_residencia_medica_expansao  where \"COD_MUN\" in('".implode("','",$arrMuncod6)."')";
		$vagasresidencia = $db->pegaLinha($sql);
		
		?>
		<table width="100%">
		<tr>
			<td width="50%">
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
					<tr><td colspan="2" class="SubtituloCentro" >Existentes</td></tr>
					<tr><td class="SubtituloDireita" >Cirurgia Geral</td><td><?=$vagasresidencia['v'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Clínica Médica</td><td><?=$vagasresidencia['x'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Ginecologia e Obstetrícia</td><td><?=$vagasresidencia['o'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Pediatria</td><td><?=$vagasresidencia['p'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Residência em Atenção Básica</td><td><?=$vagasresidencia['r'] ?></td></tr>
					<tr><td class="SubtituloDireita" >TOTAL</td><td><?=$vagasresidencia['existentes_total'] ?></td></tr>
			</table>
			</td>
			<td width="50%">
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
					<tr><td colspan="2" class="SubtituloCentro" >Ampliação</td></tr>
					<tr><td class="SubtituloDireita" >Cirurgia Geral</td><td><?=$vagasresidencia['a'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Clínica Médica</td><td><?=$vagasresidencia['b'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Ginecologia e Obstetrícia</td><td><?=$vagasresidencia['c'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Pediatria</td><td><?=$vagasresidencia['d'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Residência em Atenção Básica</td><td><?=$vagasresidencia['e'] ?></td></tr>
					<tr><td class="SubtituloDireita" >TOTAL</td><td><?=$vagasresidencia['ampliacao_total'] ?></td></tr>
			</table>
			</td>
		</tr>
		</table>

		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro">Relação Docente/Vaga necessária para expansão de vagas de gradução</td>
	</tr>
	<tr>
		<td colspan=2>
		<table width="100%">
		<tr>
			<td width="50%" valign="top">
			<p><b>Contratação de Professores</b></p>
			<?
			$sql = "select tmadsc, round(coalesce(sum(tmdvalor),0),0) from mapa.tema t
					inner join mapa.temadado td ON td.tmaid = t.tmaid
					where t.tmaid in ( 607, 608, 609) and muncod in('".implode("','",$arrMuncod)."')
					group by tmadsc, t.tmaid order by t.tmaid;";
			$db->monta_lista_simples($sql,array(),10000,5,'N','100%',$par2);
			?>
			</td>
			<td width="50%" valign="top">
			<p><b>Mestres e Doutores na Área da Saúde</b></p>
			<?
			$sql = "select tmadsc, round(coalesce(sum(tmdvalor),0),0) from mapa.tema t
					inner join mapa.temadado td ON td.tmaid = t.tmaid
					where t.tmaid in ( 610, 611, 612 ) and muncod in('".implode("','",$arrMuncod)."')
					group by tmadsc, t.tmaid order by t.tmaid;";
			$db->monta_lista_simples($sql,array(),10000,5,'N','100%',$par2);
			?>
			</td>
		</tr>
		</table>

		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="instituicoesresidenciamedica();"> Instituições que possuem residência médica no município</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro">Leitos SUS no município</td>
	</tr>
	<tr>
		<td colspan=2>
		
		<!-- INICIO TD LEITOS SUS MUN -->
		
			<style>
				.bold{font-weight:bold}
				.center{text-align:center}
				.right{text-align:right}
				.left{text-align:left}
			</style>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width=100%>
				<tr><td colspan="5" class="SubtituloCentro" >Leitos SUS</td></tr>
				<tr>
					<td></td>
					<td class="bold center" >Públicos</td>
					<td class="bold center" >Privados</td>
					<td class="bold center" >Não Lucrativos</td>
					<td class="bold center" >Total</td>
				</tr>
				<?php
					$sql = "select 
								tmadsc,
								tmdvalor 
							from 
								mapa.tema tma 
							left join
								mapa.temadado tm ON tma.tmaid = tm.tmaid
							where
								tma.tmaid between 628 and 648
							and
								muncod in('".implode("','",$arrMuncod)."')
							order by
								tma.tmaid";
					$arrDados = $db->carregar($sql);
					if($arrDados){
						foreach($arrDados as $n => $arrSus){
							if(strstr($arrSus['tmadsc'],"Cirúrgico")){
								$arrLeitosSUS['cirurgico']['tema'] = "Cirúrgico";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['cirurgico']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['cirurgico']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['cirurgico']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['cirurgico']['total'] = $arrLeitosSUS['cirurgico']['publico']+$arrLeitosSUS['cirurgico']['privado']+$arrLeitosSUS['cirurgico']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"Clínico")){
								$arrLeitosSUS['clinico']['tema'] = "Clínico";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['clinico']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['clinico']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['clinico']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['clinico']['total'] = $arrLeitosSUS['clinico']['publico']+$arrLeitosSUS['clinico']['privado']+$arrLeitosSUS['clinico']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"Obstétrico")){
								$arrLeitosSUS['obstetrico']['tema'] = "Obstétrico";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['obstetrico']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['obstetrico']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['obstetrico']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['obstetrico']['total'] = $arrLeitosSUS['obstetrico']['publico']+$arrLeitosSUS['obstetrico']['privado']+$arrLeitosSUS['obstetrico']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"Pediátrico")){
								$arrLeitosSUS['pediatrico']['tema'] = "Pediátrico";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['pediatrico']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['pediatrico']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['pediatrico']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['pediatrico']['total'] = $arrLeitosSUS['pediatrico']['publico']+$arrLeitosSUS['pediatrico']['privado']+$arrLeitosSUS['pediatrico']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"UTI")){
								$arrLeitosSUS['uti']['tema'] = "UTI";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['uti']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['uti']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['uti']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['uti']['total'] = $arrLeitosSUS['uti']['publico']+$arrLeitosSUS['uti']['privado']+$arrLeitosSUS['uti']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"Outros")){
								$arrLeitosSUS['outros']['tema'] = "Outros";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['outros']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['outros']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['outros']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['outros']['total'] = $arrLeitosSUS['outros']['publico']+$arrLeitosSUS['outros']['privado']+$arrLeitosSUS['outros']['nao_lucrativo'];
							}
						}
					}
					//dbg($arrLeitosSUS);
				?>
				<?php if($arrLeitosSUS): ?>
					<?php $i=0; foreach($arrLeitosSUS as $n => $arrSUS): ?>  
						<tr <?php echo ($i%2 == 0 ? "bgcolor='#f7f7f7'" : "") ?> >
							<td class="SubtituloDireita" ><?php echo $arrSUS['tema'] ?></td>
							<td align="right" ><?php echo number_format($arrSUS['publico'],0,',','.') ?></td>
							<td align="right" ><?php echo number_format($arrSUS['privado'],0,',','.') ?></td>
							<td align="right" ><?php echo number_format($arrSUS['nao_lucrativo'],0,',','.') ?></td>
							<td class="right bold" ><?php echo number_format($arrSUS['total'],0,',','.') ?></td>
						</tr>
						<?php
							$arrTotalSUS['total_publico']+=$arrSUS['publico'];
							$arrTotalSUS['total_privado']+=$arrSUS['privado'];
							$arrTotalSUS['total_nao_lucrativo']+=$arrSUS['nao_lucrativo'];
							$arrTotalSUS['total_geral']+=$arrSUS['total'];
						?>
					<?php $i++;endforeach; ?>
					<tr bgcolor="#f9f9f9" >
						<td class="bold SubtituloDireita" >Total</td>
						<td class="bold" align="right" ><?php echo number_format($arrTotalSUS['total_publico'],0,',','.') ?></td>
						<td class="bold" align="right" ><?php echo number_format($arrTotalSUS['total_privado'],0,',','.') ?></td>
						<td class="bold" align="right" ><?php echo number_format($arrTotalSUS['total_nao_lucrativo'],0,',','.') ?></td>
						<td class="bold" align="right" ><?php echo number_format($arrTotalSUS['total_geral'],0,',','.') ?></td>
					</tr>
				<?php else: ?>
					<tr>
						<td colspan="5" align="center" >Não existem registros.</td>
					</tr>
				<?php endif; ?>
			</table>
			
			<!-- FIM TD LEITOS SUS MUN -->
			
		</td>
	</tr>
	
	<tr>
		<td colspan=2 class="SubtituloCentro"><img src="../imagens/consultar.gif" style="cursor:pointer;" align="absmiddle" onclick="unidadesMaisCinquentaLeitos();"> Unidades com mais de 50 leitos no município</td>
	</tr>
	
</table>
				<?php

						
			die;
		}
		
		
		$sql = "select
					tma.tmaid
				from
					mapa.mapatema mpt
				inner join
					mapa.mapa map ON map.mapid = mpt.mapid
				inner join
					mapa.tema tma ON tma.tmaid = mpt.tmaid
				where
					map.mapid = {$_SESSION['painel_vars']['mapid']}
				and
					tpdid != 4
				and
					tma.tmaid not in (2,3) 
				group by 
					tma.tmaid, tma.tmadsc
				order by
					tma.tmadsc";
		$hdn_tmaid = $db->carregarColuna($sql);
		
		$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
		$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
		$arrWhere[] = "tpm.gtmid = $gtmid";
		$arrWhere[] = "tpm.tpmid = $tpmid";
		$campo = "tpm.tpmid";
		$descricao = "tpmdsc";
		$chave = "tpm.tpmid";
		$arrGroupBy[] = "tpm.tpmid";
		$arrGroupBy[] = "tpmdsc";
			
		if($hdn_tmaid){
			foreach($hdn_tmaid as $tmaid){
				if($tmaid != 1){
					$arrInnerJoin[] = "mapa.temadado  tem_{$tmaid} ON tem_{$tmaid}.muncod = mun.muncod ".($tmaid == "1" ? "" : "and tem_{$tmaid}.tmaid = $tmaid");
					$sql = "select 
								tmadsc,
								tpddsc,
								tpdcampotema 
							from 
								mapa.tipodado tpd
							inner join
								mapa.tema tma ON tma.tpdid = tpd.tpdid
							where
								tma.tmaid = $tmaid";
					$tdm = $db->pegaLinha($sql);
					if($tdm['tpddsc'] == "Boleano"){
						$arrCampos[] = "'<div style=\"width:100%;color:blue;text-align:right\" >' || (CASE WHEN tem_{$tmaid}.{$tdm['tpdcampotema']} is true THEN 'Sim' else 'Não' END) || '</div>' as capmpo{$tmaid}";
						$arrGroupBy[] = "tem_{$tmaid}.{$tdm['tpdcampotema']}";
					}else{
						$arrCampos[] = "'<div style=\"text-align:right;width:100%;color:blue\" >' || sum(tem_{$tmaid}.{$tdm['tpdcampotema']})".($tdm['tpddsc'] == "Quantitativo" ? "::integer" : "")." || '</div>' as capmpo{$tmaid}";	
					}
					$arrColunas[] = $tdm['tmadsc'];
				}else{
					$arrCampos[] = "'<div style=\"text-align:right;width:100%;color:blue\" >' || sum(mun.munpopulacao) || '</div>' as campo{$tmaid}";
					$arrColunas[] = "População";
				}
			}
		}
					
		$sql = "select distinct
					$campo as acao,
					$chave as muncod,
					$descricao as descri
					".($arrCampos ? " , ".implode(" , ",$arrCampos) : "")."
				from 
					territoriosgeo.municipio mun
				".($arrInnerJoin ? " left join ".implode(" left join ",$arrInnerJoin) : "")."
				where
					1=1
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")."
				order by
					$descricao";
		//dbg($sql,1);
		$arrCabecalho = array("Ação","Descrição");
	
		if($arrColunas){
			foreach($arrColunas as $coluna){
				array_push($arrCabecalho,$coluna);
			}
		}
		
		$dados = $db->pegaLinha($sql);
		
		if($dados){
			$n=0;
			foreach($dados as $chave => $dado){
				$n++;
				if($n > 3){
					$arrValores[] = $dado;
				}
			}
		}
		$n=0;
		foreach($arrColunas as $lista){
			$arrLista[$n] = array("descricao" => $lista, "valor" => $arrValores[$n]);
			$n++;
		}
		
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>{$dados['descri']}</td></tr>";
		echo "</table>";
		
		$sqlMun = "select 
					distinct muncod
				from
					territoriosgeo.muntipomunicipio
				where
					tpmid = $tpmid";
		$arrMuncod = $db->carregarColuna($sqlMun);
		
		$sql2 = "SELECT  CASE WHEN mpcdsc IS NOT NULL THEN dtidsc ||' ('|| mpcdsc ||')' else dtidsc END as dtidsc,
						'<div style=\"text-align:right;width:100%;color:blue\" >' ||CASE WHEN tpdid = 1 THEN replace(to_char(ROUND(sum(vlivalor),2), '999g999g999g999d99'),',','.')
							 WHEN tpdid = 2 THEN replace(to_char(ROUND(sum(vliqtd)), '999g999g999g999'),',','.') 
						END || '</div>' as valor
				FROM mapa.valorindicador vi 
				INNER JOIN mapa.detalheindicador di ON di.dtiid = vi.dtiid 
				LEFT JOIN mapa.mapacenario mpc ON mpc.mpcid = vi.mpcid  
				INNER JOIN mapa.indicador ind ON ind.indid = di.indid
				WHERE ind.indstatus = 'A' and di.tpdid!= 4 and di.tpdid!= 3 and vi.muncod in ('".implode("','",$arrMuncod)."') AND ind.mapid='".$_SESSION['painel_vars']['mapid']."' 
				group by mpcdsc,di.dtidsc,vi.vliboleano,ind.inddsc,di.dtiordem,tpdid
				ORDER BY ind.inddsc, di.dtiordem";
		//dbg($sql2);
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>".$db->pegaUm("SELECT mapdsc FROM mapa.mapa WHERE mapid='".$_SESSION['painel_vars']['mapid']."'")."</td></tr>";
		echo "</table>";
		$cabecalho = array("Detalhe","&nbsp;");
		$db->monta_lista_simples($sql2,$cabecalho,50,5,'N','100%',$par2);
		
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>Temas</td></tr>";
		echo "</table>";
		$cabecalho = array("Tema","&nbsp;");
		$db->monta_lista_simples($arrLista,$cabecalho,50,5,'N','100%',$par2);
	}

//}


function exibeMunicipiosInstituicao()
{
	global $db;

	$muncod = $_GET['muncod'];
	
	$arrMuncod = explode(",",$muncod);
	
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	monta_titulo("Municípios Supervisonados pela Instituição","&nbsp;");
	
	$sql = "select
				mundescricao,
				estuf
			from
				territorios.municipio
			where
				muncod in ('".implode("','",$arrMuncod)."')
			order by
				mundescricao";
	$arrCabecalho = array("Município","UF");
	$db->monta_lista($sql,$arrCabecalho,100,10,"N","center","N");
	
}
	
// if(!function_exists("montaBalao")) {

	function montaBalao() {
		global $db;
		
		echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
		echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
		
		echo "<script>
				function controleImg(obj,map) {
					if(obj.title=='mais') {
						obj.title='menos';
						obj.src='../imagens/menos.gif';
						document.getElementById('tr_'+map).style.display='';
					} else {
						obj.title='mais';
						obj.src='../imagens/mais.gif';
						document.getElementById('tr_'+map).style.display='none';
					}
				}
			  </script>";
		
		if($_GET['mapid'] == 32):
		
		$sql = "select 
					tpm.tpmid, tpmdsc
				from 
					territoriosgeo.muntipomunicipio mtp
				inner join
					territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtp.tpmid
				where 
					muncod = '{$_REQUEST['muncod']}'
				and
					gtmid = 1 ";
		$arrRS = $db->pegaLinha($sql);
		
		$sql = "select 
					estuf
				from 
					territorios.municipio
				where 
					muncod = '{$_REQUEST['muncod']}'";
		$UF = $db->pegaUm($sql);
		
		
		$url = "painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=montaBalaoTipoMunicipio&gtmid=1&tpmid={$arrRS['tpmid']}&muncodorigem={$_REQUEST['muncod']}&mapid={$_REQUEST['mapid']}";
		
		$url2 = "painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=montaBalaoTipoMunicipio&estuf={$UF}&muncodorigem={$_REQUEST['muncod']}&mapid={$_REQUEST['mapid']}";
		
		monta_titulo($db->pegaUm("select mundescricao from territorios.municipio where muncod = '{$_REQUEST['muncod']}' "),"Região da Saúde: <a href='$url'>".$arrRS['tpmdsc']."</a><br/>UF: <a href='$url2'>".$UF."</a>");
		
		?>
		<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script>
function unidadesExpansaoPublica() {
	window.open('<?=LINK_PADRAO."&requisicao=instituicoesRedePublica&muncod=".$_REQUEST['muncod'] ?>','instituicoespublicas','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
function unidadesExpansaoPrivada() {
	window.open('<?=LINK_PADRAO."&requisicao=instituicoesRedePrivada&muncod=".$_REQUEST['muncod'] ?>','instituicoesprivadas','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
function unidadesMaisCinquentaLeitos() {
	window.open('<?=LINK_PADRAO."&requisicao=unidadesMaisCinquentaLeitos&muncod=".$_REQUEST['muncod'] ?>','unidadescinquentaleitos','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
function  instituicoesresidenciamedica() {
	window.open('<?=LINK_PADRAO."&requisicao=instituicoesResidenciaMedica&muncod=".$_REQUEST['muncod'] ?>','instituicoesresidenciamedica','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function exibeMunIns(id){
	var img = jQuery("#img_instituicao_"+id).attr("src");
	if(img == "../imagens/mais.gif"){
		jQuery("#tr_mun_inst_"+id).show();
		jQuery("#img_instituicao_"+id).attr("src","../imagens/menos.gif")
	}else{
		jQuery("#tr_mun_inst_"+id).hide();
		jQuery("#img_instituicao_"+id).attr("src","../imagens/mais.gif")
	}
}

function abreMunInsPopUp(muncod)
{
	janela('painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=exibeMunicipiosInstituicao&muncod='+muncod,800,600);
}

</script>
		<style>
			.blue{color:blue};
			.bold{font-weight:bold};
		</style>
		<br/>
		<?php 
		$sql = "select * from mapa.instituicaomunicipio where muncod = '{$_REQUEST['muncod']}'";
		$arrInstituicoes = $db->carregar($sql);
		?>
		<?php if($arrInstituicoes): ?>
		<table class="tabela" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
		<td align="center" colspan="2" valign="top">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="SubtituloCentro" >Ação</td>
					<td class="SubtituloCentro" >Instituição Supervisora</td>
					<td class="SubtituloCentro" >Regiões de Saúde</td>
					<td class="SubtituloCentro" >Qtde. Municípios</td>
				</tr>
				<?php foreach($arrInstituicoes as $cor => $ins): ?>
					<?php 
					$sql = "select inmid,mun.muncod,
									'<a href=\"javascript: window.parent.localizaMapa2(\'' || mun.muncod || '\',' || 
										--############### LONGITUDE ###################### --
							            CASE WHEN (SPLIT_PART(munmedlog, '.', 1) <>'' AND SPLIT_PART(munmedlog, '.', 2) <>'' AND split_part(munmedlog, '.', 3) <>'') THEN
							               ((split_part(munmedlog, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlog, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlog, '.', 1)::int))*(-1)
							            ELSE
							                -- Valores do IBGE convertidos em  decimal
							               (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1)
							            END 
									|| ',' ||
									--############### LATITUDE ###################### --
											CASE WHEN (SPLIT_PART(munmedlat, '.', 1) <>'' AND SPLIT_PART(munmedlat, '.', 2) <>'' AND split_part(munmedlat, '.', 3) <>'') THEN
								               CASE WHEN split_part(munmedlat, '.', 4) <>'N' THEN
								                   (((split_part(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int)))*(-1)
								                ELSE
								                   ((SPLIT_PART(munmedlat, '.', 3)::double precision / 3600) +(SPLIT_PART(munmedlat, '.', 2)::double precision / 60) + (SPLIT_PART(munmedlat, '.', 1)::int))
								               END
								            ELSE
								            -- Valores do IBGE convertidos em  decimal
								            CASE WHEN (length (munmedlat)=8) THEN
								                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
								                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
								                ELSE
								                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
								                END
								            ELSE
								                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
								                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
								                ELSE
								                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
								                END
								            END
								            END  
									|| ')\" >' || mun.mundescricao || '</a>' as link 
									from 
										mapa.instituicaomunicipio ins inner join territorios.municipio mun ON mun.muncod = ins.muncod where inmdsc = '{$ins['inmdsc']}' order by mundescricao";
					$arrMunicipiosInstituicao = $db->carregar($sql);
					$arrMuncosInstituicao = false;
					$arrDescInstituicao = false;
					foreach($arrMunicipiosInstituicao as $m){
						$arrMuncosInstituicao[] = $m['muncod'];
						$arrDescInstituicao[] = $m['link'];
					}
					
					if($arrMuncosInstituicao){
						$sql = "select distinct tpmdsc from territoriosgeo.tipomunicipio tpo inner join territoriosgeo.muntipomunicipio mun ON mun.tpmid = tpo.tpmid where muncod in ('".implode("','",$arrMuncosInstituicao)."') and gtmid = 1";
						$arrRegioesSaudeInstituicao = $db->carregarColuna($sql);
					}
					$arrRegioesSaudeInstituicao = $arrRegioesSaudeInstituicao ? $arrRegioesSaudeInstituicao : array();
										
					?>
					 
					<tr bgcolor='<?php echo $cor%2 ? "#FFFFFF" : "" ?>' >
						<td>
							<span style="white-space:nowrap;" >
								<!-- 
								<img src="../imagens/mais.gif" id="img_instituicao_<?php echo $ins['inmid']?>" onclick="exibeMunIns('<?php echo $ins['inmid']?>')"   />
								 -->
								<input type="checkbox" name="chk_instituicao" onclick="window.parent.f_mudacores('<?php echo implode(",",$arrMuncosInstituicao)?>',this);" />
							</span>
						</td>
						<td><?php echo $ins['inmdsc']?></td>
						<td><?php echo implode(", ",$arrRegioesSaudeInstituicao)?></td>
						<td align="right" class="blue" ><a href="javascript:abreMunInsPopUp('<?php echo implode(",",$arrMuncosInstituicao) ?>')" ><?php echo number_format(count($arrDescInstituicao),0,',','.'); ?></a></td>
					</tr>
					 <!-- 
					<tr bgcolor='<?php echo $cor%2 ? "#FFFFFF" : "" ?>' id="tr_mun_inst_<?php echo $ins['inmid']?>" style="display:none" >
						<td colspan="4" >
						<?php $i = 0; ?>
							<table class="tabela" cellSpacing="1" cellPadding="3" align="center" width="100%" >
								<?php for($j=0;$j<(count($arrDescInstituicao)/4);$j++):?>
									<tr>
										<?php for($i;$i<4;$i++): ?>
											<td><?php echo $arrDescInstituicao[$i] ?></td>
										<?php endfor; ?>
									</tr>
								<?php endfor; ?>
							</table>
						</td>
					</tr>
					 -->
				<?php endforeach;?>
			</table>
		</td>
		</tr>
		</table>
		<?php endif; ?>
		
		<br/>
		<table class="tabela" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
		<td align="center" colspan="2" valign="top">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr><td colspan="2" class="SubtituloCentro" >Médicos</td></tr>
			<?php 
					$sql = "select 
								tmadsc,
								tmdvalor,
								tma.tmaid 
							from 
								mapa.temadado tm
							inner join
								mapa.tema tma ON tma.tmaid = tm.tmaid
							where
								tma.tmaid = 1
							and
								muncod = '{$_REQUEST['muncod']}'
							";

						$arrDados = $db->carregar($sql);
						
						if($arrDados[0]){
							foreach($arrDados as $tema){
								$valor = $tema['tmdvalor'];
							}
						}
							
						echo "<tr><td width='70%' class='SubtituloDireita'>População</td><td align='right' >".number_format($valor,0,',','.')."</td></tr>";
						
					$sql = "select 
								estuf
							from 
								territorios.municipio
							where
								muncod = '{$_REQUEST['muncod']}'
							";

						$UF = $db->pegaUm($sql);	


						$sql = "select sum(tmdvalor) as valor from mapa.temadado td
						inner join territorios.municipio m ON m.muncod = td.muncod
						where tmaid = 615 and m.muncod = '{$_REQUEST['muncod']}'";					
						$vl_medicos = $db->pegaUm($sql);	
						
						echo "<tr><td width='70%' class='SubtituloDireita'>Médicos total</td><td align='right' >".number_format($vl_medicos,2,',','.')."</td></tr>";

						$sql = "select sum(tmdvalor) as valor from mapa.temadado td
						inner join territorios.municipio m ON m.muncod = td.muncod
						where tmaid=580 and m.muncod = '{$_REQUEST['muncod']}'";					
						$vl_vagas_exis = $db->pegaUm($sql);

						$sql = "select sum(tmdvalor) as valor from mapa.temadado td
						inner join territorios.municipio m ON m.muncod = td.muncod
						where tmaid=624 and m.muncod = '{$_REQUEST['muncod']}'";					
						$vl_vagas_exispos = $db->pegaUm($sql);	
						
						$sql = "select sum(tmdvalor) as valor from mapa.temadado td
						inner join territorios.municipio m ON m.muncod = td.muncod
						where tmaid=656 and m.muncod = '{$_REQUEST['muncod']}'";
						$medicos_distribuicao = $db->pegaUm($sql);

						$sql = "select sum(tmdvalor) as valor from mapa.temadado td
						inner join territorios.municipio m ON m.muncod = td.muncod
						where tmaid=614 and m.muncod = '{$_REQUEST['muncod']}'";		
						$relacao_medicos_hab = $db->pegaUm($sql);
						
						/*
						
						$sql = "select 	\"Nº de médicos por 1.000 hab. (2013)\" as medicos,
										\"Nº de vagas por 10.000 hab. (2013)\" as vagas,
										\"Relação de vagas por 10.000 pós Expansão - habitantes (2013\" as expansao
								from carga.medicos_relacao_vagas
								where trim(\"Região\") = '".$UF."'";
				
						$tema = $db->pegaLinha($sql);

						if($tema){
							$medicos = $tema['medicos'];
							$vagas = $tema['vagas'];
							$expansao = $tema['expansao'];
						} else {
							$medicos = 0;
							$vagas = 0;
							$expansao = 0;
						}
						*/
						echo "<tr><td width='70%' class='SubtituloDireita'>Relação médico/1.000 habitantes </td><td align='right' >".number_format($relacao_medicos_hab,2,',','.')."</td></tr>";
						echo "<tr><td width='70%' class='SubtituloDireita'>Relação vaga de graduação em medicina/10.000 habitantes</td><td align='right' >".number_format((($vl_vagas_exis*10000)/$valor),2,',','.')."</td></tr>";
						echo "<tr><td width='70%' class='SubtituloDireita'>Relação vaga de graduação em medicina/10.000 após expansão de vagas</td><td align='right' >".number_format((($vl_vagas_exispos*10000)/$valor),2,',','.')."</td></tr>";
						echo "<tr><td width='70%' class='SubtituloDireita'>Médicos - Distribuição dos 1.000 primeiros</td><td align='right' >".number_format($medicos_distribuicao,0,',','.')."</td></tr>";
						
			?>
			</table>
		</td>
	</tr>
	<tr>
		<td align="center" width="50%" valign="top">
			<p><img src="../imagens/consultar.gif" style="cursor:pointer;" align="absmiddle" onclick="unidadesExpansaoPublica();"> <b>Expansão Pública</b></p>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr><td class="SubtituloCentro" >Criação</td></tr>
				<tr><td>
				<?php 
					$sql = "select tmadsc, coalesce(sum(tmdvalor),0) from mapa.tema t
								inner join mapa.temadado td ON td.tmaid = t.tmaid
							where t.tmaid in ( 573, 574, 578, 579 ) and muncod = '{$_REQUEST['muncod']}'
							group by tmadsc";
					$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
				?>
				</td></tr>
				<tr><td class="SubtituloCentro" >Aumento</td></tr>
				<tr><td>
				<?php 
					$sql = "select tmadsc, coalesce(sum(tmdvalor),0) from mapa.tema t
							inner join mapa.temadado td ON td.tmaid = t.tmaid
							where t.tmaid in ( 571, 572, 575, 576, 577 ) and muncod = '{$_REQUEST['muncod']}' 
							group by tmadsc;";
					$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
				?>
				</td></tr>
			</table>
		</td>
		<td align="center" width="50%" valign="top">
			<p><img src="../imagens/consultar.gif" style="cursor:pointer;" align="absmiddle" onclick="unidadesExpansaoPrivada();"> <b>Expansão Privada</b></p>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr><td class="SubtituloCentro" >Criação</td></tr>
				<tr><td>
				<?php 
					$sql = "select tmadsc, coalesce(sum(tmdvalor),0) from mapa.tema t
								inner join mapa.temadado td ON td.tmaid = t.tmaid
							where t.tmaid in ( 583, 584, 585, 588 ) and muncod = '{$_REQUEST['muncod']}'
							group by tmadsc";
					$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
				?>
				</td></tr>
				<tr><td class="SubtituloCentro" >Aumento</td></tr>
				<tr><td>
				<?php 
					$sql = "select tmadsc, coalesce(sum(tmdvalor),0) from mapa.tema t
							inner join mapa.temadado td ON td.tmaid = t.tmaid
							where t.tmaid in ( 582, 586, 587 ) and muncod = '{$_REQUEST['muncod']}' 
							group by tmadsc;";
					$db->monta_lista_simples($sql,$cabecalho,10000,5,'S','100%',$par2);
				?>
				</td></tr>
			</table>
		</td>
	
	</tr>
	
	<tr>
		<td colspan=2 class="SubtituloCentro">
		<?
		$sql = "select leitos_observacao from carga.medicos_planilha_base where muncod = '{$_REQUEST['muncod']}';";
		$leitos_observacao = $db->pegaUm($sql);
		?>
		<? if($leitos_observacao):?>
			<script>
				function exibeLeitosObservacao()
				{
					window.open('<?=LINK_PADRAO."&requisicao=exibeLeitosObservacao&muncod=".$_REQUEST['muncod'] ?>','exibeLeitosObservacao','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
				}
			</script>
			<img src='../imagens/consultar.gif' style="cursor:pointer"  onclick="exibeLeitosObservacao(<?=$_REQUEST['muncod']?>)"  />
		<? endif;?>
		Leitos SUS
		</td>
	</tr>
	<tr>
		<td colspan=2>
		<?
		$sql = "select tmadsc, round(coalesce(sum(tmdvalor),0),0) as valor from mapa.tema t
				inner join mapa.temadado td ON td.tmaid = t.tmaid
				where t.tmaid in ( 593, 594, 595, 596, 597, 598, 599, 600 ,657) and muncod = '{$_REQUEST['muncod']}'
				group by tmadsc, t.tmaid order by t.tmaid;";
		$arrDados = $db->carregar($sql);
		if($arrDados){
			foreach($arrDados as $dado){
				if(strstr($dado['tmadsc'],"Leitos Necessários (para expansão das vagas)")){
					$valor_necessario = $dado['valor'];
				}
				if(strstr($dado['tmadsc'],"Leitos Disponíveis - Necessários (Saldo)")){
					$valor_saldo = $dado['valor'];
				}
			}
		}
		if(strstr($valor_saldo,"-")){
			$valor_saldo = str_replace("-","",$valor_saldo);
			if((int)$valor_saldo > (int)$valor_necessario){
				if($arrDados){
					$n=0;
					foreach($arrDados as $dado){
						if(strstr($dado['tmadsc'],"Leitos Disponíveis - Necessários (Saldo)")){
							$arrDados[$n]['valor'] = $valor_necessario;
						}
						$n++;
					}
				}
			}
		}
		$arrDados = $arrDados ? $arrDados : array();
		$db->monta_lista_simples($arrDados,array(),10000,5,'N','100%',$par2);
		?>
		
		<? if($leitos_observacao):?>
			<b><?=$leitos_observacao;?></b>
		<? endif;?>
		
		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro">Equipes de Atenção Básica</td>
	</tr>
	<tr>
		<td colspan=2>
		<?
		$sql = "select tmadsc, round(coalesce(sum(tmdvalor),0),0) as valor from mapa.tema t
				inner join mapa.temadado td ON td.tmaid = t.tmaid
				where t.tmaid in ( 601, 602, 603, 604, 605 ) and muncod = '{$_REQUEST['muncod']}'
				group by tmadsc;";
		$arrDados = $db->carregar($sql);
		if($arrDados){
			foreach($arrDados as $dado){
				if(strstr($dado['tmadsc'],"Equipes de Atenção Básica - Necessárias")){
					$valor_necessario = $dado['valor'];
				}
				if(strstr($dado['tmadsc'],"Equipes de Atenção Básica - Saldo")){
					$valor_saldo = $dado['valor'];
				}
			}
		}
		if(strstr($valor_saldo,"-")){
			$valor_saldo = str_replace("-","",$valor_saldo);
			if(1==1){
				if($arrDados){
					$n=0;
					foreach($arrDados as $dado){
						if(strstr($dado['tmadsc'],"Equipes de Atenção Básica - Saldo")){
							$arrDados[$n]['valor'] = $valor_necessario;
						}
						$n++;
					}
				}
			}
		}
		$arrDados = $arrDados ? $arrDados : array();
		$db->monta_lista_simples($arrDados,array(),10000,5,'N','100%',$par2);
		?>
		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro">Vagas de Residência</td>
	</tr>
	<tr>
		<td colspan=2>
		<?
		$sql = "select \"Existentes_Cirurgia Geral\"::numeric as v ,
			  \"Existentes_Clínica Médica\"::numeric as x ,
			  \"Existentes_Ginecologia e Obstetrícia\"::numeric as o,
			  \"Existentes_Pediatria\"::numeric as p,
			  \"Existentes_Residência em Atenção Básica\"::numeric as r,
			  \"Existentes_Cirurgia Geral\"::numeric + \"Existentes_Clínica Médica\"::numeric + \"Existentes_Ginecologia e Obstetrícia\"::numeric + \"Existentes_Pediatria\"::numeric + \"Existentes_Residência em Atenção Básica\"::numeric as existentes_total,
				\"Ampliacao_Cirurgia Geral\"::numeric as a,
				  \"Ampliacao_Clínica Médica\"::numeric as b,
				  \"Ampliacao_Ginecologia e Obstetrícia\"::numeric as c,
				  \"Ampliacao_Pediatria\"::numeric as d,
				  \"Ampliacao_Residência em Atenção Básica \"::numeric as e,
				  \"Ampliacao_Cirurgia Geral\"::numeric + \"Ampliacao_Clínica Médica\"::numeric + \"Ampliacao_Ginecologia e Obstetrícia\"::numeric + \"Ampliacao_Pediatria\"::numeric + \"Ampliacao_Residência em Atenção Básica \"::numeric as ampliacao_total
				from carga.medicos_residencia_medica_expansao  where \"COD_MUN\" = '".substr($_REQUEST['muncod'],0,6)."'";
		$vagasresidencia = $db->pegaLinha($sql);
		
		?>
		<table width="100%">
		<tr>
			<td width="50%">
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
					<tr><td colspan="2" class="SubtituloCentro" >Existentes</td></tr>
					<tr><td class="SubtituloDireita" >Cirurgia Geral</td><td><?=$vagasresidencia['v'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Clínica Médica</td><td><?=$vagasresidencia['x'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Ginecologia e Obstetrícia</td><td><?=$vagasresidencia['o'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Pediatria</td><td><?=$vagasresidencia['p'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Residência em Atenção Básica</td><td><?=$vagasresidencia['r'] ?></td></tr>
					<tr><td class="SubtituloDireita" >TOTAL</td><td><?=$vagasresidencia['existentes_total'] ?></td></tr>
			</table>
			</td>
			<td width="50%">
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
					<tr><td colspan="2" class="SubtituloCentro" >Ampliação</td></tr>
					<tr><td class="SubtituloDireita" >Cirurgia Geral</td><td><?=$vagasresidencia['a'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Clínica Médica</td><td><?=$vagasresidencia['b'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Ginecologia e Obstetrícia</td><td><?=$vagasresidencia['c'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Pediatria</td><td><?=$vagasresidencia['d'] ?></td></tr>
					<tr><td class="SubtituloDireita" >Residência em Atenção Básica</td><td><?=$vagasresidencia['e'] ?></td></tr>
					<tr><td class="SubtituloDireita" >TOTAL</td><td><?=$vagasresidencia['ampliacao_total'] ?></td></tr>
			</table>
			</td>
		</tr>
		</table>

		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro">Relação Docente/Vaga necessária para expansão de vagas de gradução</td>
	</tr>
	<tr>
		<td colspan=2>
		<table width="100%">
		<tr>
			<td width="50%" valign="top">
			<p><b>Contratação de Professores</b></p>
			<?
			$sql = "select tmadsc, round(coalesce(sum(tmdvalor),0),0) from mapa.tema t
					inner join mapa.temadado td ON td.tmaid = t.tmaid
					where t.tmaid in ( 607, 608, 609) and muncod = '{$_REQUEST['muncod']}'
					group by tmadsc, t.tmaid order by t.tmaid;";
			$db->monta_lista_simples($sql,array(),10000,5,'N','100%',$par2);
			?>
			</td>
			<td width="50%" valign="top">
			<p><b>Mestres e Doutores na Área da Saúde</b></p>
			<?
			$sql = "select tmadsc, round(coalesce(sum(tmdvalor),0),0) from mapa.tema t
					inner join mapa.temadado td ON td.tmaid = t.tmaid
					where t.tmaid in ( 610, 611, 612 ) and muncod = '{$_REQUEST['muncod']}'
					group by tmadsc, t.tmaid order by t.tmaid;";
			$db->monta_lista_simples($sql,array(),10000,5,'N','100%',$par2);
			?>
			</td>
		</tr>
		</table>

		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro"><img src="../imagens/consultar.gif" align="absmiddle" style="cursor:pointer;" onclick="instituicoesresidenciamedica();"> Instituições que possuem residência médica no município</td>
	</tr>
	<tr>
		<td colspan=2 class="SubtituloCentro">Leitos SUS no município</td>
	</tr>
	<tr>
		<td colspan=2>
		
		<!-- INICIO TD LEITOS SUS MUN -->
		
			<style>
				.bold{font-weight:bold}
				.center{text-align:center}
				.right{text-align:right}
				.left{text-align:left}
			</style>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width=100%>
				<tr><td colspan="5" class="SubtituloCentro" >Leitos SUS</td></tr>
				<tr>
					<td></td>
					<td class="bold center" >Públicos</td>
					<td class="bold center" >Privados</td>
					<td class="bold center" >Não Lucrativos</td>
					<td class="bold center" >Total</td>
				</tr>
				<?php
					$sql = "select 
								tmadsc,
								tmdvalor 
							from 
								mapa.tema tma 
							left join
								mapa.temadado tm ON tma.tmaid = tm.tmaid
							where
								tma.tmaid between 628 and 648
							and
								muncod = '{$_REQUEST['muncod']}'
							order by
								tma.tmaid";
					$arrDados = $db->carregar($sql);
					if($arrDados){
						foreach($arrDados as $n => $arrSus){
							if(strstr($arrSus['tmadsc'],"Cirúrgico")){
								$arrLeitosSUS['cirurgico']['tema'] = "Cirúrgico";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['cirurgico']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['cirurgico']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['cirurgico']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['cirurgico']['total'] = $arrLeitosSUS['cirurgico']['publico']+$arrLeitosSUS['cirurgico']['privado']+$arrLeitosSUS['cirurgico']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"Clínico")){
								$arrLeitosSUS['clinico']['tema'] = "Clínico";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['clinico']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['clinico']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['clinico']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['clinico']['total'] = $arrLeitosSUS['clinico']['publico']+$arrLeitosSUS['clinico']['privado']+$arrLeitosSUS['clinico']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"Obstétrico")){
								$arrLeitosSUS['obstetrico']['tema'] = "Obstétrico";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['obstetrico']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['obstetrico']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['obstetrico']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['obstetrico']['total'] = $arrLeitosSUS['obstetrico']['publico']+$arrLeitosSUS['obstetrico']['privado']+$arrLeitosSUS['obstetrico']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"Pediátrico")){
								$arrLeitosSUS['pediatrico']['tema'] = "Pediátrico";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['pediatrico']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['pediatrico']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['pediatrico']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['pediatrico']['total'] = $arrLeitosSUS['pediatrico']['publico']+$arrLeitosSUS['pediatrico']['privado']+$arrLeitosSUS['pediatrico']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"UTI")){
								$arrLeitosSUS['uti']['tema'] = "UTI";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['uti']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['uti']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['uti']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['uti']['total'] = $arrLeitosSUS['uti']['publico']+$arrLeitosSUS['uti']['privado']+$arrLeitosSUS['uti']['nao_lucrativo'];
							}
							if(strstr($arrSus['tmadsc'],"Outros")){
								$arrLeitosSUS['outros']['tema'] = "Outros";
								if(strstr($arrSus['tmadsc'],"úblico")){
									$arrLeitosSUS['outros']['publico'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"rivado")){
									$arrLeitosSUS['outros']['privado'] = $arrSus['tmdvalor'];
								}
								if(strstr($arrSus['tmadsc'],"ucrativo")){
									$arrLeitosSUS['outros']['nao_lucrativo'] = $arrSus['tmdvalor'];
								}
								$arrLeitosSUS['outros']['total'] = $arrLeitosSUS['outros']['publico']+$arrLeitosSUS['outros']['privado']+$arrLeitosSUS['outros']['nao_lucrativo'];
							}
						}
					}
					//dbg($arrLeitosSUS);
				?>
				<?php if($arrLeitosSUS): ?>
					<?php $i=0; foreach($arrLeitosSUS as $n => $arrSUS): ?>  
						<tr <?php echo ($i%2 == 0 ? "bgcolor='#f7f7f7'" : "") ?> >
							<td class="SubtituloDireita" ><?php echo $arrSUS['tema'] ?></td>
							<td align="right" ><?php echo number_format($arrSUS['publico'],0,',','.') ?></td>
							<td align="right" ><?php echo number_format($arrSUS['privado'],0,',','.') ?></td>
							<td align="right" ><?php echo number_format($arrSUS['nao_lucrativo'],0,',','.') ?></td>
							<td class="right bold" ><?php echo number_format($arrSUS['total'],0,',','.') ?></td>
						</tr>
						<?php
							$arrTotalSUS['total_publico']+=$arrSUS['publico'];
							$arrTotalSUS['total_privado']+=$arrSUS['privado'];
							$arrTotalSUS['total_nao_lucrativo']+=$arrSUS['nao_lucrativo'];
							$arrTotalSUS['total_geral']+=$arrSUS['total'];
						?>
					<?php $i++;endforeach; ?>
					<tr bgcolor="#f9f9f9" >
						<td class="bold SubtituloDireita" >Total</td>
						<td class="bold" align="right" ><?php echo number_format($arrTotalSUS['total_publico'],0,',','.') ?></td>
						<td class="bold" align="right" ><?php echo number_format($arrTotalSUS['total_privado'],0,',','.') ?></td>
						<td class="bold" align="right" ><?php echo number_format($arrTotalSUS['total_nao_lucrativo'],0,',','.') ?></td>
						<td class="bold" align="right" ><?php echo number_format($arrTotalSUS['total_geral'],0,',','.') ?></td>
					</tr>
				<?php else: ?>
					<tr>
						<td colspan="5" align="center" >Não existem registros.</td>
					</tr>
				<?php endif; ?>
			</table>
			
			<!-- FIM TD LEITOS SUS MUN -->
			
		</td>
	</tr>
	
	<tr>
		<td colspan=2 class="SubtituloCentro"><img src="../imagens/consultar.gif" style="cursor:pointer;" align="absmiddle" onclick="unidadesMaisCinquentaLeitos();"> Unidades com mais de 50 leitos no município</td>
	</tr>
	
</table>
		<?php
				
		die;
		endif;
		
		$municipio = $db->pegaUm("SELECT mundescricao||' / '||estuf FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod']."'");
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>".$municipio."</td></tr>";
		echo "</table>";
		
		$sql = "SELECT  CASE WHEN mpcdsc IS NOT NULL THEN dtidsc ||' ('|| mpcdsc ||')' else dtidsc END as dtidsc,
						'<div style=\"text-align:right;width:100%;color:blue\" >' ||CASE WHEN vlivalor IS NOT NULL THEN replace(to_char(ROUND(vlivalor,2), '999g999g999g999d99'),',','.')
							 WHEN vliqtd IS NOT NULL THEN replace(to_char(ROUND(vliqtd), '999g999g999g999'),',','.') 
							 WHEN vliboleano IS NOT NULL THEN CASE WHEN vliboleano=TRUE THEN 'Sim' ELSE 'Não' END 
						END || '</div>' as valor
				FROM mapa.valorindicador vi 
				INNER JOIN mapa.detalheindicador di ON di.dtiid = vi.dtiid 
				LEFT JOIN mapa.mapacenario mpc ON mpc.mpcid = vi.mpcid  
				INNER JOIN mapa.indicador ind ON ind.indid = di.indid
				WHERE ind.indstatus = 'A' and vi.muncod='".$_REQUEST['muncod']."' AND ind.mapid='".$_SESSION['painel_vars']['mapid']."' 
				ORDER BY dtidsc, ind.inddsc, di.dtiordem";
		
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>".$db->pegaUm("SELECT mapdsc FROM mapa.mapa WHERE mapid='".$_SESSION['painel_vars']['mapid']."'")."</td></tr>";
		echo "</table>";
		$cabecalho = array("Detalhe","&nbsp;");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		
		$sql = "SELECT tmadsc,
						'<div style=\"text-align:right;width:100%;color:blue\" >' || CASE WHEN tmdvalor IS NOT NULL THEN to_char(ROUND(tmdvalor,2), '999g999g999g999d99')
						WHEN tmdboleano IS NOT NULL THEN CASE WHEN tmdboleano=TRUE THEN 'Sim' ELSE 'Não' END 
						WHEN tmdtexto IS NOT NULL THEN tmdtexto 
						END || '</div>'as valor  
				FROM mapa.temadado td 
				INNER JOIN mapa.tema tm ON tm.tmaid = td.tmaid
				INNER JOIN mapa.mapatema mtm ON tm.tmaid = mtm.tmaid 
				WHERE muncod='".$_REQUEST['muncod']."' and mtm.mapid = ".$_REQUEST['mapid']." order by tmadsc";
		
		echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
		echo "<tr><td class=SubTituloCentro>Dados Gerais</td></tr>";
		echo "</table>";
		$cabecalho = array("Tema","&nbsp;");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		
		
		
		// outros mapas 
		
		$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
		$pflcod = PAINEL_PERFIL_MAPAS;
		
		//pegando os grupos mapas
		$sql = "SELECT gm.gpmdsc, gm.gpmid FROM mapa.mapagrupomapa mg 
		 		INNER JOIN mapa.grupomapa gm ON gm.gpmid = mg.gpmid   
		 		WHERE mg.mapid='".$_SESSION['painel_vars']['mapid']."' order by gm.gpmdsc";
		$gruposdomapa = $db->carregar($sql);
		
		if($gruposdomapa[0]) {
			foreach($gruposdomapa as $gdm) {
				echo "<p><b>Outros Mapas - ".$gdm['gpmdsc']."</b></p>";
				if($db->testa_superuser()) {
					$sql_m = "SELECT m.mapid FROM mapa.mapa m 
							  INNER JOIN mapa.mapagrupomapa mgm ON mgm.mapid = m.mapid  
							  WHERE m.mapstatus = 'A' AND m.mapid!='".$_SESSION['painel_vars']['mapid']."' AND mgm.gpmid='{$gdm['gpmid']}'";
				} else {
					$sql_m = "SELECT m.mapid FROM painel.usuarioresponsabilidade ur 
							  INNER JOIN mapa.mapa m ON ur.mapid = m.mapid 
							  INNER JOIN mapa.mapagrupomapa mgm ON mgm.mapid = m.mapid 
							  WHERE	ur.rpustatus = 'A' AND mgm.gpmid='{$gdm['gpmid']}' AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod AND m.mapstatus = 'A' AND m.mapid!='".$_SESSION['painel_vars']['mapid']."'";
				}
				
				$sql = "SELECT  ind.mapid,
				 				CASE WHEN mpcdsc IS NOT NULL THEN dtidsc ||' ('|| mpcdsc ||')' else dtidsc END as dtidsc,
								CASE WHEN vlivalor IS NOT NULL THEN ROUND(vlivalor,2)::text
									 WHEN vliqtd IS NOT NULL THEN ROUND(vliqtd)::text
									 WHEN vliboleano IS NOT NULL THEN CASE WHEN vliboleano=TRUE THEN '<center>Sim</center>' ELSE '<center>Não</center>' END 
								END as valor
						FROM mapa.valorindicador vi 
						INNER JOIN mapa.detalheindicador di ON di.dtiid = vi.dtiid 
						LEFT JOIN mapa.mapacenario mpc ON mpc.mpcid = vi.mpcid  
						INNER JOIN mapa.indicador ind ON ind.indid = di.indid 
						INNER JOIN mapa.mapa map ON map.mapid = ind.mapid
						WHERE vi.muncod='".$_REQUEST['muncod']."' AND ind.mapid IN({$sql_m}) 
						ORDER BY map.mapdsc,dtidsc,ind.inddsc,di.dtiordem";
				$mapas = $db->carregar($sql);
				
				if($mapas[0]) {
					foreach($mapas as $ma) {
						$arrMapas[$ma['mapid']][] = array("dtidsc"=>$ma['dtidsc'],"valor"=>$ma['valor']); 
					}
				}
				if($arrMapas) {
					foreach($arrMapas as $mapid => $dadosMapa) {
						echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
						echo "<tr><td style=width:10px;><img src=../imagens/mais.gif style=cursor:pointer; title=mais onclick=controleImg(this,'".$mapid."');></td><td class=SubTituloEsquerda>".$db->pegaUm("SELECT mapdsc FROM mapa.mapa WHERE mapid='".$mapid."'")."</td></tr>";
						echo "<tr style=display:none; id=tr_".$mapid."><td colspan=2>";
						$cabecalho = array("Detalhe","&nbsp;");
						$db->monta_lista_simples($dadosMapa,$cabecalho,50,5,'N','100%',$par2);
						echo "</td></tr>";
						echo "</table>";
			
					}
				}
				
				
			}
		}
		
	
	}

// }

function montaSQL($dados = false) {
	global $db;
	
	$filtro[] = "m.munlatlong IS NOT NULL";
	
	if( $_REQUEST['estuf'][0] ){
		$filtro[] = "m.estuf in ('".implode("','",$_REQUEST['estuf'])."') ";
	}
	if( $_REQUEST['muncod'][0] ){
		$filtro[] = "m.muncod in ('".implode("','",$_REQUEST['muncod'])."') ";
	}
	
	if( $_REQUEST['icone'][0] ){
		$filtro[] = "ind.indid in ('".implode("','",$_REQUEST['icone'])."') ";
		
		$filtrob = array();
		if($_REQUEST['cond']) {
			foreach($_REQUEST['icone'] as $indid ) {
				
				if($_REQUEST['cond'][$indid]=="com"){
					$condicao=">0";
				}elseif($_REQUEST['cond'][$indid]=="sem"){
					$condicao="=0";
				}
				
				$nomeColuna = $db->pegaUm("SELECT t.tpdcampo||'_'||i.dtiid as nomecoluna FROM mapa.indicador i 
										   INNER JOIN mapa.detalheindicador d ON d.dtiid = i.dtiid 
							 			   INNER JOIN mapa.tipodado t ON t.tpdid = d.tpdid WHERE i.indid='".$indid."'");
				
				if(!$nomeColuna){
					$nomecoluna = $db->carregarColuna("	SELECT 
													t.tpdcampo||'_'||d.dtiid || '$condicao' as nomecoluna 
											   	FROM 
										   	  		mapa.detalheindicador d 
							 			   		INNER JOIN 
							 			   			mapa.tipodado t ON t.tpdid = d.tpdid 
							 			   		WHERE 
							 			   			d.indid='".$indid."'
							 			   		");
					if($nomecoluna){
						foreach($nomecoluna as $coluna){
							if($coluna){
								$arrColunas[] = $coluna;
							}
						}
					}
				}else{
					$arrColunas[] = $nomeColuna;
				}
				
			}
			
			$filtro2[] = "( ".implode(" ".$_REQUEST['contexto']." ", $arrColunas)." )";
			
		}
	}
		
	if( $_REQUEST['cenario'] ){
		$filtro[] = "(q.mpcid='".$_REQUEST['cenario']."' OR q.mpcid IS NULL)";
	}
	
	if($_REQUEST['filt_populacao']) {
		
		if($_REQUEST['filt_populacao'] == "entre") {
			$filtro[] = "m.munpopulacao BETWEEN ".$_REQUEST['ent_populacao1']." AND ".$_REQUEST['ent_populacao2'];
		} else {
			$filtro[] = "m.munpopulacao ".$_REQUEST['filt_populacao']." ".$_REQUEST['qtd_populacao'];
		}
		
	}
	
	if(in_array('grupo',$_REQUEST['agrupador'])) {
		$inner_ter = "INNER JOIN territoriosgeo.muntipomunicipio mtm ON mtm.muncod = m.muncod 
                      INNER JOIN territoriosgeo.tipomunicipio tmc ON tmc.tpmid = mtm.tpmid AND tmc.gtmid=".$_REQUEST['gtmid'];
		$colum_alias_ter = ", grupo"; 
		$colum_ter = "tpmdsc as grupo,";
		
	}
	
	if($dados['xml']) {
		$colum_xml.= ", icon";
	}

	if($_REQUEST['gerarXLS']) {
		
		$sql =  "SELECT municipio, regiao, uf {$colum_alias_ter} ".(($dados['d2'])?", ".implode(",",$dados['d2']):"")." FROM (
				 			SELECT  m.mundescricao as municipio,
	                                r.regdescricao as regiao,
	                                {$colum_ter}
	                                e.estuf as uf
                                    ".(($dados['d1'])?", ".implode(",",$dados['d1']):"")."
	                        FROM mapa.valorindicador q
	                        INNER JOIN mapa.detalheindicador di ON di.dtiid = q.dtiid
	                        INNER JOIN mapa.indicador ind ON ind.indid = di.indid
	                        INNER JOIN territoriosgeo.municipio m on m.muncod = q.muncod
	                        {$inner_ter} 
	                        INNER JOIN territoriosgeo.estado e on e.estuf = m.estuf
	                        INNER JOIN territoriosgeo.regiao r on r.regcod = e.regcod
	                        ".(($filtro)?"WHERE ".implode(" AND ",$filtro):"").") foo
	                        GROUP BY municipio, regiao, uf {$colum_alias_ter} 
	                        ORDER BY ".implode(",",$_REQUEST['agrupador']);
		
	} else {
		$sql = "SELECT municipio, regiao, uf, lng, lat, info {$colum_xml} {$colum_alias_ter} ".(($dados['d3'])?", ".implode(",",$dados['d3']):"")." FROM (
					SELECT municipio, regiao, uf, lng, lat, info {$colum_xml} {$colum_alias_ter} ".(($dados['d2'])?", ".implode(",",$dados['d2']):"")." FROM (
		
		                        SELECT  '<span style=cursor:pointer onclick=localizaMapa2(\''||m.muncod||'\',\''||ST_X(m.munlatlong)||'\',\''||ST_Y(m.munlatlong)||'\')>'||m.mundescricao||'</span>' as municipio,
		                                    r.regdescricao as regiao,
		                                    '<a style=cursor:pointer; onclick=e(\''||e.estuf||'\');>'||e.estuf||'</a>' as uf,
		                                    ST_X(m.munlatlong) as lng,
		                                    ST_Y(m.munlatlong) as lat,
											{$colum_ter}
		                                    '&lt;iframe class=iframeBalao src=".str_replace("&","EE",LINK_PADRAO)."EErequisicao=montaBalaoEEmuncod='||m.muncod||'EEmapid=".$_SESSION['painel_vars']['mapid']." &gt;&lt;/iframe&gt;' as info,
		                                    indicone as icon
		                                    ".(($dados['d1'])?", ".implode(",",$dados['d1']):"")."
		                        FROM mapa.valorindicador q
		                        INNER JOIN mapa.detalheindicador di ON di.dtiid = q.dtiid
		                        INNER JOIN mapa.indicador ind ON ind.indid = di.indid
		                        INNER JOIN territoriosgeo.municipio m on m.muncod = q.muncod
		                        {$inner_ter} 
		                        INNER JOIN territoriosgeo.estado e on e.estuf = m.estuf
		                        INNER JOIN territoriosgeo.regiao r on r.regcod = e.regcod
		                        ".(($filtro)?"WHERE ".implode(" AND ",$filtro):"").") foo
		                        GROUP BY municipio, regiao, uf, lng, lat, info {$colum_xml} {$colum_alias_ter} ) as foo2  
		                        ".(($filtro2)?" WHERE ".implode(" AND ",$filtro2):"")."
		                        ORDER BY ".implode(",",$_REQUEST['agrupador']);
	}

	
    return $sql;
}


function agp_relatorio($_agp_relatorio){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => $_agp_relatorio,
				"agrupadorDetalhamento" => array(
													array(
															"campo" => "regiao",
															"label" => "Região"
														  ),
													array(
															"campo" => "uf",
															"label" => "UF"
														  ),
													array(
															"campo" => "municipio",
															"label" => "Município"
														  ),
													array(
															"campo" => "grupo",
															"label" => "Grupo"
														  )
														  
														  
												)	  
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "regiao":
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Região")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Município")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")										
									   				);
			break;
			case "grupo":
				array_push($agp['agrupador'], array(
													"campo" => "grupo",
											  		"label" => "Grupo")										
									   				);
			break;
			
		}	
	}
	
	return $agp;
	
}

function carregarParametroMapa()
{
	global $db;
	
	$camada = $_GET['camada'] ? $_GET['camada'] : "municipio";
	
	if($_GET['estuf']){
		$arrEstados = explode(",",str_replace("estuf","",$_GET['estuf']));
		$arrWhere[] = "mun.estuf in ('".implode("','",$arrEstados)."')";
	}
	
	$sql = "select
				tpd.tpdid,
				tpdcampotema,
				tmaformula,
				tmacor
			from
				 mapa.tema tma
			inner join
				mapa.tipodado tpd ON tpd.tpdid = tma.tpdid
			where
				tmaid = {$_GET['parametro']}";
	$arrTpd = $db->pegaLinha($sql);
	$tpdcampotema = $arrTpd['tpdcampotema'];
	$tpdid = $arrTpd['tpdid'];
	$tmaformula = $arrTpd['tmaformula'];
	$tmacor = $arrTpd['tmacor'];
	
	if($tpdid == 6){
		include APPRAIZ . 'www/painel/_funcoes_formula_tema.php';
		$arrResultadoFinal = trataFormula($tmaformula);
		if($arrResultadoFinal){
			foreach($arrResultadoFinal as $muncod => $valor){
				$arrDados[] = array("muncod" => $muncod,"parametro" => $valor, "itemcor" => $tmacor, "tipoparametro" => $_GET['parametro']);
				$arrMuncod[] = $muncod;
			}
		}
		//Pega os outros municípios
		if($arrMuncod){
			$sql = "select distinct muncod from territorios.municipio where muncod not in ('".implode("','",$arrMuncod)."')";
		}else{
			$sql = "select distinct muncod from territorios.municipio";
		}
		$arrM = $db->carregarColuna($sql);
		foreach($arrM as $m){
			$arrDados[] = array("muncod" => $m,"parametro" => 0, "itemcor" => $tmacor, "tipoparametro" => $_GET['parametro']);
		}
		
	}else{
	
		if($_GET['parametro'] == 1){
			$tpdcampotema = "munpopulacao";
		}
		
		switch($tpdcampotema)
		{
			case "tmdboleano":
				$coalesce = "false";
				$whereCampo = "";
			break;
			case "tmdtexto":
				$coalesce = "'N/A'";
				$whereCampo = "";
			break;
			default:
				$coalesce = "0";
				$whereCampo = " and $tpdcampotema >= 0 ";
		}
	
		if($camada != "municipio" && strstr($camada,"tipmunici_")){ //Tipo de Município
			$camada = str_replace("tipmunici_","",$camada);
			$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
			$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
			$arrWhere[] = "tpm.gtmid = $camada";
			$campo = "distinct tpm.tpmid";
			if($tpdcampotema != "tmdboleano"){
				$campoQtde = "sum($tpdcampotema)";
				$arrGroupBy[] = "tpm.tpmid";
			}else{
				$campoQtde = "$tpdcampotema";
			}
		}else{ //Município
			$campo = "distinct mun.muncod";
			$campoQtde = "$tpdcampotema";
		}
		
		$sql = "select 
					$campo as muncod,
					COALESCE($campoQtde,$coalesce) as parametro,
					(SELECT tmacor from mapa.tema where tmaid = {$_GET['parametro']}) as itemcor,
					'{$_GET['parametro']}' as tipoparametro 
				from 
					territoriosgeo.municipio mun
				".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")." 
				left join
					mapa.temadado  tem ON tem.muncod = mun.muncod and tmaid = {$_GET['parametro']}
				left join
					mapa.tema tma ON tma.tmaid = tem.tmaid
				where
					1=1
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")." 
				".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")."
				order by
					parametro".($tpdcampotema == "tmdboleano" ? " desc " : "");
		//dbg($sql,1);
		
		$dados = $db->carregar($sql);
	
		if($dados){
		
			foreach($dados as $d){
					$arrMuncod[] = $d['muncod'];
			}
			$arrMunRep = array();
			foreach(array_count_values($arrMuncod) as $mun => $rep){
				if($rep > 1){
					$arrMunRep[] = $mun;
				}
			}
			
			foreach($dados as $d){
				if(in_array($d['muncod'],$arrMunRep) && ( ($tpdcampotema == "tmdboleano" && $d['parametro'] == "f") || ($tpdcampotema != "tmdboleano" && $d['parametro'] <= 0) ) ){
					
				}else{
					$arrDados[] = $d;
				}
			}
			
		}
	}
	$arrDados = !$arrDados ? array() : $arrDados;
				
	echo JSON_encode($arrDados);
}

function addTemaFiltro()
{
	global $db;
	
	$tmaid =  $_GET['tmaid'];
	$mapid =  $_GET['mapid'];
	$camada = $_GET['camada'];
	$tmadsc = utf8_decode($_GET['tmadsc']);
	
	$tpdid = $db->pegaUm("select tpdid from mapa.tema where tmaid = $tmaid");
	
	if($tpdid == 3){
		$percent = true;
	}else{
		$percent = false;
	}
	
	$arrRange = retornaRangeTema($tmaid,$mapid,$camada);
	?>
	<div style="width:100%;height:20px" ><div style="float:left;cursor:pointer;font-weight:bold;padding-left:5px" onclick="exibeTemaFiltro(this)" ><?php echo $tmadsc ?></div> <div style="float:right" ><img src="../imagens/exclui_p.gif" class="link" onclick="removerFiltroTema(this)" /></div> </div>
	<input type="hidden" name="hdn_tmaid[]" value="<?php echo $tmaid ?>"  />
	<div>
		<table width="100%" class="tabela" >
			<tr>
				<td class="SubtituloDireita" >Filtro:</td>
				<td>
					<?php foreach($arrRange as $chave => $range){
							$arrComboRange[$chave - 1]['codigo'] = $chave;
							$arrComboRange[$chave - 1]['descricao'] = $range;
						}
					?>
					<?php $db->monta_combo("range[$tmaid]",$arrComboRange,"S","Selecione","","","","","","","",$_POST['range'][$tmaid]) ?>
				</td>
			</tr>
			<?php if(!$percent): ?>
				<tr>
					<td class="SubtituloDireita" >
						<?php $arrFiltroAvancado = array(
													0 => array("codigo" => ">", "descricao" => "Maior que"),
													1 => array("codigo" => ">=", "descricao" => "Maior Igual a"),
													2 => array("codigo" => "=", "descricao" => "Igual a"),
													3 => array("codigo" => "<", "descricao" => "Menor que"),
													4 => array("codigo" => "<=", "descricao" => "Menor Igual a"),
													5 => array("codigo" => "entre", "descricao" => "Entre")
													); ?>
						<?php $db->monta_combo("condicaoavancada[$tmaid]",$arrFiltroAvancado,"S","Selecione","exibeFiltrosAvancados(this,$tmaid);changeFake","","","","","","",$_POST['condicaoavancada'][$tmaid]) ?>
					</td>
					<td>
					<?php if($_POST['condicaoavancada'][$tmaid]): ?>
						<?php if($_POST['condicaoavancada'][$tmaid] == "entre"): ?>
							<?php echo campo_texto("qtdcondicaoavancadainicio[$tmaid]","S","S","",10,11,"","","","","","","",$_POST['qtdcondicaoavancadainicio'][$tmaid]) ?> à 
							<?php echo campo_texto("qtdcondicaoavancadafim[$tmaid]","S","S","",10,11,"","","","","","","",$_POST['qtdcondicaoavancadafim'][$tmaid]) ?>
						<?php else: ?>
							<?php echo campo_texto("qtdcondicaoavancada[$tmaid]","S","S","",12,11,"","","","","","","",$_POST['qtdcondicaoavancada'][$tmaid]) ?>
						<?php endif; ?>
					<?php endif; ?>
					</td>
				</tr>
			<?php endif; ?>
		</table>
	</div>
	<?php
	
}

function carregarFiltroAvancado()
{
	global $db;
	
	$fusid =  $_POST['fusid'];
	$mapid =  $_POST['mapid'];
	$sql = "select fusfiltros,mapid from mapa.filtrousuario where fusid = $fusid";
	$fus = $db->pegaLinha($sql);
	$fusfiltros = unserialize($fus['fusfiltros']);
	extract($fusfiltros);
	$sql = "	select 
							tma.tmaid as codigo, 
							tma.tmadsc as descricao
						from
							mapa.mapatema mpt
						inner join
							mapa.tema tma ON mpt.tmaid = tma.tmaid
						where
							mpt.mapid = $mapid
						and
							tmastatus = 'A'
						order by
							tmadsc
						";
	$arrTemas = $db->carregar($sql);
	//dbg($fusfiltros);
	if($cmb_tema){
		foreach($cmb_tema as $grupo => $arrGrupo){
			if($grupo!=1){
				echo "<center>";
				comboCondicao("cmb_entregrupo[$grupo]",$cmb_entregrupo[$grupo]);
				echo "</center>";
			}
			echo '<fieldset id="grupo_avancado_'.$grupo.'" class="grupo_filtro_avancado"  >';
			echo '<legend><img src="../imagens/gif_inclui.gif" style="cursor:pointer;vertical-align:middle" onclick="addTemaGrupo('.$grupo.','.$mapid.')" /> Grupo '.$grupo.'</legend>';
			foreach($arrGrupo as $tipoFiltro => $tmaid){
				if($tipoFiltro !=1){
					echo '<div class="div_filtro_avancado" >
							<img style="cursor:pointer;float:right" src="../imagens/exclui_p.gif" onclick="jQuery(this).parent().remove();" />
							';
					comboCondicao("cmb_grupo[$grupo][$tipoFiltro]",$cmb_grupo[$grupo][$tipoFiltro]);
				}else{
					if($grupo==1){
						echo '<div>';
					}else{
						echo '<div>
								<img src="../imagens/exclui_p.gif" style="cursor:pointer;float:right" onclick="removeTemaGrupo('.$grupo.')" />
							';
					}
					
				}
				comboTemas($mapid,"cmb_tema[$grupo][$tipoFiltro]",$arrTemas,$cmb_tema[$grupo][$tipoFiltro]);
				echo '<span id="span_tema_'.$grupo.'_'.$tipoFiltro.'" >';
				$sql = "select tpdid from mapa.tema where tmaid = ".$cmb_tema[$grupo][$tipoFiltro];
				$tpdid = $db->pegaUm($sql);
				if($tpdid == 3){
					$arrOptions = array(
							0=> array("codigo" => "true", "descricao" => "Sim"),
							1=> array("codigo" => "false", "descricao" => "Não")
							);
					$db->monta_combo("cmb_filtro[$grupo][$tipoFiltro]",$arrOptions,"S","","","","","","","","",$cmb_filtro[$grupo][$tipoFiltro]);
				}else{
					comboTipoFiltro("cmb_filtro[$grupo][$tipoFiltro]",$cmb_filtro[$grupo][$tipoFiltro],$cmb_filtro_inicio[$grupo][$tipoFiltro],$cmb_filtro_fim[$grupo][$tipoFiltro]);
				}
				echo '</span>';
				echo '</div>';
			}
			echo '</fieldset>';
		}
	}else{ ?>
		<fieldset id="grupo_avancado_1" class="grupo_filtro_avancado" >
			<legend><img src="../imagens/gif_inclui.gif" style="cursor:pointer;vertical-align:middle" onclick="addTemaGrupo(1,<?php echo $mapid ?>)" /> Grupo 1</legend>
			<?php comboTemas($mapid,"cmb_tema[1][1]",$arrTemas) ?>
			<?php echo '<span id="span_tema_1_1" >'; ?>
			<?php comboTipoFiltro("cmb_filtro[1][1]") ?>
			<?php echo '</span>' ?>
		</fieldset>
	<?php	
	}
	echo "<script>jQuery('#color_filtro_avancado').next().css(\"background-color\",'$color_filtro_avancado');
		  			jQuery('#color_filtro_avancado').val('$color_filtro_avancado');
		  </script>";
	exit;
}

function carregarFiltroNormal()
{
	global $db;
	global $html_agrupador;
	global $agrupador;
	include APPRAIZ . 'includes/Agrupador.php';
	
	$sql = "SELECT * FROM mapa.indicador WHERE mapid='".$_POST['mapid']."' and indstatus = 'A' ORDER BY indid";
	$indicadores = $db->carregar($sql);
	
	$sql = "select * from mapa.filtrousuario where fusid = {$_POST['fusid']}";
	$arrDados = $db->pegaLinha($sql);
		
	$sql = "SELECT * FROM mapa.mapa WHERE mapid='".$_POST['mapid']."'";
	$dadosmapa = $db->pegaLinha($sql);
	
	$sql = "SELECT mpcid as codigo, mpcdsc as descricao FROM mapa.mapacenario WHERE mpcstatus='A' AND mapid='".$_POST['mapid']."' order by 2";
	$dadoscenario = $db->carregar($sql);
	
	if($arrDados['fusfiltros']){
		extract(unserialize($arrDados['fusfiltros']));
	}
	
	?>
	<form method="post" name="formulario" id="formulario">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<? if($dadoscenario[0]) : ?>
		<tr>
			<td class="SubTituloDireita">Cenário:</td>
			<td colspan="2"><? $db->monta_combo('cenario', $dadoscenario, 'S', '', '', '', '', '', 'N', 'cenario'); ?></td>
		</tr>
		<? endif; ?>
		<?
		if($indicadores[0]) {
			$n=0;
			foreach($indicadores as $indicador) {
				?>
				<tr>
					<td><input type="checkbox" name="icone[]" <?php echo $icone[$n] != "" ? "checked='checked'" : "" ?> onclick="if(this.checked){showOverlays(pontoMarcadores['<?=$indicador['indicone'] ?>']);}else{clearOverlays(pontoMarcadores['<?=$indicador['indicone'] ?>']);}"  id="id_<?=$indicador['indid'] ?>" value="<?=$indicador['indid'] ?>"  /></td>
					<? if($dadosmapa['mapcontexto']=="t") : ?>
					<td><? $db->monta_combo('cond['.$indicador['indid'].']', array(0 => array('codigo'=>'com','descricao'=>'Com'), 1 => array('codigo'=>'sem','descricao'=>'Sem')), 'S', '', '', '', '', '', 'N', ''); ?></td>
					<? endif; ?>
					<td><img src="<?=$indicador['indicone'] ?>"></td>
					<td><font size=1><?=$indicador['inddsc'] ?></font></td>
				</tr>
				<?
				$n++;
			}
		}
		?>
		
		<? if($dadosmapa['mapcontexto']=="t") : ?>
		<tr>
		<td colspan="2" class="SubTituloDireita">Contexto:</td>
		<td colspan="2"><?
		$db->monta_combo('contexto', array(0 => array('codigo'=>'OR','descricao'=>'OU'), 1 => array('codigo'=>'AND','descricao'=>'E')), 'S', '', '', '', '', '', 'N', 'contexto'); 
		?></td>
		</tr>
		<? endif; ?>
	</table>
	<br>
	<br>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTituloEsquerda">
			<img style="cursor: pointer" src="../imagens/<?php echo $filt_populacao != "" ? "menos" : "mais" ?>.gif" id="img_gerais" onclick="mostrar_painel('gerais');" border=0> Outros Filtros
		</td>
	</tr>
	<tr>
		<td>
			<div id="gerais" style="display:<?php echo $filt_populacao != "" ? "" : "none" ?>">
			<table align="center" border="0" class="listagem" cellpadding="0" cellspacing="0" width="100%">
			<tr>
				<td class="SubTituloDireita" width="10%">População:</td>
				<td width="10%">
				<?
				$opts = Array(Array('codigo'=>'>' ,'descricao'=>'Maior que'),
							  Array('codigo'=>'>=','descricao'=>'Maior Igual a'),
							  Array('codigo'=>'=' ,'descricao'=>'Igual a'),
							  Array('codigo'=>'<' ,'descricao'=>'Menor que'),
							  Array('codigo'=>'<=','descricao'=>'Menor igual a'),
							  Array('codigo'=>'entre','descricao'=>'Entre'));	
				
				$db->monta_combo('filtro[populacao]', $opts, 'S', 'Selecione', 'selecionaPopulacao', '', '', '', 'N', 'filt_populacao','',$filt_populacao);
				?>
				</td>
				<td>
				<?php if($filt_populacao == "entre"): ?>
					<div id="div_qtd_populacao" style="display:none;" ><? echo campo_texto('qtd_populacao', 'N', 'S', '', 10, 12, "##########", "", '', '', 0, 'id="qtd_populacao"','',$qtd_populacao); ?></div>
					<div id="div_ent_populacao" style="display:"><? echo campo_texto('ent_populacao1', 'N', 'S', '', 10, 12, "#########", "", '', '', 0, 'id="ent_populacao1"' ,'',$ent_populacao1); ?> à <? echo campo_texto('ent_populacao2', 'N', 'S', '', 10, 12, "#########", "", '', '', 0, 'id="ent_populacao2"' ,'',$ent_populacao2 ); ?></div>				
				<?php else: ?>
					<div id="div_qtd_populacao"><? echo campo_texto('qtd_populacao', 'N', 'S', '', 10, 12, "##########", "", '', '', 0, 'id="qtd_populacao"','',$qtd_populacao); ?></div>
					<div id="div_ent_populacao" style="display:none;"><? echo campo_texto('ent_populacao1', 'N', 'S', '', 10, 12, "#########", "", '', '', 0, 'id="ent_populacao1"' ,'',$ent_populacao1); ?> à <? echo campo_texto('ent_populacao2', 'N', 'S', '', 10, 12, "#########", "", '', '', 0, 'id="ent_populacao2"' ,'',$ent_populacao2 ); ?></div>				
				<?php endif; ?>
				</td>
			</tr>
			</table>
			</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda">
			<img style="cursor: pointer" src="../imagens/<?php echo $estuf[0] != "" ? "menos" : "mais" ?>.gif" id="img_uf" onclick="mostrar_painel('uf');" border=0> UF
		</td>
	</tr>
	<tr>
		<td>
			<div id="uf" style="display:<?php echo $estuf[0] != "" ? "" : "none" ?>">
				<?php
				$sql = "	SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM 
								territorios.estado
							ORDER BY
								estdescricao ";
				
				if($estuf[0] != ""){
					$sql = "SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM 
								territorios.estado
							where
								estuf in ('".implode("','",$estuf)."')
							ORDER BY
								estdescricao";
					$arrUF = $db->carregar($sql);
				}
				
				combo_popup( 'estuf', $sql, 'Selecione as Unidades Federativas', '400x400', 0, array(), '', 'S', false, false, 5, 240, '', '','','',$arrUF );
				?>
			</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda">
			<img style="cursor: pointer" src="../imagens/<?php echo $muncod[0] != "" ? "menos" : "mais" ?>.gif" id="img_municipio" onclick="mostrar_painel('municipio');" border=0> Município
		</td>
	</tr>
	<tr>
		<td>
			<div id="municipio" style="display:<?php echo $muncod[0] != "" ? "" : "none" ?>">
				<?php
				$sql = " 	SELECT	
								muncod AS codigo,
								mundescricao AS descricao
							FROM 
								territorios.municipio
							ORDER BY
								mundescricao";
				if($muncod[0] != ""){
					$sql = "SELECT	
								muncod AS codigo,
								mundescricao AS descricao
							FROM 
								territorios.municipio
							where
								muncod in ('".implode("','",$muncod)."')
							ORDER BY
								mundescricao";
					$arrMuncod = $db->carregar($sql);
				}
	
				combo_popup( 'muncod', $sql, 'Selecione os Municípios', '400x400', 0, array(), '', 'S', false, false, 5, 240, '', '','','',$arrMuncod );							?>
			</div>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloEsquerda" ><img style="cursor:pointer" id="img_agrup" onclick="mostrar_painel('agrup');" src="/imagens/mais.gif"> Agrupadores</td>
	</tr>
	<tr>
		<td>
		<div style="display:none" id="agrup">
		<p>Grupo de municípios: <?
		$gtmid= !$gtmid ? 6 : $gtmid;
		$sql = "SELECT 
					gtm.gtmid as codigo,
					gtmdsc as descricao 
				FROM 
					territoriosgeo.grupotipomunicipio gtm
				INNER JOIN
					mapa.mapacamada mpc ON mpc.gtmid = gtm.gtmid 
				WHERE 
					mpc.mapid = {$_POST['mapid']}
				AND
					gtmstatus='A' 
				ORDER BY 
					gtmdsc"; 
		$db->monta_combo('gtmid', $sql, 'S', '', '', '', '', '200', 'N', 'gtmid','',$gtmid); 
		?></p>
	
			<?
			$agrupador_selecionado = $agrupador;
			$agrupador_padrão = array(
										'regiao' => array(
																	'codigo'    => 'regiao',
																	'descricao' => 'Região'
										),
										'uf' => array(
																	'codigo'    => 'uf',
																	'descricao' => 'UF'
										),
										'municipio' => array(
																	'codigo'    => 'municipio',
																	'descricao' => 'Município'
										),
										'grupo' => array(
																	'codigo'    => 'grupo',
																	'descricao' => 'Grupo'
										)
									  );
			
				// Início dos agrupadores
				$agrupador = new Agrupador('filtros','');
				
				if($agrupador_selecionado){
					foreach($agrupador_padrão as $a => $agr){
						if(in_array($a,$agrupador_selecionado)){
							$arrAgr1[] = $agr;
						}else{
							$arrAgr2[] = $agr;
						}
					}
					$origem = $arrAgr2;
					$destino = $arrAgr1;
				}else{
					// Dados padrão de destino (nulo)
					$destino = array(
						'regiao' => array(
													'codigo'    => 'regiao',
													'descricao' => 'Região'
						),
						'uf' => array(
													'codigo'    => 'uf',
													'descricao' => 'UF'
						),
						'municipio' => array(
													'codigo'    => 'municipio',
													'descricao' => 'Município'
						)
					);
					
					// Dados padrão de origem
					$origem = array(
						'grupo' => array(
													'codigo'    => 'grupo',
													'descricao' => 'Grupo'
						)
						
					);
				}

				// exibe agrupador
				$agrupador->setOrigem( 'naoColunas', null, $origem );
				$agrupador->setDestino( 'agrupador', null, $destino );
				$agrupador->exibir();
		
		?> 
		</div>
		</td>
	</tr>
	</table> <?php
	
		echo "<script>";
		if($arrAgr1){
			echo "jQuery('#agrup').show();";
			foreach($arrAgr1 as $a1){
				echo "if(jQuery('#agrupador option[value={$a1['codigo']}]').length == 0){jQuery(\"#agrupador\").append('<option value=\"{$a1['codigo']}\" selected=\"selected\">{$a1['descricao']}</option>')};";	
			}
		}
		if($arrAgr2){
			foreach($arrAgr2 as $a2){
				echo "if(jQuery('#naoColunas option[value={$a2['codigo']}]').length == 0){jQuery(\"#naoColunas\").append('<option value=\"{$a2['codigo']}\" selected=\"selected\">{$a2['descricao']}</option>')};";
			}	
		}
		echo "</script>";
	
}

function listaFiltrosAvancados($mapid,$usucpf = null)
{
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	$sql = "select
				'<span style=\"white-space:nowrap;\" >' || CASE WHEN usucpf = '$usucpf'
					THEN '<img src=\"../imagens/consultar.gif\" class=\"link branco\" onclick=\"carregarFiltroAvancado(' || fus.fusid || ',$mapid)\"  /> <img src=\"../imagens/excluir.gif\" class=\"link branco\" onclick=\"excluirFiltroAvancado(' || fus.fusid || ',$mapid)\"  />'
					ELSE '<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"carregarFiltroAvancado(' || fus.fusid || ',$mapid)\"  />'
				END || '</span>' as acao,
				fustitulo
			from
				mapa.filtrousuario fus
			where
				(usucpf = '$usucpf' or fuspublico is true)
			and
				mapid = $mapid
			and
				fusstatus = 'A'
			and
				fustipo = 'A'
			order by
				fustitulo,
				fusdata";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		$arrCabecalho = array("Ação","Título");
		$db->monta_lista_simples($sql,$arrCabecalho,100,10,"N");
	}
	
}

function listaFiltrosNormais($mapid,$usucpf = null)
{
	global $db;
	
	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	
	$sql = "select
				'<span style=\"white-space:nowrap;\" >' || CASE WHEN usucpf = '$usucpf'
					THEN '<img src=\"../imagens/consultar.gif\" class=\"link branco\" onclick=\"carregarFiltroNormal(' || fus.fusid || ',$mapid)\"  /> <img src=\"../imagens/excluir.gif\" class=\"link branco\" onclick=\"excluirFiltroNormal(' || fus.fusid || ',$mapid)\"  />'
					ELSE '<img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"carregarFiltroNormal(' || fus.fusid || ',$mapid)\"  />'
				END || '</span>' as acao,
				fustitulo
			from
				mapa.filtrousuario fus
			where
				(usucpf = '$usucpf' or fuspublico is true)
			and
				mapid = $mapid
			and
				fusstatus = 'A'
			and
				fustipo = 'N'
			order by
				fustitulo,
				fusdata";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		$arrCabecalho = array("Ação","Título");
		$db->monta_lista_simples($sql,$arrCabecalho,100,10,"N");
	}
	
}

function buscaAvancada()
{
	global $db;
	
	$mapid = $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid'];
	
	?>
	<script type="text/javascript">
		
	</script>
	<form name="form_busca_avancada" id="form_busca_avancada" action="painel.php?modulo=principal/mapas/popUpMapaPadrao&acao=A&mapid=<?php echo $mapid ?>" method="post" target="filtroavancadoresultado" >
		<input type="hidden" name="requisicao" id="requisicao_filtros_avancados" value="exibeRelatorioFiltrosAvancados"  />
		<input type="hidden" name="estuf" id="estuf_filtros_avancados" value=""  />
		<input type="hidden" name="camada_filtros_avancados" id="camada_filtros_avancados" value=""  />
		<table width="100%" class="tabela" id="tbl_temas_filtros" >
			<tr>
				<td id="grupo_filtros_avancados" >
					<fieldset id="grupo_avancado_1" class="grupo_filtro_avancado" >
						<legend><img src="../imagens/gif_inclui.gif" style="cursor:pointer;vertical-align:middle" onclick="addTemaGrupo(1,<?php echo $mapid ?>)" /> Grupo 1</legend>
						<?php //comboCondicao("cmb_grupo[1][1]") ?>
						<?php $sql = "	select 
											tma.tmaid as codigo, 
											tma.tmadsc as descricao
										from
											mapa.mapatema mpt
										inner join
											mapa.tema tma ON mpt.tmaid = tma.tmaid
										where
											mpt.mapid = $mapid
										and
											tmastatus = 'A'
										order by
											tmadsc
										";
							  $arrTemas = $db->carregar($sql);
							  comboTemas($mapid,"cmb_tema[1][1]",$arrTemas) ?>
							  <?php echo '<span id="span_tema_1_1" >'; ?>
							  <?php comboTipoFiltro("cmb_filtro[1][1]") ?>
							  <?php echo '</span>'; ?>
					</fieldset>
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" >
					<input type="button" onclick="addNovoGrupo(<?php echo $mapid ?>)" name="btn_noco_grupo" value="Novo Grupo" />
				</td>
			</tr>
			<tr>
				<td class="SubtituloDireita" >
					<input id="color_filtro_avancado" type="text" name="color_filtro_avancado" value="#0000CC" />
					<input type="button" onclick="visualizarFiltrosAvancadosMapa(<?php echo $mapid ?>)" name="btn_visualizar_mapa" value="Mapa" /> 
					<input type="button" onclick="exibeRelatorioFiltrosAvancados(<?php echo $mapid ?>)" name="btn_relatorio_avancado_mapa" value="Relatório" />
					<input type="button" onclick="limparMapa()" name="btn_limpar_mapa" value="Limpar" />
					<input type="button" onclick="jQuery('#div_salvar_filtros_avancados').show()" name="btn_salvar_filtros_avancados" value="Salvar Filtros" />
				</td>
			</tr>
		</table>
	</form>
		<div id="div_salvar_filtros_avancados" style="display:none"  >
			<table width="100%" class="tabela" >
				<tr>
					<td width="25%" class="SubtituloDireita" >Título:</td>
					<td><?php echo campo_texto("fustitulo","N","S","",40,255,"","") ?></td>
				</tr>
				<tr>
					<td class="SubtituloDireita" >Público?</td>
					<td>
						Sim <input type="radio" name="fuspublico" id="fuspublico_sim"  value="t"  />  
						Não <input type="radio" name="fuspublico" id="fuspublico_nao" value="f"  />
					</td>
				</tr>
				<tr>
					<td class="SubtituloEsquerda" >
					</td>
					<td class="SubtituloEsquerda" >
						<input type="button" onclick="salvarFiltrosAvancados(<?php echo $mapid ?>)" name="btn_salvar_filtros_" value="Salvar"  />
						<input type="button" onclick="jQuery('#div_salvar_filtros_avancados').hide()" name="btn_cacelar_salvar_filtros_" value="Cancelar"  />
					</td>
				</tr>
			</table>
		</div>
		<div id="lista_salvar_filtros_avancados" >
			<?php listaFiltrosAvancados($mapid); ?>
		</div>
	
	<?php
}

function addGrupo()
{
	global $db;
	$mapid = $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid'];
	$num = $_GET['num']; ?>
	<center><?php comboCondicao("cmb_entregrupo[$num]") ?></center>
			<fieldset id="grupo_avancado_<?php echo $num ?>" class="grupo_filtro_avancado" >
						<legend> <img src="../imagens/gif_inclui.gif" style="cursor:pointer;vertical-align:middle" onclick="addTemaGrupo(<?php echo $num ?>,<?php echo $mapid ?>)" /> Grupo <?php echo $num ?></legend>
							<img src="../imagens/exclui_p.gif" style="cursor:pointer;float:right" onclick="removeTemaGrupo(<?php echo $num ?>)" />
						<?php $sql = "	select 
											tma.tmaid as codigo, 
											tma.tmadsc as descricao
										from
											mapa.mapatema mpt
										inner join
											mapa.tema tma ON mpt.tmaid = tma.tmaid
										where
											mpt.mapid = $mapid
										and
											tmastatus = 'A'
										order by
											tmadsc
										";
							  $arrTemas = $db->carregar($sql);
							  comboTemas($mapid,"cmb_tema[$num][1]",$arrTemas) ?>
							  <span id="span_tema_<?php echo $num ?>_1" >
							  	<?php comboTipoFiltro("cmb_filtro[$num][1]") ?>
							  </span>
					</fieldset>
<?php	
}

function addtemaGrupo()
{
	global $db;
	$mapid = $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid'];
	$grupo = $_GET['grupo'];
	$num = $_GET['num'];
	?>
	<div class="div_filtro_avancado" >
		<img style="cursor:pointer;float:right" src="../imagens/exclui_p.gif" onclick="jQuery(this).parent().remove();" />
		<?php comboCondicao("cmb_grupo[$grupo][$num]") ?>
		<?php $sql = "	select 
							tma.tmaid as codigo, 
							tma.tmadsc as descricao
						from
							mapa.mapatema mpt
						inner join
							mapa.tema tma ON mpt.tmaid = tma.tmaid
						where
							mpt.mapid = $mapid
						and
							tmastatus = 'A'
						order by
							tmadsc
						";
			  $arrTemas = $db->carregar($sql);
			  comboTemas($mapid,"cmb_tema[$grupo][$num]",$arrTemas);
			  echo '<span id="span_tema_'.$grupo.'_'.$num.'" >';
			  comboTipoFiltro("cmb_filtro[$grupo][$num]"); 
			  echo '</span>';?>
	</div>
<?php }

function comboCondicao($name,$value = "and")
{
	global $db;
	
	$arrDados = array( 
						0=> array("codigo" => "and", "descricao" => "E"),
						1=> array("codigo" => "or", "descricao" => "OU")
					  );
	$db->monta_combo($name,$arrDados,"S","","","","","","","","",$value);
}

function comboTemas($mapid,$name,$arrTemas,$value = null)
{
	global $db;
	
	$id = str_replace(array("][","[","]"),array("_","_",""),$name);
	if($arrTemas){
		$db->monta_combo($name,$arrTemas,"S","","alteraTipoFiltroAvancado(this);fake","","","220","",$id,"",$value);
	}
}

function comboTipoFiltro($name,$value = ">=",$value_inicio = null,$value_fim = null)
{
	global $db;
	
	//$value_inicio = str_replace(array(".","%",","),array("","","."),$value_inicio);
	//$value_fim = str_replace(array(".","%",","),array("","","."),$value_fim);
	
	$opts = Array(Array('codigo'=>'>' ,'descricao'=>'Maior que'),
				  Array('codigo'=>'>=','descricao'=>'Maior Igual a'),
				  Array('codigo'=>'=' ,'descricao'=>'Igual a'),
				  Array('codigo'=>'<' ,'descricao'=>'Menor que'),
				  Array('codigo'=>'<=','descricao'=>'Menor igual a'),
				  Array('codigo'=>'<>','descricao'=>'Diferente de'),
				  Array('codigo'=>'entre','descricao'=>'Entre'));	
	echo "<br />";
	$db->monta_combo($name,$opts,"S","","selecionaTipoFiltro(this);fake","","","","","","",$value); ?>
	
		<?php $name_inicio = str_replace("cmb_filtro","cmb_filtro_inicio",$name); echo campo_texto($name_inicio,"N","S","",10,50,"","","","","","","",$value_inicio) ?>
		<span style="display:<?php echo $value == "entre" ? "" : "none" ?>" id="<?php echo str_replace(array("cmb_filtro","][","[","]"),array("cmb_filtro_a","_","_","_"),$name) ?>" >à
		<?php $name_fim = str_replace("cmb_filtro","cmb_filtro_fim",$name); echo campo_texto($name_fim,"N","S","",10,50,"","","","","","","",$value_fim) ?>
		</span>
	
<?}

function gerarRelatorio() {
	global $db;
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$coluna = array();
	
	if($_REQUEST['icone']) {
		
		foreach($_REQUEST['icone'] as $ic) {
			
			$sql = "SELECT tpd.tpdtipo, tpd.tpdcampo, dli.dtiid, dli.dtidsc FROM mapa.detalheindicador dli 
					INNER JOIN mapa.tipodado tpd ON tpd.tpdid = dli.tpdid 
					INNER JOIN mapa.indicador ind ON ind.indid = dli.indid
					WHERE ind.indid='".$ic."' ORDER BY ind.inddsc,dli.dtiordem";
			
			$tipos = $db->carregar($sql);
			
			if($tipos[0]) {
				
				foreach($tipos as $tp) {
					
					$nomecoluna = $tp['tpdcampo']."_".$tp['dtiid'];
					
					$_arrSQL['d1'][] = "CASE WHEN di.dtiid=".$tp['dtiid']." THEN COALESCE({$tp['tpdcampo']},0) ELSE 0 END as {$nomecoluna}";
					$_arrSQL['d2'][] = "SUM({$nomecoluna}) as {$nomecoluna}";
					$_arrSQL['d3'][] = $nomecoluna;
					$_agp_relatorio[] = $nomecoluna;
					array_push( $coluna, array("campo" 	  => $nomecoluna,
			   								   "label" 	  => $tp['dtidsc'],
											   "type"	  => $tp['tpdtipo']
			   		   						   ) );
				}
			}
		}
	}

	$sql = montaSQL($_arrSQL);
	
	if($_REQUEST['gerarXLS']) {

		ob_clean();
		$arCabecalho=array();
		array_push( $arCabecalho,"Município" );
		array_push( $arCabecalho,"Região" );
		array_push( $arCabecalho,"UF" );
		if(in_array('grupo',$_REQUEST['agrupador'])) {
			array_push( $arCabecalho,"Grupo" );
		}
		
		if($coluna) {
			foreach($coluna as $c) {
				$arCabecalho[]=$c['label'];
			}
		}
		
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=mapapadrao_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$arCabecalho,100000,5,'N','100%',$par2);
		
	} else {
	
		$agrupador = agp_relatorio($_agp_relatorio);
		
		$dados = $db->carregar( $sql );
		$rel = new montaRelatorio();
		$rel->setColuna($coluna);
		$rel->setTolizadorLinha(true);
		$rel->setMonstrarTolizadorNivel(true);
		$rel->setTotalizador(true);
		$rel->setAgrupador($agrupador, $dados);
		$rel->setTotNivel(true);
		echo $rel->getRelatorio();
		echo "<p align=center><input type=button name=gerarxls value=\"Gerar XLS\" onclick=carregarMapasPontos(2);></p>";
		unset($rel);
		
	}
	
}

function retornaRangeTema($tmaid,$mapid,$camada)
{
	global $db;
		
	$sql = "select
				tpdcampotema
			from
				 mapa.tema tma
			inner join
				mapa.tipodado tpd ON tpd.tpdid = tma.tpdid
			where
				tmaid = $tmaid";
	$tpdcampotema = $db->pegaUm($sql);
	
	if($tmaid == 1){
		$tpdcampotema = "munpopulacao";
	}
	
	switch($tpdcampotema)
	{
		case "tmdboleano":
			$coalesce = "false";
			$whereCampo = "";
		break;
		case "tmdtexto":
			$coalesce = "'N/A'";
			$whereCampo = "";
		break;
		default:
			$coalesce = "0";
			$whereCampo = " and $tpdcampotema >= 0 ";
	}
	
	//Verifica se existe legenda do tema
	$sql = "select * from mapa.legendatema where mapid = $mapid and tmaid = $tmaid order by lgtposicao";
	$arrLegTema = $db->carregar($sql);
	if(!$arrLegTema){
		$sql = "select * from mapa.legendatema where tmaid = $tmaid and mapid is null order by lgtposicao";
		$arrLegTema = $db->carregar($sql);
	}
	
	$camada = $camada ? $camada : "municipio";
	
	if($camada != "municipio" && strstr($camada,"tipmunici_")){ //Tipo de Município
		$camada = str_replace("tipmunici_","",$camada);
		$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
		$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
		$arrWhere[] = "tpm.gtmid = $camada";
		$campo = "distinct tpm.tpmid";
		if($tpdcampotema != "tmdboleano"){
			$campoQtde = "COALESCE(sum($tpdcampotema),$coalesce)";
			$arrGroupBy[] = "tpm.tpmid";
			$arrGroupBy[] = "tmacor";
		}else{
			$campoQtde = "(COALESCE($tpdcampotema,$coalesce))";
		}
	}else{ //Município
		$campo = "distinct mun.muncod";
		$campoQtde = "$tpdcampotema";
	}
	
	//percent
	$sql = "select tpdid from mapa.tema where tmaid = $tmaid ";
	$tpdid = $db->pegaUm($sql);
	if($tpdid == 4){
		$percent = true;
	}else{
		$percent = false;
	}
	
	if($arrLegTema){
		foreach($arrLegTema as $legTema){
			
			$lgtde = str_replace(",",".",$legTema['lgtde']);
			if($legTema['lgtate']){
				$lgtate = str_replace(",",".",$legTema['lgtate']);
			}else{
				$lgtate = $lgtde;
			}
			
			$arrSteps[] = array( "inicio" => $lgtde , "fim" => $lgtate );
		}
	}else{
		$sql = "select 
					parametro,
					count(1) as qtde
					
				from (
						select 
							$campo,
							$campoQtde as parametro,
							'' as tmacor
						from 
							territoriosgeo.municipio mun
						".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")." 
						left join
							mapa.temadado pro ON mun.muncod = pro.muncod $whereCampo and pro.tmaid = $tmaid
						".($arrWhere ? " where ".implode(" and ",$arrWhere) : "")."
						".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")." 
					  ) as foo 
				group by 
					parametro
					
				order by 
					1";
		
		$arrDados = $db->carregar($sql);
		
		$sqlChave = "select sum(qtde) - {valorChave} from ( 
												select 
													parametro,
													count(1) as qtde
												from (
														select 
															$campo,
															$campoQtde as parametro,
															'' as tmacor
														from 
															territoriosgeo.municipio mun
														".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")." 
														left join
															mapa.temadado pro ON mun.muncod = pro.muncod $whereCampo and pro.tmaid = $tmaid
														".($arrWhere ? " where ".implode(" and ",$arrWhere) : "")."
														".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")." 
													  ) as foo 
												group by 
													parametro
												order by 
													1 ) as foo2";
		
		
		if($arrDados){
			$numLegenda = 5;
			if($camada == "municipio" && $percent){
				$valorChave = $db->pegaUm("select count(muncod) from territoriosgeo.municipio");
			}else{
				$valorChave = count($arrDados) == 0 ? 1 : count($arrDados);
			}
			(float)$num = 0; //incia o número de soma acumulada igual a zero
			$arrSteps = array();
			foreach($arrDados as $linha => $dado){
				if(count($arrSteps) < ($valorChave/$numLegenda)){//apenas 5 legendas
					if($dado['qtde'] >= $valorChave){//se a qtde for maior que a chave
						$arrSteps[] = array("inicio" => $dado['parametro'], "fim" => $dado['parametro']);
						(float)$valorChave = $db->pegaUm(str_replace("{valorChave}",$dado['qtde'],$sqlChave)); //total sem o número
					}else{ //se não for maior que a chave
						$arrLeg[] = $dado['parametro']; //grava o parametro
						$num+=(float)$dado['qtde']; //soma o numero de qtde
						if($numLegenda-count($arrSteps)> 0 && $num >= ($valorChave/($numLegenda-count($arrSteps))) ){ //se o numero form maior que a chave
							$arrSteps[] = array("inicio" => $arrLeg[0], "fim" => $arrLeg[count($arrLeg)-1]); //atribui o inicio e o fim da legenda
							unset($arrLeg); //reinicia o array de legendas
						}	
					}
				}
			}
		}
	}
	
	if($arrSteps && !$arrLegTema){
		$sqlMax = "select 
					parametro,
					count(1) as qtde
					
				from (
						select 
							$campo,
							$campoQtde as parametro,
							'' as tmacor
						from 
							territoriosgeo.municipio mun
						".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")." 
						left join
							mapa.temadado pro ON mun.muncod = pro.muncod $whereCampo and pro.tmaid = $tmaid
						".($arrWhere ? " where ".implode(" and ",$arrWhere) : "")."
						".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")." 
					  ) as foo 
				group by 
					parametro
					
				order by 
					1 desc";
		$arrSteps[count($arrSteps) - 1]["fim"] = $db->pegaUm($sqlMax);
	}
		
	$arrD = $db->pegaLinha("select tpdid,tmacor from mapa.tema where tmaid = $tmaid");
	$arrCores[$tmaid] = $arrD['tmacor'];
	$tpdid = $arrD['tpdid'];
	
	if($tpdcampotema == "tmdboleano"){
		return array( 1 => "Sim" , 2 => "Não" );	
	}
	
	$i = 1;
	if($arrSteps){
		foreach($arrSteps as $i => $range){
			if($range['inicio'] >= 0 && $range['fim'] >= 0){
				if($range['inicio'] == $range['fim']){
					if($tpdcampotema == "tmdboleano"){
						$arrRanges[$i+1] = ($range['inicio'] == "t" ? "Sim" : "Não");
					}else{
						$arrRanges[$i+1] = ($percent ?number_format($range['inicio'],2,',','.')."%" : number_format($range['inicio'],0,'','.'));
					}
				}else{
					$arrRanges[$i+1] = ($percent ?number_format($range['inicio'],2,',','.')."%" : number_format($range['inicio'],0,'','.'))." - ".($percent ?number_format($range['fim'],2,',','.')."%" : number_format($range['fim'],0,'','.'));
				}
			}
		}
	}
	return $arrRanges;
	
}


function carregaParametro()
{
	global $db;
	
	$camada = $_GET['camada'] ? $_GET['camada'] : "municipio";
	
	$estuf = $_GET['estuf'];
	if($estuf){
		$estuf = str_replace("estuf","",$estuf);
		$arrEstuf = explode(",",$estuf);
	}else{
		$estuf = false;
	}
	
	$_GET['mapid'] = $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid'];
	
	$sql = "select
				tpd.tpdid,
				tpdcampotema,
				tmaformula
			from
				 mapa.tema tma
			inner join
				mapa.tipodado tpd ON tpd.tpdid = tma.tpdid
			where
				tmaid = {$_GET['parametro']}";
	$arrTpd = $db->pegaLinha($sql);
	$tpdcampotema = $arrTpd['tpdcampotema'];
	$tpdid = $arrTpd['tpdid'];
	$tmaformula = $arrTpd['tmaformula'];
	
	if($tpdid == 6){
		include APPRAIZ . 'www/painel/_funcoes_formula_tema.php';
		$arrResultadoFinal = trataFormula($tmaformula);
	}else{
		if($_GET['parametro'] == 1){
			$tpdcampotema = "munpopulacao";
		}
		
		switch($tpdcampotema)
		{
			case "tmdboleano":
				$coalesce = "false";
				$whereCampo = "";
			break;
			case "tmdtexto":
				$coalesce = "'N/A'";
				$whereCampo = "";
			break;
			default:
				$coalesce = "0";
				$whereCampo = " and $tpdcampotema >= 0 ";
		}
		
		//Verifica se existe legenda do tema
		$sql = "select * from mapa.legendatema where mapid = {$_GET['mapid']} and tmaid = {$_GET['parametro']} order by lgtposicao";
		$arrLegTema = $db->carregar($sql);
		if(!$arrLegTema){
			$sql = "select * from mapa.legendatema where tmaid = {$_GET['parametro']} and mapid is null order by lgtposicao";
			$arrLegTema = $db->carregar($sql);
		}
		
		if($camada != "municipio" && strstr($camada,"tipmunici_")){ //Tipo de Município
			$camada = str_replace("tipmunici_","",$camada);
			$arrInnerJoin[] = "territoriosgeo.muntipomunicipio mtm ON mtm.muncod = mun.muncod";
			$arrInnerJoin[] = "territoriosgeo.tipomunicipio tpm ON tpm.tpmid = mtm.tpmid";
			$arrWhere[] = "tpm.gtmid = $camada";
			$campo = "distinct tpm.tpmid";
			if($tpdcampotema != "tmdboleano"){
				$campoQtde = "COALESCE(sum($tpdcampotema),$coalesce)";
				$arrGroupBy[] = "tpm.tpmid";
				$arrGroupBy[] = "tmacor";
			}else{
				$campoQtde = "(COALESCE($tpdcampotema,$coalesce))";
			}
		}else{ //Município
			$campo = "distinct mun.muncod";
			$campoQtde = "$tpdcampotema";
		}
		
		if($arrLegTema){
			foreach($arrLegTema as $legTema){
				
				$lgtde = str_replace(",",".",$legTema['lgtde']);
				if($legTema['lgtate']){
					$lgtate = str_replace(",",".",$legTema['lgtate']);
				}else{
					$lgtate = $lgtde;
				}
				
				$arrSteps[] = array( "inicio" => $lgtde , "fim" => $lgtate, "cor" => $legTema['lgtcor'] );
			}
		}else{
			if($arrEstuf){
				$arrWhere[] = "mun.estuf in ('".implode("','",$arrEstuf)."') ";
			}
			$sql = "select 
						campo as codigo,
						parametro as qtde
						
					from (
							select 
								$campo as campo,
								$campoQtde as parametro,
								'' as tmacor
							from 
								territoriosgeo.municipio mun
							".($arrInnerJoin ? " inner join ".implode(" inner join ",$arrInnerJoin) : "")." 
							left join
								mapa.temadado pro ON mun.muncod = pro.muncod $whereCampo and pro.tmaid = {$_GET['parametro']}
							".($arrWhere ? " where ".implode(" and ",$arrWhere) : "")."
							".($arrGroupBy ? " group by  ".implode(",",$arrGroupBy) : "")." 
						  ) as foo 
					order by 
						1";
			$arrRes = $db->carregar($sql);
			if($arrRes){
				foreach($arrRes as $res){
					$arrResultado[$res['codigo']] = $res['qtde'];
				}
			}
		}
	}
	
	if(!$arrSteps){
		$qtde_min = (int)min($arrResultado);
		$qtde_max = (int)max($arrResultado);
		$num_caracteres = strlen((string)$qtde_max);
		$num = substr((string)$qtde_max,0,1);
		$max_legenda = (int)$num+1;
		for($i=0;$i<($num_caracteres-1);$i++){
			 $max_legenda.="0";
		}
		
		if($unmid == UNIDADEMEDICAO_PERCENTUAL){
			$max_legenda = 100;
		}
		$max_legenda = (int)$max_legenda;
		$num_soma = $max_legenda/5;
		
		if($tpdcampotema == "tmdboleano") {

			$arrSteps[] = array("inicio" => "t", "fim" => "t");
			$arrSteps[] = array("inicio" => "f", "fim" => "f");

		} else {
		
			for($i=1;$i<=5;$i++){
				if($i==1){
					$arrSteps[] = array("inicio" => 0, "fim" => $num_soma);
				}else{
					$arrSteps[] = array("inicio" => (($num_soma*($i-1))+1), "fim" => $num_soma*($i));
				}
			}
		
		}
	}
	
	$sql = "select tmacor from mapa.tema where tmaid = {$_GET['parametro']}";
	$tmacor = $db->pegaUm($sql);
	if($tmacor){
		$arrCores[$_GET['parametro']] = $tmacor;
	}else{
		$arrCores[$_GET['parametro']] = "blue";
	}
	
	$i = 1;
//	if($arrSteps){
//		$arrJS[] = "arrIncio['{$_GET['parametro']}'] = false; arrIncio['{$_GET['parametro']}'] = new Array(); ";
//		$arrJS[] = "arrFim['{$_GET['parametro']}'] = false; arrFim['{$_GET['parametro']}'] = new Array(); ";
//		foreach($arrSteps as $i => $range){
//			if($range['inicio'] >= 0 && $range['fim'] >= 0){
//				if($range['inicio'] == $range['fim']){
//					if($tpdcampotema == "tmdboleano"){
//						echo "<div style=\"width:300px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;cursor:pointer\" onclick=\"exibeMunicipiosLegenda(".($i+1).")\" ><input type=\"hidden\" id=\"hdn_range_".($i+1)."\" name=\"hdn_range_".($i+1)."\" value=\"\" /></div> <div style=\"float:left;width:220px\" >".($range['inicio'] == "t" ? "Sim" : "Não")."</div> <div style=\"float:left;padding-left:2px;cursor:pointer;text-align:right;width:40px;\" onclick=\"marcarNoMapa(".($i+1).")\" title=\"Visualizar no Mapa\" id=\"count_range_".($i+1)."\" ></div> </div> ";
//					}else{
//						echo "<div style=\"width:300px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;cursor:pointer\" onclick=\"exibeMunicipiosLegenda(".($i+1).")\" ><input type=\"hidden\" id=\"hdn_range_".($i+1)."\" name=\"hdn_range_".($i+1)."\" value=\"\" /></div> <div style=\"float:left;width:220px\" >".($tpdcampotema != "tmdboleano" ? "<input type='text' style=\"border:0px\" id=\"ipt_valor_inicio_{$_GET['parametro']}_$i\" name=\"ipt_valor[]\" onchange=\"alteravalorRange('inicio','$i','{$_GET['parametro']}',this)\" size='".(strlen(number_format($qtde_max,2,',','.'))+($percent ? 2 : 1))."' value='" : "").($percent ?number_format($qtde_max,2,',','.')."%" : number_format($range['inicio'],0,'','.')).($tpdcampotema != "tmdboleano" ? "' />" : "")."</div> <div style=\"float:left;padding-left:2px;cursor:pointer;text-align:right;width:40px;\" title=\"Visualizar no Mapa\" onclick=\"marcarNoMapa(".($i+1).")\" id=\"count_range_".($i+1)."\" ></div> </div> ";
//					}
//				}else{
//					echo "<div style=\"width:300px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;cursor:pointer\" onclick=\"exibeMunicipiosLegenda(".($i+1).")\" ><input type=\"hidden\" id=\"hdn_range_".($i+1)."\" name=\"hdn_range_".($i+1)."\" value=\"\" /></div> <div style=\"float:left;width:220px\" >".($tpdcampotema != "tmdboleano" ? "<input type='text' style=\"border:0px\" id=\"ipt_valor_inicio_{$_GET['parametro']}_$i\" name=\"ipt_valor[]\" onchange=\"alteravalorRange('inicio','$i','{$_GET['parametro']}',this)\" size='".(strlen(number_format($qtde_max,2,',','.'))+($percent ? 2 : 1))."' value='" : "").($percent ?number_format($qtde_max,2,',','.')."%" : number_format($range['inicio'],0,'','.')).($tpdcampotema != "tmdboleano" ? "' />" : "")." - ".($range['fim'] && $tpdcampotema != "tmdboleano" ? "<input style=\"border:0px\" id=\"ipt_valor_fim_{$_GET['parametro']}_$i\" type='text' name=\"ipt_valor[]\" onchange=\"alteravalorRange('fim','$i','{$_GET['parametro']}',this)\" size='".(strlen(number_format($range['fim'],2,',','.'))+($percent ? 2 : 1))."' value='" : "").($percent ? number_format($range['fim'],2,',','.')."%" : number_format($range['fim'],0,'','.')).($range['fim'] && $tpdcampotema != "tmdboleano" ? "' />" : "")."</div> <div style=\"float:left;padding-left:2px;cursor:pointer;text-align:right;width:40px;\" title=\"Visualizar no Mapa\" onclick=\"marcarNoMapa(".($i+1).")\" id=\"count_range_".($i+1)."\" ></div> </div> ";
//				}
//				if($range['cor']) {
//					$arrJS[] = "var cor = '".$range['cor']."'; document.getElementById('div_range_".($i+1)."').style.background=cor; document.getElementById('div_range_".($i+1)."').setAttribute(\"class\",cor);";
//				} else {
//					$arrJS[] = "var cor = retornarCor('#dddddd', '".$arrCores[$_GET['parametro']]."', ".count($arrSteps)." , ".($i+1)." ); document.getElementById('div_range_".($i+1)."').style.background=cor; document.getElementById('div_range_".($i+1)."').setAttribute(\"class\",cor);";
//				}
//				$arrJS[] = "arrIncio['{$_GET['parametro']}'][$i] = ".($percent ? $range['inicio'] : round($range['inicio'],0) ).";";
//				$arrJS[] = "arrFim['{$_GET['parametro']}'][$i] = ".($percent ? $range['fim'] : round($range['fim'],0) )."; ";
//			}
//		}
//		echo "<div style=\"width:300px;height:20px;\" > <div style=\"float:left;width:15px;height:15px;margin-right:3px;\">&nbsp;</div> <div style=\"float:left;width:220px;text-align:center\" id=\"texto_total_muncod_legenda\" >&nbsp;</div> <div id=\"total_muncod_legenda\" style=\"float:left;padding-left:2px;cursor:pointer;text-align:right;width:40px;\" title=\"Total\"></div> </div> ";
//	}
	if($arrSteps){
		$arrJS[] = "arrIncio['{$_GET['parametro']}'] = false; arrIncio['{$_GET['parametro']}'] = new Array(); ";
		$arrJS[] = "arrFim['{$_GET['parametro']}'] = false; arrFim['{$_GET['parametro']}'] = new Array(); ";
        ?>
        <table class="table table-striped table-bordered table-hover well" style="border-collapse: collapse; border-spacing: 0;">
            <tbody>
        <?php
		foreach($arrSteps as $i => $range){
			if($range['inicio'] >= 0 && $range['fim'] >= 0){
				if($range['inicio'] == $range['fim']){
					if($tpdcampotema == "tmdboleano"){ ?>
<!--                        <tr>-->
<!--                            <td>-->
<!--                                --><?php //echo "<div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;cursor:pointer\" onclick=\"exibeMunicipiosLegenda(".($i+1).")\" >" ?>
<!--                            </td>-->
<!--                            <td>-->
<!--                                --><?php //echo "<input type=\"hidden\" id=\"hdn_range_".($i+1)."\" name=\"hdn_range_".($i+1)."\" value=\"\" />"; ?>
<!--                            </td>-->
<!--                        </tr>-->

                        <?php
						echo "
						<div style=\"width:300px;height:20px;\" >
						    <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;cursor:pointer\" onclick=\"exibeMunicipiosLegenda(".($i+1).")\" >
                            <input type=\"hidden\" id=\"hdn_range_".($i+1)."\" name=\"hdn_range_".($i+1)."\" value=\"\" /></div>
                            <div style=\"float:left;width:220px\" >
                                ".($range['inicio'] == "t" ? "Sim" : "Não")."
                            </div>
                            <div style=\"float:left;padding-left:2px;cursor:pointer;text-align:right;width:40px;\" onclick=\"marcarNoMapa(".($i+1).")\" title=\"Visualizar no Mapa\" id=\"count_range_".($i+1)."\" >

                            </div>
						</div> ";
					}else{
						echo "<div style=\"width:300px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;cursor:pointer\" onclick=\"exibeMunicipiosLegenda(".($i+1).")\" ><input type=\"hidden\" id=\"hdn_range_".($i+1)."\" name=\"hdn_range_".($i+1)."\" value=\"\" /></div> <div style=\"float:left;width:220px\" >".($tpdcampotema != "tmdboleano" ? "<input type='text' style=\"border:0px\" id=\"ipt_valor_inicio_{$_GET['parametro']}_$i\" name=\"ipt_valor[]\" onchange=\"alteravalorRange('inicio','$i','{$_GET['parametro']}',this)\" size='".(strlen(number_format($qtde_max,2,',','.'))+($percent ? 2 : 1))."' value='" : "").($percent ?number_format($qtde_max,2,',','.')."%" : number_format($range['inicio'],0,'','.')).($tpdcampotema != "tmdboleano" ? "' />" : "")."</div> <div style=\"float:left;padding-left:2px;cursor:pointer;text-align:right;width:40px;\" title=\"Visualizar no Mapa\" onclick=\"marcarNoMapa(".($i+1).")\" id=\"count_range_".($i+1)."\" ></div> </div> ";
					}
				}else{
                    ?>
                    <tr>
                        <td>
                            <?php echo "<div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;cursor:pointer\" onclick=\"exibeMunicipiosLegenda(".($i+1).")\" >" ?>
                            <?php echo "<input type=\"hidden\" id=\"hdn_range_".($i+1)."\" name=\"hdn_range_".($i+1)."\" value=\"\" />" ?>
                        </td>
                        <td>
                            <?php echo ($tpdcampotema != "tmdboleano" ? "<input type='text' id=\"ipt_valor_inicio_{$_GET['parametro']}_$i\" name=\"ipt_valor[]\" onchange=\"alteravalorRange('inicio','$i','{$_GET['parametro']}',this)\" size='".(strlen(number_format($qtde_max,2,',','.'))+($percent ? 2 : 1))."' value='" : "").($percent ?number_format($qtde_max,2,',','.')."%" : number_format($range['inicio'],0,'','.')).($tpdcampotema != "tmdboleano" ? "' />" : "") ?>
                            -
                            <?php echo ($range['fim'] && $tpdcampotema != "tmdboleano" ? "<input id=\"ipt_valor_fim_{$_GET['parametro']}_$i\" type='text' name=\"ipt_valor[]\" onchange=\"alteravalorRange('fim','$i','{$_GET['parametro']}',this)\" size='".(strlen(number_format($range['fim'],2,',','.'))+($percent ? 2 : 1))."' value='" : "").($percent ? number_format($range['fim'],2,',','.')."%" : number_format($range['fim'],0,'','.')).($range['fim'] && $tpdcampotema != "tmdboleano" ? "' />" : "") ?>
                        </td>
                    </tr>
                    <?php
//					echo "
//					<div style=\"width:300px;height:20px;\" >
//                        <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;cursor:pointer\" onclick=\"exibeMunicipiosLegenda(".($i+1).")\" >
//                            <input type=\"hidden\" id=\"hdn_range_".($i+1)."\" name=\"hdn_range_".($i+1)."\" value=\"\" />
//                        </div>
//                        <div style=\"float:left;width:220px\" >
//                        ".($tpdcampotema != "tmdboleano" ? "
//                            <input type='text' style=\"border:0px\" id=\"ipt_valor_inicio_{$_GET['parametro']}_$i\" name=\"ipt_valor[]\" onchange=\"alteravalorRange('inicio','$i','{$_GET['parametro']}',this)\" size='".(strlen(number_format($qtde_max,2,',','.'))+($percent ? 2 : 1))."' value='" : "").($percent ?number_format($qtde_max,2,',','.')."%" : number_format($range['inicio'],0,'','.')).($tpdcampotema != "tmdboleano" ? "' />" : "")." - ".
//                            ($range['fim'] && $tpdcampotema != "tmdboleano" ? "<input style=\"border:0px\" id=\"ipt_valor_fim_{$_GET['parametro']}_$i\" type='text' name=\"ipt_valor[]\" onchange=\"alteravalorRange('fim','$i','{$_GET['parametro']}',this)\" size='".(strlen(number_format($range['fim'],2,',','.'))+($percent ? 2 : 1))."' value='" : "").($percent ? number_format($range['fim'],2,',','.')."%" : number_format($range['fim'],0,'','.')).($range['fim'] && $tpdcampotema != "tmdboleano" ? "' />" : "")."
//                        </div>
//                        <div style=\"float:left;padding-left:2px;cursor:pointer;text-align:right;width:40px;\" title=\"Visualizar no Mapa\" onclick=\"marcarNoMapa(".($i+1).")\" id=\"count_range_".($i+1)."\" >
//                        </div>
//					</div> ";
				}
				if($range['cor']) {
					$arrJS[] = "var cor = '".$range['cor']."'; document.getElementById('div_range_".($i+1)."').style.background=cor; document.getElementById('div_range_".($i+1)."').setAttribute(\"class\",cor);";
				} else {
					$arrJS[] = "var cor = retornarCor('#dddddd', '".$arrCores[$_GET['parametro']]."', ".count($arrSteps)." , ".($i+1)." ); document.getElementById('div_range_".($i+1)."').style.background=cor; document.getElementById('div_range_".($i+1)."').setAttribute(\"class\",cor);";
				}
				$arrJS[] = "arrIncio['{$_GET['parametro']}'][$i] = ".($percent ? $range['inicio'] : round($range['inicio'],0) ).";";
				$arrJS[] = "arrFim['{$_GET['parametro']}'][$i] = ".($percent ? $range['fim'] : round($range['fim'],0) )."; ";
			}
		}
		echo "<div style=\"width:300px;height:20px;\" > <div style=\"float:left;width:15px;height:15px;margin-right:3px;\">&nbsp;</div> <div style=\"float:left;width:220px;text-align:center\" id=\"texto_total_muncod_legenda\" >&nbsp;</div> <div id=\"total_muncod_legenda\" style=\"float:left;padding-left:2px;cursor:pointer;text-align:right;width:40px;\" title=\"Total\"></div> </div> ";
	}
?>
        </tbody>
    </table>
<!--    <table class="table table-striped table-bordered table-hover">-->
<!--        <tbody>-->
<!--            <tr>-->
<!--                <td>cor</td>-->
<!--                <td><input type="text" /></td>-->
<!--                <td><input type="text" /></td>-->
<!--            </tr>-->
<!--            <tr>-->
<!--                <td>cor</td>-->
<!--                <td><input type="text" /></td>-->
<!--                <td><input type="text" /></td>-->
<!--            </tr>-->
<!--            <tr>-->
<!--                <td>cor</td>-->
<!--                <td><input type="text" /></td>-->
<!--                <td><input type="text" /></td>-->
<!--            </tr>-->
<!--        </tbody>-->
<!--    </table>-->

	<input type="button" onclick="carregarParametroMapa('<?php echo $_GET['parametro'] ?>');carregarParametroGrafico('<?php echo $_GET['parametro'] ?>')" name="btn_parametro" value="Carregar" />
	<input type="button" onclick="limparMapa()" name="btn_limpar_mapa" value="Limpar" />
	<script type="text/javascript">
	<?php if($arrJS): ?>
		<?php foreach($arrJS as $js): ?>
			<?php echo $js; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	</script> <?php
}



function mapaFiltros() {
	global $db;
?>
<style>
select.CampoEstilo {
    width: 150px;
}
</style>
<script>
var arrIncio = new Array();
var arrFim = new Array();

jQuery(function() {
	 jQuery('#camada').change(function() {
	 	exibeComboTema(jQuery(this).val());
	 	exibeComboBuscaAvancada(jQuery(this).val());
	 });
});

function carregaParametroCombo(parametro)
{
 	if(parametro)
 	{
 		exibeParametroMapa(parametro);
 		jQuery("#grafico_parametro").html("<div id=\"grafico_mapa\" ></div>");
 	}else{
 		jQuery("#span_parametro").html("");
 		jQuery("#div_parametro").html("");
 		jQuery("#grafico_parametro").html("<div id=\"grafico_mapa\" ></div>");
 	}
	 
}

function exibeComboTema(camada)
{
 	var url = "<?=LINK_PADRAO ?>&requisicao=exibeComboTema&mapid=<?php echo $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid']; ?>&camada=" + camada;
 	jQuery.ajax({
			type: "POST",
			url: url,
			async: false,
			success: function(response){
				jQuery("#span_combo_tema").html(response);
				jQuery("#span_parametro").html("");
		 		jQuery("#div_parametro").html("");
		 		jQuery("#grafico_parametro").html("<div id=\"grafico_mapa\" ></div>");
			}
		});
}

function exibeComboBuscaAvancada(camada)
{
	var url = "<?=LINK_PADRAO ?>&requisicao=exibeComboBuscaAvancada&mapid=<?php echo $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid']; ?>&camada=" + camada;
 	jQuery.ajax({
			type: "POST",
			url: url,
			async: false,
			success: function(response){
				jQuery("#td_tmaid_combo_avancado").html(response);
			}
		});
}

function exibeParametroMapa(parametro)
{
	jQuery("#div_parametro").html("Carregando...");
	jQuery("#span_parametro").html(jQuery('[name="combo_parametro"] option:selected').text());
 	var camada = jQuery("#camada").val();
 	
 	var arrUF = new Array();
 	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    var id = jQuery(this).children().attr("id");
		    if(id != ""){
		    	arrUF[i] = id;
		    	i++;
		    }
		}
	});
 	var url = "<?=LINK_PADRAO ?>&acao=A&requisicao=carregaParametro&parametro=" + parametro + "&mapid=<?php echo $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid']; ?>&camada=" + camada + "&estuf=" + arrUF;
 	jQuery.ajax({
			type: "POST",
			url: url,
			async: false,
			success: function(response){
				jQuery("#div_parametro").html(response);
			}
		});
}
html_municipio = "<div style=\"padding:5px\" ><iframe src=\"<?=LINK_PADRAO ?>&requisicao=montaBalao&muncod={muncod}&mapid=<?=$_SESSION['painel_vars']['mapid'] ?>\" frameborder=0 scrolling=\"auto\" height=\"300px\" width=\"480px\" ></iframe></div>";

//html_tipo_municipio = "<div style=\"padding:5px\" ><iframe src=\"painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=montaBalaoTipoMunicipio&gtmid={gtmid}&tpmid={tpmid}&mapid=<?=$_SESSION['painel_vars']['mapid'] ?>\" frameborder=0 scrolling=\"auto\" height=\"300px\" width=\"480px\" ></iframe></div>";

function mostrar_painel(painel) {
	if(document.getElementById(painel).style.display == "none") {
		document.getElementById("img_"+painel).src="../imagens/menos.gif";
		document.getElementById(painel).style.display = "";
	} else {
		document.getElementById("img_"+painel).src="../imagens/mais.gif";
		document.getElementById(painel).style.display = "none";
	}
}

function restaurarItens() {
	removeAllOptions(document.getElementById('agrupador'));
    addOption(document.getElementById('agrupador'),"Região","regiao",false);
    addOption(document.getElementById('agrupador'),"UF","uf",false);
    addOption(document.getElementById('agrupador'),"Município","municipio",false);
}

function carregarParametroMapa(parametro)
{
	
	jQuery("[id^='hdn_range_']").val("");
	
	divCarregando();
	
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	
	var arrCores = new Array();
	
	var totalLegenda = jQuery("[id^=div_range]").length;
	var camada = jQuery("#camada").val();
	var url = "<?=LINK_PADRAO ?>&requisicao=carregarParametroMapa&parametro=" + parametro + "&estuf=" + arrUF + "&camada=" + camada;	
	jQuery.ajax({
		type: "POST",
		url: url,
		async: false,
		dataType:'JSON',
		success: function(response){
//			jQuery("#grafico_parametro").html(response);
//			divCarregado();
//			return false;
			response = jQuery.parseJSON(response);
			var i = 0;
			jQuery.each(response,function(index,item){
			   	if(nomePoli[item.muncod]){
			   		if(item.parametro != "f" && item.parametro != "t"){
				   		var posicao = retornaPosicaoMunicipio(item.parametro,item.tipoparametro);
				   		var cor = '';
						if(posicao)
						{
							cor = '' + retornarCor('#dddddd', item.itemcor , totalLegenda , posicao );
						}
				   		f_mudacor(item.muncod,cor);
				   		criaLinkLegenda(item.muncod,posicao);
				   	}else{
				   		if(item.parametro == "f"){
				   			var posicion;
				   			if(jQuery("#div_range_1").next().html() == "Sim"){
				   				posicion = 2;
				   			}else{
				   				posicion = 1;
				   			}
				   			cor = '' + retornarCor('#dddddd', item.itemcor , totalLegenda , posicion );
				   		}else{
				   			var posicion;
				   			if(jQuery("#div_range_1").next().html() == "Sim"){
				   				posicion = 1;
				   			}else{
				   				posicion = 2;
				   			}
				   			cor = '' + retornarCor('#dddddd', item.itemcor , totalLegenda , posicion );
				   		}
				   		f_mudacor(item.muncod,cor);
				   		criaLinkLegenda(item.muncod,posicion);
				   	}
			   	}
			});
			divCarregado();
			verificaTotalMuncodLegenda();
		}
	});
}

function criaLinkLegenda(muncod,posicao)
{
	if(jQuery("#hdn_range_" + posicao).val()){
		jQuery("#hdn_range_" + posicao).val( jQuery("#hdn_range_" + posicao).val() + "," + muncod);
		//jQuery("#count_range_" + posicao).html( jQuery("#count_range_" + posicao).html()*1 + 1);
	}else{
		jQuery("#hdn_range_" + posicao).val(muncod);
		//jQuery("#count_range_" + posicao).html("1");
	}
}

function verificaTotalMuncodLegenda()
{
	var valor;
	var arrValor;
	var valorTotal = 0;
	jQuery.each(jQuery("[id^='hdn_range_']"),function(){
		valor = jQuery(this).val();
		var id = jQuery(this).attr("id");
		id = id.replace("hdn_range_","");
		if(valor){
			arrValor = valor.split(",");
			jQuery("#count_range_" + id).html( mascaraglobal("[.###]",arrValor.length) );
			valorTotal+=arrValor.length;
		}else{
			jQuery("#count_range_" + id).html( "0" );
			valorTotal+=0;
		}
	});
	jQuery("#total_muncod_legenda").html( mascaraglobal("[.###]",valorTotal) );
	jQuery("#texto_total_muncod_legenda").html( "Total" );
}

function exibeMunicipiosLegenda(posicao)
{
	
	posicao = posicao - 1;
	var combo_parametro = jQuery("[name='combo_parametro']").val();
	jQuery("#cmb_tema_1_1").val(combo_parametro);
	alteraTipoFiltroAvancado(document.getElementById("cmb_tema_1_1"));
	
	if(!jQuery("#ipt_valor_inicio_"+combo_parametro+"_"+posicao).val()){
		if(posicao == 0){
			jQuery("[name='cmb_filtro[1][1]']").val("true");
		}else{
			jQuery("[name='cmb_filtro[1][1]']").val("false");
		}
	}else{
		if(!jQuery("#ipt_valor_fim_"+combo_parametro+"_"+posicao).val()){
			jQuery("[name='cmb_filtro[1][1]']").val("=");
			var valor1 = jQuery("#ipt_valor_inicio_"+combo_parametro+"_"+posicao).val().replace(",00","");
			//valor1 = valor1.replace(",",".")
			jQuery("[name='cmb_filtro_inicio[1][1]']").val(valor1.replace("%",""));
			jQuery("[name='cmb_filtro_fim[1][1]']").val("");
		}else{
			jQuery("[name='cmb_filtro[1][1]']").val("entre");
			var valor1 = jQuery("#ipt_valor_inicio_"+combo_parametro+"_"+posicao).val().replace(",00","");
			//valor1 = valor1.replace(",",".")
			jQuery("[name='cmb_filtro_inicio[1][1]']").val(valor1.replace("%",""));
			var valor2 = jQuery("#ipt_valor_fim_"+combo_parametro+"_"+posicao).val().replace(",00","")
			//valor2 = valor2.replace(",",".")
			jQuery("[name='cmb_filtro_fim[1][1]']").val(valor2.replace("%",""));
		}
	}
	
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	
	jQuery("#estuf_filtros_avancados").val(arrUF);
	
	jQuery("#requisicao_filtros_avancados").val("exibeRelatorioFiltrosAvancados");
	jQuery("#camada_filtros_avancados").val(jQuery("#camada").val());
	var formulario = jQuery("#form_busca_avancada");
    var janela = window.open( 'painel.php?modulo=principal/mapas/popUpMapaPadrao&acao=A&mapid=<?php echo $_GET['mapid'] ?>', 'filtroavancadoresultado', 'width=800,height=600,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
    formulario.submit();
    janela.focus();
	
	/*
	if(jQuery("#hdn_range_" + posicao).val()){
		var camada = jQuery("#camada").val()
		var muncod = jQuery("#hdn_range_" + posicao).val();
		var parametro = jQuery("#span_parametro").html();
		var combo_parametro = jQuery("[name='combo_parametro']").val();
		jQuery("#textoMunicipiosLegenda").html("Carregando...");
		var url = "painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=exibeMunicipiosLegenda";
		jQuery.ajax({
			type: "POST",
			url: url,
			data: "muncod=" + muncod + "&camada=" + camada + "&parametro=" + parametro + "&combo_parametro=" + combo_parametro + "&posicao=" + posicao,
			async: false,
			success: function(response){
				jQuery("#textoMunicipiosLegenda").html(response);
				jQuery("#municipiosLegenda").show();
				jQuery( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
			}
		});

	}else{
		alert('Não existem dados para esta legenda.');
	}*/
}

function exportarMunicipiosLegendaExcel(posicao)
{
	if(jQuery("#hdn_range_" + posicao).val()){
		jQuery("#form_excel_municipios").submit();
	}else{
		alert('Não existem dados para esta legenda.');
	}
	/*
	if(jQuery("#hdn_range_" + posicao).val()){
		var camada = jQuery("#camada").val()
		var muncod = jQuery("#hdn_range_" + posicao).val();
		var parametro = jQuery("#span_parametro").html();
		var combo_parametro = jQuery("[name='combo_parametro']").val();
		var url = "painel.php?modulo=principal/mapas/mapaPadrao&acao=A&requisicao=exportarMunicipiosLegendaExcel&muncod=" + muncod + "&camada=" + camada + "&parametro=" + parametro + "&combo_parametro=" + combo_parametro + "&posicao=" + posicao;
		window.location.href = url;
	}else{
		alert('Não existem dados para esta legenda.');
	}
	*/
}

function marcarNoMapa(range)
{
	var muncod = jQuery("#hdn_range_" + range).val();
	if(muncod){
		var arrMuncod = muncod.split(",");
		if(jQuery("#count_range_" + range).attr("class") == "bold"){
			var cor = jQuery("#div_range_" + range).attr("class");
			jQuery.each(arrMuncod,function(index,value){
				if(nomePoli[value]){
					nomePoli[value].setOptions( {fillColor: cor} );
				}
			});
			jQuery("#count_range_" + range).attr( "class" ,"");
		}else{
			var cor = "#FF00FF";
			jQuery.each(arrMuncod,function(index,value){
				if(nomePoli[value]){
					nomePoli[value].setOptions( {fillColor: cor} );
				}
			});
			jQuery("#count_range_" + range).attr( "class" ,"bold");
		}
	}
}

function restaurarItens() {
	removeAllOptions(document.getElementById('agrupador'));
    addOption(document.getElementById('agrupador'),"Região","regiao",false);
    addOption(document.getElementById('agrupador'),"UF","uf",false);
    addOption(document.getElementById('agrupador'),"Município","municipio",false);
    }

<?
$sql = "SELECT * FROM mapa.indicador WHERE mapid='".$_SESSION['painel_vars']['mapid']."' and indstatus = 'A' ORDER BY indid";
$indicadores = $db->carregar($sql);
?>

function carregarCamadaMapa()
{
	
	if(!jQuery('#camada').val()){
		alert('Selecione a camada');
		return false;
	}
		
	if(jQuery('#cenario').val()) {
		filtros += '&cenario='+jQuery('#cenario').val();
	}
	
	divCarregando();
	
	<?
	$estufs = $db->carregarColuna("SELECT estuf FROM territoriosgeo.estado");
	if($estufs) : 
	 	foreach($estufs as $estuf) : 
	 	?>
	 	if(!estado['<?=$estuf ?>']) {
	 		montarPoligonoEstado('<?=$estuf ?>');
	 	}
	 	<? 
	 	endforeach;
	endif;
	?>
	
	if(jQuery('#camada').val()) {	
		if(nomePoli.length>0) {
			jQuery.each(nomePoli, function(item,value) { 
			  	if(nomePoli[item]){
					nomePoli[item].setMap(null);
				}
			});
		}
	}
	
	switch(jQuery('#camada').val().substring(0,10)) {
		case 'tipmunici_':
		carregarTipoMunicipioPorGrupoMunicipio(jQuery('#camada').val().substring(10));
		break;
		case 'municipio':
		<?
		if($estufs) : 
		 	foreach($estufs as $estuf) : 
		 	?>carregaMunicipiosPorUF('<?=$estuf ?>');<? 
		 	endforeach;
		endif;
		?>
		break;
	}
		
	<? if($indicadores[0]): ?>
	<? foreach($indicadores as $indicador) : ?>
	if(pontoMarcadores['<?=$indicador['indicone'] ?>']) {
	deleteOverlays(pontoMarcadores['<?=$indicador['indicone'] ?>']);
	}
	<? endforeach; ?>
	<? endif; ?>

	divCarregado();
	
}

function salvarFiltrosNormais(mapid)
{
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );		
	selectAllOptions( document.getElementById( 'agrupador' ) );
	var filtros='';
	filtros += '&gtmid='+jQuery('#gtmid').val();
	filtros += '&'+jQuery('#estuf').serialize();
	filtros += '&'+jQuery('#muncod').serialize();
	filtros += '&'+jQuery('#agrupador').serialize();
	if(jQuery("[name^='icone']:checked").length>0) {
		filtros += '&'+jQuery("[name^='icone']").serialize();
	}
	if(jQuery("[name^='cond[']").length > 0) {
		filtros += '&contexto='+jQuery('#contexto').val();
		filtros += '&'+jQuery("[name^='cond[']").serialize();
	}
	if(jQuery('#filt_populacao').val()) {
		filtros += '&filt_populacao='+jQuery('#filt_populacao').val();
		filtros += '&qtd_populacao='+jQuery('#qtd_populacao').val();
		filtros += '&ent_populacao1='+jQuery('#ent_populacao1').val()+'&ent_populacao2='+jQuery('#ent_populacao2').val();
	}
	if(jQuery('#cenario').val()) {
		filtros += '&cenario='+jQuery('#cenario').val();
	}
	
	var fustitulo = jQuery("[name='fustitulo_normal']").val();
	var fuspublico = jQuery("[name='fuspublico_normal']:checked").val();
	
	if(!fustitulo){
		alert('Informe o título do filtro!');
		return false;
	}
	if(!fuspublico){
		alert('O filtro será público?');
		return false;
	}
	
	jQuery.ajax({
		type: "POST",
		url: 'painel.php?modulo=principal/mapas/mapaPadrao&acao=A',
		data: "requisicao=salvarFiltrosNormais&mapid=" + mapid + filtros + "&fustitulo=" + fustitulo + "&fuspublico=" + fuspublico,
		async: false,
		dataType:'JSON',
		success: function(response){
			jQuery('#div_salvar_filtros_normais').hide();
			jQuery("#div_filtros_normais").html(response);
		}
	});
}

function carregarMapasPontos(tipo) {
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );		
	selectAllOptions( document.getElementById( 'agrupador' ) );
	
	var filtros='';

	filtros += '&gtmid='+jQuery('#gtmid').val();
	filtros += '&'+jQuery('#estuf').serialize();
	filtros += '&'+jQuery('#muncod').serialize();
	filtros += '&'+jQuery('#agrupador').serialize();
	
	if ( !document.getElementById( 'agrupador' ).options.length ){
		alert( 'Favor selecionar ao menos um agrupador!' );
		return false;
	}

	
	if(jQuery("[name^='icone']:checked").length>0) {
		filtros += '&'+jQuery("[name^='icone']").serialize();
	} 
	
	if(jQuery("[name^='cond[']").length > 0) {
		filtros += '&contexto='+jQuery('#contexto').val();
		filtros += '&'+jQuery("[name^='cond[']").serialize();
	}
	
	if(jQuery('#filt_populacao').val()) {
		if(jQuery('#filt_populacao').val()=='entre') {
			if(jQuery('#ent_populacao1').val()=='' || jQuery('#ent_populacao2').val()=='') {
				alert('Digite o intervalo da população');
				return false;
			}
		} else if(jQuery('#qtd_populacao').val()=='') {
			alert('Digite a população');
			return false;
		}
		filtros += '&filt_populacao='+jQuery('#filt_populacao').val();
		filtros += '&qtd_populacao='+jQuery('#qtd_populacao').val();
		filtros += '&ent_populacao1='+jQuery('#ent_populacao1').val()+'&ent_populacao2='+jQuery('#ent_populacao2').val();
	}
	
	if(jQuery('#cenario').val()) {
		filtros += '&cenario='+jQuery('#cenario').val();
	}
	
	if(tipo==2) {
		window.location="<?=LINK_PADRAO ?>&requisicao=gerarRelatorio&gerarXLS=true&"+filtros;
		return false;
	}
	
	divCarregando();
	
	<?
	$estufs = $db->carregarColuna("SELECT estuf FROM territoriosgeo.estado");
	if($estufs) : 
	 	foreach($estufs as $estuf) : 
	 	?>
	 	if(!estado['<?=$estuf ?>']) {
	 		montarPoligonoEstado('<?=$estuf ?>');
	 	}
	 	<? 
	 	endforeach;
	endif;
	?>
	
	if(jQuery('#camada').val()) {	
		if(nomePoli.length>0) {
			jQuery.each(nomePoli, function(item,value) { 
			  	if(nomePoli[item]){
					nomePoli[item].setMap(null);
				}
			});
		}
	}
	
	switch(jQuery('#camada').val().substring(0,10)) {
		case 'tipmunici_':
		carregarTipoMunicipioPorGrupoMunicipio(jQuery('#camada').val().substring(10));
		break;
		case 'municipio':
		<?
		if($estufs) : 
		 	foreach($estufs as $estuf) : 
		 	?>carregaMunicipiosPorUF('<?=$estuf ?>');<? 
		 	endforeach;
		endif;
		?>
		break;
	}
		
	<? if($indicadores[0]): ?>
	<? foreach($indicadores as $indicador) : ?>
	if(pontoMarcadores['<?=$indicador['indicone'] ?>']) {
	deleteOverlays(pontoMarcadores['<?=$indicador['indicone'] ?>']);
	}
	<? endforeach; ?>
	<? endif; ?>
	

	
	if(jQuery("[name^='icone']:checked").length > 0){
		marcarPontos("<?=LINK_PADRAO ?>&requisicao=gerarXml"+filtros);
		preencherRelatorio("<?=LINK_PADRAO ?>&requisicao=gerarRelatorio"+filtros);
	}
	divCarregado();

}

function selecionaPopulacao(pop) {
	if(pop == 'entre') {
		document.getElementById('qtd_populacao').value='';
		document.getElementById('div_qtd_populacao').style.display='none';
		document.getElementById('div_ent_populacao').style.display='';
	} else {
		document.getElementById('ent_populacao1').value='';
		document.getElementById('ent_populacao2').value='';
		document.getElementById('div_ent_populacao').style.display='none';
		document.getElementById('div_qtd_populacao').style.display='';
	}
}


function e(estuf) {
	centraliza(6,centroPoliEstado[estuf]);
}

function mm(lat,lng,muncod) {
	var myLatLng = new google.maps.LatLng(lat, lng);
	var contentString = replaceAll(html_municipio,"{muncod}",muncod);
  	infowindow = new google.maps.InfoWindow();
  	infowindow.setContent(contentString);
  	infowindow.setPosition(myLatLng);
  	infowindow.open(map);
}
</script>

<?
$sql = "SELECT * FROM mapa.mapa WHERE mapid='".$_SESSION['painel_vars']['mapid']."'";
$dadosmapa = $db->pegaLinha($sql);

$sql = "SELECT mpcid as codigo, mpcdsc as descricao FROM mapa.mapacenario WHERE mpcstatus='A' AND mapid='".$_SESSION['painel_vars']['mapid']."' order by 2";
$dadoscenario = $db->carregar($sql);
?>
<div id="filtro_normal" >
<form method="post" name="formulario" id="formulario">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<? if($dadoscenario[0]) : ?>
	<tr>
		<td class="SubTituloDireita">Cenário:</td>
		<td colspan="2"><? $db->monta_combo('cenario', $dadoscenario, 'S', '', '', '', '', '', 'N', 'cenario'); ?></td>
	</tr>
	<? endif; ?>
	<?
	if($indicadores[0]) {
		foreach($indicadores as $indicador) {
			?>
			<tr>
				<td><input type="checkbox" name="icone[]" onclick="if(this.checked){showOverlays(pontoMarcadores['<?=$indicador['indicone'] ?>']);}else{clearOverlays(pontoMarcadores['<?=$indicador['indicone'] ?>']);}"  id="id_<?=$indicador['indid'] ?>" value="<?=$indicador['indid'] ?>"  /></td>
				<? if($dadosmapa['mapcontexto']=="t") : ?>
				<td><? $db->monta_combo('cond['.$indicador['indid'].']', array(0 => array('codigo'=>'com','descricao'=>'Com'), 1 => array('codigo'=>'sem','descricao'=>'Sem')), 'S', '', '', '', '', '', 'N', ''); ?></td>
				<? endif; ?>
				<td><img src="<?=$indicador['indicone'] ?>"></td>
				<td><font size=1><?=$indicador['inddsc'] ?></font></td>
			</tr>
			<?
		}
	}
	?>
	
	<? if($dadosmapa['mapcontexto']=="t") : ?>
	<tr>
	<td colspan="2" class="SubTituloDireita">Contexto:</td>
	<td colspan="2"><?
	$db->monta_combo('contexto', array(0 => array('codigo'=>'OR','descricao'=>'OU'), 1 => array('codigo'=>'AND','descricao'=>'E')), 'S', '', '', '', '', '', 'N', 'contexto'); 
	?></td>
	</tr>
	<? endif; ?>
	<tr>
		<td colspan="2" class="SubTituloDireita">Agrupado:</td>
		<td colspan="2" >
			<input type="radio" name="agrupado_pontos" value="1" id="agrupado_pontos_sim" checked="checked" />Sim
			<input type="radio" name="agrupado_pontos" value="0" id="agrupado_pontos_nao" />Não
		</td>
	</tr>
</table>
<br>
<br>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td class="SubTituloEsquerda">
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_gerais" onclick="mostrar_painel('gerais');" border=0> Outros Filtros
	</td>
</tr>
<tr>
	<td>
		<div id="gerais" style="display:none">
		<table align="center" border="0" class="listagem" cellpadding="0" cellspacing="0" width="100%">
		<tr>
			<td class="SubTituloDireita" width="10%">População:</td>
			<td width="10%">
			<?
			$opts = Array(Array('codigo'=>'>' ,'descricao'=>'Maior que'),
						  Array('codigo'=>'>=','descricao'=>'Maior Igual a'),
						  Array('codigo'=>'=' ,'descricao'=>'Igual a'),
						  Array('codigo'=>'<' ,'descricao'=>'Menor que'),
						  Array('codigo'=>'<=','descricao'=>'Menor igual a'),
						  Array('codigo'=>'entre','descricao'=>'Entre'));	
			
						   
			$db->monta_combo('filtro[populacao]', $opts, 'S', 'Selecione', 'selecionaPopulacao', '', '', '', 'N', 'filt_populacao');
			?>
			</td>
			<td>
			<div id="div_qtd_populacao"><? echo campo_texto('qtd_populacao', 'N', 'S', '', 10, 12, "##########", "", '', '', 0, 'id="qtd_populacao"' ); ?></div>
			<div id="div_ent_populacao" style="display:none;"><? echo campo_texto('ent_populacao1', 'N', 'S', '', 10, 12, "#########", "", '', '', 0, 'id="ent_populacao1"' ); ?> à <? echo campo_texto('ent_populacao2', 'N', 'S', '', 10, 12, "#########", "", '', '', 0, 'id="ent_populacao2"' ); ?></div>
			</td>
		</tr>
		</table>
		</div>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_uf" onclick="mostrar_painel('uf');" border=0> UF
	</td>
</tr>
<tr>
	<td>
		<div id="uf" style="display:none">
			<?php
			$sql = "	SELECT
							estuf AS codigo,
							estdescricao AS descricao
						FROM 
							territorios.estado
						ORDER BY
							estdescricao ";

			combo_popup( 'estuf', $sql, 'Selecione as Unidades Federativas', '400x400', 0, array(), '', 'S', false, false, 5, 240, '', '' );
			?>
		</div>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_municipio" onclick="mostrar_painel('municipio');" border=0> Município
	</td>
</tr>
<tr>
	<td>
		<div id="municipio" style="display:none">
			<?php
			$sql = " 	SELECT	
							muncod AS codigo,
							mundescricao AS descricao
						FROM 
							territorios.municipio
						ORDER BY
							mundescricao";

			combo_popup( 'muncod', $sql, 'Selecione os Municípios', '400x400', 0, array(), '', 'S', false, false, 5, 240);							?>
		</div>
	</td>
</tr>

<tr>
	<td class="SubTituloEsquerda" ><img style="cursor:pointer" id="img_agrup" onclick="mostrar_painel('agrup');" src="/imagens/mais.gif"> Agrupadores</td>
</tr>
<tr>
	<td>
	<div style="display:none" id="agrup">
	<p>Grupo de municípios: <?
	$gtmid=6;
	$_GET['mapid'] = $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid'];
	$sql = "SELECT 
				gtm.gtmid as codigo,
				gtmdsc as descricao 
			FROM 
				territoriosgeo.grupotipomunicipio gtm
			INNER JOIN
				mapa.mapacamada mpc ON mpc.gtmid = gtm.gtmid 
			WHERE 
				mpc.mapid = {$_GET['mapid']}
			AND
				gtmstatus='A' 
			ORDER BY 
				gtmdsc"; 
	$db->monta_combo('gtmid', $sql, 'S', '', '', '', '', '200', 'N', 'gtmid','',$gtmid); 
	?></p>

		<?
			// Início dos agrupadores
			$agrupador = new Agrupador('filtros','');
			
			// Dados padrão de destino (nulo)
			$destino = array(
				'regiao' => array(
											'codigo'    => 'regiao',
											'descricao' => 'Região'
				),
				'uf' => array(
											'codigo'    => 'uf',
											'descricao' => 'UF'
				),
				'municipio' => array(
											'codigo'    => 'municipio',
											'descricao' => 'Município'
				)
			);
			
			// Dados padrão de origem
			$origem = array(
				'grupo' => array(
											'codigo'    => 'grupo',
											'descricao' => 'Grupo'
				)
				
			);
			
			// exibe agrupador
			$agrupador->setOrigem( 'naoColunas', null, $origem );
			$agrupador->setDestino( 'agrupador', null, $destino );
			$agrupador->exibir();
	
	?> 
	</div>
	</td>
</tr>
</table>
</div>
<p>
<input type="button" name="carregar" value="Carregar" onclick="carregarMapasPontos(1);">
<input type="button" name="btn_salvar_filtros_normais" value="Salvar Filtros" onclick="jQuery('#div_salvar_filtros_normais').show()">
</p>
<div id="div_salvar_filtros_normais" style="display:none"  >
	<table width="100%" class="tabela" >
		<tr>
			<td width="25%" class="SubtituloDireita" >Título:</td>
			<td><?php echo campo_texto("fustitulo_normal","N","S","",36,255,"","") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Público?</td>
			<td>
				Sim <input type="radio" name="fuspublico_normal" id="fuspublico_sim_normal"  value="t"  />  
				Não <input type="radio" name="fuspublico_normal" id="fuspublico_nao_normal" value="f"  />
			</td>
		</tr>
		<tr>
			<td class="SubtituloEsquerda" >
			</td>
			<td class="SubtituloEsquerda" >
				<input type="button" onclick="salvarFiltrosNormais(<?php echo $_SESSION['painel_vars']['mapid'] ?>)" name="btn_salvar_filtros_normais" value="Salvar"  />
				<input type="button" onclick="jQuery('div_salvar_filtros_normais').hide()" name="btn_cacelar_salvar_filtros_normais" value="Cancelar"  />
			</td>
		</tr>
	</table>
</div>
<div id="div_filtros_normais" style="width:100%" ><?php listaFiltrosNormais($_SESSION['painel_vars']['mapid']) ?></div>
<?
}

function mapaTemas()
{
	global $db;
	
	$sql = "select
				tma.tmaid as codigo,
				tma.tmadsc as descricao
			from
				mapa.mapatema mpt
			inner join
				mapa.mapa map ON map.mapid = mpt.mapid
			inner join
				mapa.tema tma ON tma.tmaid = mpt.tmaid
			where
				map.mapid = {$_SESSION['painel_vars']['mapid']}
			and
				tmastatus = 'A'
			order by
				tma.tmadsc";
	
?>
<table class="tabela" >
	<tr>
		<td class="SubtituloDireita" width="25%" >Tema:</td>
		<td id="span_combo_tema" ><?php $db->monta_combo("combo_parametro",$sql,"S","Selecione...","carregaParametroCombo","","","200px","") ?></td>
	</tr>
</table>
<br />
<b><span id="span_parametro" ></span></b><br /><br />
<div id="div_parametro" ></div>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<div id="grafico_parametro" >
	<div id="grafico_mapa" ></div>
</div>
<?php } ?>
<script type="text/javascript">

function retornaPosicaoMunicipio(deficit,parametro)
{
		
	var posicao = '';
	deficit = deficit*1;
	if(deficit <= 0){
	  		posicao = 1;
	  	}
	  	
	jQuery.each(arrIncio[parametro],function(key,value) { 
	  	if(deficit >= arrIncio[parametro][key] && deficit <= arrFim[parametro][key]){
	  		posicao =  key + 1;
	  	}
	});	
	
	if(!posicao)
	{
		return false;
	}else
	{
		return posicao;
	}
}

function carregarParametroGrafico(parametro)
{
	var randomnumber=Math.floor(Math.random()*10000);
	
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	var parametroCor = new Array();
	var i = 1
	jQuery.each(jQuery("[id^='div_range']"),function(index,item){
	    var cor = jQuery(this).attr("class");
	    cor = cor.replace("#","");
	    var id = jQuery(this).attr("id");
	    id = id.replace("div_range_","");
	    parametroCor[i] = cor;
	    i++;
	});
	
	var camada = jQuery("#camada").val();
	
	var params = ";parametro=" + parametro + ";estuf=" + arrUF + ';cor=' + parametroCor + ";mapid=<?php echo $_GET['mapid'] ? $_GET['mapid'] : $_SESSION['painel_vars']['mapid']; ?>;camada=" + camada + "&arrInicio=" + arrIncio[parametro] + "&arrFim=" + arrFim[parametro];
	
	swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf?t=" + randomnumber , "grafico_mapa", "320", "240", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipoMapas=mapas" + params,"loading":"Carregando gráfico..."} );
}

function callBack()
{
	
}
</script>
<?php
include APPRAIZ . 'includes/maps/maps.inc';
?>
<style>
	.PopUpHidden{display:none;width:500px;height:300px;position:absolute;z-index:100;top:50%;left:50%;margin-top:-150;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;}
</style>
<div class="PopUpHidden" id="municipiosLegenda">
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('municipiosLegenda').style.display='none'" />
	</div>
	<div style="padding:5px;overflow:auto;height:250px;" id="textoMunicipiosLegenda" ></div>
</div>
<?php
if($_GET['carregaMapaAutomativo']){
	$cmb_tema = $_GET['cmb_tema'];
	switch($cmb_tema){
		case 1:
			$tema = 193;
			break;
		case 2:
			$tema = 194;
			break;
		case 3:
			$tema = 195;
			break;
		case 4:
			$tema = 196;
			break;
		case 5:
			$tema = 197;
			break;
	}
	if($tema){ $arrUF = $db->carregarColuna("select estuf from territorios.estado order by estuf")?>
		<script>
			jQuery(function() {
				abreAcordion(6);
				jQuery("#cmb_tema_1_1").val(<?php echo $tema ?>);
				jQuery("[name='cmb_filtro_inicio[1][1]']").val(1);
				<?php foreach($arrUF as $uf): ?>
					clicaTodos('<?php echo strtoupper($uf) ?>', document.getElementById('td_<?php echo strtoupper($uf) ?>'));
				<?php endforeach; ?>
				visualizarFiltrosAvancadosMapa(24);
			});
		</script>
	<?php }
}
?>
<?php if($_GET['cockpit']): ?>
<script>
<?php if($_GET['mapid'] == 12): ?>
	jQuery("#id_69").attr("checked","checked");
	jQuery("#camada").val("municipio");
	carregarMapasPontos(1);
<?php endif; ?>
<?php if($_GET['mapid'] == 20): ?>
	jQuery("#id_104").attr("checked","checked");
	jQuery("#camada").val("municipio");
	carregarMapasPontos(1);
<?php endif; ?>
<?php if($_GET['mapid'] == 32): ?>
	jQuery("#id_132").attr("checked","checked");
	jQuery("#camada").val("municipio");
	carregarMapasPontos(1);
<?php endif; ?>
</script>
<?php endif; ?>