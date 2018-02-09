<?php
ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC. "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
//include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';

# Conexão Simec
$db = new cls_banco();

# Conexão PDDEInterativo
global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd;
if($_SERVER['SERVER_NAME'] == "simec.mec.gov.br"){
    $servidor_bd = '';
    $porta_bd = '5432';
    $nome_bd = '';
    $usuario_db = '';
    $senha_bd = '';
}else{
    $servidor_bd = '';
    $porta_bd    = '';
    $nome_bd     = '';
    $usuario_db  = '';
    $senha_bd    = '';
}
$db2 = new cls_banco();
//include_once APPRAIZ . 'www/obras2/_constantes.php';
//include_once APPRAIZ . 'www/obras2/_funcoes.php';
//include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . 'includes/funcoes_espelhoperfil.php';

# Exclui os dirigentes DO PDDEInterativo 2015 para inserir os do Módulo PAR do SIMEC
$sql = "
    DELETE FROM
        seguranca.perfilusuario
    WHERE
        pflcod IN(
        837, -- Dirigente - Estadual
        838 -- Dirigente - Municipal
    );

    DELETE FROM
        seguranca.usuario_sistema us
    WHERE
        us.pflcod IN(
        837, -- Dirigente - Estadual
        838 -- Dirigente - Municipal
    );

    DELETE FROM
        pddeinterativo2015.usuarioresponsabilidade ur
    WHERE
        ur.pflcod IN(
        837, -- Dirigente - Estadual
        838 -- Dirigente - Municipal
    );
";
$db2->executar($sql);
$db2->commit();

function executarEspelhamentoPerfil($pflcod) {
    global $db;

    /* configurações do relatorio - Memoria limite de 1024 Mbytes */
    ini_set("memory_limit", "2048M");
    set_time_limit(0);
    /* FIM configurações - Memoria limite de 1024 Mbytes */

    $sql = "
        SELECT
            u.usucpf,
            p.pflcod
        FROM
            seguranca.usuario u
            JOIN seguranca.perfilusuario p ON p.usucpf = u.usucpf
        WHERE
            p.pflcod='".$pflcod."'";
    $usrs = $db->carregar($sql);

    if(is_array($usrs)) {
        foreach($usrs as $usr) {
            inserirPerfisSlaves($usr['usucpf'], $usr['pflcod']);
            atualizarResponsabilidadesSlaves($usr['usucpf'], $usr['pflcod']);
        }
    }
}

/**
 * 672 Equipe Estadual - Aprovação
 * 674 Equipe Municipal - Aprovação
 */
executarEspelhamentoPerfil(672); // Equipe Estadual - Aprovação
executarEspelhamentoPerfil(674); // Equipe Municipal - Aprovação

?>
