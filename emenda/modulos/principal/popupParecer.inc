<?php

if ( $_REQUEST["anaid"] ){
	
	$sql = "SELECT 
				ana.anaparecer
			FROM 
				emenda.analise ana
			WHERE 
				ana.anaid = {$_REQUEST["anaid"]}";
	$dadosAnalise = $db->pegaLinha( $sql );

}

$anaid = $_REQUEST["anaid"];
?>

<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="100%">
		<tr bgColor="#e0e0e0">
			<td align="center"><?=($dadosAnalise['anaparecer'] ? $dadosAnalise['anaparecer'] : 'Não Informado') ?></td>
		</tr>
	</table>
</form>