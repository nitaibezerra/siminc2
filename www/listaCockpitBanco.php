<?php
// Lista
header('Access-Control-Allow-Origin: *');
header("Cache-Control: no-cache, must-revalidate");
header("Expires: Sat, 26 Jul 1997 05:00:00 GMT");

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();
$tipo = $_REQUEST['tipo'];
switch ($tipo){
    case 'tempo':
        $sql = "select date_part('epoch', now() - query_start)::integer as dur_segundos
                from pg_stat_activity
                       left join seguranca.ip_interno ip on ip.ip = substr(client_addr::text, 0, strpos(client_addr::text, '/'))
                where STATE not  ilike  '%IDLE%'
                and usename = 'simec'
                and date_part('epoch', now() - query_start)::integer < 86400";
        $dados = $db->pegaUm($sql);

        echo simec_json_encode((int) $dados);
        die;
    case 'qtd':
        $sql = "select datname, pid, usename, query, waiting,
                client_addr, (now() - backend_start) as tempo_backend, (now() - query_start) as tempo_query,
                date_part('epoch', now() - query_start)::integer as dur_segundos
                from pg_stat_activity
                       left join seguranca.ip_interno ip on ip.ip = substr(client_addr::text, 0, strpos(client_addr::text, '/'))
                where STATE not  ilike  '%IDLE%'
                and usename = 'simec'
                and date_part('epoch', now() - query_start)::integer < 86400";
        $dados = $db->carregar($sql);

        echo simec_json_encode((int) count($dados));
        die;
    default:
        $sql = "select COALESCE(count(*),0) as usu_online
			from seguranca.usuariosonline
			";
        $dados =  $db->pegaUm($sql);
        
        echo simec_json_encode((int) $dados);
        die;
}
