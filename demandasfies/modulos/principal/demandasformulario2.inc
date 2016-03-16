<?
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$complementoDisabled = '';

$estadoAtual['esdid'] = null;
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
    $modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
    $estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

if ('salvar' == $_REQUEST['action']) {
    $modelDemanda->popularDadosObjeto();

    if ($modelDemanda->dmdid) {
        $modelDemanda->dmddtalteracao = date('Y-m-d H:i:s');
        $modelDemanda->usucpfalteracao = $_SESSION['usucpforigem'];
    } else {
        $modelDemanda->dmddtinclusao = date('Y-m-d H:i:s');
        $modelDemanda->usucpfinclusao = $_SESSION['usucpforigem'];
    }

    $sucesso = $modelDemanda->salvar();

    $modelDemanda->commit();
    $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';

    alertlocation(array(
        'alert' => $msg,
        'location' => $url . '&dmdid=' . $modelDemanda->dmdid
    ));
}

?>
<br>

<div class="col-lg-12">
<div class="page-header">
    <h3 id="forms">
        <!--        Dados da universidade --->
        Demanda
        <small>
            <!--            Salvar dados da universidade-->
        </small>
    </h3>
</div>

<?php if (!$complementoDisabled){ ?>
    <form id="form-save" method="post" class="form-horizontal">
<?php } ?>
    <input name="controller" type="hidden" value="instituicao" >
    <input name="action" type="hidden" value="salvar" >
    <input name="dmdid" id="dmdid" type="hidden" value="<?php echo $modelDemanda->dmdid; ?>" >
    <div class="row">
        <div class="row col-md-11">
            <div class="well">
                <fieldset>
                    <legend>Dados Gerais</legend>
                    <div class="form-group">
                        <label for="dmdtitulo" class="col-lg-2 control-label">Título</label>
                        <div class="col-lg-10">
                            <input id="dmdtitulo" name="dmdtitulo" type="text" class="form-control" placeholder="" required="required"  value="<?php echo $modelDemanda->dmdtitulo; ?>" <?php echo $complementoDisabled; ?>>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="dmdtitulo" class="col-lg-2 control-label">Descrição</label>
                        <div class="col-lg-10">
                            <input id="dmddsc" name="dmddsc" type="text" class="form-control" placeholder="" required="required"  value="<?php echo $modelDemanda->dmddsc; ?>" <?php echo $complementoDisabled; ?>>
                        </div>
                    </div>
                </fieldset>
            </div>
        </div>
        <div class="row col-md-1">
            <?php wf_desenhaBarraNavegacao($modelDemanda->docid, array('dmdid' => $modelDemanda->dmdid)); ?>
        </div>
        <div class="clearfix"></div>
        <div class="modal-footer" style="text-align: left">
            <button title="Salvar" class="btn btn-success" type="submit"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar</button>
            <a title="Voltar"class="btn btn-danger" href=""><span class="glyphicon glyphicon-hand-left"></span> Voltar</a>
        </div>
    </div>
<?php if (!$complementoDisabled){ ?>
    </form>
<?php } ?>

</div>
