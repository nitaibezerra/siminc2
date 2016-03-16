<?php
/**
 * Gerenciamento de abas do acompanhamento de ações.
 * $Id: gerenciarunidades.inc 92626 2015-01-13 17:45:39Z maykelbraz $
 */

/**
 *
 */
include_once '_funcoesacoes.php';
include APPRAIZ . "includes/workflow.php";

if ($_REQUEST['requisicao']) {
    if($_REQUEST['requisicao'] == 'recuperaCoordenadorValidador'){
        echo recuperaCoordenadorValidador($_REQUEST['cpf'],$_REQUEST['pflcod']);
        die();
    }
    else if($_REQUEST['requisicao'] == 'gravarResponsabilidadeAcao'){
        echo gravarResponsabilidadeAcao($_REQUEST);
        die();
    }
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . 'includes/funcoesspo.php';
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
?>
<script type="text/javascript" language="javascript" src="/includes/funcoes.js"></script>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
        <li class="">Ações <?= $_SESSION['exercicio']; ?></li>
        <li class="active">Acompanhamento de Ações</li>
    </ol>
<?php
montaAbasSisopAcoes(preg_replace('@&acacod.*@i', '', $_SERVER['REQUEST_URI']));
$consulta = 'false';
if (in_array(PFL_CONSULTA, pegaPerfilGeral())) {
    $consulta = 'true';
}?>
<script>
    if (<?php echo $consulta; ?>) {
        $("input").attr('disabled', 'disabled');
    } else {
        $("input").removeAttr('disabled');
    }
</script>
<?
if ($_REQUEST['aba'])
    include_once APPRAIZ_SISOP . 'acoes/' . $_REQUEST['aba'] . '.inc';
?>
</section>