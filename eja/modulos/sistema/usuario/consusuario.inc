
<?php


	$configuracao = array();
	array_push( $configuracao, $_SESSION['sisid'] == 1 ? 'monitoramento' : 'projetos_especiais'  );
	include APPRAIZ .'seguranca/modulos/sistema/usuario/consusuario.inc';
?>
<script src="/includes/funcoes.js" ></script>
<script type="text/javascript">
    var jq = jQuery.noConflict();
    jq('.chosen-select').chosen({allow_single_deselect: true});
</script>