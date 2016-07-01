<?php
	if (isset($_REQUEST['t'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		//$ticket = $_REQUEST['t'];
		
		try {
			msgOutput("TESTE DE INFORMACOES DE USUARIO AUTENTICADO");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			$ticket = $_POST["t"];
			
			msgOutput("Requisitando informacoes. Ticket: " . $ticket);
			$resposta = $SSDWs->getMaintenanceTicketInfo($flag);
			msgOutput("Informacoes retornadas.");
			
			debug($resposta);
			
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
<ul>
	<li>
		<b>Tipo de Autentica&ccedil;&atilde;o:</b>
		<ul>
			<li><b>Id:</b> <?php echo $resposta->getObjAuthType()->getId() ?></li>
			<li><b>Descri&ccedil;&atilde;o:</b> <?php echo $resposta->getObjAuthType()->getDescription() ?></li>
		</ul>
	</li>
	<li>
		<b>Flag:</b> <?php echo $resposta->getFlag() ?>
	</li>
	<li>
		<b>ID do Usu&aacute;rio:</b> <?php echo $resposta->getUserId() ?>
	</li>
	<li>
		<b>Identificador do Login:</b>
		<ul>
			<li><b>Field:</b><?php echo $resposta->getObjLoginIdentifier()->getField() ?></li>
			<li><b>Value:</b><?php echo $resposta->getObjLoginIdentifier()->getValue() ?></li>
		</ul>
	</li>
	<li>
		<b>Data do &Uacute;ltimo Update:</b><?php echo $resposta->getLastUpdateDate() ?>
	</li>
	<li>
		<b>Mensagens:</b>
		<ul>
			<?php foreach ( $resposta->getMessages() as $message ): ?>
				<li><b>Mensagem:</b> <?php echo $message ?></li>
			<?php endforeach; ?>
		</ul>
	</li>
	<li>
		<b>Campo:</b> <?php echo $resposta->getField() ?>
	</li>
	<li>
		<b>Permiss&otilde;es Inv&aacute;lidas:</b>
		<?php 
			foreach ($resposta->getInvalidPermission() as $invalidPermission): 
			/* @var $invalidPermission InvalidUserPermission */
		?>
			<ul>
				<li><b>Permiss&atilde;o:</b>
					<ul>
						<li><b>Id:</b><?php echo $invalidPermission->getPermissionId() ?></li>
						<li><b>Mensagens:</b>
							<ul>
								<?php foreach ($invalidPermission->getArrExceptionMessages() as $exceptionMessage): ?>
									<li><b>Mensagem:</b> <?php echo $exceptionMessage ?></li>
								<?php endforeach; ?>
							</ul>
						
						</li>
					</ul>
				</li>
			</ul>
		<? endforeach; ?>
	</li>
	<li>
		<b>Permiss&otilde;es V&aacute;lidas:</b>
		<ul>
			<?php foreach ($resposta->getValidPermission() as $validPermission): ?>
				<li>Permiss&atilde;o:</li> <?php echo $validPermission ?>
			<?php endforeach; ?>
		</ul>
	</li>
</ul>
<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>TESTE DE INFORMACOES DE USUARIO AUTENTICADO</h3>
	<form method="POST">
		<label>Qualquer coisa:</label> <input type="text" name="t" />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>
