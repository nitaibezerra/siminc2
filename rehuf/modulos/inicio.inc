<?
/*
 * TELA DE LISTA DE HOSPITAIS (busca feita em entidade.entidade)
 * - Não necessita de nenhum parâmetro ($_GET ou $_POST);
 * - Constante HOSPITALUNIV deverá esta definida no constantes.php
 * - Validação das permissões feita com base no perfil, e deverá esta definido em constantes.php
 */

/* TRATAMENTO DE SEGURANÇA */
switch($_REQUEST['acao']) {
	case 'C';
		$_FUNCOESAUTORIZADAS = array('carregarhospitaisporunidade'=>true,
									 'confirmarmudancasitucao'=>true,
									 'salvarmudancasituacao'=>true);
	break;
}

if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}
/* FIM TRATAMENTO DE SEGURANÇA */

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';
$permissoes = verificaPerfilRehuf();
$titulo_modulo = "REHUF - Reestruturação dos Hospitais Universitários Federais";
monta_titulo( $titulo_modulo, 'Lista dos hospitais' );

if(count($permissoes['verhospitais']) > 0) {
?>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/rehuf.js"></script>
	<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
	<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
	<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
	<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
	<form method="post" id="formpesquisa" name="formpesquisa">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr><td class="SubTituloCentro" colspan="2">Argumentos de Pesquisa</td></tr>
	<tr><td class="SubTituloDireita">Agrupamento :</td><td><input type="radio" name="pes_agrupamento" value="hosp" <? echo (($_REQUEST['pes_agrupamento']=="hosp"||!$_REQUEST['pes_agrupamento'])?"checked":""); ?> > Hospitais <input type="radio" name="pes_agrupamento" value="unid" <? echo (($_REQUEST['pes_agrupamento']=="unid")?"checked":""); ?> > Unidades</td></tr>
	<tr><td class="SubTituloDireita">Unidades :</td>
	<td><? 
	$unidadeid = $_REQUEST['unidadeid'];
	$db->monta_combo('unidadeid', "SELECT ena.entid as codigo, upper(ena.entnome) as descricao FROM entidade.entidade ent 
								   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								   LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid 
								   LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid  WHERE ent.entid IN('".implode("','",$permissoes['verhospitais'])."') AND ena.entid IS NOT NULL AND fen.funid IN (".HOSPITALUNIV.",".HOSPITALFEDE.") ".$filtroentidade." 
								   GROUP BY ena.entnome,ena.entid ORDER BY ena.entnome", 'S', "Selecione...", '', '', '', '300', 'S', 'unidadeid'); ?></td></tr>
	<tr><td class="SubTituloDireita">Hospitais :</td><td><? 
	$hospitalid = $_REQUEST['hospitalid']; 
	$db->monta_combo('hospitalid', "SELECT ent.entid as codigo, ent.entnome || coalesce(' - '||ent2.entsig,'') as descricao 
									FROM entidade.entidade ent
									LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
									LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
									INNER JOIN entidade.entidade ent2 on fue.entid = ent2.entid 
									WHERE ent.entid IN('".implode("','",$permissoes['verhospitais'])."') AND fen.funid IN(".HOSPITALUNIV.",".HOSPITALFEDE.") ".$filtroentidade." ORDER BY ent.entnome", 'S', "Selecione...", '', '', '', '300', 'S', 'hospitalid'); ?></td></tr>
	<tr>
	
	</tr>
	<td class="SubTituloDireita">Aderiram a EBSERH :</td>
	<td>
	<?
	$arrAderiu = array(0=>array("codigo"=>"esudataadereebserh IS NOT NULL","descricao"=>"Sim"),1=>array("codigo"=>"esudataadereebserh IS NULL","descricao"=>"Não"));
	$aderiuebserh = $_REQUEST['aderiuebserh'];
	$db->monta_combo('aderiuebserh', $arrAderiu, 'S', "Todos", '', '', '', '', 'S', 'aderiuebserh');
	?>
	</td>

	<tr>
	<td align="center" colspan="2">
		<input type="button" value="Pesquisar" onclick="pesquisarhospital();">
		<input type="button" onclick="vertodoshospitais();" value="Ver Todos">
		<? if($permissoes['alterarsituacaocad']) { ?> 
		<input type="button" value="Trocar situação" onclick="mudarsituacao();">
		<? } ?>
	</td>
	</tr>
	</table>
	<?
	/* Analisando filtro de pesquisa */
	if($_REQUEST['unidadeid']) {
		$filtropesquisa .= " AND ena.entid='".$_REQUEST['unidadeid']."'";
	}
	
	if($_REQUEST['hospitalid']) {
		$filtropesquisa .= " AND ent.entid='".$_REQUEST['hospitalid']."'";
	}
	if($_REQUEST['aderiuebserh']) {
		$filtropesquisa .= " AND ".$_REQUEST['aderiuebserh'];
	}
	/* FIM - Analisando filtro de pesquisa */
	
	if($_REQUEST['pes_agrupamento']=="hosp" || !$_REQUEST['pes_agrupamento']) {
		
		$sql = "SELECT '<center><input type=\"checkbox\" name=\"situacao[]\" id=\"situacaohospital\" value=\"'|| ent.entid ||'\"><center>' as chk, '<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/listartabela&acao=A&entid='|| ent.entid ||'\'\"></center>' as acoes, '<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/listartabela&acao=A&entid='|| ent.entid ||'\'\">'||ent.entnome||'</a>', fun.fundsc, ena.entsig, esddsc, ende.estuf, mundescricao, 
				to_char((select ltadata from rehuf.logtabelasgrupos ll where ll.esuid=esu.esuid order by ltadata desc limit 1), 'dd/mm/YYYY HH24:MI')||' por '||usu.usunome as atualizacao FROM entidade.entidade ent 
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
				LEFT JOIN entidade.funcao fun ON fun.funid = fen.funid
				LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
				LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid 
				LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
				LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod AND mun.estuf = ende.estuf 
				LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
				LEFT JOIN seguranca.usuario usu ON usu.usucpf = esu.usucpf
				LEFT JOIN workflow.documento doc ON esu.docid = doc.docid 
				LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
				WHERE fen.funid IN ('". HOSPITALUNIV ."','".HOSPITALFEDE."') AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND 
					  ent.entid IN('".implode("','",$permissoes['verhospitais'])."') ".$filtropesquisa." 
				ORDER BY ena.entsig";
		
		$cabecalho = array("<input type='checkbox' id='marcatodos' onclick='verificarhospitalmarcado(this);'>", "Ações", "Hospitais", "Tipo", "Unidade", "Situação", "UF", "Município", "Última atualização");
		
	} else {
		$sql = "SELECT 
				'<img src=\"../imagens/mais.gif\" style=\"padding-right: 5px;cursor:pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" id=\"img' || ena.entid || '\" name=\"+\" onclick=\"abrehospoitaisporunidade(this,'||ena.entid||');\"/>' as img, 
				upper(ena.entnome), COUNT(ent.entid)
				FROM entidade.entidade ent 
				LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
				LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid
				LEFT JOIN entidade.entidade ena ON ena.entid = fue.entid
				LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid
				WHERE fen.funid IN ('". HOSPITALUNIV ."','".HOSPITALFEDE."') AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND 
					  ent.entid IN('".implode("','",$permissoes['verhospitais'])."') ".$filtropesquisa." GROUP BY ena.entid, ena.entnome, ena.entsig 
				ORDER BY ena.entsig";
		
		$cabecalho = array("", "Unidades", "Qtd. Hospitais");
	}
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>
	</form>
	<script type="text/javascript">
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url) {
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(690,400);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	function displayStaticMessage(messageContent,cssClass) {
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(600,150);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
	}
	function closeMessage() {
		messageObj.close();	
	}
	
	function exibeSolicitacaoDecreto(entid)
	{
		window.location = 'rehuf.php?modulo=principal/solicitacaodecreto&acao=A&entid=' + entid;
	}
	
	</script>
<?
} else {
?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr bgcolor="red">
		<td colspan="2" align="center">NÃO FOI ATRIBUÍDO NENHUM <strong>HOSPITAL</strong> AO SEU PERFIL. ENTRE EM CONTATO COM ADMINISTRADOR DO SISTEMA.</td>
	</tr>
	</table>
<?	
}
?>