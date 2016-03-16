<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE USUARIO - BUSCAR IDENTIFICADOR DO USUARIO PELO TIQUETE DE AUTENTICACAO DESTE USUARIO");
			$SSDWs = new SSDWsUser( $tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			msgOutput("buscando ticket do usuario");
			$ticketUserId = $_POST['ticketUserId'];
			$resposta = $SSDWs->getTicketUserInfoByUserTicketId($ticketUserId);
			msgOutput("Informacoes retornadas");
			echo "<pre>";
			var_dump($resposta);
			echo "</pre>";
		} catch (Exception $e) {
			$erro = $e->getMessage();
			echo $erro;
			exit();
		}
?>
	<a href="javascript:history.back()">Voltar</a> | 
	<h3>TESTE DE USUARIO - BUSCAR IDENTIFICADOR DO USUARIO PELO TIQUETE DE AUTENTICACAO DESTE USUARIO</h3>
	<ul>
		<li><b>Flag:</b><?php echo $resposta->getFlag() ?></li>	
		<li><b>ID do Usu&aacute;rio:</b><?php echo $resposta->getUserId() ?></li>	
	</ul>
<?
	} else {
?>
	<h3>TESTE DE USUARIO - BUSCAR IDENTIFICADOR DO USUARIO PELO TIQUETE DE AUTENTICACAO DESTE USUARIO</h3>
	<form method="POST">
		<label>Ticket do Usu&aacute;rio:</label> <input type="text" name="ticketUserId" /><br />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>