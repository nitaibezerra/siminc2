<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE USUARIO - REQUISITA URL DE MANUTENCAO DE USUARIO");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			msgOutput("Requisitando url...");
			$flag = $_POST['flag'];
			$serviceId = $_POST['serviceId'];
			$resposta = $SSDWs->getUserMaintenanceUrl($flag, $serviceId);
			
			msgOutput("Informacoes retornadas.");
			
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

		<h3>TESTE DE USUARIO - REQUISITAR URL DE MANUTENCAO DE USUARIO</h3>
		<ul>
		
			<script language=javascript>
				setTimeout("location.href='<?php echo ($resposta->getUrl()) ?>'", 2500);
			</script>

			<a href= <?php echo ($resposta->getUrl()) ?> >Redirecionando para a URL de manutencao de usuario...</a>
		
		</ul>
		<a href="javascript:history.back()">Voltar</a> |	
<?
	} else {
?>
		<h3>TESTE DE USUARIO - REQUISITAR URL DE MANUTENCAO DE USUARIO</h3>
		<form method="POST">
			<label>Flag:</label> <input type="text" name="flag" /><br />
			<label>ID do Servi&ccedil;o:</label> <input type="text" name="serviceId" /><br />
			<input type="submit" value="Enviar" name="submit">
		</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>