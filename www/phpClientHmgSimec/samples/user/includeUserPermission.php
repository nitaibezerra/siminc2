<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE USUARIO - INCLUIR PERMISSAO PARA USUARIO");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			msgOutput("requisitando alteracao de status");
			$userId = (integer) $_POST['userId'];
			$permissionId = (integer) $_POST['permissionId'];
			$resposta = $SSDWs->includeUserPermission($userId, $permissionId);
			msgOutput("Alterado? " . $resposta);
			
			echo "</br>";
			
			/*
			echo "<pre>";
			var_dump($resposta);
			echo "</pre>";
			*/
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
	<h3>TESTE DE USUARIO - INCLUIR PERMISSAO PARA USUARIO</h3>
	<form method="POST">
		<label>ID do Usu&aacute;rio:</label> <input type="text" name="userId" value="1404"/><br />
		<label>ID da Permiss&atilde;o:</label> <input type="text" name="permissionId" value="14" /><br />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>