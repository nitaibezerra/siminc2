<?php 
$linha1 = 'Incluir/Editar Imagem do Programa'; 
$linha2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';

monta_titulo($linha1, $linha2);

$prgid = $_REQUEST['prgid'];
?>
<script>
	jQuery(function(){
	
		jQuery("[name=arquivo]").addClass("required");	
			
		jQuery("#formImagem").validate();
	
		jQuery('#btnFechar').click(function(){
			jQuery( '#dialog-confirm' ).dialog( 'close' );
		});

		jQuery('#btnSalvarImagem').click(function(){
			jQuery('[name=requisicao]').val('salvarImagem');
			jQuery('#formImagem').submit();
		});
		
	});
</script>
<form action="" method="post" name="formImagem" id="formImagem" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="prgid" value="<?php echo $prgid; ?>" />
	<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita">Imagem</td>
			<td>
				<input type="file" name="arquivo">
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Salvar" id="btnSalvarImagem">
				<input type="button" value="Fechar" id="btnFechar"/>
			</td>
		</tr>
	</table>
</form>