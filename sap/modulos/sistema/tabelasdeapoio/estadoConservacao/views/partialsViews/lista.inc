<div class="tituloLista"> Estados de Conserva&ccedil;&atilde;o Cadastrados </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] == 'filtrar'){
	$oEstadoConservacao->listarEstadoConservacaoPorFiltro($_POST['ecoid'],$_POST['ecodsc']);
}
else{
	$oEstadoConservacao->listarEstadoConservacaoPorFiltro('','');
}
?>
