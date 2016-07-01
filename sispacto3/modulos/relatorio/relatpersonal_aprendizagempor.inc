<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

function carregarAprendizagem2($dados) {
	global $db;

	if(!$dados['agrupador']) {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma2(\'agrupador=conhecimento&catid='||p.catid||'\', this);\">' as mais, p.catdsc as agrup,";
		$groupby = "GROUP BY p.catid, p.catdsc";
		$orderby = "ORDER BY p.catid";
	} elseif($dados['agrupador']=='conhecimento') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma2(\'agrupador=tipoturma&catid=".$dados['catid']."&tpaetapaturma='||t.tpaetapaturma||'\', this);\">' as mais, t.tpaetapaturma as agrup,";
		$groupby = "GROUP BY t.tpaetapaturma";
		$orderby = "ORDER BY t.tpaetapaturma";
		$where = "AND p.catid='".$dados['catid']."'";
	} elseif($dados['agrupador']=='tipoturma') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma2(\'agrupador=estado&tpaetapaturma=".$dados['tpaetapaturma']."&catid=".$dados['catid']."&estuf='||m.estuf||'\', this);\">' as mais, m.estuf as agrup,";
		$groupby = "GROUP BY m.estuf";
		$orderby = "ORDER BY m.estuf";
		$where = "AND tpaetapaturma='".utf8_decode($dados['tpaetapaturma'])."' AND p.catid='".$dados['catid']."'";
	} elseif($dados['agrupador']=='estado') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=municipio&tpaetapaturma=".$dados['tpaetapaturma']."&catid=".$dados['catid']."&estuf=".$dados['estuf']."&muncod='||m.muncod||'\', this);\">' as mais, m.mundescricao as agrup,";
		$groupby = "GROUP BY m.mundescricao, m.muncod";
		$where = "AND tpaetapaturma='".$dados['tpaetapaturma']."' AND p.catid='".$dados['catid']."' AND m.estuf='".$dados['estuf']."'";
	} elseif($dados['agrupador']=='municipio') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=escola&tpaetapaturma=".$dados['tpaetapaturma']."&catid=".$dados['catid']."&estuf=".$dados['estuf']."&muncod=".$dados['muncod']."&tpacodigoescola='||t.tpacodigoescola||'\', this);\">' as mais, t.tpanomeescola as agrup,";
		$groupby = "GROUP BY t.tpacodigoescola, t.tpanomeescola";
		$where = "AND tpaetapaturma='".$dados['tpaetapaturma']."' AND p.catid='".$dados['catid']."' AND m.estuf='".$dados['estuf']."' AND m.muncod='".$dados['muncod']."'";
	} elseif($dados['agrupador']=='escola') {
		$colums  = "'' as mais, '&nbsp;'||t.tpanometurma as agrup,";
		$groupby = "GROUP BY t.tpanometurma";
		$where = "AND tpaetapaturma='".$dados['tpaetapaturma']."' AND p.catid='".$dados['catid']."' AND t.tpacodigoescola='".$dados['tpacodigoescola']."'";
	}
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto2']['universidade']['uncid']) {
		$inner_ies = "INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'";
	}

	$sql = "SELECT  {$colums}
	COUNT(DISTINCT t.iusd) as totalprofessores,
	COUNT(DISTINCT t.tpaid) as totalturmas,
	SUM(actsim) as actsim,
	ROUND((coalesce(SUM(actsim),0)*100)/(coalesce(SUM(actsim),0)+coalesce(SUM(actparcialmente),0)+coalesce(SUM(actnao),0))::numeric,1) as porc_actsim,
	SUM(actparcialmente) as actparcialmente,
	ROUND((coalesce(SUM(actparcialmente),0)*100)/(coalesce(SUM(actsim),0)+coalesce(SUM(actparcialmente),0)+coalesce(SUM(actnao),0))::numeric,1) as porc_actparcialmente,
	SUM(actnao) as actnao,
	ROUND((coalesce(SUM(actnao),0)*100)/(coalesce(SUM(actsim),0)+coalesce(SUM(actparcialmente),0)+coalesce(SUM(actnao),0))::numeric,1) as porc_actnao
	FROM sispacto2.aprendizagemconhecimentoturma2 a
	INNER JOIN sispacto2.aprendizagemconhecimento p ON p.catid = a.catid
	INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
	{$inner_ies}
	INNER JOIN territorios.municipio m on m.muncod = t.tpamuncodescola
	WHERE p.cattipo='P' AND (tpatotalmeninos9 is not null or tpatotalmeninas9 is not null) {$where}
	{$groupby}
	{$orderby}";
	
	$arr = $db->carregar($sql);

	$cabecalho = array("&nbsp;","Agrupador","Número Professores","Número Turmas","Sim","%","Parcialmente","%","Não","%");
	
	$db->monta_lista_simples($arr,$cabecalho,100000,5,'N','100%','N');
	
	return $arr;


}



