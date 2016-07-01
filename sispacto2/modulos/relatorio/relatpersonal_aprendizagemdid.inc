<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

function carregarAprendizagem($dados) {
	global $db;
	
	if(!$dados['agrupador']) {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=estado&catid='||foo.catid||'\', this);\">' as mais, foo.catdsc as agrup,";
		$groupby = "GROUP BY foo.catid, foo.catdsc";
		$orderby = "ORDER BY foo.catid";
	} elseif($dados['agrupador']=='estado') {
		$colums  = "'<img src=../imagens/mais.gif title=mais onclick=\"detalharDadosTurma(\'agrupador=municipio&catid=".$dados['catid']."&estuf='||foo.estuf||'\', this);\">' as mais, foo.estuf as agrup,";
		$groupby = "GROUP BY foo.estuf";
		$orderby = "ORDER BY foo.estuf";
		$where = "AND p.catid='".$dados['catid']."'";
	} elseif($dados['agrupador']=='municipio') {
		$colums  = "'' as mais, foo.mundescricao as agrup,";
		$groupby = "GROUP BY foo.mundescricao, foo.muncod";
		$where = "AND p.catid='".$dados['catid']."' AND m.estuf='".$dados['estuf']."'";
	}
	
	
	$perfis = pegaPerfilGeral();
	
	if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto2']['universidade']['uncid']) {
		$where .= " AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'";
	}
	
	$sql = "SELECT  {$colums}
					COUNT(DISTINCT foo.iusd) as totalprofessores,
					SUM(foo.frequencia) as fr,
					round((SUM(foo.frequencia)::numeric/COUNT(DISTINCT foo.iusd)::numeric)*100,2) as fr_p,
					SUM(foo.raramente) as ra,
					round((SUM(foo.raramente)::numeric/COUNT(DISTINCT foo.iusd)::numeric)*100,2) as ra_p,
					SUM(foo.nunca) as nu,
					round((SUM(foo.nunca)::numeric/COUNT(DISTINCT foo.iusd)::numeric)*100,2) as nu_p
	
			FROM (
			
			SELECT
			a.iusd,
			p.catid,
			p.catdsc,
			m.muncod,
			m.estuf,
			m.mundescricao,
			CASE WHEN umdopcao='F' THEN 1 ELSE 0 END as frequencia,
			CASE WHEN umdopcao='R' THEN 1 ELSE 0 END as raramente,
			CASE WHEN umdopcao='N' THEN 1 ELSE 0 END as nunca
			FROM sispacto2.usomateriaisdidaticos a 
			INNER JOIN sispacto2.aprendizagemconhecimento p ON p.catid = a.catid 
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = a.iusd 
			LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			WHERE p.cattipo='R' {$where}
			
			) foo
			{$groupby} 
			{$orderby}";
			
//			echo "<pre>";
//			echo $sql;
	
	$cabecalho = array("&nbsp;","Agrupador","Número Professores",
						"<b>Com frequência</b><br><span style=font-size:xx-small;>(Mínimo 1x por semana)</span>",
						"%",
						"<b>Raramente</b><br><span style=font-size:xx-small;>(Mínimo 1x por mês)</span>",
						"%",
						"<b>Nunca</b>",
						"%");
	
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
		ncol0.colSpan = 9;
		ncol0.id = 'id_'+agrup+'_'+nlinha.rowIndex;
		ajaxatualizar('buscar=1&relatorio=relatpersonal_aprendizagemdid.inc&requisicao=carregarAprendizagem&'+agrup,'id_'+agrup+'_'+nlinha.rowIndex);
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
        <tr><td class="SubTituloEsquerda">Com que frequência você tem utilizado os recursos didáticos sugeridos pelo Pacto?</td></tr>		
		<tr>
			<td><? carregarAprendizagem($dados); ?></td>
		</tr>
</table>