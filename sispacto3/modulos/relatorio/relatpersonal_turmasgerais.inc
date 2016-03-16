<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$perfis = pegaPerfilGeral();


function carregarDadosGeraisTurmas2($dados) {
	global $db, $perfis;

	if(!$dados['agrupador']) {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma2(\'agrupador=brasil\', this);\">' as mais, 'Brasil' as agrup,";
		$groupby = "";
	} elseif($dados['agrupador']=='brasil') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma2(\'agrupador=estado&estuf='||m.estuf||'\', this);\">' as mais, m.estuf as agrup,";
		$groupby = "GROUP BY m.estuf";
	} elseif($dados['agrupador']=='estado') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma2(\'agrupador=municipio&estuf=".$dados['estuf']."&muncod='||m.muncod||'\', this);\">' as mais, m.mundescricao as agrup,";
		$groupby = "GROUP BY m.mundescricao, m.muncod";
		$where = "AND m.estuf='".$dados['estuf']."'";
	} elseif($dados['agrupador']=='municipio') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma2(\'agrupador=escola&estuf=".$dados['estuf']."&muncod=".$dados['muncod']."&tpacodigoescola='||t.tpacodigoescola||'\', this);\">' as mais, t.tpanomeescola as agrup,";
		$groupby = "GROUP BY t.tpacodigoescola, t.tpanomeescola";
		$where = "AND m.estuf='".$dados['estuf']."' AND m.muncod='".$dados['muncod']."'";
	} elseif($dados['agrupador']=='escola') {

		if(!$dados['xls']) {
			$colums  = "'' as mais, '&nbsp;'||t.tpanometurma as agrup,";
			$groupby = "GROUP BY t.tpanometurma";
			$where = "AND t.tpacodigoescola='".$dados['tpacodigoescola']."'";
		} else {
			$colums     = "'' as mais, COALESCE(trim(t.tpanometurma), 'Em branco') as agrup,";
			$groupby    = "GROUP BY t.tpanometurma, m.estuf, m.mundescricao, t.tpacodigoescola, t.tpanomeescola";
			$colunasxls = ", m.estuf, m.mundescricao, t.tpacodigoescola, t.tpanomeescola";
		}
	}


	if(in_array(PFL_COORDENADORIES,$perfis)) {
		if($_SESSION['sispacto2']['universidade']['uncid']) $where .= " AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'";
		else $where .= " AND 1=2";
	}


	$sql = "SELECT  {$colums}
	SUM(tpatotalmeninos9)+SUM(tpatotalmeninos9) as nalunos,
	COUNT(*) as nturmas,
	COUNT(DISTINCT t.iusd) as nprofessores,
	ROUND((SUM(tpatotalmeninos9)+SUM(tpatotalmeninas9))/count(*)::numeric,1) as media,
	SUM(tpatotalmeninos9) as totalmeninos,
	ROUND((SUM(tpatotalmeninos9)*100)/(SUM(tpatotalmeninos9)+SUM(tpatotalmeninas9))::numeric,1) as porc_meninos,
	SUM(tpatotalmeninas9) as totalmeninas,
	ROUND((SUM(tpatotalmeninas9)*100)/(SUM(tpatotalmeninos9)+SUM(tpatotalmeninas9))::numeric,1) as porc_meninas,
	 
	COALESCE(SUM(tpafaixaetariaabaixo6anos9),0) as tpafaixaetariaabaixo6anos,
	ROUND((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixaetariaabaixo6anos,
	 
	COALESCE(SUM(tpafaixaetaria6anos9),0) as faixaetaria6anos,
	ROUND((COALESCE(SUM(tpafaixaetaria6anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa6anos,
	COALESCE(SUM(tpafaixaetaria7anos9),0) as tpafaixaetaria7anos,
	ROUND((COALESCE(SUM(tpafaixaetaria7anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa7anos,
	COALESCE(SUM(tpafaixaetaria8anos9),0) as tpafaixaetaria8anos,
	ROUND((COALESCE(SUM(tpafaixaetaria8anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa8anos,
	COALESCE(SUM(tpafaixaetaria9anos9),0) as tpafaixaetaria9anos,
	ROUND((COALESCE(SUM(tpafaixaetaria9anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa9anos,
	COALESCE(SUM(tpafaixaetaria10anos9),0) as tpafaixaetaria10anos,
	ROUND((COALESCE(SUM(tpafaixaetaria10anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa10anos,
	COALESCE(SUM(tpafaixaetaria11anos9),0) as tpafaixaetaria11anos,
	ROUND((COALESCE(SUM(tpafaixaetaria11anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixa11anos,
	COALESCE(SUM(tpafaixaetariaacima11anos9),0) as tpafaixaetariaacima11anos,
	ROUND((COALESCE(SUM(tpafaixaetariaacima11anos9),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos9),0)+COALESCE(SUM(tpafaixaetaria6anos9),0)+COALESCE(SUM(tpafaixaetaria7anos9),0)+COALESCE(SUM(tpafaixaetaria8anos9),0)+COALESCE(SUM(tpafaixaetaria9anos9),0)+COALESCE(SUM(tpafaixaetaria10anos9),0)+COALESCE(SUM(tpafaixaetaria11anos9),0)+COALESCE(SUM(tpafaixaetariaacima11anos9),0)), 0)::numeric,1) as porc_faixaetariaacima11anos
	 
	{$colunasxls}
	FROM sispacto2.turmasprofessoresalfabetizadores t
	INNER JOIN territorios.municipio m on m.muncod = t.tpamuncodescola
	INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd
	WHERE tpastatus='A' AND tpaconfirmaregencia=true AND (tpatotalmeninos9 is not null or tpatotalmeninas9 is not null) {$where}
	{$groupby}
	ORDER BY 2";

	$cabecalho = array("","Agrupador","Número Alunos","Número Turmas","Número de professores","Média Alunos / Turmas",
	"Número Meninos","%","Número Meninas","%","Abaixo de 6anos","%","Número 6anos","%","Número 7anos","%","Número 8anos","%",
	"Número 9anos","%","Número 10anos","%","Número 11anos","%","Acima de 11anos","%");

	if($dados['xls']) {

	$cabecalho[1] = "Nome da Turma";
	$cabecalho[] = "UF";
	$cabecalho[] = "Município";
	$cabecalho[] = "Código INEP";
	$cabecalho[] = "Nome da escola";

	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=rel_contatos_".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=rel_infoturmas_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,100000000,5,'N','100%','');
		exit;

} else {

	$db->monta_lista_simples($sql,$cabecalho,100000000,5,'N','100%','N');

	}


}


function carregarDadosGeraisTurmas($dados) {
	global $db, $perfis;
	
	if(!$dados['agrupador']) {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=brasil\', this);\">' as mais, 'Brasil' as agrup,";
		$groupby = "";
	} elseif($dados['agrupador']=='brasil') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=estado&estuf='||m.estuf||'\', this);\">' as mais, m.estuf as agrup,";
		$groupby = "GROUP BY m.estuf";
	} elseif($dados['agrupador']=='estado') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=municipio&estuf=".$dados['estuf']."&muncod='||m.muncod||'\', this);\">' as mais, m.mundescricao as agrup,";
		$groupby = "GROUP BY m.mundescricao, m.muncod";
		$where = "AND m.estuf='".$dados['estuf']."'";
	} elseif($dados['agrupador']=='municipio') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=escola&estuf=".$dados['estuf']."&muncod=".$dados['muncod']."&tpacodigoescola='||t.tpacodigoescola||'\', this);\">' as mais, t.tpanomeescola as agrup,";
		$groupby = "GROUP BY t.tpacodigoescola, t.tpanomeescola";
		$where = "AND m.estuf='".$dados['estuf']."' AND m.muncod='".$dados['muncod']."'";
	} elseif($dados['agrupador']=='escola') {
		
		if(!$dados['xls']) {
			$colums  = "'' as mais, '&nbsp;'||t.tpanometurma as agrup,";
			$groupby = "GROUP BY t.tpanometurma";
			$where = "AND t.tpacodigoescola='".$dados['tpacodigoescola']."'";
		} else {
			$colums     = "'' as mais, COALESCE(trim(t.tpanometurma), 'Em branco') as agrup,";
			$groupby    = "GROUP BY t.tpanometurma, m.estuf, m.mundescricao, t.tpacodigoescola, t.tpanomeescola";
			$colunasxls = ", m.estuf, m.mundescricao, t.tpacodigoescola, t.tpanomeescola";
		}
	}
	
	
	if(in_array(PFL_COORDENADORIES,$perfis)) {
		if($_SESSION['sispacto2']['universidade']['uncid']) $where .= " AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'";
		else $where .= " AND 1=2";
	}
	
	
	$sql = "SELECT  {$colums}
	                SUM(tpatotalmeninos)+SUM(tpatotalmeninos) as nalunos,
	                COUNT(*) as nturmas,
	                COUNT(DISTINCT t.iusd) as nprofessores,
	                ROUND((SUM(tpatotalmeninos)+SUM(tpatotalmeninas))/count(*)::numeric,1) as media,
	                SUM(tpatotalmeninos) as totalmeninos, 
	                ROUND((SUM(tpatotalmeninos)*100)/(SUM(tpatotalmeninos)+SUM(tpatotalmeninas))::numeric,1) as porc_meninos,
	                SUM(tpatotalmeninas) as totalmeninas,
	                ROUND((SUM(tpatotalmeninas)*100)/(SUM(tpatotalmeninos)+SUM(tpatotalmeninas))::numeric,1) as porc_meninas,
	                
	                COALESCE(SUM(tpafaixaetariaabaixo6anos),0) as tpafaixaetariaabaixo6anos,
	                ROUND((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixaetariaabaixo6anos,
	                
	                COALESCE(SUM(tpafaixaetaria6anos),0) as faixaetaria6anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria6anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa6anos,
	                COALESCE(SUM(tpafaixaetaria7anos),0) as tpafaixaetaria7anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria7anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa7anos,
	                COALESCE(SUM(tpafaixaetaria8anos),0) as tpafaixaetaria8anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria8anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa8anos,
	                COALESCE(SUM(tpafaixaetaria9anos),0) as tpafaixaetaria9anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria9anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa9anos,
	                COALESCE(SUM(tpafaixaetaria10anos),0) as tpafaixaetaria10anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria10anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa10anos,
	                COALESCE(SUM(tpafaixaetaria11anos),0) as tpafaixaetaria11anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria11anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixa11anos,
	                COALESCE(SUM(tpafaixaetariaacima11anos),0) as tpafaixaetariaacima11anos,
	                ROUND((COALESCE(SUM(tpafaixaetariaacima11anos),0)*100)/NULLIF((COALESCE(SUM(tpafaixaetariaabaixo6anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetariaacima11anos),0)), 0)::numeric,1) as porc_faixaetariaacima11anos,
	                
	                COALESCE(SUM(tpabolsafamiliaabaixo6anos),0)+COALESCE(SUM(tpabolsafamilia6anos),0)+COALESCE(SUM(tpabolsafamilia7anos),0)+COALESCE(SUM(tpabolsafamilia8anos),0)+COALESCE(SUM(tpabolsafamilia9anos),0)+COALESCE(SUM(tpabolsafamilia10anos),0)+COALESCE(SUM(tpabolsafamilia11anos),0)+COALESCE(SUM(tpabolsafamiliaacima11anos),0) as tpatotalbolsafamilia,
	                ROUND(((COALESCE(SUM(tpabolsafamiliaabaixo6anos),0)+COALESCE(SUM(tpabolsafamilia6anos),0)+COALESCE(SUM(tpabolsafamilia7anos),0)+COALESCE(SUM(tpabolsafamilia8anos),0)+COALESCE(SUM(tpabolsafamilia9anos),0)+COALESCE(SUM(tpabolsafamilia10anos),0)+COALESCE(SUM(tpabolsafamilia11anos),0)+COALESCE(SUM(tpabolsafamiliaacima11anos),0))*100)/NULLIF((COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0)),0)::numeric,1) as porc_bolsafamilia,
	                
	                COALESCE(SUM(tpavivemcomunidadeabaixo6anos),0)+COALESCE(SUM(tpavivemcomunidade6anos),0)+COALESCE(SUM(tpavivemcomunidade7anos),0)+COALESCE(SUM(tpavivemcomunidade8anos),0)+COALESCE(SUM(tpavivemcomunidade9anos),0)+COALESCE(SUM(tpavivemcomunidade10anos),0)+COALESCE(SUM(tpavivemcomunidade11anos),0)+COALESCE(SUM(tpavivemcomunidadeacima11anos),0) as tpatotalvivemcomunidade,
	                ROUND(((COALESCE(SUM(tpavivemcomunidadeabaixo6anos),0)+COALESCE(SUM(tpavivemcomunidade6anos),0)+COALESCE(SUM(tpavivemcomunidade7anos),0)+COALESCE(SUM(tpavivemcomunidade8anos),0)+COALESCE(SUM(tpavivemcomunidade9anos),0)+COALESCE(SUM(tpavivemcomunidade10anos),0)+COALESCE(SUM(tpavivemcomunidade11anos),0)+COALESCE(SUM(tpavivemcomunidadeacima11anos),0))*100)/NULLIF((COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0)),0)::numeric,1) as porc_vivemcomunidade,
	                
	                COALESCE(SUM(tpafreqcrecheabaixo6anos),0)+COALESCE(SUM(tpafreqcreche6anos),0)+COALESCE(SUM(tpafreqcreche7anos),0)+COALESCE(SUM(tpafreqcreche8anos),0)+COALESCE(SUM(tpafreqcreche9anos),0)+COALESCE(SUM(tpafreqcreche10anos),0)+COALESCE(SUM(tpafreqcreche11anos),0)+COALESCE(SUM(tpafreqcrecheacima11anos),0) as tpatotalfreqcreche,
	                ROUND(((COALESCE(SUM(tpafreqcrecheabaixo6anos),0)+COALESCE(SUM(tpafreqcreche6anos),0)+COALESCE(SUM(tpafreqcreche7anos),0)+COALESCE(SUM(tpafreqcreche8anos),0)+COALESCE(SUM(tpafreqcreche9anos),0)+COALESCE(SUM(tpafreqcreche10anos),0)+COALESCE(SUM(tpafreqcreche11anos),0)+COALESCE(SUM(tpafreqcrecheacima11anos),0))*100)/NULLIF((COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0)),0)::numeric,1) as porc_freqcreche,
	                
	                COALESCE(SUM(tpafreqpreescolaabaixo6anos),0)+COALESCE(SUM(tpafreqpreescola6anos),0)+COALESCE(SUM(tpafreqpreescola7anos),0)+COALESCE(SUM(tpafreqpreescola8anos),0)+COALESCE(SUM(tpafreqpreescola9anos),0)+COALESCE(SUM(tpafreqpreescola10anos),0)+COALESCE(SUM(tpafreqpreescola11anos),0)+COALESCE(SUM(tpafreqpreescolaacima11anos),0) as tpatotalfreqpreescola,
	                ROUND(((COALESCE(SUM(tpafreqpreescolaabaixo6anos),0)+COALESCE(SUM(tpafreqpreescola6anos),0)+COALESCE(SUM(tpafreqpreescola7anos),0)+COALESCE(SUM(tpafreqpreescola8anos),0)+COALESCE(SUM(tpafreqpreescola9anos),0)+COALESCE(SUM(tpafreqpreescola10anos),0)+COALESCE(SUM(tpafreqpreescola11anos),0)+COALESCE(SUM(tpafreqpreescolaacima11anos),0))*100)/NULLIF((COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0)),0)::numeric,1) as porc_freqpreescola
	                {$colunasxls}
			FROM sispacto2.turmasprofessoresalfabetizadores t
			INNER JOIN territorios.municipio m on m.muncod = t.tpamuncodescola 
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd  
			WHERE tpastatus='A' AND tpaconfirmaregencia=true AND (tpatotalmeninos is not null or tpatotalmeninas is not null) {$where}
			{$groupby} 
			ORDER BY 2";
	
	$cabecalho = array("","Agrupador","Número Alunos","Número Turmas","Número de professores","Média Alunos / Turmas",
					   "Número Meninos","%","Número Meninas","%","Abaixo de 6anos","%","Número 6anos","%","Número 7anos","%","Número 8anos","%",
					   "Número 9anos","%","Número 10anos","%","Número 11anos","%","Acima de 11anos","%","Número recebem bolsa familia","%","Número vivem comunidade","%","Número frequentaram creche","%","Número frequentaram pre escola","%");
	
	if($dados['xls']) {
		
		$cabecalho[1] = "Nome da Turma";
		$cabecalho[] = "UF";
		$cabecalho[] = "Município";
		$cabecalho[] = "Código INEP";
		$cabecalho[] = "Nome da escola";
		
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=rel_contatos_".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=rel_infoturmas_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,100000000,5,'N','100%','');
		exit;
		
	} else {
	
		$db->monta_lista_simples($sql,$cabecalho,100000000,5,'N','100%','N');
	
	}
	
	
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="./js/sispacto2.js"></script>
<script>
function detalharDadosTurma2(agrup, obj) {

	var linha = obj.parentNode.parentNode;
	var table = obj.parentNode.parentNode.parentNode;


	if(obj.title=='mais') {
		var nlinha = table.insertRow(linha.rowIndex);
		var ncol0 = nlinha.insertCell(0);
		ncol0.colSpan = 30;
		ncol0.id = 'id_'+agrup+'_'+nlinha.rowIndex;
		ajaxatualizar('buscar=1&relatorio=relatpersonal_turmasgerais.inc&requisicao=carregarDadosGeraisTurmas2&'+agrup,'id_'+agrup+'_'+nlinha.rowIndex);
		obj.title='menos'
		obj.src='../imagens/menos.gif'
	} else {
		table.deleteRow(linha.rowIndex);
		obj.title='mais'
		obj.src='../imagens/mais.gif'
	
	}
}


function detalharDadosTurma(agrup, obj) {

	var linha = obj.parentNode.parentNode;
	var table = obj.parentNode.parentNode.parentNode;


	if(obj.title=='mais') {
		var nlinha = table.insertRow(linha.rowIndex);
		var ncol0 = nlinha.insertCell(0);
		ncol0.colSpan = 34;
		ncol0.id = 'id_'+agrup+'_'+nlinha.rowIndex;
		ajaxatualizar('buscar=1&relatorio=relatpersonal_turmasgerais.inc&requisicao=carregarDadosGeraisTurmas&'+agrup,'id_'+agrup+'_'+nlinha.rowIndex);
		obj.title='menos'
		obj.src='../imagens/menos.gif'
	} else {
		table.deleteRow(linha.rowIndex);
		obj.title='mais'
		obj.src='../imagens/mais.gif'
	
	}
}

function verXLS() {
	window.location='sispacto2.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_turmasgerais.inc&requisicao=carregarDadosGeraisTurmas&agrupador=escola&xls=1';
}
function verXLS2() {
	window.location='sispacto2.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_turmasgerais.inc&requisicao=carregarDadosGeraisTurmas2&agrupador=escola&xls=1';
}
</script>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr>
        	<td class="SubTituloEsquerda">Dados Gerais das turmas - INÍCIO</td>
        </tr>	
        <? if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto2']['universidade']['uncid']) : ?>
        <tr>
        	<td class="SubTituloCentro">Âmbito : <? echo $db->pegaUm("SELECT unisigla||' - '||uninome FROM sispacto2.universidade u 
        													 INNER JOIN sispacto2.universidadecadastro uu ON u.uniid = uu.uniid 
        													 WHERE uu.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'") ?></td>
        </tr>
        <? endif; ?>
		<tr>
			<td><? carregarDadosGeraisTurmas($dados); ?></td>
		</tr>
		<tr>
			<td align="center"><input type="button" name="verxls" value="Ver XLS" onclick="verXLS();"></td>
		</tr>
</table>


	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr>
        	<td class="SubTituloEsquerda">Dados Gerais das turmas - FIM</td>
        </tr>	
        <? if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto2']['universidade']['uncid']) : ?>
        <tr>
        	<td class="SubTituloCentro">Âmbito : <? echo $db->pegaUm("SELECT unisigla||' - '||uninome FROM sispacto2.universidade u 
        													 INNER JOIN sispacto2.universidadecadastro uu ON u.uniid = uu.uniid 
        													 WHERE uu.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'") ?></td>
        </tr>
        <? endif; ?>
		<tr>
			<td><? carregarDadosGeraisTurmas2($dados); ?></td>
		</tr>
		<tr>
			<td align="center"><input type="button" name="verxls" value="Ver XLS" onclick="verXLS2();"></td>
		</tr>
</table>