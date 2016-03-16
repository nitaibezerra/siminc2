<?php
include_once APPRAIZ . 'includes/funcoesspo.php';
include_once '_funcoesquestionario.php';

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>
<div class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
        <li>Gerenciar questionário - Ações</li>
        <li class="active"><?=$_REQUEST['aba'] == 'dados' ? 'Dados' : 'Cadastrar perguntas'?></li>
    </ol>
    <?php
    montaAbasQuestionario(preg_replace('@&fnc.*@i', '', $_SERVER['REQUEST_URI']));
    ?>
</div>
<?php
if ($_REQUEST['aba'])
    include_once APPRAIZ_SISOP.'questionario/'.$_REQUEST['aba'].'.inc';
?>