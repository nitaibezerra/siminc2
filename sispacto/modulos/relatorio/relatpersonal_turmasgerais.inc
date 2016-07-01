<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

function carregarDadosGeraisTurmas($dados) {
	global $db;
	
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
		$colums  = "'' as mais, '&nbsp;'||t.tpanometurma as agrup,";
		$groupby = "GROUP BY t.tpanometurma";
		$where = "AND t.tpacodigoescola='".$dados['tpacodigoescola']."'";
	}
		
	
	
	$sql = "SELECT  {$colums}
	                SUM(tpatotalmeninos)+SUM(tpatotalmeninos) as nalunos,
	                COUNT(*) as nturmas,
	                COUNT(DISTINCT iusd) as nprofessores,
	                ROUND((SUM(tpatotalmeninos)+SUM(tpatotalmeninas))/count(*)::numeric,1) as media,
	                SUM(tpatotalmeninos) as totalmeninos, 
	                ROUND((SUM(tpatotalmeninos)*100)/(SUM(tpatotalmeninos)+SUM(tpatotalmeninas))::numeric,1) as porc_meninos,
	                SUM(tpatotalmeninas) as totalmeninas,
	                ROUND((SUM(tpatotalmeninas)*100)/(SUM(tpatotalmeninos)+SUM(tpatotalmeninas))::numeric,1) as porc_meninas,
	                COALESCE(SUM(tpafaixaetaria5anos),0) as faixaetaria5anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria5anos),0)*100)/(COALESCE(SUM(tpafaixaetaria5anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0))::numeric,1) as porc_faixa5anos,
	                COALESCE(SUM(tpafaixaetaria6anos),0) as faixaetaria6anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria6anos),0)*100)/(COALESCE(SUM(tpafaixaetaria5anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0))::numeric,1) as porc_faixa6anos,
	                COALESCE(SUM(tpafaixaetaria7anos),0) as tpafaixaetaria7anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria7anos),0)*100)/(COALESCE(SUM(tpafaixaetaria5anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0))::numeric,1) as porc_faixa7anos,
	                COALESCE(SUM(tpafaixaetaria8anos),0) as tpafaixaetaria8anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria8anos),0)*100)/(COALESCE(SUM(tpafaixaetaria5anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0))::numeric,1) as porc_faixa8anos,
	                COALESCE(SUM(tpafaixaetaria9anos),0) as tpafaixaetaria9anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria9anos),0)*100)/(COALESCE(SUM(tpafaixaetaria5anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0))::numeric,1) as porc_faixa9anos,
	                COALESCE(SUM(tpafaixaetaria10anos),0) as tpafaixaetaria10anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria10anos),0)*100)/(COALESCE(SUM(tpafaixaetaria5anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0))::numeric,1) as porc_faixa10anos,
	                COALESCE(SUM(tpafaixaetaria11anos),0) as tpafaixaetaria11anos,
	                ROUND((COALESCE(SUM(tpafaixaetaria11anos),0)*100)/(COALESCE(SUM(tpafaixaetaria5anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0))::numeric,1) as porc_faixa11anos,
	                COALESCE(SUM(tpafaixaetariaacima11anos),0) as tpafaixaetariaacima11anos,
	                ROUND((COALESCE(SUM(tpafaixaetariaacima11anos),0)*100)/(COALESCE(SUM(tpafaixaetaria5anos),0)+COALESCE(SUM(tpafaixaetaria6anos),0)+COALESCE(SUM(tpafaixaetaria7anos),0)+COALESCE(SUM(tpafaixaetaria8anos),0)+COALESCE(SUM(tpafaixaetaria9anos),0)+COALESCE(SUM(tpafaixaetaria10anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0)+COALESCE(SUM(tpafaixaetaria11anos),0))::numeric,1) as porc_faixaetariaacima11anos,
	                
	                COALESCE(SUM(tpatotalbolsafamilia),0) as tpatotalbolsafamilia,
	                ROUND((COALESCE(SUM(tpatotalbolsafamilia),0)*100)/(COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0))::numeric,1) as porc_bolsafamilia,
	                COALESCE(SUM(tpatotalvivemcomunidade),0) as tpatotalvivemcomunidade,
	                ROUND((COALESCE(SUM(tpatotalvivemcomunidade),0)*100)/(COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0))::numeric,1) as porc_vivemcomunidade,
	                COALESCE(SUM(tpatotalfreqcreche),0) as tpatotalfreqcreche,
	                ROUND((COALESCE(SUM(tpatotalfreqcreche),0)*100)/(COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0))::numeric,1) as porc_freqcreche,
	                COALESCE(SUM(tpatotalfreqpreescola),0) as tpatotalfreqpreescola,
	                ROUND((COALESCE(SUM(tpatotalfreqpreescola),0)*100)/(COALESCE(SUM(tpatotalmeninos),0)+COALESCE(SUM(tpatotalmeninas),0))::numeric,1) as porc_freqpreescola
			FROM sispacto.turmasprofessoresalfabetizadores t
			INNER JOIN territorios.municipio m on m.muncod = t.tpamuncodescola 
			WHERE tpastatus='A' AND tpaconfirmaregencia=true AND (tpatotalmeninos is not null or tpatotalmeninas is not null) {$where}
			{$groupby} 
			ORDER BY 2";
	
	$cabecalho = array("&nbsp;","Agrupador","Número Alunos","Número Turmas","Número de professores","Média Alunos / Turmas",
					   "Número Meninos","%","Número Meninas","%","Abaixo de 6anos","%","Número 6anos","%","Número 7anos","%","Número 8anos","%",
					   "Número 9anos","%","Número 10anos","%","Número 11anos","%","Acima de 11anos","%",
					   "Número recebem bolsa familia","%","Número vivem comunidade","%","Número frequentaram creche","%",
					   "Número frequentaram pre escola","%");
	
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
		ncol0.colSpan = 33;
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
</script>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td class="SubTituloEsquerda">Dados Gerais das turmas</td></tr>		
		<tr>
			<td><? carregarDadosGeraisTurmas($dados); ?></td>
		</tr>
</table>