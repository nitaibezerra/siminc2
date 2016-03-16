<?php
$configuracao = array();
array_push( $configuracao, $_SESSION['sisid'] == 1 ? 'monitoramento' : 'projetos_especiais'  );
include_once APPRAIZ .'zimec/public/temas/simec/partial/usuario/consulta.phtml';
?>