<?php
/**
 * Listagem dos dados do arquivo de previsão.
 * $Id: previsaolistagem.inc 102347 2015-09-11 14:15:27Z maykelbraz $
 */
$retornoListagem = $db->carregar($sqlDadosBanco);
$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
$listagem->setCabecalho(array('Exercicio', 'Unidade Orçamentária', 'Fonte', 'Natureza', 'Mês', 'Valor'));
$listagem->setDados($retornoListagem);
$listagem->addCampo(array('name' => 'dados[prfid]', 'id' => 'dados_prfid', 'type' => 'hidden'))
    ->addCampo(array('name' => 'action', 'id' => 'listagem_action', 'type' => 'hidden'));
$listagem->mostrarImportar();
?>
<div class="col-md-12" style="clear:both"><?php $listagem->render(); ?></div>