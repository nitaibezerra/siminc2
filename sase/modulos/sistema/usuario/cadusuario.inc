<?php

#	$configuracao = array();
#	array_push( $configuracao, $_SESSION['sisid'] == 1 ? 'monitoramento' : 'projetos_especiais'  );
	include APPRAIZ .'includes/cadastro_usuario_geral.php';

?>
<script src="/includes/funcoes.js" ></script>
<script type="text/javascript">
    var jq = jQuery.noConflict();
    jq('.chosen-select').chosen({allow_single_deselect: true});
</script>