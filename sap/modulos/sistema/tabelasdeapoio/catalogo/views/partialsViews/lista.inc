<div class="tituloLista"> Cat&aacute;logos Cadastrados </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] =='filtrar'){
	$oCatalogo->listarCatalogoPorFiltro($_POST['clscodclasse'],$_POST['ccbid'],$_POST['icbid']);
}
else{
	$oCatalogo->listarCatalogoPorFiltro('','','');
}
?>
