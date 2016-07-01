<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasse/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasse/classes/DemandaObservacao.class.inc";
include_once APPRAIZ . "demandasse/classes/DemandaArquivo.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelDemandaObservacao = new DemandaObservacao($_REQUEST['dmoid']);
$modelDemandaArquivo = new DemandaArquivo($_REQUEST['dmaid']);
$complementoDisabled = '';

$estadoAtual['esdid'] = null;
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
    $modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
    $estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('demandasse/demandas');

switch ($_REQUEST['action']){
    case 'salvar':
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
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

        $url .= '&dmdid=' . $modelDemanda->dmdid;

        ob_clean();
        $fm->addMensagem($msg,$tipo);
        header("Location: {$url}");

        die;
    case 'salvar_observacao':
        $modelDemandaObservacao->popularDadosObjeto();

        if ($modelDemandaObservacao->dmoid) {
            $modelDemandaObservacao->dmodtalteracao = date('Y-m-d H:i:s');
            $modelDemandaObservacao->usucpfalteracao = $_SESSION['usucpforigem'];
        } else {
            $modelDemandaObservacao->dmodtinclusao = date('Y-m-d H:i:s');
            $modelDemandaObservacao->usucpfinclusao = $_SESSION['usucpforigem'];
        }

        $sucesso = $modelDemandaObservacao->salvar();

        $modelDemandaObservacao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

        die;
    case 'form_observacao':
        $modelDemandaObservacao->popularDadosObjeto();
        $modelDemandaObservacao->formularioObservacoes($complementoDisabled);
        die;
    case 'salvar_arquivo':
        
        // -- Verifica se tem algum arquivo para salvar no servidor
        if ($_FILES['file']['size'] > 0) {

            $nomeArquivo = $_REQUEST['dmadsc'];

            $descricao = explode(".", $_FILES['file']['name']);
            $campos = array(
                "dmdid" => "'" . $_REQUEST['dmdid'] . "'",
                "dmadsc" => "'" . $nomeArquivo . "'",
                "dmadtinclusao" => "'" . date('Y-m-d H:i:s') . "'",
                "usucpfinclusao" => "'" . $_SESSION['usucpforigem'] . "'",
            );

            $file = new FilesSimec("demandaarquivo", $campos, "demandasse");
            $arquivoSalvo = $file->setUpload($_FILES ['file']['name'], '', true);
            if ($arquivoSalvo) {
                echo "<script>alert('Arquivo cadastrado com sucesso!');</script>";
            }
        }
/*
        $modelDemandaArquivo->popularDadosObjeto();
        if ($modelDemandaArquivo->dmaid) {
            $modelDemandaArquivo->dmadtalteracao = date('Y-m-d H:i:s');
            $modelDemandaArquivo->usucpfalteracao = $_SESSION['usucpforigem'];
        } else {
            $modelDemandaArquivo->dmadtinclusao = date('Y-m-d H:i:s');
            $modelDemandaArquivo->usucpfinclusao = $_SESSION['usucpforigem'];
        }
        $modelDemandaArquivo->arqid = 10165;

        $sucesso = $modelDemandaArquivo->salvar();

        $modelDemandaArquivo->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;
*/
        die;
    case 'form_arquivo':
        $modelDemandaArquivo->popularDadosObjeto();
        $modelDemandaArquivo->formularioArquivos($complementoDisabled);
        die;
}

include  APPRAIZ."includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');
?>


