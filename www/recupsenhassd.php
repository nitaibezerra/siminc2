<?

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

session_start();

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


/*
 * Caso o formulario seja true, efetuar a mudança de senha
 */

if($_POST['formulario']) {
	
	include_once("connector.php");
	$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
	if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
		$SSDWs->useProductionSSDServices();
	} else {
		$SSDWs->useHomologationSSDServices();
	}
			
	$hash = $_POST['hash'];
	$code = $_POST['code'];
	$password = base64_encode($_POST['novasenha']);
	$cpfOrCnpj = str_replace(array(".","-"),array("",""), $_POST['usucpf']);
	$resposta = $SSDWs->recoveryUserPasswordByHashAndCode($hash, $code, $password, $cpfOrCnpj);
	if($resposta != "true") {
		session_unset();
		$_SESSION['MSG_AVISO'] = $resposta['erro'];
		header('location: login.php');
    	exit;
	} else {
		$usuarioExiste = $db->pegaLinha("SELECT usucpf FROM seguranca.usuario WHERE usucpf='".$cpfOrCnpj."'");
		if($usuarioExiste) {
			// atribuições requeridas para que a auditoria do sistema funcione
			$_SESSION['sisid'] = 4; # seleciona o sistema de segurança
			$_SESSION['usucpf'] = $cpfOrCnpj;
			$_SESSION['usucpforigem'] = $cpfOrCnpj;
			
			$sql = "UPDATE seguranca.usuario SET ususenha='".md5_encrypt_senha( $_POST['novasenha'], '' )."', usuchaveativacao='t' WHERE usucpf='".$cpfOrCnpj."'";
			$db->executar($sql);
			$db->commit();
		}
		
		session_unset();
		$_SESSION['MSG_AVISO'] = "SSD : Sua senha foi alterada com sucesso.";
		header('location: login.php');
    	exit;
	}
}



?>
<script language="JavaScript" src="./includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="./includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="./includes/listagem.css"/>

<title>Recuperação de senha de Usuário</title> 
<body bgcolor="#ffffff" vlink="#666666" bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">

<form method="POST"  name="formulario">
<input type=hidden name="hash" value="<? echo $_REQUEST['crypto']; ?>">
<input type=hidden name="formulario" value="1">

<?php include "cabecalho.php"; ?>

<br />

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Recuperar senha no padrão SSD</td>
</tr>

<tr>
	<td align='right' class="SubTituloDireita">CPF:</td>
	<td><input type="text" name="usucpf" value="" size="20" onkeypress="return controlar_foco_cpf( event );" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);"  class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);"><?=obrigatorio();?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita" >Código de ativação:</td>
	<td><input type='text' name='code' size='20'><?=obrigatorio();?></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita" >Nova senha:</td>
    <td><input type='password' name='novasenha' size='20'><?=obrigatorio();?> <font size=1>A senha deve conter no mínimo de <b>4 (quatro)</b> letras e <b>2 (dois)</b> números</font></td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita">Confirma nova senha:</td>
    <td><input type='password' name='confsenha' size='20'><?=obrigatorio();?></td>
</tr>
<tr bgcolor="#C0C0C0">
	<td></td>
	<td><input type="button" class="botao" name="btalterar" value="Atualizar" onclick="validar_cadastro()"></td>
</tr>
</table>
</form>
</body>
<script>

	function IsNumeric(sText) {
		var ValidChars = "0123456789";
		var IsNumber=true;
		var Char;
		for (i = 0; i < sText.length && IsNumber == true; i++) { 
			Char = sText.charAt(i); 
			if (ValidChars.indexOf(Char) == -1) {
				IsNumber = false;
			}
		}
		return IsNumber;
	}


	function validaSenhaSSD(senha) {
		var letras=0;
		var numeros=0;
		for(var i=0;i<senha.value.length;i++) {
			if(IsNumeric(senha.value.substr(i,1))) {
				numeros = numeros+1;	
			} else {
				letras = letras+1;
			}
		}
		if(numeros >= 2 && letras >= 4) {
			return true
		} else {
			return false;
		}
	}

	function validar_cadastro() {
        e =document.formulario.novasenha.value;
		if (!validaBranco(document.formulario.usucpf, 'CPF')) return;
		if (!validaBranco(document.formulario.code, 'Código de ativação')) return;
		if (!validaBranco(document.formulario.novasenha, 'Nova senha')) return;
        if ( e.length > 12 ) {alert ('A senha deve ter um máximo de 12 caracteres'); return;}
        if ( e.length < 4 ) {alert ('A senha deve ter um mínimo de 4 caracteres'); return;}
		if (!validaBranco(document.formulario.confsenha, 'Confirma nova senha')) return;
		if (!validaSenhaSSD(document.formulario.novasenha)) {alert('A senha deve ter no mínimo 4 caracteres e 2 números'); return;}
		if (document.formulario.novasenha.value != document.formulario.confsenha.value ) {
	       		alert('A nova senha não confere com a segunda digitação.');
	       		return;
	    }
		document.formulario.submit();
	}

	function controlar_foco_cpf( evento ) {
		if ( window.event || evento.which ) {
			if ( evento.keyCode == 13) {
				return document.formulario.ususenha.focus();
			};
		} else {
			return true;
		}
	}

</script>