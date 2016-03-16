<?php

	$configuracao = array();
	array_push( $configuracao, $_SESSION['sisid'] == 1 ? 'monitoramento' : 'projetos_especiais'  );
	include APPRAIZ .'seguranca/modulos/sistema/usuario/consusuario_bootstrap.inc';
?>
<!-- <script src="/includes/funcoes.js" ></script> -->
