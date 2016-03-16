<?
verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid']));

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function inserirDocumento() {

	if(jQuery('#arquivo').val()=='') {
		alert('Selecione um arquivo');
		return false;
	}

	if(jQuery('#douobservacoes').val()=='') {
		alert('Preencha alguma observação');
		return false;
	}

	document.getElementById('formulario').submit();
}

function excluirDocumento(douid) {

	var conf = confirm('Deseja realmente excluir o arquivo selecionado');

	if(conf) {
		window.location='sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&requisicao=excluirDocumentoUniversidade&douid='+douid;
	}
	
}

</script>

<form method="post" id="formulario" enctype="multipart/form-data">

<input type="hidden" name="goto" id="goto" value="">
<input type="hidden" name="uncid" id="uncid" value="<?=$_SESSION['sismedio']['universidade']['uncid'] ?>">
<input type="hidden" name="requisicao" id="requisicao" value="inserirDocumento">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Documentos</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Arquivo</td>
		<td><input type="file" name="arquivo" id="arquivo"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Observações</td>
		<td><?=campo_textarea('douobservacoes', 'N', 'S', '', '70', '4', '250'); ?></td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloCentro">
		<input type="button" value="Anexar" onclick="inserirDocumento();">
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<?php 
		
		$sql = "SELECT '<center><img src=\"../imagens/anexo.gif\"   style=\"cursor:pointer;\" onclick=\"window.location=\'sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&requisicao=downloadDocumentoDesignacao&arqid='||ar.arqid||'\';\"> 
								<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirDocumento('||du.douid||');\"></center>' as acao, 
					   ar.arqnome||'.'||ar.arqextensao as arquivo,
					   du.douobservacoes, 
					   us.usunome,
					   to_char(doudatains,'dd/mm/YYYY HH24:MI') as dtins
				FROM sismedio.documentosuniversidade du 
				INNER JOIN public.arquivo ar ON ar.arqid = du.arqid 
				INNER JOIN seguranca.usuario us ON us.usucpf = ar.usucpf  
				WHERE uncid='".$_SESSION['sismedio']['universidade']['uncid']."' ANd du.doustatus='A'";

		$cabecalho = array("&nbsp;","Arquivo","Observações","Inserido por","Data");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
		
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=acompanhamentoavaliacaobolsas';">
		</td>
	</tr>
</table>
</form>