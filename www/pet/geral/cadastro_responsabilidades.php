<?
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";


$_SESSION['sisdiretorio'] = 'pet';
include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';
include_once APPRAIZ . 'includes/library/simec/Autoload.php';

$usuarioResponsabilidade = new Model_Usuarioresponsabilidade();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];
?>

<?php if (!$pflcod && !$usucpf): ?>
    <span style="color: red">Requisição inválida</span>
    <?php exit(); ?>
<?php endif; ?>

<?php
$dadosTipoResposabilidade = $usuarioResponsabilidade->getTipoResponsabilidadeByPerfil($pflcod);

if (is_array($dadosTipoResposabilidade) and count($dadosTipoResposabilidade) < 1): ?>
    <div style='color: red'>Não foram encontrados registros</div>
<?php else: ?>

    <?php foreach ($dadosTipoResposabilidade as $rp) :
        $respUsuario = $usuarioResponsabilidade->getLista($rp, $usucpf, $pflcod);
        if (!$respUsuario):
            ?>
            <table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;" class="table table-striped table-bordered table-hover table-condensed">
                <tr>
                    <td style="text-align: center;">
                        <span style="color: red">Não existem atribuções a este Perfil.</span>
                    </td>
                </tr>
            </table>
        <?php else: ?>
            <table style="width: 100%; font-size: 11px;" class="table table-striped table-bordered table-hover table-condensed">
                <tr>
                    <td colspan="3"><?= $rp["tprdsc"] ?></td>
                </tr>
                <tr>
                    <td>&nbsp;</td>
                    <td>Código</td>
                    <td>Descrição</td>
                </tr>
                <?php foreach ($respUsuario as $ru): ?>
                    <tr>
                        <td><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
                        <td><?= $ru["codigo"]; ?></td>
                        <td><?= $ru["descricao"] ?></td>
                    </tr>
                <?php endforeach; ?>
                <tr>
                    <td colspan="3" align="right" style="border-top: 2px solid #000000;"> <strong>Total: <?= count($respUsuario) ?></strong></td>
                </tr>
            </table>
        <?php  endif; ?>
    <?php endforeach; ?>
<?php  endif; ?>