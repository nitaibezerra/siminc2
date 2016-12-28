<?php
/**
 * Listagem de registros prontos para tramitação.
 * $Id: listarControleEnvio.inc 100795 2015-08-04 20:12:27Z maykelbraz $
 */

// -- Filtros do relatório
$dados['filtros']['acacod'] = $_REQUEST['dados']['acacod'];
$dados['filtros']['esdid'] = $_REQUEST['dados']['esdid'];
$dados['filtros']['fonte'] = $_REQUEST['dados']['fonte'];
$dados['filtros']['retornosiop'] = $_REQUEST['dados']['retornosiop'];

// -- Informações de limites de fontes para controlar envio de ações
// -- que tem sua despesa composta por recursos de uma fonte que, atualmente, excede seu limite.
$fontesComLimiteExcedido = array();

if ($retornoFontes = consultarFontes($dados, $_SESSION['exercicio'])) {
    foreach ($retornoFontes as $fonte) {
        if ((double)$fonte['vldespesa'] > (double)$fonte['vllimite']) {
            $fontesComLimiteExcedido[] = $fonte['codigo'];
        }
    }
    unset($retornoFontes);
}

// -- Formatando uma mensagem de aviso sobre as fontes com limite excedido
if (!empty($fontesComLimiteExcedido)) {
    $pluralMsg = '';
    if (count($fontesComLimiteExcedido) > 1 ) {
        $pluralMsg = '(s)';
    }

    $mensagem = "A{$pluralMsg} seguinte{$pluralMsg} fonte{$pluralMsg} estão com seu limite excedido e<br />"
              . "as ações associada{$pluralMsg} a ela{$pluralMsg} não poderão ser tramitadas: "
              . '<b>' . implode('</b>, <b>', $fontesComLimiteExcedido) . '</b>.';
    // -- Trocando o último ', ' por ' e '
    $mensagem = substr_replace($mensagem, ' e ', strrpos($mensagem, ', '), 2);
    $fm->addMensagem($mensagem , Simec_Helper_FlashMessage::INFO);
}
?>
<link rel="stylesheet" href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<style type="text/css">
.popover-content table td{padding:3px !important}
.popover.right .arrow{top:35px !important}
.popover-title{text-align:center}
.marcado{background-color:#C1FFC1 !important;}
.remover{ display:none;}
.modal-dialog{width:70%!important}
</style>
<script src="/library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
<script type="text/javascript" lang="JavaScript">
$(function(){
    if ($('.make-switch').size() > 0) {
        $('#btn-enviar').show();
        $('#btn-marcar').show();
    }
    $('#modal-alert .modal-title').text('Retorno da SOF');
    $('#modal-alert .modal-dialog').removeClass('modal-lg');

    // -- Envio em massa de propostas
    $('#btn-enviar').click(function(){
        $('#requisicao_envio').val('enviarProposta');

        // -- Copiando os elementos para o form de envio
        $('.form-listagem input[type="checkbox"]:checked').clone().appendTo('#selecionados');
        $('#formtramitacao').submit();
    });

    // -- Selecionar/Desselecionar todos
    $('#btn-marcar').click(function(){
        if ('marcar' === $(this).attr('data-acao')) {
            $('.make-switch').prop('checked', true);
            $(this).attr('data-acao', 'desmarcar');
        } else {
            $('.make-switch').prop('checked', false);
            $(this).attr('data-acao', 'marcar');
        }
    });

    // -- Filtros da listagem - hack
    $('.form-listagem').attr('data-form-filtros', 'filtrouo');
});

function recuperaWorkflow(id, docid)
{
    $.post(
        window.location,
        {requisicao:'drawWorkflow', 'dados[acaid]': id, 'dados[docid]': docid},
        function(html){
            var id_content = '#popover-content-' + id;
            $(id_content).empty().html(html.replace(/<br\/>/g, ''));
        }
    );
    return '<div id="popover-content-'+id+'">carregando...</div>';
}

function showWorkflow(acaid, docid)
{
    var id = '#arow-' + acaid;

    $(id).popover({html: true, trigger: 'manual', content: function() {
        return recuperaWorkflow(acaid, docid);
    }});

    if ($(id).attr('data-popover-shown')) { // -- esconder
        $(id).popover('hide');
        $(id).removeAttr('data-popover-shown'); // -- esconder popovers que tem esse atributo
    } else { // -- mostrar
        if (!$(id).attr('data-popover-loaded')) {

        }
        $(id).attr('data-popover-shown', true);
        $(id).popover('show');
    }
}

function detalharProgramatica(acaid)
{
    $.post(
        window.location,
        {'requisicao': 'consultarMotivoFalhaSIOP', 'dados[acaid]': acaid},
        function(html){
            $('#modal-alert .modal-body').html(html);
            $('#modal-alert').modal();
        }
    );
}

function enviarProposta(acaid)
{
    $('#acaid').val(acaid);
    $('#requisicao').val('enviarProposta');
    $('#formtramitacao').submit();
}
</script>
<br />
<?php
// -- Nova chamada para exibição de mensagens de limite de fonte excedido
echo $fm->getMensagens();
?>
<div id="listagem">
<?php imprimirAcoes($dados, $dados['ppoid'], $_SESSION['exercicio'], 'tramitacao'); ?>
</div>
<form name="formtramitacao" id="formtramitacao" method="POST" role="form">
    <input type="hidden" name="requisicao" id="requisicao_envio" />
    <input type="hidden" name="dados[acaid]" id="acaid" />
    <div style="display:none" id="selecionados">

    </div>
    <button type="button" class="btn btn-success" style="display:none" id="btn-enviar">Enviar selecionados para o SIOP</button>
    <button type="button" class="btn btn-info" style="display:none" id="btn-marcar" data-acao="marcar">Marcar todos</button>
</form>