/**
 * Preenche o id do item e envia o formulário para carregar
 */
function carregarItem(id, item)
{
    jQuery('#' + item).val(id);
    jQuery('#action').val('carregar');
    jQuery('#formPrincipal').submit();
}

function gravarSiop(vieid)
{
    window.location.href = 'recorc.php?modulo=principal/captacao/listar&acao=A&execucao=siop&vieid='+vieid;
}

function detalharRespostaSiop(capid)
{
    $.get('recorc.php?modulo=principal/captacao/listar&acao=A',{execucao:'respostaSiop',capid:capid},function(data){
        $('#modal-alert .modal-body').html(data);
        $('#modal-alert').modal();
    });
}
