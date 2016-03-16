<?php
/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */

#require_once APPRAIZ . "spo/ws/sof/Quantitativo.php";

$fm = new Simec_Helper_FlashMessage('spo/integracao/acompanhamentoFisicoFinanceiro');
if(isset($_POST['requisicao']) && $_POST['requisicao'] == 'carga'){
    obterAcompanhamentoFisicoFinanceiro($_POST['form'],$fm);
}
include APPRAIZ . "includes/cabecalho.inc";
$sqlMomento = <<<DML
    SELECT distinct
        codigomomento as codigo,
        codigomomento || ' - ' || descricao as descricao
    FROM wssof.ws_momentosdto m
    WHERE m.snativo = '1'
    ORDER BY codigomomento;
DML;
?>
<script>
    function enviaFormulario(){
        if($('#form #periodo').val() == ''){
            bootbox.alert('Selecione um período de referência.');
            return false;
        }
        if($('#form #momento').val() == ''){
            bootbox.alert('Selecione um momento.');
            return false;
        }
        if(!($('#form [class=btn-group] label').hasClass('active'))){
            bootbox.alert('Selecione um tipo de captação.');
            return false;
        }
        submit();
    }

    function submit()
    {
        var dados = $('#form').serialize();
        $.post(window.location, dados,function(data){obterAcompanhamento(data);},'text').fail(function(res){console.log('Erro: ' + res);});
    }

    function obterAcompanhamento(data) {
        $('#progress').show();
        console.log(data);
        var _data = JSON.parse(data);

        if (!_data.terminate){
            $('#pagina').val(parseInt($('#pagina').val())+1);
            $('#progress-pagina').css('width', parseInt($('#pagina').val()) * 10 + '%').find('span').text('Página ' + $('#pagina').val());
            submit();
        }else{
            $('#progress-pagina').css('width', '100%').find('span').text('Concluído. Aguarde...');
            setTimeout(function(){
                window.location.href = window.location.href;
            },1500);
        }
    }

    $(document).ready(function(){
        $('#progress').hide();
        $('body').on('click','button',function(e){
            if($(e.target).attr('id') == 'buttonExecute'){
                enviaFormulario();
            }else if($(e.target).attr('id') == 'buttonBack'){
                window.location.href = $(e.target).attr('data-url');
            }else{
                console.log('Botão sem função definida.');
                return true;
            }
        });
    });
</script>
<link rel="stylesheet" href="/includes/spo.css">
<section class='col-lg-12'>
    <ol class="breadcrumb">
        <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li>Integração</li>
        <li>Sof Quantitativo</li>
        <li class="active">Consultar Acompanhamento Físico e Financeiro</li>
    </ol>
    <div class="progress" id='progress' style='margin-bottom:10px;'>
        <div class="progress-bar progress-bar-info progress-bar-striped active" id='progress-pagina' role="progressbar" aria-valuenow="10" aria-valuemin="10" aria-valuemax="100" style="width:10%;">
            <span>Página 1</span>
        </div>
    </div>
    <?php
    echo $fm->getMensagens();

    $simecForm = new Simec_View_Form('form');
    $simecForm->setRequisicao('carga');

    $simecForm->addHidden('pagina','1','pagina');

    $dados = array(array('codigo' => 1, 'descricao' => 'Período 1'),array('codigo' => 2, 'descricao' => 'Período 2'));
    $simecForm->addInputCombo('periodo',$dados,$value,'periodo',array('flabel' => 'Período de Referência'));

    $options = array('titulo' => 'Selecione ao menos um momento','obrig' => 'S','flabel' => 'Momento');
    $simecForm->addInputCombo('momento',$sqlMomento,$value,'momento',$options);

    $options = array('Localizadores' => 'L', 'Planos Orçamentários' => 'P');
    $simecForm->addInputChoices('tipoCaptacao',$options,$value,'tipoCaptacao',array('flabel' => 'Tipo de Captação'));

    $simecForm->addInputCheckbox('log','1','log',array('flabel' => 'Registrar log do serviço?<br>'));

    $simecForm->addBotao('avancado',array('id' => 'buttonBack', 'label'=>'Voltar','span'=>'glyphicon glyphicon-arrow-left','class'=>'btn-warning','extra'=>'data-url="spo.php?modulo=inicio&acao=C"'));
    $simecForm->addBotao('avancado',array('id' => 'buttonExecute','label'=>'Executar','span'=>'glyphicon glyphicon-ok','class'=>'btn-success'));

    $simecForm->render();
    ?>
</section>