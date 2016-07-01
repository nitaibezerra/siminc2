<div class="tituloLista"> Empenhos Cadastrados </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] == 'filtrar'){
	$oEmpenho->listarEmpenhoPorFiltro($_POST['empnumero'],$_POST['empdata']);
}
else{
	$oEmpenho->listarEmpenhoPorFiltro('','');
}
?>
