<div class="tituloLista"> Entrada de Bens Cadastrados </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] == 'filtrar'){
	$oBens->listarBemPorFiltro($_POST['tesid'],$_POST['bendtentrada'],$_POST['bennumproc'],$_POST['bendtproc'],$filtroBenStatus);
}
else{
	$oBens->listarBemPorFiltro('','','','',$filtroBenStatus);
}
?>
