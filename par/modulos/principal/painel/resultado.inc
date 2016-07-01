<?php

require (APPRAIZ . 'includes/library/simec/Listagem.php');
require (APPRAIZ . 'par/modulos/principal/painel/classes/controle/PainelControle.class.inc');

$obPainelControle = new PainelControle();
$strQuery = $obPainelControle->gerenciarPainel($_SESSION['sessaoPainel'], $_POST);

$listagem = new Simec_Listagem();
$listagem->setCabecalho(array('Id Obra', 'Preid', 'Nome da Obra', 'Tipologia', 'UF', 'Município', 'Processo', 'Valor da Obra', 'Valor do empenho', 'valor Pago', 'Demandado'))
        ->setQuery($strQuery, 300)
        ->setTitulo('Lista de Obras')
        ->AddAcao('empenho', array('func' => 'openPopUp', 'extra-params' => array('proid'), 'external-params' => array('tipo' => 'solicitacaoEmpenho')))
        ->AddAcao('pagamento', array('func' => 'openPopUp', 'extra-params' => array('proid'), 'external-params' => array('tipo' => 'solicitacaoPagamento')))
        ->esconderColunas('proid')
        ->setTamanhoPagina(100)
        ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
        ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
exit();