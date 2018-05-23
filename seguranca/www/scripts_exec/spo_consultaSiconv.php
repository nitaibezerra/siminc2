<?php
/**
 * Carrega os dados financeiros do SIOP para a base do SIMEC.
 *
 * Assim que termina de baixar os dados financeiros, o script roda um processamento
 * que coloca os dados na tabela <tt>spo.siopexecucao</tt>. O acompanhamento das páginas
 * da execução já baixadas é feito na tabela <tt>spo.siopexecucao_acompanhamento</tt>.
 * Ao final da execução, é enviado um e-mail com o resultado do processo.
 *
 * Sequência de execução:<br />
 * <ol><li>Baixa os dados do webservice (WSQuantitativo.consultarExecucaoOrcamentaria);</li>
 * <li>Apaga os dados da tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Insere os dados retornados pelo webservice na tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Executa o script de atualização de finaceiros na seguinte tabela: spo.siopexecucao;</li>
 * <li>Envia e-mail com resultado da execução.</li></ol>
 *
 * @version $Id: spo_BaixarDadosFinanceirosSIOP.php 101880 2015-08-31 19:50:33Z maykelbraz $
 * @link http://simec.mec.gov.br/seguranca/scripts_exec/spo_BaixarDadosFinanceirosSIOP.php URL de execução.
 */

// -- Setup
ini_set('display_errors', 1);
ini_set('error_reporting', E_ALL & ~E_NOTICE);
set_time_limit(0);
ini_set("memory_limit", "2048M");
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
session_start();
$_SESSION['baselogin'] = "simec_espelho_producao";

// -- Includes necessários ao processamento
/**
 * Carrega as configurações gerais do sistema.
 * @see config.inc
 */
require_once BASE_PATH_SIMEC . "/global/config.inc";

/**
 * Carrega as classes do simec.
 * @see classes_simec.inc
 */
require_once APPRAIZ . "includes/classes_simec.inc";

/**
 * Carrega as funções básicas do simec.
 * @see funcoes.inc
 */
require_once APPRAIZ . "includes/funcoes.inc";

# Verificando IP de origem da requisição é autorizado para executar os SCRIPTS.
controlarExecucaoScript();

/**
 * Classe de conexão com o SIOP, serviço WSQuantitativo.
 * @see Spo_Ws_Sof_Quantitativo
 */
//require_once(APPRAIZ . 'spo/ws/sof/Quantitativo.php');

// -- Abrindo conexão com o banco de dados
$db = new cls_banco();

include_once APPRAIZ. 'emendas/classes/model/Siconv.inc';
include_once APPRAIZ. 'emendas/classes/model/Emenda.inc';
include_once APPRAIZ. 'emendas/classes/model/Programa.inc';
include_once APPRAIZ. 'emendas/classes/model/Beneficiario.inc';
include_once APPRAIZ. 'emendas/classes/model/SiconvParecer.inc';
include_once APPRAIZ. 'emendas/classes/model/SiconvPrograma.inc';
include_once APPRAIZ. 'emendas/classes/model/SiconvSituacao.inc';
include_once APPRAIZ. 'emendas/classes/model/SiconvBeneficiario.inc';
include_once APPRAIZ. 'siconv/classes/model/PropostaWs.inc';
include_once APPRAIZ. 'emendas/classes/service/WSIntegracaoSiconv.class.inc';

$exercicio = date('Y');

$sql = "select distinct eme.emeid 
        from emendas.emenda eme 
        where eme.prsano = '{$exercicio}'
        and (
          emeatualizacaosiconv is null 
          or to_char(emeatualizacaosiconv, 'YYYYMMDD') < to_char(now(), 'YYYYMMDD')
        )
        ";

$emendas = $db->carregar($sql);
$emendas = $emendas ? $emendas : [];

foreach($emendas as $emenda){

    $mEmenda = new Emendas_Model_Emenda($emenda['emeid']);
    $mEmenda->atualizarSiconv();

    $timestampFim = time();
    $duracao = $timestampFim - $timestampInicio;

    sleep(5);
}