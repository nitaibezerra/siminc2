<?php

$oSubacaoControle = new SubacaoControle();

if($_FILES["arquivo"]){	
	$oSubacaoControle->salvarDocumentosPreObra($_POST);
}
 
if($_GET['arqidDel']){
	$oSubacaoControle->excluirDocumentosPreObra($_GET['arqidDel'],$_GET['icoid']);
}
?>
<script language="javascript" type="text/javascript">
 
	jQuery(document).ready(function(){

		jQuery("#formulario").validate({
			ignoreTitle: true,
			rules: {
				podid: "required",
				arquivo: "required",
				poadescricao: "required"
			}
		});
		
	});
	 
	function excluirAnexo( arqid ){
		var icoid = jQuery('#icoid').val();
 		if ( confirm( 'Deseja excluir o Documento?' ) ) {
 			location.href= window.location+'&arqidDel='+arqid+'&icoid='+icoid;
 		}
 	}
 
</script>
<?php echo carregaAbasItensComposicao("par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=documento&icoid=".$_REQUEST['icoid'], $_REQUEST['icoid'], $descricaoItem); ?> 
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data" >
	<input type="hidden" name="tipoAba" value="<?php echo $_REQUEST['tipoAba']; ?>"> 
	<input type="hidden" name="icoid" id="icoid" value="<?php echo $_REQUEST['icoid']; ?>"> 
	<input type="hidden" name="salvar"> 
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
	        <td width='50%'>
	            <input type="file" name="arquivo" id="arquivo" />
	            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
	        </td>      
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
	        <td>
	        	<?php
				 $arTipoObraDocumentos = $oSubacaoControle->recuperarTiposObraDocumentos();
				 $podid = $arDados['podid'];			 
				 $db->monta_combo( "podid", $arTipoObraDocumentos, 'S', 'Selecione...', '', '', '', '', 'S', 'podid',false,$podid,'Tipos Obras Documentos');
			 	?>	
			</td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
	        <td><?= campo_textarea( 'poadescricao', 'S', 'S', '', 60, 2, 250 ); ?></td>
	    </tr>
	    <tr style="background-color: #cccccc">
	        <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
	        <td height="30"><input type="submit" name="botao" value="Salvar" ></td>
	    </tr> 
	</table>
</form>
<?php $oSubacaoControle->listaDocumentos(); ?>
