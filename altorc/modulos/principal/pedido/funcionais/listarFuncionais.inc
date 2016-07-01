<?php
if ( empty($dadospao['mcrid']) ) {
        die("<script>
                alert('Faltam parâmetros para acessar esta tela!');
                location.href = '?modulo=inicio&acao=C';
             </script>");
}
/**
 * Listagem e manipulação de funcionais.
 * Permite o usuário mover os itens de uma lista para a outra.
 * $Id: listarFuncionais.inc 102362 2015-09-11 19:01:00Z maykelbraz $
 */

$listagem = new Simec_Listagem();
$listagem->setCabecalho(array('Programática','Descrição', 'Status do Localizador'));
$listagem->addAcao('edit', array('func' => 'editarLocalizador', 'extra-params' => array('locstatus')));

if ($podeSalvar && $dadospao['mcrid'] == $mcridAtual && $mcrtipocancelamento !== 't') {
    $listagem->addAcao('clear', array('func' => 'limparAlteracoesLocalizador', 'extra-params' => array('locstatus')));
}
$listagem->setAcaoComoCondicional(
    'clear', array(array('campo' => 'locstatus', 'valor' => '-1', 'op' => 'diferente'))
);

// -- Filtros do formulário
$params = array($dadospao['paoid'], $_SESSION['exercicio'], $dadospao['unicod']); // -- parametros fixos da query
$whereFiltros = '';
if (isset($_REQUEST['dados'])) {
    $whereFiltros = array();
    if (chaveTemValor($_REQUEST['dados'], 'acacod')) {
        $whereFiltros[] = "sna.acacod = '%s'";
        $params[] = $_REQUEST['dados']['acacod'];
    }
    if (chaveTemValor($_REQUEST['dados'], 'prgcod')) {
        $whereFiltros[] = "sna.prgcod = '%d'";
        $params[] = $_REQUEST['dados']['prgcod'];
    }
}
if ($dadospao['mcrid'] != $mcridAtual) {
    $whereFiltros[] = "paf.paoid IS NOT NULL";
}
if (!empty($whereFiltros)) {
    $whereFiltros = 'AND ' . implode(' AND ', $whereFiltros);
} else {
    $whereFiltros = '';
}

$sql = <<<DML
SELECT sna.snaid AS id,
       esfcod || '.' || unicod || '.' || funcod || '.' || sfucod || '.' || prgcod || '.' || acacod || '.' || loccod AS programatica,
       acadsc || ' - ' || COALESCE(locdsc, '???') || ' - ' || tic.ticdsc as locdsc,
       COALESCE(paf.paoid, -1) AS locstatus
  FROM altorc.snapshotacao sna
    INNER JOIN altorc.tipoinclusao tic ON tic.ticid = tilcod::INTEGER
    LEFT JOIN altorc.pedidoalteracaofisico paf ON (sna.snaid = paf.snaid AND paf.paoid = %d)
  WHERE sna.snaexercicio = '%d'
    AND sna.unicod = '%s'
    AND sna.mcrid =  {$dadospao['mcrid']}
    {$whereFiltros}
  ORDER BY locstatus DESC
DML;
$sql = vsprintf($sql, $params);
$listagem->setQuery($sql)
    ->turnOnPesquisator();
// -- Localizadores para alteração
$listagem->addCallbackDeCampo('locstatus', 'statusLocalizador');
$listagem->addCallbackDeCampo(array('locdsc'), 'alinhaParaEsquerda');

?>
<script type="text/javascript" language="javascript">
function editarLocalizador(snaid, paoid)
{
    window.location = 'altorc.php?modulo=principal/pedido/inicio&acao=A' + '&aba=detalhamento&dados[snaid]=' + snaid;
}
</script>
<script type="text/javascript">
    jQuery(document).ready(function() {
        // -- Inicializando a modal de confirmacao
        $('#modal-confirm .btn-primary').click(function() {
            $('#limparFuncional').submit();
        });

    });
    function limparAlteracoesLocalizador(snaid, paoid)
    {
        $('#limpar_snaid').val(snaid);
        $('#limpar_paoid').val(paoid);
        $('#modal-confirm .modal-title').html('Limpar localizador');
        $('#modal-confirm .modal-body p').html('Tem certeza que deseja limpar <strong>TODAS as alterações</strong> do localizador selecionado?');
        $('#modal-confirm').modal();
    }
</script>
<form name="limparFuncional" id="limparFuncional" method="POST">
    <input type="hidden" name="requisicao" value="limparFuncional" />
    <input type="hidden" name="dados[snaid]" id="limpar_snaid" />
    <input type="hidden" name="dados[paoid]" id="limpar_paoid" />
</form>
<div class="listaFuncionais">
    <div id="conteudo_list">
        <div class="page-header">Localizadores</div>
        <?php $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM); ?>
    </div>
</div>
<br />
<br />