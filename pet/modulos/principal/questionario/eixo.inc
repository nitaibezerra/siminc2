<?php
include APPRAIZ . "includes/cabecalho_bootstrap.inc";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
$controller = new Controller_Eixo();
?>

<div class="container-fluid" style="padding: 0 10px ">
	<?php $controller->indexAction(); ?>
</div>