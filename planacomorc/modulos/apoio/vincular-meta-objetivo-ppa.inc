<?php

switch ($_REQUEST['req']){
    case 'salvar':
        $cMetaPpa = new Public_Controller_ObjetivoMetaPpa();
        $cMetaPpa->salvar($_REQUEST);
        die;
}

$aMetaPpa = (new Public_Model_MetaPpa())->recuperarTodos('*', ["prsano = '{$_SESSION['exercicio']}'", "mppstatus = 'A'"], 'mppcod');
$aObjetivos = simec_preparar_array((new Public_Model_ObjetivoPpa())->recuperarTodos("oppid as codigo, oppdsc as descricao", ["prsano = '{$_SESSION['exercicio']}'", "oppstatus = 'A'"], 'descricao'));
$aVinculos = (new Public_Model_ObjetivoMetaPpa())->recuperarPorExercicio($_SESSION['exercicio']);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Víncular Metas PPA com Objetivos</h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Metas PPA</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />

                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover" >
                                <thead>
                                <tr>
                                    <th width="50%">Meta PPA</th>
                                    <th>Objetivo</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($aMetaPpa as $dados){ ?>
                                    <tr>
                                        <td><?php echo $dados['mppcod'] . ' - ' . $dados['mppnome']; ?></td>
                                        <td>
                                            <?php
                                            $vinculados = !empty($aVinculos[$dados['mppid']]) ?  $aVinculos[$dados['mppid']]: [];
                                            echo $simec->select("vinculos[{$dados['mppid']}][]", null, $vinculados, $aObjetivos, null, ['input-size'=>12]);
                                            ?>
                                        </td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                        <?php if (!(in_array(PFL_CONSULTA, $perfis))) {?>
                            <div class="form-group">
                                <div class="text-center">
                                    <input type="submit" class="btn btn-primary" name="btg" value="Salvar" />
                                    <a href="planacomorc.php?modulo=inicio&acao=C" class="btn btn-warning" id="btnVoltar" type="button">Voltar</a>
                                </div>
                            </div>
                        <?php }?>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>