function carregarAprendizagem($dados) {
	global $db;
	
	if(!$dados['agrupador']) {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=conhecimento&catid='||p.catid||'\', this);\">' as mais, p.catdsc as agrup,";
		$groupby = "GROUP BY p.catid, p.catdsc";
		$orderby = "ORDER BY p.catid";
	} elseif($dados['agrupador']=='conhecimento') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=tipoturma&catid=".$dados['catid']."&tpaetapaturma='||t.tpaetapaturma||'\', this);\">' as mais, t.tpaetapaturma as agrup,";
		$groupby = "GROUP BY t.tpaetapaturma";
		$orderby = "ORDER BY t.tpaetapaturma";
		$where = "AND p.catid='".$dados['catid']."'";
	} elseif($dados['agrupador']=='tipoturma') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=estado&tpaetapaturma=".$dados['tpaetapaturma']."&catid=".$dados['catid']."&estuf='||m.estuf||'\', this);\">' as mais, m.estuf as agrup,";
		$groupby = "GROUP BY m.estuf";
		$orderby = "ORDER BY m.estuf";
		$where = "AND tpaetapaturma='".utf8_decode($dados['tpaetapaturma'])."' AND p.catid='".$dados['catid']."'";
	} elseif($dados['agrupador']=='estado') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=municipio&tpaetapaturma=".$dados['tpaetapaturma']."&catid=".$dados['catid']."&estuf=".$dados['estuf']."&muncod='||m.muncod||'\', this);\">' as mais, m.mundescricao as agrup,";
		$groupby = "GROUP BY m.mundescricao, m.muncod";
		$where = "AND tpaetapaturma='".$dados['tpaetapaturma']."' AND p.catid='".$dados['catid']."' AND m.estuf='".$dados['estuf']."'";
	} elseif($dados['agrupador']=='municipio') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=escola&tpaetapaturma=".$dados['tpaetapaturma']."&catid=".$dados['catid']."&estuf=".$dados['estuf']."&muncod=".$dados['muncod']."&tpacodigoescola='||t.tpacodigoescola||'\', this);\">' as mais, t.tpanomeescola as agrup,";
		$groupby = "GROUP BY t.tpacodigoescola, t.tpanomeescola";
		$where = "AND tpaetapaturma='".$dados['tpaetapaturma']."' AND p.catid='".$dados['catid']."' AND m.estuf='".$dados['estuf']."' AND m.muncod='".$dados['muncod']."'";
	} elseif($dados['agrupador']=='escola') {
		$colums  = "'' as mais, '&nbsp;'||t.tpanometurma as agrup,";
		$groupby = "GROUP BY t.tpanometurma";
		$where = "AND tpaetapaturma='".$dados['tpaetapaturma']."' AND p.catid='".$dados['catid']."' AND t.tpacodigoescola='".$dados['tpacodigoescola']."'";
	}
		
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto2']['universidade']['uncid']) {
		$inner_ies = "INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'";
	}
	
	$sql = "SELECT  {$colums}
			COUNT(DISTINCT t.iusd) as totalprofessores,
			COUNT(DISTINCT t.tpaid) as totalturmas,
			SUM(actsim) as actsim,
			ROUND((coalesce(SUM(actsim),0)*100)/(coalesce(SUM(actsim),0)+coalesce(SUM(actparcialmente),0)+coalesce(SUM(actnao),0))::numeric,1) as porc_actsim,
			SUM(actparcialmente) as actparcialmente,
			ROUND((coalesce(SUM(actparcialmente),0)*100)/(coalesce(SUM(actsim),0)+coalesce(SUM(actparcialmente),0)+coalesce(SUM(actnao),0))::numeric,1) as porc_actparcialmente,
			SUM(actnao) as actnao,
			ROUND((coalesce(SUM(actnao),0)*100)/(coalesce(SUM(actsim),0)+coalesce(SUM(actparcialmente),0)+coalesce(SUM(actnao),0))::numeric,1) as porc_actnao
			FROM sispacto2.aprendizagemconhecimentoturma a 
			INNER JOIN sispacto2.aprendizagemconhecimento p ON p.catid = a.catid
			INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
			{$inner_ies}
			INNER JOIN territorios.municipio m on m.muncod = t.tpamuncodescola 
			WHERE p.cattipo='P' AND (tpatotalmeninos is not null or tpatotalmeninas is not null) {$where}
			{$groupby} 
			{$orderby}";
			
	$arr = $db->carregar($sql);
	
	$cabecalho = array("&nbsp;","Agrupador","Número Professores","Número Turmas","Sim","%","Parcialmente","%","Não","%");
	$db->monta_lista_simples($arr,$cabecalho,100000,5,'N','100%','N');
	
	return $arr;
	
	
}


