<?php
error_reporting(1);
error_reporting(E_ALL ^ E_NOTICE);

session_start();

//set_include_path('.;D:\Workspace\php\pdeinterativo\includes;D:\Workspace\php\pdeinterativo\global;');
//$_SESSION['usucpforigem'] = '';
//$_SESSION['usucpf'] = '';
//$_SESSION['superuser'] = '1';

$_SESSION["sisid"] = 4;
$_POST['ajaxCPF'] = true;
$_SESSION['usucpf'] = $_SESSION['usucpforigem'] = '';

include "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "www/includes/webservice/cpf.php";
include_once APPRAIZ . "www/includes/webservice/pj.php";
include_once "classes/Encode.class.inc";
include_once "classes/Questionario.class.inc";
include_once "classes/Participante.class.inc";
switch ($_REQUEST['action']) 
{	
        case 'carregarCPFRep':            
            $participante = new Participante();
            $cpfSomenteNumero = preg_replace("/[^0-9]/", "", trim($_REQUEST['usucpf']));
            $pessoaFisica = $participante->carregarPessoaFisica($cpfSomenteNumero);                   
            ob_clean();
            print json_encode(array('parnome'=>$pessoaFisica->no_pessoa_rf));
            die;
            break;
}
$cpf = $_POST['usucpf'];
$cnpj = $_POST['usucnpj'];
$cpfSomenteNumero = preg_replace("/[^0-9]/", "", trim($cpf));
$cnpjSomenteNumero = preg_replace("/[^0-9]/", "", trim($cnpj));
$parrepresentacao = $_POST['parrepresentacao'];
$captcha = $_POST['captcha'];

$_SESSION['usucpf_pne'] = $cpf;
if (validaForm($cpfSomenteNumero, $cnpjSomenteNumero, $parrepresentacao, $captcha)) {    
	$participante = new Participante();
	
	$questionario = new Questionario();
	
	$participante->recuperarParticipante($cpfSomenteNumero);

	if ($participante->parid == null)
	{
		$pessoaFisica = $participante->carregarPessoaFisica($cpfSomenteNumero);
		$participante->parnome = $pessoaFisica->no_pessoa_rf;
		$participante->parcpf = $pessoaFisica->nu_cpf_rf;
		$participante->pardatanascimento = $pessoaFisica->dt_nascimento_rf;
		$participante->parsexo = $pessoaFisica->sg_sexo_rf;
		$participante->parrepresentacao = $_POST['parrepresentacao'];
		
		if ($participante->parrepresentacao != 3) {
                    $pessoaJuridica = $participante->carregarPessoaJuridica($cnpjSomenteNumero);
                    $participante->parcnpj = $pessoaJuridica->nu_cnpj_rf;
                    $participante->parrepnomefantasia = is_string($pessoaJuridica->no_fantasia_rf) ? $pessoaJuridica->no_fantasia_rf : '';
                    $participante->parrepnome = $pessoaJuridica->no_responsavel_rf;
                    $participante->parrepcpf = $pessoaJuridica->nu_cpf_responsavel_rf;
		}
		
		$participante->parid = $participante->salvar();
		
		$participante->commit();
		
		$questionario->parid = $participante->parid;
		$questionario->quedtcriacao = date('Y-m-d H:i:s');
		$questionario->quesituacao = 'A';
		$questionario->queid = $questionario->salvar();
        
		$questionario->commit();
	}
	
	$questionario = $questionario->recuperarPorParticipante($participante->parid);

	$_SESSION['queid_pne'] = $questionario->queid;
	$_SESSION['parid_pne'] = $participante->parid;        
	if ($questionario->quesituacao == 'F') {
		ob_clean();
		header("Location: vizualizar.php");
		die;
	}
	
	header("Location: formulario.php");
}

function validaForm($cpf, $cnpj, $parrepresentacao, $captcha)
{
    $flagusucnpj = null;

    if(3 != $parrepresentacao){
	    $flagusucnpj = empty($cnpj) ? 1 : (!dv_cpf_cnpj_ok($cnpj) ? '2' : null);
    }
	$flagusucpf  = empty($cpf) ? '1' : (!validaCPF($cpf) ? '2' : null);
    $flagrepresentacao  = empty($parrepresentacao) ? '1' : null;
	$flagcaptcha = empty($captcha) ? '1' : ($_SESSION['session_textoCaptcha'] != $captcha ? '2' : null);

	if ($flagusucpf || $flagcaptcha || $flagusucnpj || $flagrepresentacao) {
		$flagusucpf = $flagusucpf ? "&usucpf={$flagusucpf}" : null;
        $flagusucnpj = $flagusucnpj ? "&usucnpj={$flagusucnpj}" : null;
        $flagrepresentacao = $flagrepresentacao ? "&flagrepresentacao={$flagrepresentacao}" : null;
        $parrepresentacao = "&parrepresentacao={$parrepresentacao}";
		$flagcaptcha = $flagcaptcha ? "&captcha={$flagcaptcha}" : null;

		header("Location: index.php?error=1{$flagusucpf}{$flagusucnpj}{$flagrepresentacao}{$parrepresentacao}{$flagcaptcha}");
		
		return false;
	}
	
	return true;
}