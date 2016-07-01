<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/Procurador.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelProcurador = new Procurador($_REQUEST['proid']);

$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
    $modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
    $estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

$perfis = pegaPerfilGeral();
$perfis = $perfis ? $perfis : array();

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('demandasfies/demandas');
switch ($_REQUEST['action']) {
    case 'salvar_procurador':
        $modelProcurador->popularDadosObjeto();

        $sucesso = $modelProcurador->salvar();

        $modelProcurador->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

        $url .= '&dmdid=' . $modelDemanda->dmdid . '&aba=procuradores';

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
    case 'inativar_procurador':
        $modelProcurador->dmostatus = 'I';
        $modelProcurador->dmodtinativacao = date('Y-m-d H:i:s');
        $modelProcurador->usucpfinativacao = $_SESSION['usucpforigem'];
        $sucesso = $modelProcurador->salvar();
        $modelProcurador->commit();

        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

        $url .= '&dmdid=' . $modelDemanda->dmdid . '&aba=sinteses';

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");
        die;
}

include APPRAIZ . "includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');
?>
<div class="col-lg-12">
<div class="page-header">
    <h3 id="forms"><?php echo $modelDemanda->dmdid ? 'Demanda ' . $modelDemanda->dmdid : 'Nova Demanda'; ?></h3>
</div>

<?php

// -- HTML das abas
echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");

echo $fm->getMensagens();

?>

<div class="row">
    <div class="row col-md-12">

        <div class="well">

            <form id="form-observacao" method="post" class="form-horizontal">
                <input name="proid" type="hidden" value="<?php echo $modelProcurador->proid; ?>">
                <input name="dmdid" type="hidden" value="<?php echo $modelDemanda->dmdid; ?>">
                <input name="action" type="hidden" value="salvar_procurador">
                <fieldset>
                    <legend>Dados Gerais</legend>

                    <div class="form-group">
                        <label for="pronome" class="col-lg-2 col-md-2 control-label">Nome:</label>

                        <div class="col-lg-10 col-md-10 ">
                            <input id="pronome" name="pronome" type="text" class="form-control" value="<?php echo $modelProcurador->pronome; ?>" >
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="protelefone" class="col-lg-2 col-md-2 control-label">Telefone:</label>

                        <div class="col-lg-10 col-md-10 ">
                            <input id="protelefone" name="protelefone" type="text" class="form-control telefone" value="<?php echo $modelProcurador->protelefone; ?>" >
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="proemail" class="col-lg-2 col-md-2 control-label">E-mail:</label>

                        <div class="col-lg-10 col-md-10 ">
                            <input id="proemail" name="proemail" type="text" class="form-control" value="<?php echo $modelProcurador->proemail; ?>" >
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="estuf" class="col-lg-2 col-md-2 control-label">UF:</label>

                        <div class="col-lg-10 col-md-10 ">
                            <select class="form-control chosen" id="estuf" name="estuf" value="<?php echo $modelProcurador->estuf; ?>" >
                                <?php echo $modelProcurador->getComboUfs(); ?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="proobservacao" class="col-lg-2 col-md-2 control-label">Observação:</label>

                        <div class="col-lg-10 col-md-10 ">
                            <textarea class="form-control" rows="5" name="proobservacao" id="proobservacao"><?php echo $modelProcurador->proobservacao; ?></textarea>
                        </div>
                    </div>
                </fieldset>
                <div>
                    <button title="Salvar" id="btn-salvar-observacao" class="btn btn-success" type="submit"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar </button>
                </div>
            </form>

            <div style="margin-top: 20px; background: #ffffff !important;">
                <?php
                $sql = "select proid, pronome, protelefone, proemail, estuf, substr(proobservacao, 0, 100) as proobservacao
                        from demandasfies.procurador
                        where dmdid = {$modelDemanda->dmdid}
                        order by proid desc";

                $listagem = new Simec_Listagem();

                $listagem->setCabecalho(array('Nome', 'Telefone', 'E-mail', 'UF', 'Observação'));

                $listagem->addCallbackDeCampo(array('pronome', 'proemail'), 'alinhaParaEsquerda');

                $listagem->addAcao('edit', 'editarProcurador');
//                $listagem->addAcao('delete', 'inativarProcurador');
                $listagem->setQuery($sql);
                $listagem->render();
                ?>
            </div>

        </div>

    </div>
    <div class="clearfix"></div>

</div>


<script src="/library/jquery/jquery.form.min.js" type="text/javascript"></script>

<script type="text/javascript">
    $(document).ready(function () {
        $('.cnpj').mask('99.999.999/9999-99');
        $('.moeda').mask('000.000.000.000.000,00', {reverse: true});
        $('.data').mask('99/99/9999');
        $('.data').datepicker();
    });

    function editarProcurador(proid) {
        window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?php echo $modelDemanda->dmdid; ?>&aba=procuradores&proid=' + proid;
    }

    function inativarProcurador(proid) {
        if (confirm('Deseja realmente excluir o registro?')) {
            window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=<?php echo $modelDemanda->dmdid; ?>&aba=procuradores&proid=' + proid + '&action=inativar_procurador';
    }
        }
</script>