function carregarAprendizagemDif($dados) {
	global $db;
	
	if(!$dados['agrupador']) {
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Conhecimento</td>';
		echo '<td>';
		
		$sql = "SELECT 'agrupador=conhecimento&catid='||p.catid as codigo, catdsc as descricao FROM sispacto2.aprendizagemconhecimento p WHERE p.cattipo='P'";
		$db->monta_combo('filtro1', $sql, 'S', 'Selecione', 'detalharDadosTurmaDif', '', '', '', 'S', 'filtro1', '', '', '', 'style="font-size:x-small;"');
		
		echo '</td>';
		echo '</tr>';
		echo '</table>';
		
		
		
	} elseif($dados['agrupador']=='conhecimento') {
		
		$dados['r1'] = carregarAprendizagem(array('agrupador'=>$dados['agrupador'],'catid'=>$dados['catid']));
		$dados['r2'] = carregarAprendizagem2(array('agrupador'=>$dados['agrupador'],'catid'=>$dados['catid']));
		
		ob_clean();
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Conhecimento</td>';
		echo '<td>';
		
		$sql = "SELECT 'agrupador=conhecimento&catid='||p.catid as codigo, catdsc as descricao FROM sispacto2.aprendizagemconhecimento p WHERE p.cattipo='P'";
		$db->monta_combo('filtro1', $sql, 'S', 'Selecione', 'detalharDadosTurmaDif', '', '', '', 'S', 'filtro1', '', 'agrupador=conhecimento&catid='.$dados['catid'], '', 'style="font-size:x-small;"');
		
		echo '</td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Turma</td>';
		echo '<td>';
		
		$sql = "SELECT distinct 'agrupador=tipoturma&catid=".$dados['catid']."&tpaetapaturma='||t.tpaetapaturma as codigo, t.tpaetapaturma as descricao from sispacto2.turmasprofessoresalfabetizadores t order by t.tpaetapaturma";
		$db->monta_combo('filtro2', $sql, 'S', 'Selecione', 'detalharDadosTurmaDif', '', '', '', 'S', 'filtro2', '', '', '', 'style="font-size:x-small;"');
		
		echo '</td>';
		echo '</tr>';
		
		echo '</table>';
		
		

	} elseif($dados['agrupador']=='tipoturma') {
		
		$dados['r1'] = carregarAprendizagem(array('agrupador'=>$dados['agrupador'],'catid'=>$dados['catid'],'tpaetapaturma' => $dados['tpaetapaturma']));
		$dados['r2'] = carregarAprendizagem2(array('agrupador'=>$dados['agrupador'],'catid'=>$dados['catid'],'tpaetapaturma' => $dados['tpaetapaturma']));
		
		ob_clean();
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Conhecimento</td>';
		echo '<td>';
		
		$sql = "SELECT 'agrupador=conhecimento&catid='||p.catid as codigo, catdsc as descricao FROM sispacto2.aprendizagemconhecimento p WHERE p.cattipo='P'";
		$db->monta_combo('filtro1', $sql, 'S', 'Selecione', 'detalharDadosTurmaDif', '', '', '', 'S', 'filtro1', '', 'agrupador=conhecimento&catid='.$dados['catid'], '', 'style="font-size:x-small;"');
		
		echo '</td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Turma</td>';
		echo '<td>';
		
		$sql = "SELECT distinct 'agrupador=tipoturma&catid=".$dados['catid']."&tpaetapaturma='||t.tpaetapaturma as codigo, t.tpaetapaturma as descricao from sispacto2.turmasprofessoresalfabetizadores t";
		$db->monta_combo('filtro2', $sql, 'S', 'Selecione', 'detalharDadosTurmaDif', '', '', '', 'S', 'filtro2', '', 'agrupador=tipoturma&catid='.$dados['catid'].'&tpaetapaturma='.utf8_decode($dados['tpaetapaturma']), '', 'style="font-size:x-small;"');
		
		echo '</td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">UF</td>';
		echo '<td>';
		
		$sql = "SELECT distinct 'agrupador=estado&tpaetapaturma=".utf8_decode($dados['tpaetapaturma'])."&catid=".$dados['catid']."&estuf='||e.estuf as codigo, estuf as descricao from territorios.estado e order by estuf";
		$db->monta_combo('filtro3', $sql, 'S', 'Selecione', 'detalharDadosTurmaDif', '', '', '', 'S', 'filtro3', '', '', '', 'style="font-size:x-small;"');
		
		echo '</td>';
		echo '</tr>';
		
		
		echo '</table>';
		
		
	} elseif($dados['agrupador']=='estado') {

		
		$dados['r1'] = carregarAprendizagem(array('agrupador'=>$dados['agrupador'],'catid'=>$dados['catid'],'tpaetapaturma' => $dados['tpaetapaturma'],'estuf' => $dados['estuf']));
		$dados['r2'] = carregarAprendizagem2(array('agrupador'=>$dados['agrupador'],'catid'=>$dados['catid'],'tpaetapaturma' => $dados['tpaetapaturma'],'estuf' => $dados['estuf']));
		
		ob_clean();
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Conhecimento</td>';
		echo '<td>';
		
		$sql = "SELECT 'agrupador=conhecimento&catid='||p.catid as codigo, catdsc as descricao FROM sispacto2.aprendizagemconhecimento p WHERE p.cattipo='P'";
		$db->monta_combo('filtro1', $sql, 'S', 'Selecione', 'detalharDadosTurmaDif', '', '', '', 'S', 'filtro1', '', 'agrupador=conhecimento&catid='.$dados['catid'], '', 'style="font-size:x-small;"');
		
		echo '</td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Turma</td>';
		echo '<td>';
		
		$sql = "SELECT distinct 'agrupador=tipoturma&catid=".$dados['catid']."&tpaetapaturma='||t.tpaetapaturma as codigo, t.tpaetapaturma as descricao from sispacto2.turmasprofessoresalfabetizadores t";
		$db->monta_combo('filtro2', $sql, 'S', 'Selecione', 'detalharDadosTurmaDif', '', '', '', 'S', 'filtro2', '', 'agrupador=tipoturma&catid='.$dados['catid'].'&tpaetapaturma='.utf8_decode($dados['tpaetapaturma']), '', 'style="font-size:x-small;"');
		
		echo '</td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">UF</td>';
		echo '<td>';
		
		$sql = "SELECT distinct 'agrupador=estado&tpaetapaturma=".utf8_decode($dados['tpaetapaturma'])."&catid=".$dados['catid']."&estuf='||e.estuf as codigo, estuf as descricao from territorios.estado e order by estuf";
		$db->monta_combo('filtro3', $sql, 'S', 'Selecione', 'detalharDadosTurmaDif', '', '', '', 'S', 'filtro3', '', 'agrupador=estado&tpaetapaturma='.utf8_decode($dados['tpaetapaturma']).'&catid='.$dados['catid'].'&estuf='.$dados['estuf'], '', 'style="font-size:x-small;"');
		
		echo '</td>';
		echo '</tr>';
		
		
		echo '</table>';
		
		
		
	}
	
	$arrDiferenca = array();
	
	if($dados['r2']) {
		
		$indices = array_keys($dados['r1']);
		foreach($dados['r2'] as $key2 => $lin2) {
			
			unset($key);
			foreach($indices as $ind) {
				if($lin2['agrup'] == $dados['r1'][$ind]['agrup']) $key = $ind;
			}
			
			$diferenca['agrup'] = $lin2['agrup'];
			if(isset($key)) {
				$diferenca['porc_actsim'] = $lin2['porc_actsim'] - $dados['r1'][$key]['porc_actsim'];
				$diferenca['porc_actparcialmente'] = $lin2['porc_actparcialmente'] - $dados['r1'][$key]['porc_actparcialmente'];
				$diferenca['porc_actnao'] = $lin2['porc_actnao'] - $dados['r1'][$key]['porc_actnao'];
			} else {
				$diferenca['porc_actsim'] = "--";
				$diferenca['porc_actparcialmente'] = "--";
				$diferenca['porc_actnao'] = "--";
			}
			$arrDiferenca[] = $diferenca;
		}
	}
	
	$cabecalho = array("Agrupador","Sim %","Parcialmente %","Não %");
	$db->monta_lista_simples($arrDiferenca,$cabecalho,100000,5,'N','100%','N');
	
	
	
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
		ncol0.colSpan = 10;
		ncol0.id = 'id2_'+agrup+'_'+nlinha.rowIndex;
		ajaxatualizar('buscar=1&relatorio=relatpersonal_aprendizagempor.inc&requisicao=carregarAprendizagem2&'+agrup,'id2_'+agrup+'_'+nlinha.rowIndex);
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
		ncol0.colSpan = 10;
		ncol0.id = 'id_'+agrup+'_'+nlinha.rowIndex;
		ajaxatualizar('buscar=1&relatorio=relatpersonal_aprendizagempor.inc&requisicao=carregarAprendizagem&'+agrup,'id_'+agrup+'_'+nlinha.rowIndex);
		obj.title='menos'
		obj.src='../imagens/menos.gif'
	} else {
		table.deleteRow(linha.rowIndex);
		obj.title='mais'
		obj.src='../imagens/mais.gif'
	
	}
}

function detalharDadosTurmaDif(agrup) {
	ajaxatualizar('buscar=1&relatorio=relatpersonal_aprendizagempor.inc&requisicao=carregarAprendizagemDif&'+agrup,'td_carregarAprendizagemDif');
}
</script>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td class="SubTituloEsquerda">Dados Gerais da aprendizagem por conhecimento ( Português ) - INÍCIO</td></tr>		
		<tr>
			<td><? $r1 = carregarAprendizagem($dados); ?></td>
		</tr>
</table>

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td class="SubTituloEsquerda">Dados Gerais da aprendizagem por conhecimento ( Português ) - FIM</td></tr>		
		<tr>
			<td><? $r2 = carregarAprendizagem2($dados); ?></td>
		</tr>
</table>

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td class="SubTituloEsquerda">Dados Gerais da aprendizagem por conhecimento ( Português ) - Diferença %</td></tr>		
		<tr>
			<td id="td_carregarAprendizagemDif"><? 
			
			carregarAprendizagemDif(array('r1'=>$r1,'r2'=>$r2));
			
			?></td>
		</tr>
</table>