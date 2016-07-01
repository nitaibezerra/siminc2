<?php
/**
 * $Id: enviarPedidos.inc 99569 2015-07-01 14:25:24Z wesleysilva $
 */

/**
 * Classe de acesso ao WS alterações orçamentárias.
 * @see WSAlteracoesOrcamentarias
 */
require(APPRAIZ . 'altorc/classes/WSAlteracoesOrcamentarias.php');

/**
 * Cabeçalho simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";


$ws = new WSAlteracoesOrcamentarias();
ver($ws->obterEmendasAprovadas());



//$pedidoAlteracao = new PedidoAlteracaoDTO();
//$pedidoAlteracao->exercicio = 2013;
//$pedidoAlteracao->codigoMomento = 3000;
//$pedidoAlteracao->codigoClassificacaoAlteracao = 2;
//$pedidoAlteracao->codigoTipoAlteracao = 2;
//$pedidoAlteracao->snOrcamentoInvestimento = false;
//$pedidoAlteracao->descricao = 'hiha!!';
//$pedidoAlteracao->codigoOrgao = 26101;
//$pedidoAlteracao->fisicosPedidoAlteracao = array();
//$pedidoAlteracao->fisicosPedidoAlteracao[0] = new FisicoPedidoAlteracaoDTO();
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->exercicio = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->codigoEsfera = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->codigoUO = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->codigoFuncao = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->codigoSubFuncao = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->codigoPrograma = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->codigoAcao = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->codigoLocalizador = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->codigoTipoInclusaoLocalizador = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->quantidadeAcrescimo = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->quantidadeReducao = 2013;
//$pedidoAlteracao->fisicosPedidoAlteracao[0]->listaFinanceiroPedidoAlteracaoDTO = array();
//
//
////$pedidoAlteracao->listaFinanceiroPedidoAlteracaoDTO[0]
//
//
//$pedidoAlteracao->respostasJustificativa = array();
//
//$ws = new WSAlteracoesOrcamentarias(WSAlteracoesOrcamentarias::DEVELOPMENT);
//$ws = new WSAlteracoesOrcamentarias(WSAlteracoesOrcamentarias::DEVELOPMENT);
//$ws = new WSAlteracoesOrcamentarias(WSAlteracoesOrcamentarias::DEVELOPMENT);
//$ws = new WSAlteracoesOrcamentarias(WSAlteracoesOrcamentarias::DEVELOPMENT);
//$ws = new WSAlteracoesOrcamentarias(WSAlteracoesOrcamentarias::DEVELOPMENT);
//$ws = new WSAlteracoesOrcamentarias(WSAlteracoesOrcamentarias::DEVELOPMENT);
//$ws = new WSAlteracoesOrcamentarias(WSAlteracoesOrcamentarias::DEVELOPMENT);
//$ws = new WSAlteracoesOrcamentarias(WSAlteracoesOrcamentarias::DEVELOPMENT);
//$ws->connect();
//dbg($ws->cadastrarPedidoAlteracao($pedidoAlteracao));


//$ws = new WSAlteracoesOrcamentarias(WSAlteracoesOrcamentarias::DEVELOPMENT);
//$ws->connect();

//ver($ws->verificarPedidoAlteracao(32150, 2014));



//$dados = $ws->obterTabelasApoioAlteracoesOrcamentarias(2014);
//ver($dados);


//
//$dados = $dados->return->tiposAlteracaoDTO;
//
//
//foreach ($dados as $dado) {
//    $dml = <<<DML
//INSERT INTO tipoalteracao(tipbaselegal, clasalteracaocod, tipalteracaocod, tipdsc, tipano)
//  VALUES('%s', %d, '%s', '%s', '%s');
//DML;
//
//    print(sprintf($dml, $dado->baseLegal, $dado->codigoClassificacaoAlteracao, $dado->codigoTipoAlteracao, str_replace("'", "''", $dado->descricao), $dado->exercicio));
//    print("<br />");
//}


//ver($dados);
//
//
//
//
//
//
//
//ver();




