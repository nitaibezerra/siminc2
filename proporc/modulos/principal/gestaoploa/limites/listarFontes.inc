<?php
/**
 * Listagem das fontes utilizadas na proposta orçamentária.
 * $Id: listarFontes.inc 83516 2014-07-25 15:13:08Z maykelbraz $
 */

if ($dadosfontes = consultarFontes($dados, $_SESSION['exercicio'])): ?>
<div class="panel-group" id="accordion2">
<?php
$list = new Simec_Listagem();
$list->setCabecalho(array('Código', 'Fonte', 'Limite (a)', 'Despesas Programadas (b)', 'Saldo a programar (a - b)'))
    ->setDados($dadosfontes)
    ->trocaTipoSaida(Simec_Listagem::RETORNO_BUFFERIZADO)
    ->addCallbackDeCampo(array('vllimite', 'vldespesa', 'vlsaldo'), 'mascaraNumero')
    ->setTotalizador(
        Simec_Listagem::TOTAL_SOMATORIO_COLUNA,
        array('vllimite', 'vldespesa', 'vlsaldo')
    );
$conteudo = $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

montaItemAccordion(
    '<span class="glyphicon glyphicon-info-sign"></span>Detalhamento das fontes (R$)',
    'fontdet',
    $conteudo,
    array('accordionID' => 'accordion2')
);
?>
</div>
<?php else: ?>

<?php endif; ?>
