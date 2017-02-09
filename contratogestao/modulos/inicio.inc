<?php /* ?>
<?php require(APPRAIZ . "includes/cabecalho.inc"); ?>
<?php $controllerInstituicao = new Controller_Default(); ?>

<div class="container-fluid" style="padding: 0 10px ">
    <div class="page-header">
        <h3>Controle de Contrato</h3>
    </div>
    <?php $controllerInstituicao->indexAction(); ?>
</div>
<?php */ ?>
<?php
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

// selecionando o perfil de maior nivel
$sql = "SELECT p.pflcod FROM seguranca.perfil p
		LEFT JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod
		WHERE pu.usucpf = '". $_SESSION['usucpf'] ."' and p.pflstatus = 'A' and p.sisid =  '". $_SESSION['sisid'] ."'
		--AND p.pflcod IN (".CONTRATO_PERFIL_EXECUTOR.",".CONTRATO_PERFIL_VALIDADOR.",".CONTRATO_PERFIL_CERTIFICADOR.")
		ORDER BY pflnivel ASC LIMIT 1";
$perfilcod = $db->pegaUm($sql);

switch($perfilcod) {
    case CONTRATO_PERFIL_EXECUTOR:
        header('Location: contratogestao.php?modulo=principal/execucaofatoresavaliados&acao=C');
        break;
    case CONTRATO_PERFIL_VALIDADOR:
        header('Location: contratogestao.php?modulo=principal/execucaofatoresavaliados&acao=C');
        break;
    case CONTRATO_PERFIL_CERTIFICADOR:
        header('Location: contratogestao.php?modulo=principal/execucaofatoresavaliados&acao=C');
        break;
    default:
        require(APPRAIZ . "includes/cabecalho.inc");
        $controllerInstituicao = new Controller_Default();
        ?>
        <div class="container-fluid" style="padding: 0 10px ">
            <div class="page-header">
                <h3>Controle de Contrato</h3>
            </div>
            <?php $controllerInstituicao->indexAction(); ?>
        </div>
        <?php
        break;
}
