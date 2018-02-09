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
$_SESSION['sisid'] = 4;

$qtd = $_GET['qtd'] ? $_GET['qtd'] : 1;

/****************************************
*				PREFEITO				*
****************************************/

$conteudo = "Participe do concurso Melhores Receitas da Alimentação Escolar! Acesse o regulamento em melhoresreceitas.mec.gov.br.";

// Pendências do município > 60 dias
$dados = getDados($qtd);

enviarNotificacoes($dados, $conteudo);

//registraAtividade($obras);

echo "FIM";

function getDados($qtd)
{
    $total  = 5000;
    $limit  = $qtd*$total;
    $offset = $limit - $total;

    $sql = "select distinct usucelddd  ||  usucelnum as celular
            from pddeinterativo2015.listapdeinterativo lp
            inner join pddeinterativo.usuariocelular  cel on cel.usucpf = lp.usucpfdiretor
            where usucelnum is not null
            order by celular
            limit $total
            offset $offset
            ";

    ver($sql);

    $dados = adapterConnection::pddeinterativo()->carregar($sql);
    $dados = $dados ? $dados : array();
    connection::getInstance()->close();
 	return $dados;
}

function enviarNotificacoes($dados, $conteudo)
{
	$db = new cls_banco();

    $aCelularEnvio = array();
		$aCelularEnvio = array(
			'556191434894', // Daniel
 			'556181054537', // Orion
 			'556193332616', // Manuelita
		);
	foreach ($dados as $contato) {
        $aCelularEnvio[] = 55 . preg_replace("/[^0-9]/", "", trim($contato['celular']));
//		$aCelularEnvio[] = 55 . str_replace(array(' ', '-', '/', 'D'), array(''), $contato['celular']);
	}

	$aCelularEnvio = array_unique($aCelularEnvio);
    
//    ver($aCelularEnvio, d);

    $sms = new Sms();
    $sms->enviarSms(array('556181054537'), 'Início');

    $sms->enviarSms($aCelularEnvio, $conteudo);

    $sms->enviarSms(array('556181054537'), 'FIM');
}

?>