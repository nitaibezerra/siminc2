<?php
/**
 * Arquivo de execução da consulta da execução orçamentária no SIOP.
 *
 * $Id: consultarExecucaoOrcamentaria.inc 103367 2015-10-06 18:29:00Z maykelbraz $
 * @filesource
 */

$fm = new Simec_Helper_FlashMessage('consultar-execucao-orcamentaria');

if (isset($_POST['requisicao']) && 'consultarExecucaoOrcamentaria' == $_POST['requisicao']) {

    $dadosForm = $_POST['consultarexecucaoorcamentaria'];
    $log = isset($dadosForm['log'])?'spo':null;
    unset($dadosForm['log']);

    $camposDeRetorno = $dadosForm['selecaoretorno'];
    unset($dadosForm['selecaoretorno']);
    $pagina = $dadosForm['pagina'];
    unset($dadosForm['pagina']);
    $filtros = $dadosForm;

    if (key_exists('unidadesorcamentarias', $filtros)) {
        $filtros['unidadesOrcamentarias'] = $filtros['unidadesorcamentarias'];
        unset($filtros['unidadesorcamentarias']);
    }

    if (1 == $pagina) {
        // -- Limpando a tabela de armazenamento temporário
        $db->executar('DELETE FROM wssof.ws_execucaoorcamentariadto');
        $db->commit();
    }

    $ws = new Spo_Ws_Sof_Quantitativo($log);
    $retorno = $ws->consultarExecucaoOrcamentaria($filtros, $camposDeRetorno, $pagina)->return;

    // -- Falha ao executar a consulta
    if ($retorno->mensagensErro) {
        if (!is_array($retorno->mensagensErro)) {
            $fm->addMensagem($retorno->mensagensErro, Simec_Helper_FlashMessage::ERRO);
        } else {
            foreach ($retorno->mensagensErro as $mensagem) {
                $fm->addMensagem($mensagem, Simec_Helper_FlashMessage::ERRO);
            }
        }

        echo simec_json_encode(array('terminate' => true, 'erros' => $retorno->mensagensErro));
        die();
    }

    // -- processando o retorno da consulta
    if ($numRegistros = count($retorno->execucoesOrcamentarias->execucaoOrcamentaria)) {

        $sql = "INSERT INTO wssof.ws_execucaoorcamentariadto(" . implode(', ', $camposDeRetorno) . ") VALUES(%s)";
        foreach ($retorno->execucoesOrcamentarias->execucaoOrcamentaria as $execucao) {
            $valores = array();
            foreach ($camposDeRetorno as $campo) {
                $valores[] = "'{$execucao->$campo}'";
            }
            $db->executar(sprintf($sql, implode(', ', $valores)));
        }
        $db->commit();

        echo simec_json_encode(array('terminate' => false, 'pagina' => $pagina, 'numRegistros' => $numRegistros));
    } else {
        echo simec_json_encode(array('terminate' => true));
    }
    die();
}

/**
 * Cabeçalho padrão do sistema.
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<script type="text/javascript">
function consultarExecucaoOrcamentaria()
{
    var dados = $('#consultarexecucaoorcamentaria').serialize();

    $.post(window.location.href, dados, function(data){
        if (!data.terminate) {
            $('#consultarexecucaoorcamentaria_pagina').val(
                parseInt($('#consultarexecucaoorcamentaria_pagina').val()) + 1
            );
            $('#progress').show();
            $('#progress-pagina')
                .text($('#consultarexecucaoorcamentaria_pagina').val() + ' Página(s)')
                .css('width', parseInt($('#consultarexecucaoorcamentaria_pagina').val()) * 3 + '%');
            consultarExecucaoOrcamentaria();

        } else {
            if (data.erros) {
                bootbox.alert(data.erros, function(){
                    window.location.assign(window.location.href);
                });
            }
            $('#progress').hide();
        }

    }, 'json').fail(function(res){
        console.log('Erro na requisição: ' + res);
    });
}

function onConsultarexecucaoorcamentariaSubmit(e)
{
    if (!$('#consultarexecucaoorcamentaria_selecaoRetorno').val()) {
        alert('O campo "Campos de retorno" não pode ser deixado em branco.');
        return false;
    }

    e.preventDefault();
    consultarExecucaoOrcamentaria();
}
</script>
<div class="row col-md-12">
<?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Integração')
        ->add('Sof Quantitativo')
        ->add('Consultar Execução Orçamentária')
        ->render();

    echo $fm;
?>
    <div class="progress" id="progress" style="height:25px;display:none">
        <div class="progress-bar progress-bar-striped .active" id="progress-pagina" role="progressbar"
             aria-valuenow="60" aria-valuemin="5" aria-valuemax="100" style="min-width:5%;padding-top:6px">
            1 Página(s)
        </div>
    </div>
    <?php
        $colunasRetorno = Spo_Ws_Sof_QuantitativoMap::getComponentesSelecaoRetornoExecucaoOrcamentariaDTO();
        $dados = array();
        foreach ($colunasRetorno as $coluna) {
            $dados[] = array('codigo' => $coluna, 'descricao' => $coluna);
        }
        $multiple = array('multiple' => true);

        $form = new Simec_View_Form('consultarexecucaoorcamentaria');
        $form->carregarDados($_GET['consultarexecucaoorcamentaria'])
            ->addCombo('Campos de retorno', 'selecaoRetorno', $dados, $multiple)
            ->addCombo('Unidade(s) Orçamentária(s)', 'unidadesOrcamentarias', Spo_Model_Unidade::queryCombo((object)[]), $multiple)
            ->addCheckbox('Registrar log do serviço?', 'log')
            ->addBotoes(array('limpar', 'importar'))
            ->addHidden('anoExercicio', $_SESSION['exercicio'])
            ->addHidden('pagina', 1)
            ->setRequisicao('consultarExecucaoOrcamentaria')
            ->render();
        ?>
</div>