<div class="row">
<div class="row col-md-11">

    <div class="row">
        <div class="row col-md-7">

            <?php if (!$complementoDisabled){ ?>
            <form id="form-save" method="post" class="form-horizontal">
                <?php } ?>
                <input name="controller" type="hidden" value="instituicao">
                <input name="action" type="hidden" value="salvar">
                <input name="dmdid" id="dmdid" type="hidden" value="<?php echo $modelDemanda->dmdid; ?>">

                <div class="well">
                    <fieldset>
                        <legend>Dados Gerais</legend>

                        <?php if ($modelDemanda->dmdid) { ?>
                            <div class="form-group">
                                <label for="dmdid" class="col-lg-4 col-md-4 control-label">Código:</label>

                                <div class="col-lg-8 col-md-8 " style="padding-top: 5px;">
                                    <span style="color: red;"><?php echo $modelDemanda->dmdid; ?></span>
                                </div>
                            </div>
                        <?php } ?>

                        <div class="form-group">
                            <label for="dmdtitulo" class="col-lg-4 col-md-4 control-label">Título:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmdtitulo" name="dmdtitulo" type="text" class="form-control" placeholder=""
                                       required="required"
                                       value="<?php echo $modelDemanda->dmdtitulo; ?>" <?php echo $complementoDisabled; ?>>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmddsc" class="col-lg-4 col-md-4 control-label">Descrição:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmddsc" name="dmddsc" type="text" class="form-control" placeholder=""
                                       required="required"
                                       value="<?php echo $modelDemanda->dmddsc; ?>" <?php echo $complementoDisabled; ?>>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdcriticidade" class="col-lg-4 col-md-4 control-label">Criticidade:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <div class="radio">
                                    <label>
                                        <input type="radio" name="dmdcriticidade" id="dmdcriticidade1" value="1"
                                            <?= ($modelDemanda->dmdcriticidade == 1 ? 'checked' : ''); ?>>
                                        <span style="color: #008f0b">Baixo</span>
                                    </label>
                                </div>
                                <div class="radio">
                                    <label>
                                        <input type="radio" name="dmdcriticidade" id="dmdcriticidade2" value="2"
                                            <?= ($modelDemanda->dmdcriticidade == 2 ? 'checked' : ''); ?>>
                                        <span style="color: #976d00">Médio</span>
                                    </label>
                                </div>
                                <div class="radio">
                                    <label>
                                        <input type="radio" name="dmdcriticidade" id="dmdcriticidade3" value="3"
                                            <?= ($modelDemanda->dmdcriticidade == 3 ? 'checked' : ''); ?>>
                                        <span style="color: #970012">Alto</span>
                                    </label>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdmandatoseguranca" class="col-lg-4 col-md-4 control-label">Mandato de Segurança:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-primary <?= ($modelDemanda->dmdmandatoseguranca == 't' ? 'active' : ''); ?>">
                                        <input type="radio" name="dmdmandatoseguranca" id="dmdmandatoseguranca1" value="t"> Sim
                                    </label>
                                    <label class="btn btn-primary <?= ($modelDemanda->dmdmandatoseguranca == 'f'  ? 'active' : ''); ?>">
                                        <input type="radio" name="dmdmandatoseguranca" id="dmdmandatoseguranca2" value="f" checked> Não
                                    </label>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdtribunalorigem" class="col-lg-4 col-md-4 control-label">Tribunal de
                                Origem:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmdtribunalorigem" name="dmdtribunalorigem" type="text" class="form-control"
                                       placeholder="" required="required"
                                       value="<?php echo $modelDemanda->dmdtribunalorigem; ?>" <?php echo $complementoDisabled; ?>>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdcompetencia" class="col-lg-4 col-md-4 control-label">Competência:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <select class="form-control" id="dmdcompetencia" name="dmdcompetencia">
                                    <?php echo $modelDemanda->getCompetencias(); ?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmduf" class="col-lg-4 col-md-4 control-label">UF:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <select class="form-control" id="dmduf" name="dmduf">
                                    <?php echo $modelDemanda->getComboUfs(); ?>
                                </select>
                            </div>
                        </div>


                        <div class="form-group">
                            <label for="dmdtipo" class="col-lg-4 col-md-4 control-label">Tipo:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <select class="form-control" id="dmdtipo" name="dmdtipo">
                                    <?php echo $modelDemanda->getTipos(); ?>
                                </select>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdprazo" class="col-lg-4 col-md-4 control-label">Prazo:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <?php $dmdprazo = $modelDemanda->dmdprazo ? substr($modelDemanda->dmdprazo, 8, 2) . '/' . substr($modelDemanda->dmdprazo, 5, 2) . '/' . substr($modelDemanda->dmdprazo, 0, 4) : ''; ?>
                                <input id="dmdprazo" name="dmdprazo" type="text" class="data form-control"
                                       placeholder=""
                                       value="<?php echo $dmdprazo; ?>" <?php echo $complementoDisabled; ?>>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdprocesso" class="col-lg-4 col-md-4 control-label">Nº Processo:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmdprocesso" name="dmdprocesso" type="text" class="form-control"
                                       placeholder=""
                                       value="<?php echo $modelDemanda->dmdprocesso; ?>" <?php echo $complementoDisabled; ?>>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdnumeroprocessoadm" class="col-lg-4 col-md-4 control-label">Nº Processo
                                Administrativo:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmdnumeroprocessoadm" name="dmdnumeroprocessoadm" type="text"
                                       class="form-control" placeholder=""
                                       value="<?php echo $modelDemanda->dmdnumeroprocessoadm; ?>" <?php echo $complementoDisabled; ?>>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdvalorcausa" class="col-lg-4 col-md-4 control-label">Valor da Causa:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmdvalorcausa" name="dmdvalorcausa" type="text" class="form-control moeda"
                                       placeholder="" required="required"
                                       value="<?php echo $modelDemanda->dmdvalorcausa; ?>" <?php echo $complementoDisabled; ?>>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdinstituicaoensino" class="col-lg-4 col-md-4 control-label">Instituição de
                                Ensino:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmdinstituicaoensino" name="dmdinstituicaoensino" type="text"
                                       class="form-control" placeholder=""
                                       value="<?php echo $modelDemanda->dmdinstituicaoensino; ?>" <?php echo $complementoDisabled; ?>>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdcnpjmantedora" class="col-lg-4 col-md-4 control-label">CNPJ
                                Mantenedora:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmdcnpjmantedora" name="dmdcnpjmantedora" type="text"
                                       class="form-control cnpj" placeholder=""
                                       value="<?php echo $modelDemanda->dmdcnpjmantedora; ?>" <?php echo $complementoDisabled; ?>>
                                <span id="mantedora" class="help-block"></span>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdobjeto" class="col-lg-4 col-md-4 control-label">Objeto:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmdobjeto" name="dmdobjeto" type="text" class="form-control" placeholder=""
                                       value="<?php echo $modelDemanda->dmdobjeto; ?>" <?php echo $complementoDisabled; ?>>
                            </div>
                        </div>

                        <div class="form-group">
                            <label for="dmdresumo" class="col-lg-4 col-md-4 control-label">Resumo:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <textarea class="form-control" rows="10" name="dmdresumo"
                                          id="dmdresumo"><?php echo $modelDemanda->dmdresumo; ?></textarea>
                            </div>
                        </div>

                    </fieldset>
                    <div>
                        <button title="Salvar" class="btn btn-success" type="submit"><span
                                class="glyphicon glyphicon-thumbs-up"></span> Salvar
                        </button>
                        <a title="Voltar" class="btn btn-danger" href="/demandasfies/demandasfies.php?modulo=inicio&acao=C"><span class="glyphicon glyphicon-hand-left"></span> Voltar</a>
                    </div>
                </div>
                <?php if (!$complementoDisabled){ ?>
            </form>
        <?php } ?>
        </div>

        <div class="row col-md-5">

            <div id="div_listagem_autor">
                <?php if ($modelDemanda->dmdid) {
                    $modelDemandaPartesAcao->dmdid = $modelDemanda->dmdid;
                    $modelDemandaPartesAcao->formularioAutor($complementoDisabled);
                } ?>
            </div>

            <div id="div_listagem_reu">
                <?php if ($modelDemanda->dmdid) {
                    $modelDemandaPartesAcao->dmdid = $modelDemanda->dmdid;
                    $modelDemandaPartesAcao->formularioReu($complementoDisabled);
                } ?>
            </div>

            <div id="div_listagem_observacao">
                <?php if ($modelDemanda->dmdid) {
                    $modelDemandaObservacao->dmdid = $modelDemanda->dmdid;
                    $modelDemandaObservacao->formularioObservacoes($complementoDisabled);
                } ?>
            </div>

            <div id="div_listagem_arquivo">
                <?php if ($modelDemanda->dmdid) {
                    $modelDemandaArquivo->dmdid = $modelDemanda->dmdid;
                    $modelDemandaArquivo->formularioArquivos($complementoDisabled);
                } ?>
            </div>
        </div>
    </div>
