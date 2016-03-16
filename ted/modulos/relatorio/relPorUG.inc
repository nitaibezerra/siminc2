<?php
$form = new Ted_Form_RelPorUG();
//$lotemacro = new Ted_Model_LoteMacro();

if ($_REQUEST['funcao']){
	$form->populate($_POST);
}

switch ($_REQUEST['funcao']){
	case 'rel':
		include  APPRAIZ."includes/cabecalho.inc";
		require APPRAIZ . 'includes/library/simec/Listagem.php';
		break;
	case 'xls':
		$form->mostraRelatorioXls();
		exit();
	default:
		include  APPRAIZ."includes/cabecalho.inc";
		require APPRAIZ . 'includes/library/simec/Listagem.php';
		break;
}

?>
<script>
	function gerarRelatorio(funcao){ 
		$('#funcao').val(funcao);
		$('#filtroTed').submit();
	}
</script>
<div class="row col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
		<li class="active">Relatório por Unidade Gestora</li>
	</ol>
    <div class="well col-md-12">
        <?php echo $form->showForm(); ?>
    </div>
</div>
<?php
	if ($_REQUEST['funcao'] == 'rel' || $_REQUEST['listagem']['p'] != ''){ 
		$form->mostraRelatorio();
	}
?>