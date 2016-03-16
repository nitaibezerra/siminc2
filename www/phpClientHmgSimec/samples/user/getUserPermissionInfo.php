<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE USUARIO - BUSCAR INFORMACOES DE PERMISSOES DO USUARIO");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			msgOutput("buscando informacoes de permissao");
			$userId = (integer) $_POST['userId'];
			
			//$permissionId = $_POST['permissionId'];
			
			$resposta = $SSDWs->getUserPermissionsInfo($userId/*, $permissionId*/);
			msgOutput("Informacoes retornadas");
			
			debug ($resposta);
			
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

	<h3>TESTE DE USUARIO - BUSCAR INFORMACOES DE PERMISSAO DO USUARIO</h3>
	
	<?php foreach ( $resposta as $eachUserPermissionId ): ?>
	
	<ul>
		<li><b>Justificativa da Mudan&ccedil;a de Status: </b><?php echo $eachUserPermissionId->getJustificationOfStatusChange() ?></li>
		<li><b>Id da permiss&atilde;o: </b><?php $eachUserPermissionId->getPermissionId() ?> </b></li>
		<li><b>Dados obrigat&oacute;rios preenchidos: </b>
			<?php
				if ( strcmp($eachUserPermissionId->getRequiredDataStatus(), "true") == 0 )
					echo ("Sim");
				else
					echo ("Nao");
			?></li>
		<li><b>Id do respons&aacute;vel pela mudan&ccedil;a de status: </b><?php echo $eachUserPermissionId->getResponsibleIdForStatusChange() ?></li>
		<li><b>Status da Permiss&atilde;o do Usu&aacute;rio: </b><?php echo $eachUserPermissionId->getUserPermissionStatus() ?></li>
	</ul>
	
	<?php endforeach; ?>

<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>TESTE DE USUARIO - BUSCAR INFORMACOES DE PERMISSOES DO USUARIO</h3>
	<form method="POST">
		<label>ID do Usu&aacute;rio:</label> <input type="text" name="userId" value="1404"/><br />
		<!-- <label>ID da Permiss&atilde;o:</label> <input type="text" name="permissionId" value="15"/><br /> -->
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>
