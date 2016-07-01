<?php
/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Setando Objetos
$fm = new Simec_Helper_FlashMessage('ted/lotemacro');
$form = new Ted_Form_ExportacaoMacro();
$lotemacro = new Ted_Model_LoteMacro();

//Url base
$urlBse = 'ted.php?modulo=relatorio/exportacaoMacro';
$queryString = array(
    'acao' => 'A',
);

switch ($_REQUEST['funcao']) {
	case 'geraXls':

        if (!criaLoteMacroNCTeste()) {
            $fm->addMensagem('Ocorreu um problema ao gerar o lote, tente novamente!', Simec_Helper_FlashMessage::ERRO);
            echo Ted_Utils_Model::redirect($urlBse.'/', $queryString);
        }

		$lotemacro->geraListaMacrosExcel();
		exit();
	case 'visualizar':
		//Cabeçalho do sistema
		include APPRAIZ . 'includes/cabecalho.inc';
		require APPRAIZ . 'includes/library/simec/Listagem.php';
		$lotemacro->geraListaMacros();
		exit();
	default:
		//Cabeçalho do sistema
		include APPRAIZ . 'includes/cabecalho.inc';
		require APPRAIZ . 'includes/library/simec/Listagem.php';
		break;
}


//ver($lotemacro->getQueryListaMacros($lotemacro->getWhereListaMacros()));

if (isset($_POST['search'])) {
	$form->populate($_POST);
}

$form->getElement('lotid')->addMultiOptions($lotemacro->pegaListaMacro());

?>
<script>
	function visualizar(t, id){
		document.location = 'ted.php?modulo=relatorio/exportacaoMacro&acao=A&funcao=visualizar&lotid='+id;
	}

	function novoLote(){
		document.location = 'ted.php?modulo=relatorio/exportacaoMacro&acao=A&funcao=visualizar';
	}

	function geraXls(t, id){
		document.location = 'ted.php?modulo=relatorio/exportacaoMacro&acao=A&funcao=geraXls&lotid='+id;
	}
</script>
<div class="row col-md-12">
	<ol class="breadcrumb">
		<li><a href="ted.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
		<li class="active">Lista de Macros</li>
	</ol>

    <div class="well col-md-12">
        <?php if (is_object($fm)) { echo $fm->getMensagens(); } ?>
        <?php echo $form->showForm(); ?>
    </div>
</div>
<?php 
$lotemacro->getListaMacros($lotemacro->getWhereListaMacros());
?>