<?php
/**
 * Arquivo principal de criaчуo de um novo pedido de recurso financeiro.
 * $Id: novasolicitacao.inc 91268 2014-12-01 13:16:14Z maykelbraz $
 */

/**
 * Funчѕes de apoio рs solicitaчѕes recurso.
 * @see _funcoessolicitacoesrecursos.php
 */
require(APPRAIZ . 'www/progfin/_funcoessolicitacoesrecursos.php');

/**
 * Helper de exibiчуo de alertas entre requisiчѕes.
 * @see Simec_Helper_FlashMessage
 */
require APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
$fm = new Simec_Helper_FlashMessage('progfin/novasolicitacao');