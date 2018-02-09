<?php

set_time_limit(0);

define('TIPO_PREFEITO', 'P');
define('TIPO_FISCAL', 'F');

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//  Checa se esta no período em que é possivél fazer a transfência de alunos.
//	Caso esteja fora do período, cancela as transerência pendentes.
// $sql="SELECT
//             true
//          FROM
//             projovemurbano.periodocurso
//          WHERE
//             to_char(perdtfim + 6,'YYYYMMDD') <= to_char(now(),'YYYYMMDD')
//          AND to_char(perdtfim + 8,'YYYYMMDD') >= to_char(now(),'YYYYMMDD')";
// //    ver($sql,d);
// $permiteTransferencia = $db->pegaUm($sql);

// if($permiteTransferencia !='t'){
// 	$sql = "UPDATE projovemurbano.historico_transferencia
//                SET shtid_status = 5
//              WHERE
//                 shtid_status in(1,2)";
	 
// 	$db->executar($sql);
// 	$db->commit();
// }
//   fim checa transferência

?>