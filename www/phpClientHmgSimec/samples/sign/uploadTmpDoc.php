<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();

		try {
			msgOutput("TESTE DE ASSINATURA - UPLOAD DE DOCUMENTO");
			$SSDWs = new SSDWSSignDocs($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);

			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}

			msgOutput("carregando documento");
			$marker = $_POST['flag'];
			//$cpf = $_POST['cpf'];

			$destino = "../docs/" . $_FILES['arquivo']['name'];

			//echo ($destino);

			//var_dump( ini_get('upload_max_filesize') );
			
			if(!move_uploaded_file($_FILES['arquivo']['tmp_name'], $destino)) {
				exit("Erro ao tentar fazer upload do arquivo.");
			}

			$file = $destino;

			// sem restri��o, cpf null
			$resposta = $SSDWs->loadDocumentForUserSigning($file, null, $marker);
			msgOutput("Documento enviado com sucesso.");
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
<h3>RESPOSTA</h3>

<?php echo $SSDWs->getAppletHtmlSampleCode( $resposta->getAppletTicketId() ) ?>

<a href="javascript:history.back()">Voltar</a> |
<?
	} else {
?>
	<h3>TESTE DE ASSINATURA - UPLOAD DE DOCUMENTO</h3>
	<form method="POST" enctype="multipart/form-data">
		<label>Flag:</label> <input type="text" name="flag" /><br />
		
		<?php //<label>Cpf:</label> <input type="text" name="cpf" /><br /> ?>
		
		<label>Arquivo:</label> <input type="file" name="arquivo" /><Br />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>