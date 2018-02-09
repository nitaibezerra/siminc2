<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = 'simec_espelho_producao';

// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';

require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '72324414104';
$_SESSION['usucpf'] = '72324414104';
$_SESSION['sisid'] = 194;

$db = new cls_banco();

echo "
    <form action='' method='post'>
        <input type='text' name='form[login]' id='login' placeholder='login'>
        <input type='password' name='form[senha]' id='senha' placeholder='senha'>
        <button type='submit' value='enviar'>
            Executar
        </button>
    </form>
";


if (isset($_POST['form']['login']) && isset($_POST['form']['senha']) && $_POST['form']['login'] == '72324414104' && $_POST['form']['senha'] == 'simeclucas123') {

    $startTime = microtime();

    $_roleMaps = array(
        23 => 1233, //super usuario
        852 => 1262, //=> 'Gestor Orçamentário do Proponente',
        864 => 1263, //=> 'Representante Legal do Proponente',
        859 => 1264, //=> 'Gabinete Secretaria/Autarquia',
        866 => 1265, //=> 'Coordenador da Secretaria/Autarquia',
        860 => 1266, //=> 'Diretoria da Secretaria/Autarquia',
        865 => 1267, //=> 'Representante Legal do Concedente',
        863 => 1268, //=> 'Gestor Orçamentário do Concedente',
        871 => 1269, //=> 'Área técnica do FNDE',
        1052 => 1270, //=> 'Diretoria FNDE',
        54 => 1271, //=> 'UO/Equipe Técnica',
        862 => 1273, //=> 'UG Repassadora',
        388 => 1371, //=>'Auditor Interno'
        57  => 1372,  //=>'UO / Consulta Orçamento'
        851 => 1373  //=>'Diretor Administrativo'
    );

    $strSQL = "
        select
          usucpf, rpustatus, rpudata_inc, pflcod, unicod, prsano, ungcod, cooid, dircod
        from elabrev.usuarioresponsabilidade where rpustatus = 'A'
    ";

    $collections = $db->carregar($strSQL);
    if ($collections) {
        $i = 1;

        $strSQL = "
            insert into ted.usuarioresponsabilidade (rpuid, pflcod, usucpf, rpustatus, rpudata_inc, unicod, prsano, ungcod, cooid, dircod) values
        ";

        foreach ($collections as $row) {

            if (!array_key_exists($row['pflcod'], $_roleMaps))
                continue;

            $cooid = ($row['cooid']) ? $row['cooid'] : 'null';
            $dircod = ($row['dircod']) ? $row['dircod'] : 'null';

            $strSQL .= "($i, {$_roleMaps[$row['pflcod']]}, '{$row['usucpf']}', '{$row['rpustatus']}', '{$row['rpudata_inc']}',
                  '{$row['unicod']}', '{$row['prsano']}', '{$row['ungcod']}', $cooid, $dircod),";

            $i++;
        }

        $strSQL = substr($strSQL, 0, -1);
        $db->executar($strSQL);
        $db->commit();
    }

    $endTime = microtime();
    $timeFinal = ($endTime - $startTime);
    $hours = (int)($timeFinal/60/60);
    $minutes = (int)($timeFinal/60)-$hours*60;
    $seconds = (int)$timeFinal-$hours*60*60-$minutes*60;

    $mensagem1 = "<h1>Foram executados {$i} insert's em {$hours}:{$minutes}:{$seconds} ou {$timeFinal} milisegundos</h1>";

    //end of first call

    $startTime = microtime();

    $results = $db->carregar("select distinct usucpf from ted.usuarioresponsabilidade");
    if ($results) {
        $strInsert = ''; $x = 1;
        foreach ($results as $row) {
            $usucpf = trim($row['usucpf']);
            $test = $db->pegaLinha("select * from seguranca.usuario_sistema where usucpf = '{$usucpf}' and sisid = 194");
            if (!$test) {

                $pflcod = $db->pegaUm("select pflcod from ted.usuarioresponsabilidade where usucpf = '{$usucpf}'");

                $strInsert .= "
                    insert into seguranca.usuario_sistema (usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod) values
                    ('{$row['usucpf']}', 194, 'A', {$pflcod}, NOW(), 'A');
                ";

                $x++;
            }
        }

        if (strlen($strInsert)) {
            $db->executar($strInsert);
        }
    }

    $db->commit();

    $endTime = microtime();
    $timeFinal = ($endTime - $startTime);
    $hours = (int)($timeFinal/60/60);
    $minutes = (int)($timeFinal/60)-$hours*60;
    $seconds = (int)$timeFinal-$hours*60*60-$minutes*60;

    $mensagem2 = "<h1>Foram executados {$x} insert's em {$hours}:{$minutes}:{$seconds} ou {$timeFinal} milisegundos</h1>";

    //end of second call

    echo $mensagem1;

    echo $mensagem2;

    $db->close();
}


