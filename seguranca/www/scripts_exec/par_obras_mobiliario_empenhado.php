<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 147;

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "INSERT INTO obras2.mobiliarioempenhado
(obrid,moeempenhadoflag,moeconveniotipo,moedtinclusao,moedtupdate,usucpf,moestatus,moeorigemdado,moenumprocesso)
select distinct obrid ,'S','TC',now(),now(),'54002192172','A',2,e.empnumeroprocesso from par.subacaoobravinculacao sov
inner join par.subacao s on s.sbaid = sov.sbaid and s.ppsid in (924,906,913)
inner join par.empenhosubacao es on es.sbaid = s.sbaid
inner join par.empenho e on e.empid = es.empid and trim(empsituacao) <> 'CANCELADO' and empstatus = 'A'
where obrid not in (select obrid from obras2.mobiliarioempenhado where moestatus = 'A');";

$db->executar($sql);

$sql = "INSERT INTO obras2.mobiliarioempenhado
(obrid,moeempenhadoflag,moeconveniotipo,moedtinclusao,moedtupdate,usucpf,moestatus,moeorigemdado,moenumprocesso)
select distinct obrid ,'S','TC',now(),now(),'54002192172','A',2,e.empnumeroprocesso  from par.subacaoobravinculacao sov
inner join par.subacao s on s.sbaid = sov.sbaid and s.ppsid in (925,914,904 )
inner join par.empenhosubacao es on es.sbaid = s.sbaid
inner join par.empenho e on e.empid = es.empid and trim(empsituacao) <> 'CANCELADO' and empstatus = 'A'
where obrid not in (select obrid from obras2.mobiliarioempenhado where moestatus = 'A');";

$db->executar($sql);
$db->commit();


?>