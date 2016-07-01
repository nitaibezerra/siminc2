<div class="tituloLista"> Fornecedores Cadastrados </div>
<?php
//verifica se é para disparar a consulta no banco por filtro ou busca todos
if($_POST['requisicao'] == 'filtrar'){
	$oFornecedor->listarFornecedorPorFiltro($_POST['forcpfcnpj'],$_POST['fortipo'],$_POST['forrazaosocial'],$_POST['fornomefantasia']);
}
else{
	$oFornecedor->listarFornecedorPorFiltro('','','','');
}
?>
