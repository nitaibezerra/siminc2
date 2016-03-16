<div class="tituloLista"> Lista das Unidades </div>
<?php
// Caso seja para filtrar os dados de unidade
if($_POST['requisicao'] == 'filtrar'){
	// filtra
	$oEnderecoUnidade->filtrarUnidade($_POST['uorsg'], $_POST['uorno'],$_POST['viaEntradaBens']);
}else if($_POST['requisicao'] == 'naofiltrar'){
	$oEnderecoUnidade->filtrarUnidade('', '',$_POST['viaEntradaBens']);
}
else{
	$oEnderecoUnidade->filtrarUnidade('', '',$_GET['viaEntradaBens'],$_GET['uorco_uorg_lotacao_servidor']);
}
?>


<div class="tituloLista"> Endere&ccedil;os Cadastrados Para a Unidade <?php echo empty($_POST['uorsg']) ? $uorsg : $_POST['uorsg']; ?> </div>
<?php
//caso seja para filtrar os dados de endereço de unidade
if(!empty($_POST['uorco_uorg_lotacao_servidor']) || !empty($_GET['uorco_uorg_lotacao_servidor'])){
	//filtra
	if(!empty($_POST['uorco_uorg_lotacao_servidor'])){
		$uorco_uorg_lotacao_servidor = $_POST['uorco_uorg_lotacao_servidor'];
	}
	else if(!empty($_GET['uorco_uorg_lotacao_servidor'])){
		$uorco_uorg_lotacao_servidor = $_GET['uorco_uorg_lotacao_servidor'];
	}

	if(!empty($_POST['viaEntradaBens'])){
		$viaEntradaBens = $_POST['viaEntradaBens'];
	}
	else if(!empty($_GET['viaEntradaBens'])){
		$viaEntradaBens = $_GET['viaEntradaBens'];
	}
	$oEnderecoUnidade->filtrarEndereco($uorco_uorg_lotacao_servidor,$viaEntradaBens);
}
?>

<div class="tituloLista">
	<input type='button' class="botao" name='btnNovoEndereco' id='btnNovoEndereco' value='Cadastrar Novo Endereço Para a Unidade' onclick="javascript:novoEndereco();" />
</div>