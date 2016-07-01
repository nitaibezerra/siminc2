<?php
$_SESSION["sisid"] = 4;
$_POST['ajaxCPF'] = true;
$_SESSION['usucpf'] = $_SESSION['usucpforigem'] = '';

include "config.inc";
include_once APPRAIZ . "educriativa/autoload.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "www/includes/webservice/cpf.php";
include_once APPRAIZ . "www/includes/webservice/pj.php";
//023.473.981-99
$cpf = $_POST['usucpf'];
$cpfSomenteNumero = preg_replace("/[^0-9]/", "", trim($cpf));
$captcha = $_POST['captcha'];

if (validaForm($cpf, $captcha)) {
	$participante = new Educriativa_Model_Participante();
	
	$organizacao = new Educriativa_Model_Organizacao();
	
	$questionario = new Educriativa_Model_Questionario();
		
	$questionario->carregarPorCpf($cpfSomenteNumero);

	if ($questionario->queid == null)
	{
		$pessoaFisica = $participante->carregarPessoaFisica($cpfSomenteNumero);
		
		$participante->parnome = $pessoaFisica->no_pessoa_rf;
		$participante->parcpf = $pessoaFisica->nu_cpf_rf;
		$participante->pardatanascimento = $pessoaFisica->dt_nascimento_rf;
		$participante->parsexo = $pessoaFisica->sg_sexo_rf;
		
		$organizacao->orgcnpj = '';
		$organizacao->orgid = $organizacao->salvar();
		$participante->parid = $participante->salvar();

		$questionario->quesituacao = 'A';
		$questionario->quedtcriacao = date('Y-m-d H:i:s');
		$questionario->parid = $participante->parid;
		$questionario->orgid = $organizacao->orgid;
		$questionario->queid = $questionario->salvar();

		$questionario->salvar();

		$organizacao->commit();
		$participante->commit();
		$questionario->commit();
	}
	
	$_SESSION['queid'] = $questionario->queid;
	$_SESSION['parid'] = $questionario->parid;
	$_SESSION['orgid'] = $questionario->orgid;
	
	if ($questionario->quesituacao == 'F') {
		ob_clean();
        $info = 'f=1&q=' . $questionario->queid;
		header("Location: formularioAcompanhamento.php?i=" . base64_encode($info));
		die;
	}
	
	header("Location: formulario.php");
}

function validaForm($cpf, $captcha)
{
	$flagusucpf  = empty($cpf) ? '1' : (!validaCPF($cpf) ? '2' : null);
	$flagcaptcha = empty($captcha) ? '1' : ($_SESSION['session_textoCaptcha'] != $captcha ? '2' : null);

	if ($flagusucpf || $flagcaptcha || $flagrepresentacao) {
		$flagusucpf = $flagusucpf ? "&cpf={$flagusucpf}" : null;
		$flagcaptcha = $flagcaptcha ? "&captcha={$flagcaptcha}" : null;

		header("Location: login.php?error=1{$flagusucpf}{$flagcaptcha}");
		
		return false;
	}
	
	return true;
}

function validaCNPJ($cpfcnpj) {
	$dv = false;
	$cpfcnpj = ereg_replace("[^0-9]","",$cpfcnpj);
	
	if ( strlen($cpfcnpj) == 14 ) {
		$cnpj_dv = substr($cpfcnpj,-2);
		for ( $i = 0; $i < 2; $i++ ) {
			$soma = 0;
			for ( $j = 0; $j < 12; $j++ )
			$soma += substr($cpfcnpj,$j,1)*((11+$i-$j)%8+2);
			if ( $i == 1 ) $soma += $digito * 2;
			$digito = 11 - $soma  % 11;
			if ( $digito > 9 ) $digito = 0;
			$controle .= $digito;
		}
		if ( $controle == $cnpj_dv )
		$dv = true;
	}
	
	if ( strlen($cpfcnpj) == 11 ) {
		$cpf_dv = substr($cpfcnpj,-2);
		for ( $i = 0; $i < 2; $i++ ) {
			$soma = 0;
			for ( $j = 0; $j < 9; $j++ )
			$soma += substr($cpfcnpj,$j,1)*(10+$i-$j);
			if ( $i == 1 ) $soma += $digito * 2;
			$digito = ($soma * 10) % 11;
			if ( $digito == 10 ) $digito = 0;
			$controle .= $digito;
		}
		if ( $controle == $cpf_dv )
		$dv = true;
	}
	
	return $dv;
}
