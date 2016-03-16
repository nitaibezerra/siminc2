<?php
/**
 * Arquivo principal de controle de limites e metas da PLOA.
 *
 * $Id: controleenvio.inc 101459 2015-08-19 13:29:06Z werteralmeida $
 * @filesource
 */

/**
 * Funções do workflow0
 * @see workflow.php
 */
include_once APPRAIZ . 'includes/workflow.php';

/**
 * Funções de apoio à gestão da PLOA.
 * @see _funcoesgestaoploa.php
 */
require_once(APPRAIZ . "www/{$_SESSION['sisdiretorio']}/_funcoesgestaoploa.php");

$fm = new Simec_Helper_FlashMessage("{$_SESSION['sisdiretorio']}/gestao");

// -- Carregando dados da gestão
$dados = &dadosDaSessao('gestaoploa');

/* Pegando dados de Filtro do Formulário para alterar a variável $dados */
unset($dados['unicod']);
if (isset($_REQUEST['dados']['unicod']) && $_REQUEST['dados']['unicod'] != '') {
    $dados['unicod'] = $_REQUEST['dados']['unicod'];
}

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'enviarProposta':
            if (is_array($_POST['dados']['acaid'])) {
                $listaAcoes = $_POST['dados']['acaid'];
                $sucessoNoEnvio = true;
                foreach ($listaAcoes as $acaid => $_) {
                    $dadosEnvio = array('acaid' => $acaid, 'tramitar' => true);
                    $retorno = enviarProposta($dadosEnvio);
                    if (!$retorno['sucesso']) {
                        $sucessoNoEnvio = false;
                    }
                }
                // -- Todos os registros enviados com sucesso
                if ($sucessoNoEnvio) {
                    $fm->addMensagem('Registros selecionados enviados com sucesso.');
                } else {
                    $fm->addMensagem('Alguns registros selecionados não foram enviados. Veja o log de erros para maiores detalhes.', Simec_Helper_FlashMessage::ERRO);
                }
            } else {
                $dadosEnvio = $_POST['dados'];
                $dadosEnvio['tramitar'] = true;
                $returno = enviarProposta($dadosEnvio);
                if ($retorno['sucesso']) {
                    $fm->addMensagem('Registros selecionados enviados com sucesso.');
                } else {
                    $fm->addMensagem('Alguns registros selecionados não foram enviados. Veja o log de erros para maiores detalhes.');
                }
            }
            break;
        case 'drawWorkflow':
            wf_desenhaBarraNavegacao(
                $_POST['dados']['docid'],
                array(
                    'dados' => array(
                        'acaid' => $_POST['dados']['acaid'],
                        'tramitar' => false
                    )
                )
            );
            die();
        case 'consultarMotivoFalhaSIOP':
            imprimirMotivoFalhaSIOP($_POST['dados']);
            die();
        default:
            die($requisicao);
    }

    // -- Redirecionamento padrão
    if (!isset($novaURL)) {
        $novaURL = $_SERVER['REQUEST_URI'];
    }
    header('Location: ' . $novaURL);
    die();
}
// -- Carregando e verificando se existe uma proposta
if (!$existeProposta = verificarPropostaOrcamentaria($_SESSION['exercicio'])) {
    $exercicioatual = $_SESSION['exercicio'];
    $exercicioseguinte = $exercicioatual + 1;
    $fm->addMensagem(
            <<<HTML
<strong><span class="glyphicon glyphicon-warning-sign"></span> Atenção!</strong>
Não foi localizada uma proposta orçamentária no exercício atual ({$exercicioatual})
referente ao exercício seguinte ({$exercicioseguinte}).
HTML
            , Simec_Helper_FlashMessage::ERRO);
}

/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<style type="text/css">
    label{margin-top:10px;cursor:pointer}
    .panel-collapse table{margin-bottom:0}
    .glyphicon-chevron-right,.glyphicon-info-sign{font-size:.8em;margin-right:5px}
    .modal-body{text-align:left}
    @media(min-width:992px){.modal-lg{width:100%}}
</style>
<script type="text/javascript" lang="javascript">
    $(document).ready(function() {
        $('#modal-alert .modal-dialog').addClass('modal-lg');
        $('.info-elabrev').click(function() {
            var requisicao = $(this).attr('data-requisicao');
            var titulo = $(this).attr('data-titulo');
            $.post(
                    window.location,
                    {
                        requisicao: requisicao,
                        'dados[unicod]': $('#unicod').val(),
                        'dados[ppoid]': $('#ppoid').val(),
                        'dados[exercicio]': $('#exercicio_spo').val()
                    },
            function(html) {
                $('#modal-alert .modal-title').text(titulo);
                $('#modal-alert .modal-body').html(html.trim());
                $('#modal-alert').modal();
            }
            );
        });
    });
</script>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script type="text/javascript">
    $(document).ready(function() {
        $('#back').click(function() {
            window.location.href = 'proporc.php?modulo=inicio&acao=C';
        });
    });
