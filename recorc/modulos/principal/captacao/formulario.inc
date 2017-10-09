<?php
/**
 * Controle do formulário de alteração de previsão da receita.
 *
 * @package simec
 * @subpackage receitas-orcamentarias
 * @version $Id: formulario.inc 98171 2015-06-03 14:22:44Z maykelbraz $
 */

// -- Iniciando o gerenciador de mensagens
$fm = new Simec_Helper_FlashMessage('recorc/captacao');
// -- Consutlando os perfis do usuário
$perfis = pegaPerfilGeral();

// -- Carregando os dados da captação
$vieid = $_REQUEST['vieid'];
try {
    $dadoscaptacao = carregarCaptacao($vieid, $_SESSION['usucpf']);
} catch (Exception $e) {
    $fm->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
    header('Location: recorc.php?modulo=principal/captacao/listar&acao=A');
    die();
}
unset($vieid);

if (isset($_POST['acao'])) {
    try {
        // -- Execução de requisições do formulário de atualização da captação
        switch ($_POST['acao']) {
            case 'salvar': // -- Salva alterações da captação
                salvarCaptacao($_POST, $dadoscaptacao['docid'], $dadoscaptacao['esdid']);
                $fm->addMensagem('Os dados da captação foram salvos com sucesso.');
                // -- Atualiza os dados do usuário que fez a captação
                atualizarDadosUsuario($dadoscaptacao, $_POST);
                $fm->addMensagem('Os dados do usuário foram atualizados com sucesso.');
                break;
            case 'incluirArquivo': // -- Incluir um novo arquivo
                incluirArquivo($dadoscaptacao['capid'], $_FILES);
                $fm->addMensagem('Arquivo adicionado com sucesso.');
                break;
            case 'inserirJustificativa': // -- Salvando a análise da SOF
                inserirJustificativa($dadoscaptacao['capid'], $_POST);
                $fm->addMensagem('Análise salva com sucesso.');
                tramitarAposAnalise(
                    $dadoscaptacao['docid'],
                    $dadoscaptacao['esdid'],
                    $_POST['statusJustificativa'],
                    $_POST['justificativaDesc']
                );
                break;
        }
    } catch (Exception $e) {
        $fm->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
    }
    header('Location: ' . $_SERVER['HTTP_REFERER']);
    die();
}

// -- Operações de arquivo
if ('S' == $_REQUEST['download']) {
    $file = new FilesSimec();
    $arqid = $_REQUEST['arqid'];
    ob_clean();
    $file->getDownloadArquivo($arqid);
    exit();
}

// -- função para apagar arquivo anexado
if ($_REQUEST['arqiddoc']) {
    global $db;
    $sql = "DELETE FROM recorc.anexogeral WHERE arqid = {$_REQUEST['arqiddoc']}";
    if (!$db->executar($sql)) {
        $fm->addMensagem('Não foi possível apagar o arquivo indicado.', Simec_Helper_FlashMessage::ERRO);
    } else {
        $db->commit('Arquivo apagadao com sucesso.');
    }
    header('Location: ' . $_SERVER['HTTP_REFERER']);
    die();
}

// -- Carregando os valores da previsão
$previsoes = carregarPrevisoes($dadoscaptacao['vieid']);

/* Desabilita por Período, exeto para os em acertos UO */
$periodoFormulario = recuperaPeriodo('atualformulario');
$periodoFormulario = is_array($periodoFormulario)?$periodoFormulario:array();

foreach ($periodoFormulario as $chave => $valor) {
    $periodoFormulario[$chave] = $valor['prfid'];
}
unset($foraPeriodoPreenchimento, $sql);

