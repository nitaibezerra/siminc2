<?php

function getmicrotime() {
    list($usec, $sec) = explode(" ", microtime());
    return ((float) $usec + (float) $sec);
}

$Tinicio = getmicrotime();

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
include_once "../../global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

# Verificando IP de origem da requisição é autorizado para executar os SCRIPTS.
controlarExecucaoScript();

//Definindo como superuser para que o commit ocorra
$_SESSION['superuser'] = true;
if (!$_SESSION['usucpf']) {
    $_SESSION['usucpforigem'] = '03700155689';
    $_SESSION['usucpf'] = '03700155689';
}

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$tm = time();

if (!is_dir('./scripts_exec/')) {
    mkdir(APPRAIZ . 'seguranca/www/scripts_exec/', 0777);
}

if (!is_dir('./scripts_exec/scripts_logs/')) {
    mkdir(APPRAIZ . 'seguranca/www/scripts_exec/scripts_logs/', 0777);
}


$horainicio = date("d/m/Y H:i:s");

$sql = "SELECT * FROM seguranca.agendamentoscripts WHERE agsstatus='A'";
$agendamentos = $db->carregar($sql);

if ($agendamentos[0]) {
    foreach ($agendamentos as $agen) {
        switch ($agen['agsperiodicidade']) {
            case 'diario':
                $agen['agsperdetalhes'] = str_replace(";", ":00;", $agen['agsperdetalhes']) . ":00";
                $diahor = explode(";", $agen['agsperdetalhes']);

                if (in_array(date("H:i"), $diahor)) {
                    $_LISTAGEN[$agen['agsid']] = $agen['agsfile'];
                    $sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec='" . date("Y-m-d H:i:s") . "' WHERE agsid='" . $agen['agsid'] . "';";
                }
                break;
            case 'semanal':
                if ($agen['agsdataexec'] != date("Y-m-d")) {
                    $diasem = explode(";", $agen['agsperdetalhes']);
                    if (in_array(date("w"), $diasem)) {
                        $_LISTAGEN[$agen['agsid']] = $agen['agsfile'];
                        $sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec='" . date("Y-m-d") . "' WHERE agsid='" . $agen['agsid'] . "';";
                    }
                }
                break;
            case 'mensal':
                if ($agen['agsdataexec'] != date("Y-m-d")) {
                    $diamen = explode(";", $agen['agsperdetalhes']);
                    if (in_array(date("d"), $diamen)) {
                        $_LISTAGEN[$agen['agsid']] = $agen['agsfile'];
                        $sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec='" . date("Y-m-d") . "' WHERE agsid='" . $agen['agsid'] . "';";
                    }
                }
                break;
        }
    }
}

$out = array();

if ($_LISTAGEN) {
    foreach ($_LISTAGEN as $agsid => $file) {

        if (is_file('./scripts_exec/' . $file)) {
            $linhaComando = "/usr/bin/php ". APPRAIZ. "seguranca/www/scripts_exec/". $file. " &";
            $resexec .= shell_exec($linhaComando);
            $log .= "Script(s) executado(s): ". $linhaComando. " &\n";

            $logBanco .= $log. $resexec;
            $logBanco .= "<br>";
        } else {
            $log .= "Não foi encontrado o arquivo '" . $file . "'\n";
            $logBanco = "Não foi encontrado o arquivo '" . $file . "'<br/>";
        }

        //gravando o log da operação
        $sql = "INSERT INTO
		          seguranca.logexecucaoscripts (agsid,leslog)
		        VALUES
		          ('{$agsid}','" . addslashes($logBanco) . "');";

        $db->executar($sql);
        $db->commit();
    }
} else {
    $log .= "Nenhum agendamento encontrados\n";
}

if ($sqls) {
    $db->executar(implode("", $sqls));
    $db->commit();
    $log .= "Atualizações efetuadas com sucesso\n";
} else {
    $log .= "Nenhuma atualização efetuada\n";
}

ob_start();
echo "<pre>";
print_r($resexec);
$dadosserv = ob_get_contents();
ob_end_clean();

//Limpando o superuser
$_SESSION['superuser'] = null;
$_SESSION['usucpforigem'] = null;
$_SESSION['usucpf'] = null;
