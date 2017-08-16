<?php

switch ($_REQUEST['req']){
    case 'salvar':
        $cMetaPpa = new Spo_Controller_SubUnidadeMetaPpa();
        $cMetaPpa->salvar($_REQUEST);
        die;
}

$oMetaPpa = new Public_Model_MetaPpa();
$oMetaPnc = new Public_Model_MetaPnc();
$oSubUnidadeOrcamentaria = new Public_Model_SubUnidadeOrcamentaria();

$aMetaPpa = $oMetaPpa->recuperarTodos('*', ["prsano = '2018'", "mppstatus = 'A'"], 'mppcod');
$aMetaPnc = $oMetaPnc->recuperarTodos('mpnid, mpncod, mpnnome', ["prsano = '2018'", "mpnstatus = 'A'"], 'mpncod');
$aSubUnidadeOrcamentaria = simec_preparar_array($oSubUnidadeOrcamentaria->recuperarTodos("suoid as codigo, unosigla || ' - ' || suonome as descricao", ["prsano = '2018'", "suostatus = 'A'"], 'descricao'));

/**
 * Cabeçalho padrão do simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Víncular Metas PPA com Sub-Unidade Orçamentária</h2>
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
                            <table class="table table-striped table-bordered table-hover dataTables" >
                                <thead>
                                <tr>
                                    <th>Meta PPA</th>
                                    <th>Sub-Unidade</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($aMetaPpa as $dados){ ?>
                                    <tr>
                                        <td width="50%"><?php echo $dados['mppcod'] . ' - ' . $dados['mppnome']; ?></td>
                                        <td><?php echo $simec->select("vinculos[{$dados['mppid']}][]", null, null, $aSubUnidadeOrcamentaria); ?></td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                        <div class="form-group">
                            <div class="text-center">
                                <input type="submit" class="btn btn-primary" name="btg" value="Salvar" />
                                <a href="planacomorc.php?modulo=principal/unidade/listapimanter&acao=A" class="btn btn-warning" id="btnVoltar" type="button">Voltar</a>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        $('.dataTables-example').DataTable({
            pageLength: 100,
            responsive: true,
            dom: '<"html5buttons"B>lTfgitp',
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });
    })
</script>