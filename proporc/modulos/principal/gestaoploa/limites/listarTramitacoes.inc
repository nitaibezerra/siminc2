<?php
/**
 * Listagem de registros prontos para tramitação.
 * $Id: listarTramitacoes.inc 95385 2015-03-16 20:43:34Z maykelbraz $
 */

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
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<style type="text/css">
.marcado{background-color:#C1FFC1!important}
.remover{ display:none;}
.modal-dialog{width:70%!important}
.tr_erro{color:red!important}
</style>
<script type="text/javascript" lang="JavaScript">
$(document).ready(function(){
    if ($('.make-switch')) {
        $('#btn-enviar').show();

        // -- Envio em massa de propostas
        $('#btn-enviar').click(function(){
            // -- Verificando se existe ao menos uma programatica selecionada para envio
            if (0 == $('.tabela-listagem input[type=checkbox]:checked').length) {
                $('#modal-alert .modal-title').html('<p class="text-left">Envio em lote</p>');
                $('#modal-alert .modal-body')
                    .html('<p class="text-center">Você deve selecionar ao menos uma programática para realizar o envio.</p>');
                $('#modal-alert').modal();
                return false;
            }
            // -- Envio da requisição de envio de lotes em massa
            $('#requisicao').val('enviarProposta');
            $('#formtramitacao').submit();
        });
    }
    $('#modal-alert .modal-title').text('Retorno da SOF');
    $('#modal-alert .modal-dialog').removeClass('modal-lg');
});

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
<form name="formtramitacao" id="formtramitacao" method="POST" role="form">
    <input type="hidden" name="requisicao" id="requisicao" />
    <input type="hidden" name="dados[acaid]" id="acaid" />
    <div id="listagem">
    <?php imprimirAcoes($dados, $dados['ppoid'], $_SESSION['exercicio'], 'tramitacao'); ?>
    </div>
    <?php if (!in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral($_SESSION['usucpf']))): ?>
    <button type="button" class="btn btn-success" style="display:none"
            id="btn-enviar">Enviar selecionados para o SIOP</button>
    <?php endif; ?>
</form>