</script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Controle de envio da PLOA <?php echo $_SESSION['exercicio'] + 1; ?></li>
    </ol>
    <?php echo $fm->getMensagens(); ?>
    <br style="clear:both" />
    <?php
    if ($existeProposta = verificarPropostaOrcamentaria($_SESSION['exercicio'])) {
        ?>
        <form name="filtrouo" id="filtrouo" method="POST" role="form">
            <input type="hidden" name="requisicao" id="requisicao" />
            <div class="well">
                <div class="form-group row">
                    <div class="col-md-2">
                        <label class="control-label" for="unicod">Unidade Orçamentária:</label>
                    </div>
                    <div class="col-md-10">
                        <?php inputComboUnicod($dados['unicod']); ?>
                    </div>
                </div>
                <div class="form-group row">
                    <div class="col-md-2">
                        <label class="control-label" for="unicod">Ação:</label>
                    </div>
                    <div class="col-md-10">
                        <?php
                        $sql = " SELECT DISTINCT
                                    acacod AS codigo,
                                    acacod AS descricao
                                FROM
                                    elabrev.ppaacao_orcamento
                                WHERE
                                    prgano = '{$_SESSION['exercicio']}'
                                ORDER BY
                                    1";
                        inputCombo("dados[acacod]", $sql, $_REQUEST['dados']['acacod'], "acacod");
                        ?>
                    </div>
                </div>
                <div class="form-group row">
                    <div class="col-md-2">
                        <label class="control-label" for="unicod">Situação:</label>
                    </div>
                    <div class="col-md-10">
                        <?php
                        $sql = "  SELECT
                                    esd.esdid  AS codigo,
                                    esd.esddsc AS descricao
                                FROM
                                    workflow.estadodocumento esd
                                WHERE
                                    tpdid = 188
                                AND esdstatus = 'A'
                                ORDER BY
                                    esdordem";
                        inputCombo("dados[esdid]", $sql, $_REQUEST['dados']['esdid'], "esdid");
                        ?>
                    </div>
                </div>
                <div class="form-group row">
                    <div class="col-md-2">
                        <label class="control-label" for="unicod">Fonte:</label>
                    </div>
                    <div class="col-md-10">
                        <div class="btn-group" data-toggle="buttons">
                            <?php $fonte = $_REQUEST['dados']['fonte']; ?>
                            <label
                                class="btn btn-default <?php
                                if ($fonte == '' || !$fonte || $fonte == 'todos') {
                                    echo 'active';
                                }
                                ?>"> <input type="radio" name="dados[fonte]"
                                     id="fonte_todos" value="todos" />Todos
                            </label> <label
                                class="btn btn-default <?php
                                if ($fonte == 'O') {
                                    echo 'active';
                                }
                                ?>"> <input type="radio" name="dados[fonte]"
                                     id="fonte_O" value="O"
                                     <?php
                                     if ($fonte == 'O') {
                                         echo 'checked=checked';
                                     }
                                     ?> /> OK
                            </label> <label
                                class="btn btn-default <?php
                                if ($fonte == 'P') {
                                    echo 'active';
                                }
                                ?>"> <input type="radio" name="dados[fonte]"
                                     id="fonte_P" value="P"
                                     <?php
                                     if ($fonte == 'P') {
                                         echo 'checked=checked';
                                     }
                                     ?> /> Excedida
                            </label>
                        </div>
                    </div>
                </div>

                <div class="form-group row">
                    <div class="col-md-2">
                        <label class="control-label" for="unicod">Retorno SIOP:</label>
                    </div>
                    <div class="col-md-10">
                        <div class="btn-group" data-toggle="buttons">
                            <?php $retornosiop = $_REQUEST['dados']['retornosiop']; ?>
                            <label
                                class="btn btn-default <?php
                                if ($retornosiop == '' || !$retornosiop || $retornosiop == 'todos') {
                                    echo 'active';
                                }
                                ?>"> <input type="radio" name="dados[retornosiop]"
                                     id="retornosiop_todos" value="todos" />
                                Todos
                            </label>
                            <label
                                class="btn btn-default <?php
                                if ($retornosiop == 'OK') {
                                    echo 'active';
                                }
                                ?>">
                                <input type="radio" name="dados[retornosiop]"
                                     id="retornosiop_OK" value="OK"
                                     <?php
                                     if ($retornosiop == 'OK') {
                                         echo 'checked=checked';
                                     }
                                     ?> /> <span class="glyphicon glyphicon-thumbs-up" style="color:green"></span>
                            </label>
                            <label
                                class="btn btn-default <?php
                                if ($retornosiop == 'ERRO') {
                                    echo 'active';
                                }
                                ?>"> <input type="radio" name="dados[retornosiop]"
                                     id="retornosiop_ERRO" value="ERRO"
                                     <?php
                                     if ($retornosiop == 'ERRO') {
                                         echo 'checked=checked';
                                     }
                                     ?> /> <span class="glyphicon glyphicon-thumbs-down" style="color:red"></span>
                            </label>
                        </div>
                    </div>
                </div>
            </div>
            <button type="button" class="btn btn-warning" id="back">Voltar</button>
            <button type="submit" class="btn btn-primary" id="pesquisar" role="submit">Pesquisar</button>
        </form>
        <?php
        require_once dirname(__FILE__) . "/controleenvio/listarControleEnvio.inc";
    } // Fim de exibir apenas se houver período
    ?>
</div>
<br />
<br />