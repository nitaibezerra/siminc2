<div class="tituloLista"> Patrimônios - resultado da pesquisa </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] =='filtrar'){		
	$obPatrimonio->listaPatrimonios($_POST);
}
else{
	$obPatrimonio->listaPatrimonios();
}
?>