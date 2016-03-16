function editarDemanda(dmdid)
{
    window.location = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid=' + dmdid;
}

function filtrarSituacao(esdid)
{
    window.location = 'demandasfies.php?modulo=principal/demandas&acao=A&action=pesquisar&esdid=' + esdid;
}

function exibirHistorico( dmdid, docid )
{
    var url = '/geral/workflow/historico.php' + '?modulo=principal/tramitacao' + '&acao=C' + '&docid=' + docid;
    window.open(url, 'alterarEstado', 'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no');
}

function detalharObservacao(dmoid) {
    var url = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&action=modal_observacao&dmoid=' + dmoid;
    $('#conteudo_modal_observacao').load(url, function(){
        $('#modal-observacao').modal();
    });
}

$(function(){
    $('.chosen').chosen({no_results_text: "Nenhum registro encontrado: "});

    $('.cnpj').mask('99.999.999/9999-99');
    $('.moeda').mask('000.000.000.000.000,00', {reverse: true});
    $('.data').mask('99/99/9999');
    $('.data').datepicker();
    $('.telefone').mask('(99) 9999-9999');
});