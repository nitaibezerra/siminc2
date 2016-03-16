<?
include "_funcoes_universidade.php";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/sispacto.js"></script>

<script>

function anexarDocumentoDesignacao() {

	if(jQuery('#documentodesignacao').val()=='') {
		alert('Selecione um arquivo');
		return false;	
	}
	
    divCarregando();
	document.getElementById('formulario').submit();

}

</script>

<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="goto" id="goto" value="sispacto.php?modulo=principal/universidade/inserirdocumento&acao=A&iusd=<?=$_REQUEST['iusd'] ?>">
<input type="hidden" name="requisicao" value="anexarDocumentoDesignacao">
<input type="hidden" name="uncid" value="<?=$_SESSION['sispacto']['universidade']['uncid'] ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Inserir Equipe</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Documento de Designação:</td>
	<td><input type="file" name="arquivo" id="documentodesignacao"> <input type="button" value="Anexar" onclick="anexarDocumentoDesignacao();"></td>
</tr>
<tr>
	<td colspan="2"><?
	$sql = "SELECT '<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=window.location+\'&requisicao=downloadDocumentoDesignacao&arqid='||a.arqid||'\'\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerDocumentoDesignacao('||i.iuaid||')\"></center>' as acao, a.arqnome||'.'||a.arqextensao as arquivo 
			FROM sispacto.identificacaousuarioanexo i 
			INNER JOIN public.arquivo a ON a.arqid = i.arqid  
			WHERE iusd='".$_REQUEST['iusd']."' AND iuastatus='A'";
	$cabecalho = array("&nbsp;","Arquivo");
	
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?></td>
</tr>
</table>
</form>
