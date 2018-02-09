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
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
include_once APPRAIZ . "includes/classes/Sms.class.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 98;

/****************************************
*				PREFEITO				*
****************************************/

$conteudo = "MAIS EDUCAÇÃO: Prazo de adesão estendido até o dia 30 de junho. Inclua sua escola no sítio pdeinterativo.mec.gov.br";

// Pendências do município > 60 dias
$dados = getDados();

enviarNotificacoes($dados, $conteudo);

//registraAtividade($obras);

echo "FIM";

function getDados()
{
	$db = new cls_banco();

    $sql = "select celular
			From dblink('host= user= password= port= dbname=',
			'SELECT distinct
                usucelddd  || usucelnum as celular
            FROM pddeinterativo.pdinterativo pde
                inner JOIN pddeinterativo.pessoatipoperfil ptp ON ptp.pdeid = pde.pdeid and ptp.tpeid=2
                inner JOIN pddeinterativo.pessoa pes ON ptp.pesid = pes.pesid and pes.pesstatus  = ''A''
                inner JOIN seguranca.usuario usu ON usu.usucpf = pes.usucpf and usu.suscod = ''A''
                inner JOIN seguranca.perfilusuario pfl ON usu.usucpf = pfl.usucpf
                inner join pddeinterativo.usuariocelular  cel on cel.usucpf = usu.usucpf
                inner join pdeescola.pddemepriorizadas me on me.entcodent = pde.pdicodinep and mepstatus = ''A''
                inner join pdeescola.memaiseducacao ma on ma.entcodent = me.entcodent
                left join workflow.documento doc on doc.docid = ma.docid
                left join workflow.estadodocumento est on est.esdid = doc.esdid
            WHERE pesstatus = ''A''  and pde.pdistatus = ''A'' and ma.memanoreferencia = ''2014'' and  ma.docid is null and coalesce(usucelnum, ''0'') != ''0''
            ')
			AS pd (
			celular text
			)";

 	return $db->carregar($sql);
}

function enviarNotificacoes($dados, $conteudo)
{
	$db = new cls_banco();

    $aCelularEnvio = array();
		$aCelularEnvio = array(
			'556191434894', // Daniel
 			'556181054537', // Orion
            '556199440097', // Leandro
            '556186183665', // Luiz
		);
	foreach ($dados as $contato) {
		$aCelularEnvio[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['celular']);
	}
	
	$aCelularEnvio = array_unique($aCelularEnvio);

    $data = date('Y-m-d') . ' 08:00:00';

    $sms = new Sms();
    $sms->enviarSms($aCelularEnvio, $conteudo, $data);
}

?>
