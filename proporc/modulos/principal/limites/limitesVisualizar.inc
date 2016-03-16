<?php
/**
 * Conteúdo da aba de limites da definição de limites.
 *
 * $Id: limitesVisualizar.inc 101153 2015-08-12 12:59:53Z maykelbraz $
 *
 * @filesource
 */
$list = new Simec_Listagem();
$list->setQuery(Proporc_Model_Limitesfonteunidadeorcamentaria::querySomatorioGrupos(array('gdp.prfid' => $prfref->prfid)))
    ->setCabecalho(array('Nome do grupo', 'Montante (R$)', 'Limite (R$)', 'Detalhado (R$)', 'M - L (R$)', 'L - D (R$)'))
    ->addCallbackDeCampo(array('vlrlimite', 'vlrmontante', 'vlrdetalhado', 'saldomontante', 'saldolimite'), 'mascaraMoeda')
    ->addCallbackDeCampo('gdpnome', 'alinharEsquerda')
    ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array(
        'vlrmontante',
        'vlrlimite',
        'vlrdetalhado',
        'saldomontante',
        'saldolimite'
    ))->addAcao('plus', 'detalharCategoriasDoGrupo')
    ->setId('lista-de-grupos')
    ->setIdLinha('gdp')
    ->turnOnPesquisator()
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

bootstrapPopup(
    'Detalhamento de limites',
    'mdl-detalhamentolimites',
    dirname(__FILE__) . '/popupDetalhamentoLimites.inc',
    array('cancelar', 'salvar'),
    array('tamanho' => 'lg')
);
?>
<script type="text/javascript">
$(function(){
    $('#mdl-detalhamentolimites .btn-salvar').click(salvarLimitesCategoria);
});
function detalharLimitesCategoria(dspid)
{
    var dados = {requisicao: 'detalharLimitesCategoria', 'detalharlimitescategoria[dspid]': dspid};
    $.get(window.location.href, dados, function(response){
        $('#mdl-detalhamentolimites .modal-body').empty().html(response);
        $('#mdl-detalhamentolimites abbr[data-toggle="tooltip"]').tooltip();
        $('#mdl-detalhamentolimites #detalhe-dspid').val(dspid);
        $('#mdl-detalhamentolimites #detalhe-requisicao').val('salvarLimitesCategoria');
        $('#mdl-detalhamentolimites').modal();
    }, 'html');
}

function salvarLimitesCategoria()
{
    var dados = $('#mdl-detalhamentolimites .form-listagem').serializeArray();
    $.post(window.location.href, dados, function(response){
        // -- Total
        $('#lista-de-grupos tfoot td:nth-child(3)').empty().html(response.total.vlrmontante)
                .next().empty().html(response.total.vlrlimite)
                .next().empty().html(response.total.vlrdetalhado)
                .next().empty().html(response.total.saldomontante)
                .next().empty().html(response.total.saldolimite);

        // -- Grupos
        $('#lista-de-grupos tr#gdp'+response.grupo.gdpid+' td:nth-child(3)').empty().html(response.grupo.vlrmontante)
                .next().empty().html(response.grupo.vlrlimite)
                .next().empty().html(response.grupo.vlrdetalhado)
                .next().empty().html(response.grupo.saldomontante)
                .next().empty().html(response.grupo.saldolimite);

        // -- Categorias
        $('tr#cat'+response.categoria.dspid+' td:nth-child(3)').empty().html(response.categoria.vlrmontante)
                .next().empty().html(response.categoria.vlrlimite)
                .next().empty().html(response.categoria.vlrdetalhado)
                .next().empty().html(response.categoria.saldomontante)
                .next().empty().html(response.categoria.saldolimite);

        // -- Atualizando a barra de montantes
        var porcentagemValor = response.total.pctmontante;
        var porcentagem = (porcentagemValor > 100)?100:porcentagemValor;
        $('#barra-montante').html('<strong>' + porcentagemValor + '%</strong>')
            .attr('aria-valuenow', porcentagemValor)
            .css('width', porcentagem + '%');
        if (porcentagemValor > porcentagem) {
            $('#barra-montante').removeClass('progress-bar-success').addClass('progress-bar-warning');
        } else {
            $('#barra-montante').removeClass('progress-bar-warning').addClass('progress-bar-success');
        }

        bootbox.alert('Limite atualizado com sucesso.');
    }, 'json');
}

</script>
<button type="button" class="btn btn-danger" id="exportar">Exportar resumo de Grupos XLS</button>
<br style="clear:both" />
<br />