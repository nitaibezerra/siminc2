<?php
/**
 * Listagem dos limites orçamentários atribuídos a uma determinada Unidade orçamentária.
 * $Id: listarLimites.inc 83516 2014-07-25 15:13:08Z maykelbraz $
 */

$dadoslimites = consultarLimites($dados, 'grupo');
$depoisDoPrimeiro = false;
$linhaReferencia = $itensDoGrupo = array();
$nomeDoGrupo = null;
?>
<script type="text/javascript" src="/library/simec/js/listagem.js"></script>
<script type="text/javascript" lang="JavaScript">
function toggleProgramaticas(gpmid, mtrid)
{
    var unicod = $('#unicod').val();

    var arowid = '#arow-' + gpmid + '_' + mtrid;
    var $parentTR = $(arowid).parents('tr');

    if ($(arowid + ' span').hasClass('glyphicon-plus')) {
        $.post(
            window.location,
            {requisicao:'consultarAcoes', 'dados[unicod]':unicod, 'dados[gpmid]':gpmid, 'dados[mtrid]':mtrid},
            function(html){
                $(arowid + ' span').removeClass('glyphicon-plus').addClass('glyphicon-minus');
                var numCols = $('td', $parentTR).length;
                $parentTR.after('<tr><td colspan="' + numCols + '">' + html + '</td></tr>');
            }
        );
    } else {
        $parentTR.next().remove();
        $(arowid + ' span').removeClass('glyphicon-minus').addClass('glyphicon-plus');
    }
}

function detalharDespesas(acaid, gpmid, mtrid)
{
    $('#acaid').val(acaid);
    $('#gpmid').val(gpmid);
    $('#mtrid').val(mtrid);
    $('#requisicao').val('detalharDespesas');
    $('#detalhardespesas').submit();
}
</script>
<br />
<?php if ($dadoslimites): ?>
<input type="hidden" name="unicod" id="unicod" value="<?php echo $dados['unicod']; ?>" />
<form name="detalhardespesas" id="detalhardespesas" method="POST" role="form">
    <input type="hidden" name="requisicao" id="requisicao" />
    <input type="hidden" name="dados[gpmid]" id="gpmid" />
    <input type="hidden" name="dados[mtrid]" id="mtrid" />
    <input type="hidden" name="dados[acaid]" id="acaid" />
</form>
<div class="panel-group" id="accordion">
<?php
$list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
$list->setDados($dadoslimites)
    ->setCabecalho(array('Grupo', 'Limites (R$)' => array('Tesouro', 'Próprios', 'Total'), 'Programado (R$)', 'Saldo (R$)'))
    ->addAcao('plus', 'detalharGrupo')
    ->addCallbackDeCampo(array('valortesouro', 'valoroutros', 'valortotal', 'valorprogramado', 'saldo'), 'mascaraNumero')
    ->addCallbackDeCampo('descgrupo', 'alinharEsquerda')
    ->setTotalizador(
        Simec_Listagem::TOTAL_SOMATORIO_COLUNA,
        array('valortesouro', 'valoroutros', 'valortotal', 'valorprogramado', 'saldo')
    );

$list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
</div>
<?php else: ?>
<div class="alert alert-danger col-md-8 col-md-offset-2 text-center">
  <strong><span class="glyphicon glyphicon-warning-sign"></span> Atenção!</strong>
  Os limites da proposta atual
  (<?php echo $_SESSION['exercicio']?>/<?php echo $_SESSION['exercicio'] + 1; ?>) não estão configurados.
</div>
<?php endif;
