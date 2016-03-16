<?
include_once '_funcoesquestionariosubacao.php';
require APPRAIZ . 'includes/library/simec/Listagem.php';

if($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
        <li>Gerenciar questionário - Subações</li>
        <li class="active"><?=$_REQUEST['aba'] == 'dados' ? 'Dados' : 'Cadastrar perguntas'?></li>
    </ol>
    <?php
    montaAbasQuestionario(preg_replace('@&fnc.*@i', '', $_SERVER['REQUEST_URI']));
    ?>
</section>
<?php
if ($_REQUEST['aba']) {
    include_once APPRAIZ_SISOP.'questionariosubacoes/'.$_REQUEST['aba'].'.inc';
}
