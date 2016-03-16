<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE ASSINATURA - TIQUETE DE APPLET PARA ASSINATURA DE DOCUMENTO GERADO POR USUARIO");
			$SSDWs = new SSDWSSignDocs($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			msgOutput("requisitando ticket");
			$marker = $_POST['mark'];
			$resposta = $SSDWs->getAppletTicketInfoForUserDocSigning($marker);
			msgOutput("tiquete requisitado");
			
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
<h3>RESPOSTA</h3>
<!--
<ul>
	<li><b>ID do T&iacute;quete:</b> <?php echo $resposta->getTicketId() ?></li>
	<li><b>Data de Expira&ccedil;&atilde;o:</b><?php echo date("d/m/Y H:i:s", $resposta->getExpirationTimestamp()) ?></li>
	<li><b>Data de Cria&ccedil;&atilde;o:</b> <?php echo date("d/m/Y H:i:s", $resposta->getCreationTimestamp()) ?></li>
</ul>
-->
	<?php echo $SSDWs->getAppletHtmlSampleCode($resposta->getTicketId()) ?>
	<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>TESTE DE ASSINATURA - TIQUETE DE APPLET PARA ASSINATURA DE DOCUMENTO GERADO POR USUARIO</h3>
	<form method="POST">
		<label>Flag:</label> <input type="text" name="mark" />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>