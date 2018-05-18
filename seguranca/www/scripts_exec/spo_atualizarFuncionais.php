<?php

/**
 * Atualiza os dados qualitativos de a��es, localizadores, POs para a base do SIMEC.
 *
 * Assim que termina de baixar os dados qualitativos da API do SIOP, o script roda um processamento
 * que coloca os dados nas tabelas <tt>wssof.ws_acoesdto, wssof.ws_localizadoresdto, wssof.ws_planosorcamentariosdto</tt>. O acompanhamento das p�ginas
 * da execu��o j� baixadas � feito na tabela <tt>monitora.acao e monitora.ptres</tt>.
 * Ao final da execu��o, � enviado um e-mail com o resultado do processo.
 *
 * Sequ�ncia de execu��o:<br />
 * <ol><li>Baixa os dados do webservice (WSQuanlitativo.obterProgramacaoCompleta);</li>
 * <li>Apaga os dados das tabelas wssof.ws_acoesdto, wssof.ws_localizadoresdto, wssof.ws_planosorcamentariosdto;</li>
 * <li>Insere os dados retornados pelo webservice nas tabelas wssof.ws_acoesdto, wssof.ws_localizadoresdto, wssof.ws_planosorcamentariosdto;</li>
 * <li>Executa o script de atualiza��o de funcionais(A��es, localizadores e POs) na seguinte tabela: monitora.acao e monitora.ptres;</li>
 * <li>Envia e-mail com resultado da execu��o.</li></ol>
 *
 * @version $Id: spo_atualizarFuncionais.php
 * @link http://siminc2.cultura.gov.br/seguranca/scripts_exec/spo_atualizarFuncionais.php URL de execu��o.
 */

// -- Setup
ini_set('display_errors', 1);
ini_set('error_reporting', E_ALL & ~E_NOTICE);
set_time_limit(0);
ini_set("memory_limit", "2048M");
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
session_start();

/**
 * Carrega as configura��es gerais do sistema.
 * @see config.inc
 */
require_once BASE_PATH_SIMEC . "/global/config.inc";

/**
 * Carrega as classes do simec.
 * @see classes_simec.inc
 */
require_once APPRAIZ . "includes/classes_simec.inc";

/**
 * Carrega as fun��es b�sicas do simec.
 * @see funcoes.inc
 */
require_once APPRAIZ . "includes/funcoes.inc";

# Verificando IP de origem da requisi��o � autorizado para executar os SCRIPTS.
controlarExecucaoScript();

require_once APPRAIZ. 'includes/classes/Modelo.class.inc';
require_once APPRAIZ. 'wssof/classes/Importador.inc';
require_once APPRAIZ. 'monitora/classes/model/Ptres.inc';

# Abrindo conex�o com o banco de dados
$db = new cls_banco();

$exercicio = date('Y');
$momento = 9000;

$mPtres = new Monitora_Model_Ptres();
$mPtres->importarSiop($exercicio, $momento);

ver('FIM', d);