/**
 * Cabecalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<style>
.form-control-static{margin-top:8px}
.input-group img,.form-group img{display:none}
.periodoCorrente{color:#CD3333!important;font-weight:bold}
.periodoCorrente.active{color:yellow!important}
.table-text-centered td,.table-text-centered th{text-align:center}
</style>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/bootstrap.file-input.js"></script>
<script typ="text/javascript" lang="JavaScript" src="js/recorc.js"></script>
<script type="text/javascript" language="javascript">
jQuery(document).ready(function() {
    // -- Formatando o botão de input de arquivo
    $('input[type=file]').bootstrapFileInput();
    // -- Correção paliativa do bug #9920 do bootstrap.
    // -- Correção agendada para a versão 3.0.3 do bootstrap.
    // -- <https://github.com/twbs/bootstrap/issues/9920>
    $("input:radio").change(function() {
        $(this).prop('checked', true);
    });
    // -- Alternando entre os períodos
    $("input[id*='vieid_']").change(function() {
        var locationAtual = window.location.href;
        var locationSeguinte = locationAtual.substr(
            0,
            locationAtual.lastIndexOf('vieid=') + 6
            ) + $(this).val();
        window.location = locationSeguinte;
    });

    // -- Selecionando o Status da análise
    $('#statusJustificativa_<?php echo $dadoscaptacao['capretornosof']; ?>').click();
});
</script>
<div class="row">
    <div class="col-md-12">
        <div class="row">
            <div class="col-lg-12">
                <ol class="breadcrumb">
                    <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
                    <li><a href="recorc.php?modulo=principal/captacao/listar&acao=A">Lista de NRc <?php echo $_SESSION['exercicio']; ?></a></li>
                    <li class="active">Alterar Previsão</li>
                </ol>
            </div>
        </div>
        <div class="row">
            <div class="col-lg-8 col-lg-offset-2">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <h3 class="panel-title">Dados da previsão</h3>
                    </div>
                    <table class="table table-striped">
                        <tbody>
                            <tr>
                                <td style="width:20%;padding-top:15px"><strong>Período de referência:</strong></td>
                                <td style="text-align:left">
                                    <?php echo seletorPeriodo($dadoscaptacao['vieid'], $_SESSION['exercicio']); ?>
                                </td>
                            </tr>
                            <tr>
                                <td><b>Unidade Orçamentária:</b></td>
                                <td style="text-align:left">
                                    <?php echo "{$dadoscaptacao['unicod']} - {$dadoscaptacao['unidsc']}"; ?>
                                </td>
                            </tr>
                            <tr>
                                <td><b>Natureza da Receita:</b></td>
                                <td style="text-align:left">
                                    <?php echo "{$dadoscaptacao['nrccod']} - {$dadoscaptacao['nrcdsc']}"; ?>
                                </td>
                            </tr>
                            <tr>
                                <td><b>Fonte:</b></td>
                                <td style="text-align:left">
                                    <?php echo "{$dadoscaptacao['foncod']} - {$dadoscaptacao['fondsc']}"; ?>
                                </td>
                            </tr>
                            <tr>
                                <td colspan="2">
                                <?php echo tabelaDePrevisoes($previsoes['mensais']); ?>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
<?php

// -- Exibindo flash mensagem
echo $fm->getMensagens();

// -- Identificando a aba ativa
$abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'valores');
// -- URL base das abas
$urlBaseDasAbas = "recorc.php?modulo=principal/captacao/formulario&acao=A&vieid={$_REQUEST['vieid']}&aba=";

$listaAbas = array();
$listaAbas[] = array("id" => 1, "descricao" => "Valores Anualizados", "link" => "{$urlBaseDasAbas}valores");
if (isset($dadoscaptacao['capid'])) {
    $listaAbas[] = array("id" => 2, "descricao" => "Arquivos", "link" => "{$urlBaseDasAbas}arquivos");
}
if (mostrarAnalise($dadoscaptacao['capid'], $dadoscaptacao['esdid'])) {
    $listaAbas[] = array("id" => 2, "descricao" => "Análise SOF", "link" => "{$urlBaseDasAbas}analise");
}

// -- HTML das abas
echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");
?>
        <div class="row">
            <div class="col-lg-11">
            <?php
            $abaAtiva = ucfirst($abaAtiva);
            require_once(dirname(__FILE__) . "/formulario/form{$abaAtiva}.inc");
            ?>
            </div>
            <div id="wfmenu" class="col-lg-1">
                <?php if (array_intersect($perfis, array(PFL_CGO_EQUIPE_ORCAMENTARIA, PFL_SUPER_USUARIO))
                          && ('Valores' == $abaAtiva) && (STDOC_ANALISE_SPO == $dadoscaptacao['esdid'])): ?>
                    <div style="margin-bottom:15px">
                        <button class="btn btn-info" type="button" id="enviarAoSiop" style="width:80px;height:50px"
                                onclick="gravarSiop(<?php echo $_REQUEST['vieid']; ?>)">
                            <span class="glyphicon glyphicon-send"></span>
                            <br />Enviar SIOP
                        </button>
                    </div>
                <?php endif; ?>
                <?php
                    /* Barra de estado atual e ações e Historico */
                    wf_desenhaBarraNavegacao(
                        $dadoscaptacao['docid'],
                        array(
                            'capid' => $dadoscaptacao['capid'],
                            'usuario' => $dadoscaptacao['usunome'],
                            'email' => $dadoscaptacao['usuemail']
                        )
                    );
                ?>
            </div>
        </div>
    </div>
</div>
</div>
<script type="text/javascript">
function voltarLista(sunicod, sfoncod, snrccod, swrfstatus, sprfid) {
    window.location = 'recorc.php?modulo=principal/captacao/listar&acao=A&unicod=' + sunicod + '&nrccod=' + snrccod + '&foncod=' + sfoncod + '&prfid=' + sprfid + '&wrfstatus=' + swrfstatus;
}
</script>
<div class="modal fade" id="modal-Justificativa">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"
                        aria-hidden="true">&times;</button>
                <h4 class="modal-title">Justificativa</h4>
            </div>
            <div class="modal-body">
                <table>
                    <tr>
                        <td><textarea name="justificativa" cols="80" rows="13"
                                      id="justificativa" disabled="disabled"></textarea></td>
                    </tr>
                </table>
                <div class="modal-footer">
                    <!--  <button type="button" class="btn btn-primary">Ok</button> -->
                </div>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>
    <!-- /.modal -->
</div>