<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE ASSINATURA - DOWNLOAD DE PACOTE POR TIQUETE");
			$SSDWs = new SSDWSSignDocs($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			$ticketId = $_POST['ticketId'];
			msgOutput("requisitando informacoes. Ticket: " . $ticketId);
			$resposta = $SSDWs->downloadSignaturePackageByTicket($ticketId, $header);
			
			//$extension = substr(strstr($header['content_type'], "/" ), 1);
			$fileName = "../docs/packages/file" . date("dmYHis" ) . ".zip";
			//$fileName = substr( strstr( $header['Content-Disposition'] , "attachment; filename=" ) , 1 );
			
			echo "<br />";
			echo "<pre>";
			print_r($header);
			echo "</pre>";
			
			file_put_contents($fileName, $resposta);
			msgOutput("pacote retornado");
		} catch (Exception $e) {
			$erro = $e->getMessage();
			echo $erro;
			exit();
		}
?>
<h3>RESPOSTA</h3>
	<ul>
		<li><a href='<?php echo $fileName ?>'>Pacote para download</a></li>
	</ul>

	<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>TESTE DE ASSINATURA - DOWNLOAD DE PACOTE PELO T&Iacute;QUETE DO DOCUMENTO ASSINADO</h3>
	<form method="POST">
		<label>Ticket:</label><input type="text" name="ticketId" size="110" /><br />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>