<div class="tituloLista"> Termos - resultado da pesquisa </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] =='filtrar'){		
	$obTermo->listaTermos($_POST);
}
else{
	$obTermo->listaTermos();
}
?>