</div>
<div class="row col-md-1">
    <?php wf_desenhaBarraNavegacao($modelDemanda->docid, array('dmdid' => $modelDemanda->dmdid)); ?>

    <table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
        <tr style="background-color: #c9c9c9; text-align: center;">
            <td style="font-size: 7pt; text-align: center;">
                <span title="Ações Complementares"><strong>Ações</strong></span>
            </td>
        </tr>

        <tr style="text-align: center;">
            <td style="font-size: 7pt; text-align: center;">
                <a style="cursor: pointer" data-toggle="modal" data-target="#modal-intervencao" title="Solicitar intervenção de área">Solicitar intervenção de área</a>
            </td>
        </tr>
    </table>

</div>
<div class="clearfix"></div>
</div>

</div>

<div id="modal-intervencao" class="modal fade">
    <div class="modal-dialog">
        <div class="modal-content text-center">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title" style="font-weight: bold;">Solicitar intervenção da área</h4>
            </div>
            <div class="modal-body text-left">

                <form id="form-save-intervencao" method="post" class="form-horizontal">
                    <input name="action" type="hidden" value="salvar-intervencao">
                    <input name="dmdid" id="dmdid" type="hidden" value="<?php echo $modelDemanda->dmdid; ?>">
                    <input name="esdidorigem" id="esdidorigem" type="hidden" value="<?php echo $estadoAtual['esdid']; ?>">

                    <div class="form-group">
                        <label for="dmdid" class="col-lg-4 col-md-4 control-label">Área:</label>

                        <div class="col-lg-8 col-md-8 " style="padding-top: 5px;">
                            <?php
                            $sql = "select pflcod codigo, pfldsc as descricao
                                    from seguranca.perfil
                                    where sisid = " . SIS_DEMANDASFIES . "
                                    and pflcod not in (" . PFL_SUPER_USUARIO . ", " . PFL_ADMINISTRADOR . ", " . PFL_CONSULTA . ")
                                    order by pfldsc;";

                            echo $db->monta_combo("pflcod", $sql, 'S', "Selecione", "", "", "", "200", "S", "pflcod", "");
                            ?>
                        </div>
                    </div>
                </form>

            </div>
            <div class="modal-footer">
                <button title="Salvar" class="btn btn-success" type="button" id="tramitar-intervencao"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar</button>
                <button type="button" class="btn btn-danger" data-dismiss="modal"><span class="glyphicon glyphicon-thumbs-down"></span> Cancelar</button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->

<script src="/library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>

<script type="text/javascript">
    $(document).ready(function () {
        $('.cnpj').mask('99.999.999/9999-99');
        $('.moeda').mask('000.000.000.000.000,00', {reverse: true});
        $('.data').mask('99/99/9999');
        $('.data').datepicker();

        $('#dmdcnpjmantedora').on('blur', function () {
            $.post(window.location.href, {action: 'getPessoaJuridica', cnpj: $('#dmdcnpjmantedora').val() }, function (data) {
                $('#mantedora').html(data);
            });
        });

        $('#tramitar-intervencao').click(function () {
            $('#form-save-intervencao').submit();

        });
    });
</script>