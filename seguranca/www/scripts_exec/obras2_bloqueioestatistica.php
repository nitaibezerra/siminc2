<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$obras = array();

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 147;

$db = new cls_banco();

$sql = "INSERT INTO obras2.bloqueioestatistica(blequantidade, estuf, muncod, bledataprocessamento, esfera, bletipo)

        (select count(*), estuf, muncod,  NOW() as data, empesfera, 'obras2.v_regras_obras_em_vermelho' as tipo
        from obras2.v_regras_obras_em_vermelho
        group by estuf, muncod, data, empesfera, tipo)

        union all

        (select count(*), estuf, muncod,  NOW() as data, empesfera, 'obras2.v_regras_obras_evolucao' as tipo
        from obras2.v_regras_obras_evolucao
        group by estuf, muncod, data, empesfera, tipo)

        union all

        (select count(*), estuf, muncod,  NOW() as data, empesfera, 'obras2.v_regras_obras_paralisadas' as tipo
        from obras2.v_regras_obras_paralisadas
        group by estuf, muncod, data, empesfera, tipo)

        union all

        (select count(*), estuf, muncod,  NOW() as data, empesfera, 'obras2.v_regras_obras_saldo_sem_execucao' as tipo
        from obras2.v_regras_obras_saldo_sem_execucao
        group by estuf, muncod, data, empesfera, tipo)

        union all

        (select count(*), estuf, muncod,  NOW() as data, empesfera, 'obras2.v_regras_obras_vinculadas' as tipo
        from obras2.v_regras_obras_vinculadas
        group by estuf, muncod, data, empesfera, tipo)

        union all

        (select count(*), estuf, muncod,  NOW() as data, empesfera, 'obras2.v_regras_obras_em_reformulacao' as tipo
        from obras2.v_regras_obras_em_reformulacao
        group by estuf, muncod, data, empesfera, tipo)

        union all

        (select count(*), estuf, muncod,  NOW() as data, empesfera, 'obras2.v_regras_obras_execucao_sem_vistoria' as tipo
        from obras2.v_regras_obras_execucao_sem_vistoria
        group by estuf, muncod, data, empesfera, tipo)
		";


$db->executar($sql);
$db->commit();

echo 'FIM';

?>