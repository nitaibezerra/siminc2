<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE USUARIO - BUSCAR USUARIOS DE ACORDO COM O STATUS DE PERMISSOES" );
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			$sts = ($_POST["status"]);
			
			//debug ($_POST["status"]);
			
			//var_dump($status);
			
			msgOutput("buscando informacoes de usuario");
			$resposta = $SSDWs->getUsersByPermissionStatus($sts);
			msgOutput("Informacoes retornadas");

			/*
			echo "<pre> resposta";
			var_dump($resposta);
			echo "</pre>";
			*/

		} catch (Exception $e) {
			$erro = $e->getMessage();
			echo $erro;
			exit();
		}
?>
	<h3>TESTE DE USUARIO - BUSCAR USUARIOS QUE POSSUEM PERMISSOES COM STATUS DE AGUARDANDO LIBERACAO</h3>
	<?php 
		foreach ( $resposta as $eachUserInfoAndPermissionId ): 
		/* @var $userAndPermissionId UserAndPermissionIds */
	?>
	
		<?php 
			//$userPermissionId = $userInfoAndPermissionId->permissionId;
			$eachUserInfo = $eachUserInfoAndPermissionId->getUserInfo();
			/*
			echo "eachUserInfo > ";
			echo "<pre>";
			var_dump($eachUserInfo);
			echo "<pre>";			
			*/
		?>
		
		<ul>
			<!-- <li><b>ID do Usu&aacute;rio:</b> <?php /* echo $userAndPermissionId->getUserId()*/ ?></li> -->
			<li><b>ID do Usu&aacute;rio</b> <?php echo $eachUserInfoAndPermissionId->getUserId() ?></li>
			<li><b>ID da Permiss&atilde;o:</b> <?php echo $eachUserInfoAndPermissionId->getUserPermissionId() ?></li>			
		</ul>	
			<?php /***************************************************************************************************************/ ?>

				<li><b>Endere&ccedil;o: </b><?php echo $eachUserInfo->getAddress() ?></li>
				<li><b>E-mail Alternativo: </b><?php echo $eachUserInfo->getAlternativeEmail() ?></li>
				<!---
				<li><b>Anivers&aacute;rio: </b><?php echo $eachUserInfo->getBirthDate() ?></li>
				--->
				<li><b>Celular: </b><?php echo $eachUserInfo->getCellPhoneNumber() ?></li>
				<!--
				<li><b>Cidade: </b>
				<ul>
					<li><b>C&oacute;digo IBGE: </b><?php echo $eachUserInfo->getCityAddress()->getCodigoIBGE() ?></li>
					<li><b>Nome da Cidade: </b><?php echo $eachUserInfo->getCityAddress()->getDescricao() ?></li>
					<li><b>Estado: </b>
						<ul>
							<li><b>Nome: </b><?php echo $eachUserInfo->getCityAddress()->getEstado()->getDescricao() ?></li>		
							<li><b>Sigla: </b><?php echo $eachUserInfo->getCityAddress()->getEstado()->getSigla() ?></li>		
						</ul>
					</li>
					<li><b>Nome da Cidade: </b><?php echo $eachUserInfo->getCityAddress()->getSiglaEstado() ?></li>
				</ul>
				--->
				<li><b>CNPJ: </b><?php echo $eachUserInfo->getCnpj() ?></li>
				<li><b>CPF: </b><?php echo $eachUserInfo->getCpf() ?></li>
				<li><b>Ag&ecirc;ncia Despachante: </b><?php echo $eachUserInfo->getDispatcherAgency() ?></li>
				<li><b>E-mail: </b><?php echo $eachUserInfo->getEmail() ?></li>
				<li><b>Login: </b><?php echo $eachUserInfo->getLogin() ?></li>
				<li><b>Lota&ccedil;&atilde;o: </b><?php echo $eachUserInfo->getLotacao() ?></li>
				<li><b>Nome: </b><?php echo $eachUserInfo->getName() ?></li>
				<li><b>Nacionalidade: </b><?php echo $eachUserInfo->getNationality() ?></li>
				<!---
				<li><b>Naturalidade: </b>
				<ul>
					<li>
						<b>Cidade: </b>
							<ul>
								<li><b>C&oacute;digo IBGE: </b><?php echo $eachUserInfo->getNaturality()->getCodigoIBGE() ?></li>
								<li><b>Nome da Cidade: </b><?php echo $eachUserInfo->getNaturality()->getDescricao() ?></li>
								<li><b>Estado: </b>
									<ul>
										<li><b>Nome: </b><?php echo $eachUserInfo->getNaturality()->getEstado()->getDescricao() ?></li>
										<li><b>Sigla: </b><?php echo $eachUserInfo->getNaturality()->getEstado()->getSigla() ?></li>
									</ul>
								</li>
								<li><b>Nome da Cidade: </b><?php echo $eachUserInfo->getNaturality()->getSiglaEstado() ?></li>
							</ul>
					</li>
				</ul>
			</li>
			--->
			<li><b>NIS: </b><?php echo $eachUserInfo->getNis() ?></li>
			<li><b>C&oacute;digo Postal: </b><?php echo $eachUserInfo->getPostalCode() ?></li>
			<li><b>CPF do Respons&aacute;vel: </b><?php echo $eachUserInfo->getResponsibleCpf() ?></li>
			<li><b>Nome do Respons&aacute;vel: </b><?php echo $eachUserInfo->getResponsibleName() ?></li>
			<li><b>RG: </b><?php echo $eachUserInfo->getRg() ?></li>
			<li><b>Raz&atilde;o Social: </b><?php echo $eachUserInfo->getSocialReason() ?></li>
			<!---
			<li><b>Estado: </b>
			<ul>
				<li><b>Nome: </b><?php echo $eachUserInfo->getStateAddress()->getDescricao() ?></li>
				<li><b>Sigla: </b><?php echo $eachUserInfo->getStateAddress()->getSigla() ?></li>
			</ul>			
			</li>
			--->
			<li><b>Telefone: </b><?php echo $eachUserInfo->getTelephoneNumber() ?></li>
			<li><b>Institui&ccedil;&atilde;o de Trabalho: </b><?php echo $eachUserInfo->getWorkInstitution() ?></li>
		</ul>
		
		<?php /***************************************************************************************************************/ ?>			
			
	<?php endforeach; ?>
	<br />
	<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>TESTE DE USUARIO - BUSCAR USUARIOS QUE POSSUEM PERMISSOES COM STATUS DE AGUARDANDO LIBERACAO</h3>
	<form method="POST">
		<label>Status (A = Ativo, B = Bloqueado, C = Cancelado, L = Aguardando liberação):</label> <input type="text" name="status"/><br />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>