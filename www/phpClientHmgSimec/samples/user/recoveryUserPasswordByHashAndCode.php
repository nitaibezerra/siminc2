<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("[SIMEC] RECUPERAR SENHA DO USU&Aacute;RIO POR HASH E C&Oacute;DIGO");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODU&Ccedil;&Atilde;O conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de HOMOLOGA&Ccedil;&Atilde;O conectado. WSDL baixada.");
			}
			
			$hash = $_POST['hash'];
			$code = $_POST['code'];
			$password = base64_encode($_POST['password']);
			$cpfOrCnpj = $_POST['cpfOrCnpj'];
			$resposta = $SSDWs->recoveryUserPasswordByHashAndCode($hash, $code, $password, $cpfOrCnpj);
			echo "<pre>";
			print_r($resposta);
			echo "</pre>";
			
		} catch (Exception $e) {
			$erro = $e->getMessage();
			echo $erro;
			exit();
		}
?>
	<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>[SIMEC] RECUPERAR SENHA DO USU&Aacute;RIO POR HASH E C&Oacute;DIGO</h3>
	<form method="POST">
		<label>Hash:</label> <input type="text" name="hash"/><br />
		<label>C&oacute;digo:</label> <input type="text" name="code"/><br />
		<label>Senha:</label> <input type="text" name="password"/><br />
		<label>CPF/CNPJ:</label> <input type="text" name="cpfOrCnpj"/><br />
		<input type="submit" value="Enviar" name="submit"/>
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>
