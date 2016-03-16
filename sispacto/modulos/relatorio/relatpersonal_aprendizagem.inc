<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

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
		
	
	
	$sql = "SELECT  {$colums}
			COUNT(DISTINCT t.iusd) as totalprofessores,
			COUNT(DISTINCT t.tpaid) as totalturmas,
			SUM(actsim) as actsim,
			ROUND((coalesce(SUM(actsim),0)*100)/(coalesce(SUM(actsim),0)+coalesce(SUM(actparcialmente),0)+coalesce(SUM(actnao),0))::numeric,1) as porc_actsim,
			SUM(actparcialmente) as actparcialmente,
			ROUND((coalesce(SUM(actparcialmente),0)*100)/(coalesce(SUM(actsim),0)+coalesce(SUM(actparcialmente),0)+coalesce(SUM(actnao),0))::numeric,1) as porc_actparcialmente,
			SUM(actnao) as actnao,
			ROUND((coalesce(SUM(actnao),0)*100)/(coalesce(SUM(actsim),0)+coalesce(SUM(actparcialmente),0)+coalesce(SUM(actnao),0))::numeric,1) as porc_actnao
			FROM sispacto.aprendizagemconhecimentoturma a 
			INNER JOIN sispacto.aprendizagemconhecimento p ON p.catid = a.catid
			INNER JOIN sispacto.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
			INNER JOIN territorios.municipio m on m.muncod = t.tpamuncodescola 
			WHERE (tpatotalmeninos is not null or tpatotalmeninas is not null) {$where}
			{$groupby} 
			{$orderby}";
	
	$cabecalho = array("&nbsp;","Agrupador","Número Professores","Número Turmas","Sim","%","Parcialmente","%","Não","%");
	
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	
	
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
<script language="JavaScript" src="./js/sispacto.js"></script>
<script>
function detalharDadosTurma(agrup, obj) {

	var linha = obj.parentNode.parentNode;
	var table = obj.parentNode.parentNode.parentNode;


	if(obj.title=='mais') {
		var nlinha = table.insertRow(linha.rowIndex);
		var ncol0 = nlinha.insertCell(0);
		ncol0.colSpan = 10;
		ncol0.id = 'id_'+agrup+'_'+nlinha.rowIndex;
		ajaxatualizar('buscar=1&relatorio=relatpersonal_aprendizagem.inc&requisicao=carregarAprendizagem&'+agrup,'id_'+agrup+'_'+nlinha.rowIndex);
		obj.title='menos'
		obj.src='../imagens/menos.gif'
	} else {
		table.deleteRow(linha.rowIndex);
		obj.title='mais'
		obj.src='../imagens/mais.gif'
	
	}
}
</script>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td class="SubTituloEsquerda">Dados Gerais da aprendizagem por conhecimento</td></tr>		
		<tr>
			<td><? carregarAprendizagem($dados); ?></td>
		</tr>
</table>