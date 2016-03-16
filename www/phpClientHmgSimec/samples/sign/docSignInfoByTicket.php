<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE ASSINATURA - INFORMACOES DE ASSINATURA DE DOCUMENTO POR TIQUETE");
			$SSDWs = new SSDWSSignDocs($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			msgOutput("requisitando informacoes");
			
			$signatureTicket = $_POST['ticket'];
			$resposta = $SSDWs->getDocumentSignatureInfoByTicket($signatureTicket);
			msgOutput("informacoes recebidas");
			echo "<pre>";
			var_dump($resposta);
			echo "</pre>";
		} catch (Exception $e) {
			$erro = $e->getMessage();
			echo $erro;
			exit();
		}
?>
<h3>RESPOSTA</h3>
<ul>
	<li><b>Protocolo:</b><?php echo $resposta->getProtocol() ?></li>
	<li><b>ID do T&iacute;quete:</b><?php echo $resposta->getTicketId() ?></li>
	<li><b>Hash da assinatura:</b><?php echo $resposta->getHash() ?></li>
	<li><b>Data de recebimento da assinatura:</b><?php echo $resposta->getReceivingDate() ?></li>
	<li><b>Tipo de assinatura:</b><?php echo $resposta->getSignatureType() ?></li>
	<li><b>Flag:</b><?php echo $resposta->getFlag() ?></li>
	<li>
		<b>Informa&ccedil;&otilde;es do Documento</b>
		<ul>
			<li>
				<b>Informa&ccedil;&otilde;es do Sistema Externo que Enviou o Documento</b>
				<ul>
					<li><b>ID:</b><?php echo $resposta->getObjDocumentInfo()->getObjExternalSystemInfo()->getId() ?></li>
					<li><b>Nome do Sistema:</b><?php echo $resposta->getObjDocumentInfo()->getObjExternalSystemInfo()->getName() ?></li>
				</ul>
			</li>
			<li><b>Hash do Documento:</b><?php echo $resposta->getObjDocumentInfo()->getHash() ?></li>
			<li><b>MimeType do Documento:</b><?php echo $resposta->getObjDocumentInfo()->getMimeType() ?></li>
			<li><b>Nome Original do Documento:</b><?php echo $resposta->getObjDocumentInfo()->getOriginalName() ?></li>
			<li><b>Data de recebimento do Documento:</b><?php echo $resposta->getObjDocumentInfo()->getReceivingDate() ?></li>
			<li><b>Tamanho do Documento:</b><?php echo $resposta->getObjDocumentInfo()->getSize() ?></li>
		</ul>
	</li>
	<li>
		<b>Informa&ccedil;&otilde;es do Sistema Externo que Requisitou a Assinatura do Documento</b>
		<ul>
			<li><b>ID:</b><?php echo $resposta->getObjExternalSystemInfo()->getId() ?></li>
			<li><b>Nome do Sistema:</b><?php echo $resposta->getObjExternalSystemInfo()->getName() ?></li>
		</ul>
	</li>
	<li>
		<b>Informa&ccedil;&otilde;es do Usu&aacute;rio:</b>
		<ul>
			<li><b>CPF:</b><?php echo $resposta->getObjUserBasicInfo()->getCpf() ?></li>
			<li><b>CNPJ:</b><?php echo $resposta->getObjUserBasicInfo()->getCnpj() ?></li>
			<li><b>ID:</b><?php echo $resposta->getObjUserBasicInfo()->getId() ?></li>
			<li><b>Nome:</b><?php echo $resposta->getObjUserBasicInfo()->getName() ?></li>
		</ul>
</ul>
	<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>TESTE DE ASSINATURA - INFORMACOES DE ASSINATURA DE DOCUMENTO POR TIQUETE</h3>
	<form method="POST">
		<label>Ticket:</label><input type="text" name="ticket" size="110" /><br />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>