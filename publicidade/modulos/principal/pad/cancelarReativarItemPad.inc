<?php
// incluindo as classes necessárias
$oItemPad = new ItemPad();

if($_GET['ipastatus'] == 'A'){
	$oItemPad->atualizar_status($_GET['ipaid'],'C');	
	direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_GET['id'],'Item de PAD cancelado com sucesso!');
}
else if($_GET['ipastatus'] == 'C'){
	$oItemPad->atualizar_status($_GET['ipaid'],'A');
	direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_GET['id'],'Item de PAD reativado com sucesso!');
}
