<?php
$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/quantitativo/obterAcoesDisponiveisParaAcompanhamentoOrcamentario');

// -- Processamento de requisições
if (chaveTemValor($_REQUEST, 'requisicao'))
{
    switch ($_REQUEST['requisicao'])
    {
        case 'obterProgramacaoCompletaQuantitativo':
          	obterProgramacaoCompletaQuantitativo($_POST, $fm);
          	break;
    }
    die('-');
}

require_once APPRAIZ . "includes/cabecalho.inc";
?>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<script type="text/javascript">
	/**
 	* obterProgramacaoCompletaQuantitativo com base no momento escolhido.
 	*/
 	function obterProgramacaoCompletaQuantitativo()
	{
            var dados = {
                requisicao:'obterProgramacaoCompletaQuantitativo',
                pagina:$('#pagina').val(),
                codigomomento:$('#frmConsultar_codigoMomento').val(),
                log:$('#frmConsultar_log').val()
            };

            $.post(window.location, dados,
            function(data) {
                var _data = JSON.parse(data);

                if (!_data.terminate) {
                    $('#pagina').val(parseInt($('#pagina').val())+1);
                    $('#progress').show();
                    $('#progress-pagina').text($('#pagina').val() + ' Pagina').css('width', parseInt($('#pagina').val()) * 10 + '%');
                    obterProgramacaoCompletaQuantitativo();
                } else {
                    if (_data.erros) {
                        alert(_data.erros);
                    }

                    $('#progress').hide();
                }
            }, 'text').fail(function(res){console.log('Erro: ' + res);});
	}

function onFrmConsultarSubmit(e)
{
    e.preventDefault();
    if (!$('#frmConsultar_codigoMomento').val()) {
        alert('O Campo "Momento" deve ser preenchido.');
        return false;
    }

    obterProgramacaoCompletaQuantitativo();
}
</script>
<div class="col-lg-12">
    <?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Integração')
        ->add('SOF Quantitativo')
        ->add('Obter Programação Completa')
        ->render();
    echo $fm->getMensagens();
    ?>
    <div class="progress" id="progress" style="display: none; height: 25px;">
        <div class="progress-bar" id="progress-pagina" role="progressbar" aria-valuenow="60" aria-valuemin="0" aria-valuemax="100" style="width: 10%; padding-top: 6px;">
            1 Pagina
        </div>
    </div>
    <?php

    $form = new Simec_View_Form('frmConsultar');
    $form->carregarDados($_REQUEST['frmConsultar']);

    // -- input momento
    $sql = <<<DML
        SELECT distinct
            codigomomento as codigo,
            codigomomento || ' - ' || descricao as descricao
        FROM wssof.ws_momentosdto m
        WHERE m.snativo = '1'
        ORDER BY codigo
DML;
    $opcoes = array(
        'titulo' => 'Selecione ao menos um momento',
        'obrig' => 'S'
    );
    $form->addCombo('Momento', 'codigoMomento', $sql, $opcoes)
        ->setRequisicao('obterProgramacaoCompletaQuantitativo')
        ->addHidden('pagina', 1, 'pagina')
        ->addCheckbox('Registrar log?', 'log')
        ->addBotoes(array('limpar', 'importar'))
        ->render();
    ?>
</div>