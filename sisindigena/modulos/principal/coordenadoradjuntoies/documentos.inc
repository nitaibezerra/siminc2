<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function downloadDocumento(arqid) {
	window.location='sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoiesexecucao&acao=A&requisicao=downloadDocumentoDesignacao&arqid='+arqid;
}

function inserirDocumento() {
	
	if(jQuery('#arquivo').val()=='') {
		alert('Selecione um arquivo');
		return false;
	}

	if(jQuery('#tipo').val()=='') {
		alert('Selecione um tipo');
		return false;
	}

	if(jQuery('#observacoes').val()=='') {
		alert('Preencha as observações');
		return false;
	}

	jQuery('#formulario').submit();
	

	
}

function excluirdocumento(domid) {
	var conf = confirm('Deseja realmente excluir este documento?');
	
	if(conf) {
		window.location='sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoiesexecucao&acao=A&requisicao=excluirDocumento&domid='+domid;
	}

}

</script>
<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="inserirDocumento">
<input type="hidden" name="goto" id="goto" value="">

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
		<td class="SubTituloDireita" width="20%">Tipo</td>
		<td>
		<? 
		$tipo_documento = array(0 => array('codigo' => 'T','descricao' => 'Termos de adesão'),
								1 => array('codigo' => 'R','descricao' => 'Relatórios'),
								2 => array('codigo' => 'D','descricao' => 'Designações'),
								3 => array('codigo' => 'I','descricao' => 'Pareceres'),
								4 => array('codigo' => 'N','descricao' => 'Notas técnicas'),
								5 => array('codigo' => 'P','descricao' => 'Planos de trabalho')
								);
		$db->monta_combo('tipo', $tipo_documento, 'S', 'Selecione', '', '', '', '', 'S', 'tipo', '', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Observações</td>
		<td><?=campo_textarea('observacoes', 'S', 'S', '', '70', '4', '250'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Salvar" onclick="inserirDocumento()">
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<? 
		
		$sql = "SELECT '<center><img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirdocumento('||d.domid||');\"> <img src=\"../imagens/anexo.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"downloadDocumento('||d.arqid||');\"></center>' as acao, 
						a.arqnome || '.' || a.arqextensao as arquivo, 
						CASE WHEN domtipo='T' THEN 'Termo de adesão'
							 WHEN domtipo='R' THEN 'Relatórios'
							 WHEN domtipo='D' THEN 'Designações' END as tipo,
						domdsc as descricao
				FROM sisindigena.documentos d 
				INNER JOIN public.arquivo a ON a.arqid = d.arqid 
				WHERE d.picid='".$_SESSION['sisindigena']['coordenadoradjuntoies']['picid']."'";
		
		$cabecalho = array("&nbsp;","Arquivo","Tipo","Observações");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true,false,false,true);
		
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=dados';">
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=nucleos';">
		</td>
	</tr>
</table>
</form>