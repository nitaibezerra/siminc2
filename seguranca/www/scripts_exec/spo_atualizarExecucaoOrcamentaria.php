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
require_once(APPRAIZ . 'spo/ws/sof/Quantitativo.php');

// -- Abrindo conexão com o banco de dados
$db = new cls_banco();

$exercicio = date('Y');
$filtro = array('anoExercicio' => $exercicio);

$ws = new Spo_Ws_Sof_Quantitativo('spo', Spo_Ws_Sof_Quantitativo::PRODUCTION);

$pagina = 0;
$continuar = true;

$sql = "DELETE FROM wssof.ws_execucaoorcamentariadto WHERE anoexercicio = $exercicio";
$db->executar($sql);
do {
    // -- Consultando os dados no WS
    $resultados = $ws->consultarExecucaoOrcamentaria($filtro, null, $pagina, true);

    if ($resultados) {
        include_once  APPRAIZ . 'wssof/classes/Ws_ExecucaoOrcamentariaDto.inc';
        foreach ($resultados as $resultado) {
            $model = new Wssof_Ws_ExecucaoOrcamentariaDto();
            $model->realizarCarga($resultado);
//            $model->commit();
            unset($model);
        }
    }

    $pagina++;
    if (count($resultados) < 2000) {
        $continuar = false;
        break;
    }
} while ($continuar);
$db->commit();

$sql = "DELETE FROM spo.siopexecucao WHERE exercicio = '{$exercicio}'";
$db->executar($sql);

$sql = "
    INSERT INTO spo.siopexecucao(
        exercicio,
        esfcod,
        unicod,
        funcod,
        sfucod,
        prgcod,
        acacod,
        loccod,
        plocod,
        ptres,
        plicod,
        vlrdotacaoinicial,
        vlrdotacaoatual,
        vlrempenhado,
        vlrliquidado,
        vlrpago,
        vlrautorizado
    )
    SELECT
        exo.anoexercicio AS exercicio,
        exo.esfera AS esfcod,
        exo.unidadeorcamentaria AS unicod,
        exo.funcao AS funcod,
        exo.subfuncao AS sfucod,
        exo.programa AS prgcod,
        exo.acao AS acacod,
        exo.localizador AS loccod,
        exo.planoorcamentario AS plocod,
        exo.numeroptres AS ptres,
        exo.planointerno AS plicod,
        CASE WHEN exo.dotacaoinicial <> '' THEN
            exo.dotacaoinicial::NUMERIC
        ELSE
            0
        END AS vlrdotacaoinicial,
        CASE WHEN exo.dotatual <> '' THEN
            exo.dotatual::NUMERIC
        ELSE
            0
        END AS vlrdotacaoatual,
        exo.empliquidado::NUMERIC + exo.empenhadoaliquidar::NUMERIC AS vlrempenhado,
        exo.empliquidado::NUMERIC AS vlrliquidado,
        exo.pago::NUMERIC AS vlrpago,
        exo.autorizado::NUMERIC AS vlrautorizado
    FROM wssof.ws_execucaoorcamentariadto exo
    WHERE
        anoexercicio = '".(int)$exercicio. "'";

$db->executar($sql);
$db->commit();
