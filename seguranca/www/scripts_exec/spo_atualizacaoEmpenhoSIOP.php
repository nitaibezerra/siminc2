<?php
/**
 * Carrega os dados financeiros do SIOP para a base do SIMEC e, na sequencia, <br/>
 * distribui esses dados nas tabelas financeiras.<br />
 * Sequência de execução:<br />
 * <ol><li>Baixa os dados do webservice;</li>
 * <li>Apaga os dados da tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Insere os dados retornados pelo webservice na tabela wssof.ws_execucaoorcamentaria;</li>
 * <li>Executa o script de atualização de finaceiros na seguinte tabela: spo.siopexecucao;</li>
 * <li>Envia e-mail com resultado da execução.</li></ol>
 *
 * Veja o link abaixo para URL de execução do script. Agendamento:
 *
 * @version $Id: spo_atualizacaoEmpenhoSIOP.php 85960 2014-09-02 12:47:19Z maykelbraz $
 * @link http://simec/seguranca/scripts_exec/spo_atualizacaoEmpenhoSIOP.php
 */

// -- Modificando o tempo de execução do script
set_time_limit(0);

// -- Modificando o limite de memória para execução do script
ini_set("memory_limit", "2048M");

/**
 * PATH do sistema.
 */
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

//$_REQUEST['baselogin'] = 'simec_espelho_producao';

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

/**
 * Classe de conexão com o SIOP, serviço WSQuantitativo.
 * @see Spo_Ws_Quantitativo
 */
require_once(APPRAIZ . 'spo/ws/Quantitativo.php');

// -- CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

// -- Ano utilizado pelo script para execução
$anoDeExecucao = date('Y');

// -- Filtros de consulta do webservice
$filtro = array(
    'anoReferencia' => $anoDeExecucao,
    'anoExercicio' => $anoDeExecucao,
);

// -- Selecionando os campos de retorno do webservice
$retorno = array(
    'anoExercicio',
    'programa',
    'acao',
    'localizador',
    'unidadeOrcamentaria',
    'planoOrcamentario',
    'planoInterno',
    'numeroptres',
    'esfera',
    'funcao',
    'subFuncao',
    'dotacaoInicial',
    'dotAtual',
    'empLiquidado',
    'empenhadoALiquidar',
    'pago'
);

// -- Consultando os dados no WS
$ws = new Spo_Ws_Quantitativo('');
if (!$wsdata = $ws->consultarExecucaoOrcamentaria($filtro, $retorno, null, true)) {
    // -- Enviar e-mail
    die();
}

// -- Apagando os registros armazenados em wssof.ws_execucaofinanceira
$dml = <<<DML
DELETE FROM wssof.ws_execucaoorcamentariadto
DML;

$db = new cls_banco();
$db->executar($dml);

// -- Processando os dados retornados pelo WS
$campos = implode(', ', $retorno);
$coringas = implode(',', array_fill(0, count($retorno), "'%s'"));
$dml = <<<DML
INSERT INTO wssof.ws_execucaoorcamentariadto({$campos})
  VALUES({$coringas})
DML;

foreach ($wsdata as $data) {
    $stmt = vsprintf($dml, $data);
    $db->executar($stmt);
}

// -- Atualizando os financeiros em: spo.siopexecucao
$dml = <<<DML
DELETE
  FROM spo.siopexecucao
  WHERE exercicio = '{$anoDeExecucao}';

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
   vlrpago
)
SELECT DISTINCT exo.anoexercicio AS exercicio,
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
                CASE WHEN exo.dotacaoinicial <> '' THEN exo.dotacaoinicial::NUMERIC
                     ELSE 0
                  END AS vlrdotacaoinicial ,
                CASE WHEN exo.dotatual <> '' THEN exo.dotatual::NUMERIC
                     ELSE 0
                  END AS vlrdotacaoatual ,
                exo.empliquidado::NUMERIC + exo.empenhadoaliquidar::NUMERIC AS vlrempenhado,
                exo.empliquidado::NUMERIC AS vlrliquidado,
                exo.pago::NUMERIC AS vlrpago
  FROM wssof.ws_execucaoorcamentariadto exo
  WHERE anoexercicio = '{$anoDeExecucao}';
DML;

$db->executar($dml);
$resultadoExecucao = 'SUCESSO';
if (!$db->commit()) {
    $resultadoExecucao = 'FALHA';
}

enviar_email(
    '',
    array(
        array('usuemail' => $_SESSION['email_sistema'], 'usunome' => SIGLA_SISTEMA)
    ),
    'Carga SIOP - ' . date('d/m/Y') . ' - ' . $resultadoExecucao,
    "Execução da carga do SIAFI.\nResultado: {$resultadoExecucao}\n"
    . "Servidor: {$_SERVER['SERVER_NAME']}\n"
    . "Arquivo: " . __FILE__
);
