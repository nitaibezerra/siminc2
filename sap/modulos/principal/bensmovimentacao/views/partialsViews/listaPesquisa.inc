<?php 
($filtroBenStatus == 'I') ? $msg = 'Movimentação de Bens Pendentes de Conclusão' : $msg = 'Movimentação de Bens Cadastrados';
?>
<div class="tituloLista"> <?php echo $msg; ?> </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] == 'filtrar'){
	$oMovimentacao->listarMovimentacaoPorFiltro($_POST,$filtroBenStatus);
}
else{
	$oMovimentacao->listarMovimentacaoPorFiltro(array(),$filtroBenStatus);
}
?>
