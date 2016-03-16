<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE AUTENTICACAO POR ID E SENHA SEM APPLET");
			$SSDWs = new SSDWsAuth($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			msgOutput("requisitando autenticacao");
			$marker = $_POST['mark'];
			$resposta = $SSDWs->getAuthenticationByIdServletUrl($marker);
			msgOutput("Informacoes retornadas.");

			/*
			echo "<pre>";
				echo ($resposta->getUrl());
			echo "</pre>";
			*/
			
		} catch (Exception $e) {
			$erro = $e->getMessage();
			echo $erro;
			exit();
		}
?>
	<h3>RESPOSTA</h3>
	<ul>

		<script language=javascript>
			setTimeout("location.href='<?php echo ($resposta->getUrl()) ?>'", 2500);
		</script>

		<a href= <?php echo ($resposta->getUrl()) ?> >Redirecionando para a servlet de login...</a>
		
	</ul>
	<a href="javascript:history.back()">Voltar</a> |
<?
	} else {
?>
	<h3>TESTE DE AUTENTICACAO POR ID E SENHA POR SERVLET</h3>
	<form method="POST">
		<label>Flag:</label> <input type="text" name="mark" />
		<input type="submit" value="Enviar" name="submit">
	</form>

	<?php /*

	<h3>TESTE DE AUTENTICACAO POR ID E SENHA SEM APPLET</h3>
	<form method="POST">
		<div id="divHtmlRespostaSsdServlet">
			
			<?php echo $SSDWs->getAuthenticationByIdServletUrl( $resposta->getTicketId() ) ?>
			
			<?php /*<span>Aqui ser&aacute; inserido o formul&aacute;rio html montado pelo SSD.</span>*/	?>
	<?php /*		
		</div>
		<!--<label>Login:</label> <input type="text" name="login" />
		<label>Password:</label> <input type="text" name="password" />
		<input type="submit" value="Enviar" name="submit">-->
	</form>
	
	*/
	?>
	
	
<?php
	}
?>
<a href="../index.php">Menu Principal</a>