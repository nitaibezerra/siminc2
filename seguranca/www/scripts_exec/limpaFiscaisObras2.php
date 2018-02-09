<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao";//simec_desenvolvimento
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
include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";


$sql = "

    SELECT
        o.obrid,
        array_to_string(array_agg( ur.usucpf ), ',') as usucpfs,
        array_to_string(array_agg( ur.rpudata_inc), ',') as datasinclusao,
        array_to_string(array_agg( ur.rpuid), ',') as id,
        (SELECT s.usucpf
            FROM obras2.supervisao s
            WHERE
                s.obrid = o.obrid
                AND s.emsid IS NULL
                AND s.smiid IS NULL
                AND s.supstatus = 'A'::bpchar
                AND s.validadapelosupervisorunidade = 'S'::bpchar
                AND s.rsuid = 1
                AND s.usucpf IN (SELECT ur.usucpf FROM obras2.usuarioresponsabilidade ur WHERE ur.rpustatus = 'A' AND ur.pflcod = 948)
            ORDER BY s.supdata DESC LIMIT 1) as usucpf
    FROM obras2.obras o
    JOIN obras2.usuarioresponsabilidade ur ON ur.empid = o.empid AND ur.rpustatus = 'A' AND ur.pflcod = 948
    inner join seguranca.usuario u on u.usucpf = ur.usucpf AND u.suscod = 'A'
    inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
    WHERE o.obridpai IS NULL AND o.obrstatus = 'A'  -- AND o.obrid IN (5649)
    GROUP BY o.obrid
    HAVING COUNT (ur.usucpf) > 2


";
$dados = $db->carregar($sql);
$sql = "";

if (!empty($dados)) {

    foreach ($dados as $dado) {
        $ids = explode(',', $dado['id']);
        $cpfs = explode(',', $dado['usucpfs']);

        $rpu = array();
        foreach ($cpfs as $key => $cpf)
            $rpu[$ids[$key]] = $cpf;

        ksort($rpu, SORT_NUMERIC);

        $ids = [];
        $cpfs = [];
        foreach ($rpu as $id => $cpf) {
            $ids[] = $id;
            $cpfs[] = $cpf;
        }
        if (empty($dado['usucpf'])) {
            for ($x = 0; $x < count($ids) - 1; $x++) {
                $sql .= "UPDATE obras2.usuarioresponsabilidade SET rpustatus = 'I' WHERE rpuid = {$ids[$x]}; -- {$dado['obrid']}\n";
            }
        } else {
            $dx = array_search($dado['usucpf'], $cpfs);
            if ($dx !== false) {
                unset($ids[$dx]);
                foreach ($ids as $id) {
                    $sql .= "UPDATE obras2.usuarioresponsabilidade SET rpustatus = 'I' WHERE rpuid = {$id}; -- {$dado['obrid']}\n";
                }
            } else {
                for ($x = 0; $x < count($ids) - 1; $x++) {
                    $sql .= "UPDATE obras2.usuarioresponsabilidade SET rpustatus = 'I' WHERE rpuid = {$ids[$x]}; -- {$dado['obrid']}\n";
                }
            }
        }
    }
    $db->carregar($sql);
    $db->commit();
}

ver('EXECUTADO COM SUCESSO.', $sql, d);