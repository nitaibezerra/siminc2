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

// Pendências do município > 60 dias
$dados = getDados();

enviarNotificacoes($dados);

//registraAtividade($obras);

echo "FIM";

function getDados()
{
	$db = new cls_banco();

    $sql = "select cpf, diretor, celular, esdid
            From dblink('host= user= password= port= dbname=',
                ' select distinct
                    lp.usucpfdiretor as cpf,
                    lp.usunome as diretor,
                    usucelddd  || usucelnum as celular,
                    est.esdid
                from pdeescola.memaiseducacao as me
                    inner join pddeinterativo.listapdeinterativo lp on lp.pdicodinep = me.entcodent
                    left join workflow.documento as doc on doc.docid = me.docid
                    left join workflow.estadodocumento as est on est.esdid = doc.esdid
                    inner join pddeinterativo.usuariocelular  cel on cel.usucpf = lp.usucpfdiretor
                where me.memanoreferencia = ''2014''
                and (
                    est.esddsc is null
                    or est.esdid in (32, 35, 36)
                ) '
            )
            AS pd (
            cpf text,
            diretor text,
            celular text,
            esdid integer
            )
             ";

 	return $db->carregar($sql);
}

function enviarNotificacoes($dados)
{
	$db = new cls_banco();

    $aCelularEnvio = array();
    $aCelularEnvio = $aCelularEnvioNaoIniciado = $aCelularEnvioCorrecaoEscola = $aCelularEnvioCorrecaoSecretaria = array(
        '556191434894', // Daniel
        '556181054537', // Orion
        '556199440097', // Leandro
        '556186183665', // Luiz
    );
	foreach ($dados as $contato) {
        switch($contato['esdid']){
            case(32): $aCelularEnvio[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['celular']);                     break;
            case(35): $aCelularEnvioCorrecaoEscola[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['celular']);       break;
            case(36): $aCelularEnvioCorrecaoSecretaria[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['celular']);   break;
    		default:  $aCelularEnvioNaoIniciado[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['celular']);          break;
        }
	}
	
	$aCelularEnvio = array_unique($aCelularEnvio);
	$aCelularEnvioNaoIniciado = array_unique($aCelularEnvioNaoIniciado);
    $aCelularEnvioCorrecaoEscola = array_unique($aCelularEnvioCorrecaoEscola);
    $aCelularEnvioCorrecaoSecretaria = array_unique($aCelularEnvioCorrecaoSecretaria);

    $data = date('Y-m-d') . ' 08:00:00';

    $conteudo = "Sr(a). Diretor(a), prorrogamos o prazo do Mais Educação para 31/08. Conclua o cadastro da sua escola em  pddeinterativo.mec.gov.br";
    $conteudoNaoIniciados = "Sr(a). Diretor(a), o Mais Educação foi prorrogado para 31/08. Sua escola poderá fazer a adesão em  pddeinterativo.mec.gov.br";
    $conteudoCorrecaoEscola = "Sr.(a) Diretor(a), o Plano de Atendimento de sua escola no Mais Educação apresentou erro. Aguardamos correção até 31/08.";
    $conteudoCorrecaoSecretaria = "MAIS EDUCAÇÃO: Plano de Atendimento de Escolas vinculadas a essa Secretaria apresentou erro. Aguardamos correção até 31/08.";

    $sms = new Sms();
    $sms->enviarSms($aCelularEnvio, $conteudo, $data);
    $sms->enviarSms($aCelularEnvioNaoIniciado, $conteudoNaoIniciados, $data);
    $sms->enviarSms($aCelularEnvioCorrecaoEscola, $conteudoCorrecaoEscola, $data);
    $sms->enviarSms($aCelularEnvioCorrecaoSecretaria, $conteudoCorrecaoSecretaria, $data);
}

?>
