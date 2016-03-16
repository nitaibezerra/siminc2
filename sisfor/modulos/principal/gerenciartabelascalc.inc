<?php

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( "Gerenciamento de calculadoras", "&nbsp;");


?>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function exibirInserção() {

	jQuery("#inserirArquivo").dialog({
        draggable:true,
        resizable:true,
        width: 800,
        height: 600,
        modal: true,
     	close: function(){} 
    });

	
}


function inserirArquivoCalculadora() {

	jQuery("#formulario").submit();
	
}


function downloadArquivo(arqid) {
	window.location=window.location+'&requisicao=downloadArquivo&arqid='+arqid;
}


</script>


<div id="inserirArquivo" style="display:none;">
<form id="formulario" method="post" name="formulario" enctype="multipart/form-data">
	<input type="hidden" id="requisicao" name="requisicao" value="inserirArquivoCalculadora" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita">Arquivo:</td>
			<td><input type="file" name="arquivo" id="arquivo"></td>
		</tr>				
		<tr>
			<td class="subtituloDireita">Secretaria:</td>
			<td>
			<?
			$sec = array(0 => array("codigo" => "A","descricao" => "SECADI"),
						 1 => array("codigo" => "B","descricao" => "SEB"));
			$db->monta_combo('tcasecretaria', $sec, 'S', 'Selecione', '', '', '', '200', 'N', 'tcasecretaria','');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Descrição:</td>
			<td><?php echo campo_texto('tcadsc', 'N', 'S', 'Descrição', '70', '1000', '', '','',''); ?></td>
		</tr>			
		<tr>
			<td align="center" bgcolor="#DCDCDC" colspan="2">
				<input type="button" value="Pesquisar" id="btnPesquisar" onclick="inserirArquivoCalculadora();"/>
			</td>
		</tr>
	</table>
</form>
</div>



<p><input type="button" name="inserir" value="Inserir Arquivo" onclick="exibirInserção();"></p>
<br>
<?php 

$sql = "SELECT '<img src=../imagens/anexo.gif style=cursor:pointer; onclick=downloadArquivo('||t.arqid||');> <img src=../imagens/excluir.gif>' as acao, 
			    a.arqnome||'.'||a.arqextensao as arquivo, 
				CASE WHEN t.tcasecretaria='A' THEN 'SECADI' 
					 WHEN t.tcasecretaria='B' THEN 'SEB' END as secretaria,
				t.tcadsc
		FROM sisfor.tabelacalculadora t 
		INNER JOIN public.arquivo a ON a.arqid = t.arqid 
		WHERE tcastatus='A'";

$cabecalho = array("&nbsp;","Arquivo","Secretaria","Descrição");

$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);


?>
