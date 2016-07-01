<div class="tituloLista"> Tipos de Entrada/Sa&iacute;da Cadastrados </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] == 'filtrar'){
	$oTipoEntradaSaida->listarTipoEntradaSaidaPorFiltro($_POST['tesslg'],$_POST['tesdsc'],$_POST['testpo'],$_POST['tdcid']);
}
else{
	$oTipoEntradaSaida->listarTipoEntradaSaidaPorFiltro('','','','');
}
?>
