<?php
/** 
 * Desenvolvedor: MESOTEC Informática LTDA
 * Analistas: Adonias Malosso <adonias@mesotec.com.br>, Davison Silva <davison@mesotec.com.br>
 * Programadores: Adonias Malosso <adonias@mesotec.com.br>, Halisson Gomides <halisson@mesotec.com.br>, Paulo Estevao <paulo@mesotec.com.br>
 * Baseado no SIMEC - cedido pela DTI/SE/MEC como software público brasileiro
 */


	$configuracao = array();
	array_push( $configuracao, $_SESSION['sisid'] == 1 ? 'monitoramento' : 'projetos_especiais'  );
	include APPRAIZ .'seguranca/modulos/sistema/usuario/consusuario.inc';
?>