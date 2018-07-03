<?php
// ver($_REQUEST,d);
$cObjetivoppa = new Public_Controller_Objetivoppa();
$cIniciativappa = new Public_Controller_Iniciativappa();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cObjetivoppa->salvar($_REQUEST);
        die;
    case 'excluir':
        $cObjetivoppa->excluir($_REQUEST['oppid']);
        die;
    case 'salvarIniciativa':
        $cIniciativappa->salvar($_REQUEST);
        die;        
    case 'excluirIniciativa':
        $cObjetivoppa->excluir($_REQUEST['oppid']);
        die;        
}

$mObjetivoppa = new Public_Model_ObjetivoPpa($_REQUEST['oppid']);
if ($mObjetivoppa->oppid!=''){
    $mIniciativappa = (new Public_Model_IniciativaPpa())->recuperarTodos('*', ["prsano = '{$_SESSION['exercicio']}'", "ippstatus = 'A'", " oppid = {$_REQUEST['oppid']}"]);
}
include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo;?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="oppid" id="oppid" type="hidden" value="<?php echo $mObjetivoppa->oppid; ?>">

                        <?php
                        echo $simec->input('oppcod', 'Código', $mObjetivoppa->oppcod, ['required', 'class'=>'inteiro','maxlength' => 4]);
                        echo $simec->input('oppnome', 'Nome', $mObjetivoppa->oppnome , ['required', 'maxlength' => 250]);
                        echo $simec->input('oppdsc', 'Descricao', $mObjetivoppa->oppdsc, ['required', 'maxlength' => 250]);
                        ?>
                        <div class="form-group">
                            <div class="text-center">                        
                                <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Metas: </label>
                                <div class="col-md-6">
                                    <?php
                                    $aVinculos = [];
                                    if ($mObjetivoppa->oppid!=''){
                                        $aVinculos = (new Public_Model_ObjetivoMetaPpa())->recuperarPorExercicio($_SESSION['exercicio'], $mObjetivoppa->oppid);
                                    }                        
                                    $aMetaPpa = simec_preparar_array((new Public_Model_MetaPpa())->recuperarTodos("mppid as codigo, mppid || ' - ' || mppdsc as descricao", ["prsano = '{$_SESSION['exercicio']}'"]));
                                    echo $simec->select("mppid[]", null, $aVinculos, $aMetaPpa, null, ['input-size'=>12]);?>
                                </div>                            
                                <div class="col-md-3" style="text-align: right;">
                                    <button class="btn btn-success" type="button" id="btnNovaMeta"><i class="fa fa-check"></i>&nbsp;Nova Meta</button>
                                </div>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/objetivoppa&acao=A" class="btn btn-warning" id="btnVoltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mObjetivoppa->oppid){ ?>
                                    <a href="?modulo=apoio/objetivoppa_form&acao=A&req=excluir&oppid=<?php echo $mObjetivoppa->oppid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Iniciativas</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvarIniciativa" />
                        <input name="ippid" id="ippid" type="hidden" value="">
                        <input name="oppid" id="oppid" type="hidden" value="<?php echo $mObjetivoppa->oppid; ?>">

                        <?php
                        if ($mObjetivoppa->oppid!=''){
                            echo $simec->input('ippcod', 'Código', $mIniciativappa->ippcod, ['required', 'class'=>'inteiro','maxlength' => 4]);
                            echo $simec->input('ippnome', 'Nome', $mIniciativappa->ippnome , ['required', 'maxlength' => 250]);
                            echo $simec->input('ippdsc', 'Descricao', $mIniciativappa->ippdsc, ['required', 'maxlength' => 250]);
                            // ver($mObjetivoppa->oppid);
                        }
                        ?>

                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <?php if($mObjetivoppa->oppid){ ?>
                                    <a href="?modulo=apoio/objetivoppa_form&acao=A&req=excluirIniciativa&oppid=<?php echo $mObjetivoppa->oppid; ?>" class="btn btn-danger link-excluir" id="btnExcluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>
                            </div>
                        </div>
                        <?php
                        $podeEditar = !in_array(PFL_SUBUNIDADE, $perfis)? TRUE: FALSE;
                        if($mIniciativappa): ?>
                            <div class="table-responsive" id="lista-segmento">
                                <table class="table table-striped table-bordered table-hover" >
                                    <thead>
                                    <tr class="text-center">
                                        <th width="10%">Ações</th>
                                        <th>Segmento</th>
                                    </tr>
                                    </thead>
                                    <tbody>
                                    <?php foreach($mIniciativappa as $dados){?>
                                        <tr>
                                            <td class="text-center">
                                                <a title="Alterar" class="editar-iniciativa" href="javascript:alterarIniciativa(<?= $dados['ippid'];?>,<?= $dados['ippcod'];?>,'<?= $dados['ippdsc'];?>', '<?= $dados['ippnome'];?>')"><i class="fa fa-pencil"></i></a>
                                                <?php if($podeEditar) { ?>
                                                    <a title="excluir" name="req" value="excluir-segmento" id="excluir-segmento" href="?modulo=apoio/area_cultural_form&acao=A&req=excluir-segmento&mdeid=<?= $dados['mdeid'];?>&neeid=<?= $dados['neeid'];?>" style="margin-left: 5px;"><i class="fa fa-close"></i></a>
                                                <?php } ?>
                                            </td>
                                            <td><?php echo $dados['ippdsc']; ?></td>
                                        </tr>
                                    <?php } ?>
                                    </tbody>
                                </table>
                            </div>
                        <?php else: ?>
                            <div class="alert alert-danger">Nenhum registro cadastrado</div>
                        <?php endif; ?>                        
                    </form>
                </div>
            </div>
        </div>        
    </div>
</div>
<div class="modal fade" id="detalhe-pi" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Pi's Vinculados</h4>
            </div>
            <div class="modal-body">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>
<script>
    $(document).ready(function(){
        $("#btnNovaMeta").click(function(){
            $('#detalhe-pi').modal();
        });
    });
    function alterarIniciativa(ippid, ippcod, ippdsc, ippnome){
        $("#ippid").val(ippid);
        $("#ippcod").val(ippcod);
        $("#ippdsc").val(ippdsc);
        $("#ippnome").val(ippnome);
    }
</script>