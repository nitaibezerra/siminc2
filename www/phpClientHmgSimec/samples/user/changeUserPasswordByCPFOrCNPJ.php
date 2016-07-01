<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("[SIMEC] ALTERAR SENHA DO USU&Aacute;RIO POR CPF/CNPJ");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODU&Ccedil;&Atilde;O conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de HOMOLOGA&Ccedil;&Atilde;O conectado. WSDL baixada.");
			}
			
			$cpfOrCnpj = $_POST['cpfOrCnpj'];
			$oldPassword = base64_encode($_POST['oldPassword']);
			$newPassword = base64_encode($_POST['newPassword']);
			$resposta = $SSDWs->changeUserPasswordByCPFOrCNPJ($cpfOrCnpj, $oldPassword, $newPassword);
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
	<h3>[SIMEC] ALTERAR SENHA DO USU&Aacute;RIO POR CPF/CNPJ</h3>
	<form method="POST">
		<label>CPF/CNPJ:</label> <input type="text" name="cpfOrCnpj"/><br />
		<label>Senha Antiga:</label> <input type="text" name="oldPassword"/><br />
		<label>Senha Nova:</label> <input type="text" name="newPassword"/><br />
		<input type="submit" value="Enviar" name="submit"/>
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>
