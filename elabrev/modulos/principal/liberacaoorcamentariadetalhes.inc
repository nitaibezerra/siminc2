<?php

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

if($_REQUEST['lbrid'])
	$_SESSION['elabrev_var']['lbrid'] = $_REQUEST['lbrid'];
	
if(!$_SESSION['elabrev_var']['lbrid']) {
	
	echo "<script>
			alert('Problemas nas variaveis enviadas');
			window.location='elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A';
		  </script>";
	exit;
	
}
	
$sql = "SELECT * FROM elabrev.liberacao WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."'";
$liberacao = $db->pegaLinha($sql);

$abas = menuAbasLiberacao();

/*
 * REGRA, SE A LIBERAÇÃO ESTIVER EM ELABORAÇÃO, NÃO DEVERÁ MOSTRAR AS ABAS
 */

echo montarAbasArray($abas, $_SERVER['REQUEST_URI']);

cabecalhoLiberacao($_SESSION['elabrev_var']['lbrid']);



// códigos das Unidades Gestoras
$filtro[] = " entid IN('391295','391282','391296','391283','391284','391285','391286','391287','391288','391289','391290','391291','391292')";


?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script>

function ajaxatualizar(params,iddestinatario, pai) {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}else{
						document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}	
				} 
			},
			onLoading: function(){
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}else{
						document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}
				}	
			}
		});
}

function detalharLiberacaoOrc(obj, entid) {
	var linha = obj.parentNode.parentNode.parentNode;
	linha.cells[0].vAlign="top";
	linha.cells[1].vAlign="top";
	
	document.getElementById('entidadedetalhe'+entid).innerHTML='<img src="/imagens/carregando.gif" align="absmiddle"> Carregando...';
	
	if(document.getElementById('img'+entid).title=='mais') {
		document.getElementById('img'+entid).title='menos';
		document.getElementById('img'+entid).src='../imagens/menos.gif';
		ajaxatualizar('requisicao=detalharliberacaoorcamentaria&entid='+entid, 'entidadedetalhe'+entid);
	} else {
		document.getElementById('entidadedetalhe'+entid).innerHTML='';
		document.getElementById('img'+entid).title='mais';
		document.getElementById('img'+entid).src='../imagens/mais.gif';
	}
}

function liberacaoOrcamentariaPopup(params) {
	var popup = window.open('?modulo=principal/liberacaoorcamentariapopup&acao=A' + params, 'Liberação','scrollbars=yes,height=540,width=630');
	popup.focus();

}

function gerarLiberacao() {
	var conf = confirm('Deseja realmente gerar liberação?');
	if(conf) {
		window.location='elabrev.php?modulo=principal/liberacaoorcamentariaresumo&acao=A';		
	}
}

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="95%">
<?
$sql = "SELECT '<center><img src=\"../imagens/mais.gif\" id=\"img'||entid||'\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharLiberacaoOrc(this, \''||entid||'\');\"></center>' as img, 
			   CASE  
			   	WHEN entungcod IS NOT NULL THEN entungcod 
			   	WHEN entungcod IS NULL THEN entunicod 
			   END as codigo,
			   '<strong>'||entnome||'</strong><br><div id=\"entidadedetalhe'||entid||'\"></div>' as descricao
			   FROM entidade.entidade ".(($filtro)?"WHERE ".implode(" AND ", $filtro):"")." ORDER BY codigo";

$cabecalho = array("","Código","Descrição");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>
</td>
<td valign="top"><?
	$docid = criarDocumento( $_SESSION['elabrev_var']['lbrid'] );
	wf_desenhaBarraNavegacao( $docid , array( 'lbrid' => $_SESSION['elabrev_var']['lbrid'], 'url' => $_SESSION['favurl'] ) );
?></td>
</tr></table>
<?
if($liberacao['lbrsituacao'] == "E") {
?>
<table class="tabela" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td colspan="2" align="right"><input type='button' onclick="gerarLiberacao();" value="Gerar Liberação"></td>
</tr>
</table>
<?
}

if($ents) {
?>
<script>
document.observe("dom:loaded", function() {
<?
foreach($ents as $entid) {
	echo "detalharLiberacaoOrc(document.getElementById('img".$entid."'), '".$entid."');";	
}
?>
});
</script>
<?
}
?>