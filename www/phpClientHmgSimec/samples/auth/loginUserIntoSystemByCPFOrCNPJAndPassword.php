<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("[SIMEC] LOGAR USU&Aacute;RIO NO SISTEMA POR CPF/CNPJ E SENHA");
			$SSDWs = new SSDWsAuth($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODU&Ccedil;&Atilde;O conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de HOMOLOGA&Ccedil;&Atilde;O conectado. WSDL baixada.");
			}
			
			$cpfOrCnpj = base64_encode($_POST['cpfOrCnpj']);
			$password = base64_encode($_POST['password']);
			$resposta = $SSDWs->loginUserIntoSystemByCPFOrCNPJAndPassword($cpfOrCnpj, $password);
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
	<h3>[SIMEC] LOGAR USU&Aacute;RIO NO SISTEMA POR CPF/CNPJ E SENHA</h3>
	<form method="POST">
		<label>CPF/CNPJ:</label> <input type="text" name="cpfOrCnpj" /><br />
		<label>Senha:</label> <input type="password" name="password" /><br />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>
