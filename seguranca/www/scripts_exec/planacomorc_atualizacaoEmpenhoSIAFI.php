<?php
/**
 * Atualiza os empenhos armazenados no esquema SIAFI.
 * O script está dividido em:
 * 1) Atualização do empenho das subações;
 * 2) Atualização do empenho dos PTRESs;
 * 3) Atualização do empenho dos PIs;
 * 4) Atualização do empenho PI/PTRES (combinado).
 *
 * Execução:
 * http://simec/seguranca/scripts_exec/planacomorc_atualizacaoEmpenhoSIAFI.php
 * Agendamento:
 * Todos os dias às 11h AM.
 * ---
 * $Id: planacomorc_atualizacaoEmpenhoSIAFI.php 81580 2014-06-13 14:08:58Z werteralmeida $
 */

set_time_limit(0);

/**
 * PATH do sistema.
 */
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = 'simec_espelho_producao';

/**
 * Carrega as configurações gerais do sistema.
 */
require_once BASE_PATH_SIMEC . "/global/config.inc";
/**
 * Carrega as classes do simec.
 */
require_once APPRAIZ . "includes/classes_simec.inc";
/**
 * Carrega as funções básicas do simec.
 */
require_once APPRAIZ . "includes/funcoes.inc";

// -- CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

$sql = <<<SQL
-----------------------------------------------------------------
/* SUBAÇÃO */
-----------------------------------------------------------------
DELETE  FROM siafi.sbaempenho;
  -- 2013
  INSERT INTO siafi.sbaempenho (
  SELECT * FROM   dblink
    (
    'dbname= hostaddr= user= password= port='
    ,
    '
                 SELECT
                    SUBSTR(sld.plicod, 2, 4) as sbacod,
                    ''2013'' AS exercicio,
                    SUM(
                        CASE
                            WHEN sld.sldcontacontabil IN (''292130100'',
                                                          ''292130201'',
                                                          ''292130202'',
                                                          ''292130203'',
                                                          ''292130301'')
                            THEN
                                CASE
                                    WHEN sld.ungcod=''154004''
                                    THEN (sld.sldvalor)*2.2088
                                    ELSE (sld.sldvalor)
                                END
                            ELSE 0
                        END ) AS total
                FROM
                    dw.saldo2013 sld
                WHERE
                    sld.sldcontacontabil IN (''292130100'',
                                             ''292130201'',
                                             ''292130202'',
                                             ''292130203'',
                                             ''292130301'')
                AND plicod IS NOT NULL
                AND plicod <> ''''
                AND LENGTH(sld.plicod) = 11
                AND sld.unicod IN (''26101'', ''26298'', ''26291'', ''26443'', ''26290'', ''74902'')
                GROUP BY
                    SUBSTR(sld.plicod, 2, 4);
    ')
    AS sba
    (
        sbacod VARCHAR(4),
        exercicio VARCHAR(4),
        total NUMERIC(15,2)
    )
    );
    -- 2014
    INSERT INTO siafi.sbaempenho (
    SELECT * FROM   dblink
    (
    'dbname= hostaddr= user= password= port='
    ,
    '
                 SELECT
                    SUBSTR(sld.plicod, 2, 4) as sbacod,
                    ''2014'' AS exercicio,
                    SUM(
                        CASE
                            WHEN sld.sldcontacontabil IN (''292130100'',
                                                          ''292130201'',
                                                          ''292130202'',
                                                          ''292130203'',
                                                          ''292130301'')
                            THEN
                                CASE
                                    WHEN sld.ungcod=''154004''
                                    THEN (sld.sldvalor)*2.2088
                                    ELSE (sld.sldvalor)
                                END
                            ELSE 0
                        END ) AS total
                FROM
                    dw.saldo2014 sld
                WHERE
                    sld.sldcontacontabil IN (''292130100'',
                                             ''292130201'',
                                             ''292130202'',
                                             ''292130203'',
                                             ''292130301'')
                AND plicod IS NOT NULL
                AND plicod <> ''''
                AND LENGTH(sld.plicod) = 11
                AND sld.unicod IN (''26101'', ''26298'', ''26291'', ''26443'', ''26290'', ''74902'')
                GROUP BY
                    SUBSTR(sld.plicod, 2, 4);
    ')
    AS sba
    (
        sbacod VARCHAR(4),
        exercicio VARCHAR(4),
        total NUMERIC(15,2)
    )
    );
-----------------------------------------------------------------
/* PTRES */
-----------------------------------------------------------------
DELETE  FROM siafi.ptrempenho;
  INSERT INTO siafi.ptrempenho (
  SELECT * FROM   dblink
    (
    'dbname= hostaddr= user= password= port='
    ,
'SELECT
    ptres,
    ''2013'' AS exercicio,
    SUM(
        CASE
            WHEN sld.sldcontacontabil IN (''292130100'',
                                          ''292130201'',
                                          ''292130202'',
                                          ''292130203'',
                                          ''292130301'')
            THEN
                CASE
                    WHEN sld.ungcod=''154004''
                    THEN (sld.sldvalor)*2.2088
                    ELSE (sld.sldvalor)
                END
            ELSE 0
        END ) AS total
FROM
    dw.saldo2013 sld
INNER JOIN
    financeiro.subacao sac
ON
    sac.sbastatus = ''A''
AND sac.sbacod = SUBSTR(sld.plicod, 2, 4)
WHERE
    sld.sldcontacontabil IN (''292130100'',
                             ''292130201'',
                             ''292130202'',
                             ''292130203'',
                             ''292130301'')
AND ptres IS NOT NULL
AND ptres <> ''''
AND LENGTH(sld.plicod) = 11
AND sld.unicod IN (''26101'', ''26298'', ''26291'', ''26443'', ''26290'', ''74902'')
GROUP BY
    ptres')
    AS ptr
    (
        ptres VARCHAR(11),
        exercicio VARCHAR(4),
        total NUMERIC(15,2)
    )
    );

    -- 2014
    INSERT INTO siafi.ptrempenho (
  SELECT * FROM   dblink
    (
    'dbname= hostaddr= user= password= port='
    ,
'SELECT
    ptres,
    ''2014'' AS exercicio,
    SUM(
        CASE
            WHEN sld.sldcontacontabil IN (''292130100'',
                                          ''292130201'',
                                          ''292130202'',
                                          ''292130203'',
                                          ''292130301'')
            THEN
                CASE
                    WHEN sld.ungcod=''154004''
                    THEN (sld.sldvalor)*2.2088
                    ELSE (sld.sldvalor)
                END
            ELSE 0
        END ) AS total
FROM
    dw.saldo2014 sld
INNER JOIN
    financeiro.subacao sac
ON
   sac.sbastatus = ''A''
AND sac.sbacod = SUBSTR(sld.plicod, 2, 4)
WHERE
    sld.sldcontacontabil IN (''292130100'',
                             ''292130201'',
                             ''292130202'',
                             ''292130203'',
                             ''292130301'')
AND ptres IS NOT NULL
AND ptres <> ''''
AND LENGTH(sld.plicod) = 11
AND sld.unicod IN (''26101'', ''26298'', ''26291'', ''26443'', ''26290'', ''74902'')
GROUP BY
    ptres')
    AS ptr
    (
        ptres VARCHAR(11),
        exercicio VARCHAR(4),
        total NUMERIC(15,2)
    )
    );
-----------------------------------------------------------------
/* PI */
-----------------------------------------------------------------
DELETE  FROM siafi.pliempenho;
INSERT INTO siafi.pliempenho (
SELECT * FROM   dblink
    (
    'dbname= hostaddr= user= password= port='
    ,'
SELECT
    plicod,
    unicod,
    ''2013'' AS exercicio,
    SUM(
        CASE
            WHEN sld.sldcontacontabil IN (''292130100'',
                                          ''292130201'',
                                          ''292130202'',
                                          ''292130203'',
                                          ''292130301'')
            THEN
                CASE
                    WHEN sld.ungcod=''154004''
                    THEN (sld.sldvalor)*2.2088
                    ELSE (sld.sldvalor)
                END
            ELSE 0
        END ) AS total
FROM
    dw.saldo2013 sld
WHERE
    sld.sldcontacontabil IN (''292130100'',
                             ''292130201'',
                             ''292130202'',
                             ''292130203'',
                             ''292130301'')
AND plicod IS NOT NULL
AND plicod <> ''''
AND unicod IS NOT NULL
AND unicod <> ''''
AND LENGTH(sld.plicod) = 11
GROUP BY
    unicod,
    plicod')
    AS pli
    (
        plicod VARCHAR(11),
        unicod VARCHAR(5),
        exercicio VARCHAR(4),
        total NUMERIC(15,2)
    )
    );
    -- 2014
INSERT INTO siafi.pliempenho (
SELECT * FROM   dblink
    (
    'dbname= hostaddr= user= password= port='
    ,'
SELECT
    plicod,
    unicod,
    ''2014'' AS exercicio,
    SUM(
        CASE
            WHEN sld.sldcontacontabil IN (''292130100'',
                                          ''292130201'',
                                          ''292130202'',
                                          ''292130203'',
                                          ''292130301'')
            THEN
                CASE
                    WHEN sld.ungcod=''154004''
                    THEN (sld.sldvalor)*2.2088
                    ELSE (sld.sldvalor)
                END
            ELSE 0
        END ) AS total
FROM
    dw.saldo2014 sld
WHERE
    sld.sldcontacontabil IN (''292130100'',
                             ''292130201'',
                             ''292130202'',
                             ''292130203'',
                             ''292130301'')
AND plicod IS NOT NULL
AND plicod <> ''''
AND unicod IS NOT NULL
AND unicod <> ''''
AND LENGTH(sld.plicod) = 11
GROUP BY
    unicod,
    plicod')
    AS pli
    (
        plicod VARCHAR(11),
        unicod VARCHAR(5),
        exercicio VARCHAR(4),
        total NUMERIC(15,2)
    )
    );
-----------------------------------------------------------------
/* PI - PTRES*/
-----------------------------------------------------------------
DELETE  FROM siafi.pliptrempenho;
INSERT INTO siafi.pliptrempenho (
SELECT * FROM   dblink
    (
    'dbname= hostaddr= user= password= port='
    ,'
SELECT
    plicod,
    ptres,
    ''2013'' AS exercicio,
    SUM(
        CASE
            WHEN sld.sldcontacontabil IN (''292130100'',
                                          ''292130201'',
                                          ''292130202'',
                                          ''292130203'',
                                          ''292130301'')
            THEN
                CASE
                    WHEN sld.ungcod=''154004''
                    THEN (sld.sldvalor)*2.2088
                    ELSE (sld.sldvalor)
                END
            ELSE 0
        END ) AS total
FROM
    dw.saldo2013 sld
WHERE
    sld.sldcontacontabil IN (''292130100'',
                             ''292130201'',
                             ''292130202'',
                             ''292130203'',
                             ''292130301'')
AND plicod IS NOT NULL
AND plicod <> ''''
AND LENGTH(sld.plicod) = 11
AND sld.unicod IN (''26101'', ''26298'', ''26291'', ''26443'', ''26290'', ''74902'')
GROUP BY
    plicod,
    ptres')
    AS pli
    (
        plicod VARCHAR(11),
        ptres VARCHAR(11),
        exercicio VARCHAR(4),
        total NUMERIC(15,2)
    )
    );
    -- 2014
    INSERT INTO siafi.pliptrempenho (
SELECT * FROM   dblink
    (
    'dbname= hostaddr= user= password= port='
    ,'
SELECT
    plicod,
    ptres,
    ''2014'' AS exercicio,
    SUM(
        CASE
            WHEN sld.sldcontacontabil IN (''292130100'',
                                          ''292130201'',
                                          ''292130202'',
                                          ''292130203'',
                                          ''292130301'')
            THEN
                CASE
                    WHEN sld.ungcod=''154004''
                    THEN (sld.sldvalor)*2.2088
                    ELSE (sld.sldvalor)
                END
            ELSE 0
        END ) AS total
FROM
    dw.saldo2014 sld
WHERE
    sld.sldcontacontabil IN (''292130100'',
                             ''292130201'',
                             ''292130202'',
                             ''292130203'',
                             ''292130301'')
AND plicod IS NOT NULL
AND plicod <> ''''
AND LENGTH(sld.plicod) = 11
AND sld.unicod IN (''26101'', ''26298'', ''26291'', ''26443'', ''26290'', ''74902'')
GROUP BY
    plicod,
    ptres')
    AS pli
    (
        plicod VARCHAR(11),
        ptres VARCHAR(11),
        exercicio VARCHAR(4),
        total NUMERIC(15,2)
    )
    );
SQL;

$db = new cls_banco();
$db->executar($sql);
$resultadoExecucao = 'SUCESSO';
if (!$db->commit()) {
    $resultadoExecucao = 'FALHA';
}

enviar_email(
    '',
    array($_SESSION['email_sistema']),
    'Carga SIAFI - ' . date('d/m/Y') . ' - ' . $resultadoExecucao,
    "Execução da carga do SIAFI. Resultado: {$resultadoExecucao}\n" . __FILE__
);
