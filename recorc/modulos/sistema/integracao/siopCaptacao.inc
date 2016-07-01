<?php
/**
 * Consulta os códigos de captação do SIOP.
 *
 * @version $Id$
 */

/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include_once APPRAIZ . "includes/cabecalho.inc";


$ws = new Spo_Ws_Sof_Receita('spo', Spo_Ws_Sof_Receita::DEVELOPMENT);
$obj = new ConsultarDisponibilidadeCaptacaoBaseExterna();
ver($ws->consultarDisponibilidadeCaptacaoBaseExterna($obj));
