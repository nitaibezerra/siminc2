<?php
	$pflcod = $_REQUEST['pflcod'];
?>
<tr>
	<td align="right" class="SubTituloDireita">Perfil desejado:</td>
	<td>
		<?php include APPRAIZ .'seguranca/modulos/sistema/usuario/incperfilusuario.inc'; ?>
	</td>
</tr>
<script src="/includes/funcoes.js" ></script>
<script type="text/javascript">
    var jq = jQuery.noConflict();
    jq('.chosen-select').chosen({allow_single_deselect: true});
</script>