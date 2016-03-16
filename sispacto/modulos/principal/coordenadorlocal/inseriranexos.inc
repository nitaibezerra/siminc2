<?
include "_funcoes_coordenadorlocal.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function inserirDocumentosCoordenadorLocal() {
	if(jQuery('#arquivo').val()=='') {
		alert('Selecione um arquivo');
		return false;
	}
	
	if(jQuery('#tdaid').val()=='') {
		alert('Selecione tipo');
		return false;
	}
	
	document.getElementById('formulario').submit();
}

function excluirDocumento(doaid) {
	var conf = confirm('Deseja realmente excluir documento?');
	if(conf) {
		window.location='sispacto.php?modulo=principal/coordenadorlocal/inseriranexos&acao=A&apaid=<?=$_REQUEST['apaid'] ?>&requisicao=excluirDocumento&doaid='+doaid;
	}
}

function downloadDocumento(arqid) {
	window.location='sispacto.php?modulo=principal/coordenadorlocal/inseriranexos&acao=A&requisicao=downloadDocumento&arqid='+arqid;
}
</script>
<form name="formulario" method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirAnexoPrincipalCoordenadorLocal">
<input type="hidden" name="goto" value="sispacto.php?modulo=principal/coordenadorlocal/inseriranexos&acao=A&apaid=<?=$_REQUEST['apaid'] ?>">
<input type="hidden" name="apaid" value="<?=$_REQUEST['apaid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir Documentos - Coordenador Local</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Subatividade:</td>
	<td><?=$db->pegaUm("SELECT suadesc FROM sispacto.atividadepacto ap 
						INNER JOIN sispacto.subatividades su ON su.suaid = ap.suaid 
						WHERE ap.apaid='".$_REQUEST['apaid']."'") ?></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Arquivo:</td>
	<td><input type="file" id="arquivo" name="arquivo"></td>
</tr>
<tr>
	<td class="SubTituloDireita" width="25%">Tipo:</td>
	<td><? 
	$sql = "SELECT tdaid as codigo, tdadesc as descricao FROM sispacto.tipodocumentoatividade WHERE tdastatus='A'"; 
	$db->monta_combo('tdaid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'tdaid');
	?></td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="button" value="Salvar" onclick="inserirDocumentosCoordenadorLocal();"> <input type="button" value="Fechar" onclick="window.close();"></td>
</tr>
<tr>
	<td colspan="2"><? 
	$sql = "SELECT '<center><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirDocumento(\''||d.doaid||'\');\"> <img src=\"../imagens/anexo.gif\" style=\"cursor:pointer;\" onclick=\"downloadDocumento(\''||d.arqid||'\');\"></center>' as acao, a.arqnome||'.'||a.arqextensao as nome, tdadesc as tipo FROM sispacto.documentoatividade d
			INNER JOIN public.arquivo a ON a.arqid = d.arqid 
			INNER JOIN sispacto.tipodocumentoatividade t ON t.tdaid = d.tdaid
			WHERE doastatus='A' AND d.apaid='".$_REQUEST['apaid']."'"; 
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?></td>
</tr>

</table>
</form>