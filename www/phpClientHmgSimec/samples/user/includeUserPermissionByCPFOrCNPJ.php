<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("[SIMEC] INCLUIR PERMISS&Atilde;O DO USU&Aacute;RIO POR CPF/CNPJ (COM RESPONS&Aacute;VEL)");
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
			$permissionId = (integer) $_POST['permissionId'];
			$responsibleForChangeCpfOrCnpj = $_POST['responsibleForChangeCpfOrCnpj'];
			$resposta = $SSDWs->includeUserPermissionByCPFOrCNPJ($cpfOrCnpj, $permissionId, $responsibleForChangeCpfOrCnpj);
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
	<h3>[SIMEC] INCLUIR PERMISS&Atilde;O DO USU&Aacute;RIO POR CPF/CNPJ (COM RESPONS&Aacute;VEL)</h3>
	<form method="POST">
		<label>CPF/CNPJ do Usu&aacute;rio:</label> <input type="text" name="cpfOrCnpj"/><br />
		<label>ID da Permiss&atilde;o:</label> <input type="text" name="permissionId"/><br />
		<label>CPF/CNPJ do Usu&aacute;rio Respons&aacute;vel:</label> <input type="text" name="responsibleForChangeCpfOrCnpj"/><br />
		<input type="submit" value="Enviar" name="submit"/>
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>
