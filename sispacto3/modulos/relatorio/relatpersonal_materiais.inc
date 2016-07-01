<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

function carregarRelatorioMateriais($dados) {
	global $db;
	
	if(!$dados['agrupador']) {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=estado\', this);\">' as mais, 'Brasil' as agrup,";
	} elseif($dados['agrupador']=='estado') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=municipio&catid=".$dados['catid']."&estuf='||foo.estuf||'\', this);\">' as mais, foo.estuf as agrup,";
		$groupby = "GROUP BY foo.estuf";
		$orderby = "ORDER BY foo.estuf";
	} elseif($dados['agrupador']=='municipio') {
		$colums  = "'' as mais, foo.mundescricao as agrup,";
		$groupby = "GROUP BY foo.mundescricao, foo.muncod";
		$where = "AND m.estuf='".$dados['estuf']."'";
	}
		
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto2']['universidade']['uncid']) {
		$where .= " AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'";
	}
	
	
	$sql = "SELECT  {$colums}
					COUNT(DISTINCT foo.iusd) as totalprofessores,
					SUM(foo.simintegral) as simintegral,
					round((SUM(foo.simintegral)::numeric/COUNT(DISTINCT foo.iusd)::numeric)*100,1) as simintegral_p,
					SUM(foo.simcopia) as simcopia,
					round((SUM(foo.simcopia)::numeric/COUNT(DISTINCT foo.iusd)::numeric)*100,1) as simcopia_p,
					SUM(foo.nao) as nao,
					round((SUM(foo.nao)::numeric/COUNT(DISTINCT foo.iusd)::numeric)*100,1) as nao_p,
					SUM(foo.simcriou) as simcriou,
					round((SUM(foo.simcriou)::numeric/COUNT(DISTINCT foo.iusd)::numeric)*100,1) as simcriou_p,
					SUM(foo.naocriou) as naocriou,
					round((SUM(foo.naocriou)::numeric/COUNT(DISTINCT foo.iusd)::numeric)*100,1) as naocriou_p
					
	
			FROM (
			
			SELECT
			ma.iusd,
			m.muncod,
			m.estuf,
			m.mundescricao,
			CASE WHEN recebeumaterialpacto='1' THEN 1 ELSE 0 END as simintegral,
			CASE WHEN recebeumaterialpacto='2' THEN 1 ELSE 0 END as simcopia,
			CASE WHEN recebeumaterialpacto='3' THEN 1 ELSE 0 END as nao,
			CASE WHEN criadocantinholeitura='1' THEN 1 ELSE 0 END as simcriou,
			CASE WHEN criadocantinholeitura='3' THEN 1 ELSE 0 END as naocriou 
			FROM sispacto2.materiaisprofessores ma 
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = ma.iusd 
			LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			WHERE ma.mapstatus='A' {$where}
			
			) foo
			{$groupby} 
			{$orderby}";
	
	$cabecalho = array("&nbsp;","<span style=font-size:xx-small;>Agrupador</span>","<span style=font-size:xx-small;>Número Professores</span>",
						"<span style=font-size:xx-small;>Sim, recebeu o material fornecido pelo MEC</span>",
						"%",
						"<span style=font-size:xx-small;>Sim, recebi uma cópia do material providenciada pelo município</span>",
						"%",
						"<span style=font-size:xx-small;>Não recebi material</span>",
						"%",
						"<span style=font-size:xx-small;>Sim, criamos o cantinho de leitura</span>",
						"%",
						"<span style=font-size:xx-small;>Não criamos o cantinho de leitura</span>",
						"%"
				
				
	);
	
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
<script language="JavaScript" src="./js/sispacto2.js"></script>
<script>
function detalharDadosTurma(agrup, obj) {

	var linha = obj.parentNode.parentNode;
	var table = obj.parentNode.parentNode.parentNode;


	if(obj.title=='mais') {
		var nlinha = table.insertRow(linha.rowIndex);
		var ncol0 = nlinha.insertCell(0);
		ncol0.colSpan = 13;
		ncol0.id = 'id_'+agrup+'_'+nlinha.rowIndex;
		ajaxatualizar('buscar=1&relatorio=relatpersonal_materiais.inc&requisicao=carregarRelatorioMateriais&'+agrup,'id_'+agrup+'_'+nlinha.rowIndex);
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
        <tr><td class="SubTituloEsquerda">Recebimento de materiais</td></tr>		
		<tr>
			<td><? carregarRelatorioMateriais($dados); ?></td>
		</tr>
</table>