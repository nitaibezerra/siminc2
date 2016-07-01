<div class="tituloLista"> Tipos de Movimenta&ccedil;&atilde;o Cadastrados </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] == 'filtrar'){
	$oTipoMovimentacao->listarTipoMovimentacaoPorFiltro($_POST['tmvid'],$_POST['tmvdsc'],$_POST['tmvstatus']);
}
else{
	$oTipoMovimentacao->listarTipoMovimentacaoPorFiltro('','','');
}
?>
