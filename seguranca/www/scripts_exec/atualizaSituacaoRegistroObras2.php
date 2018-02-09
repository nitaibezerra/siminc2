<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';


$db = new cls_banco();


include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

/**
 * Script para atualizar o a situação do registro de todas as obras
 */

$sql = "
            UPDATE obras2.obras SET strid = foo.strid
            FROM (

                SELECT
                    o.obrid,
                    e.esdid,
                    CASE WHEN e.esdid IN (689, 771, 884, 870, 861, 862, 875, 1230, 864, 872, 873, 874) THEN 1 -- Planejamento
                    WHEN e.esdid IN (763) THEN 2 -- Licitação
                    WHEN e.esdid IN (764, 863, 871) THEN 3 -- Contratação
                    WHEN e.esdid IN (690) THEN 4 -- Execução
                    WHEN e.esdid IN (691) THEN 5 -- Paralisada
                    WHEN e.esdid IN (693) THEN 6 -- Concluída
                    WHEN e.esdid IN (1084) THEN 7 -- Inacabada
                    WHEN e.esdid IN (769) THEN 8 -- Obra Cancelada
                    WHEN e.esdid IN (768) THEN 9 -- Em Reformulação
                    ELSE NULL END AS strid
                FROM obras2.obras o
                JOIN workflow.documento d ON d.docid = o.docid
                JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                WHERE o.obrstatus = 'A' AND o.obridpai IS NULL
            ) as foo
            WHERE obras2.obras.obrid = foo.obrid;";

$db->executar($sql);
$db->commit();
$db->close();

?>

EXECUTADO!