<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE USUARIO - CONSULTAR DADOS DE UM USUARIO");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
			}
			
			$login = $_POST['login'];
			$cpfOrCnpj = $_POST['cpfOrCnpj'];
			$email = $_POST['email'];						
			$nis = $_POST['nis'];
									
			msgOutput("buscando informacoes de usuario");			
			$resposta = $SSDWs->getUserAndPermissionInfo($login, $cpfOrCnpj, $email, $nis);
			msgOutput("Informacoes retornadas");
			
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

	<h3>TESTE DE USUARIO - CONSULTAR DADOS DE UM USUARIO</h3>
	
	<ul><b> INFORMACOES SOBRE O USUARIO </b></ul>	
	
	<ul>
		<li><b>Endere&ccedil;o: </b><?php echo $resposta->getUserInfo()->getAddress() ?></li>
		<li><b>E-mail Alternativo: </b><?php echo $resposta->getUserInfo()->getAlternativeEmail() ?></li>
		<li><b>Anivers&aacute;rio: </b><?php echo $resposta->getUserInfo()->getBirthDate() ?></li>
		<li><b>Celular: </b><?php echo $resposta->getUserInfo()->getCellPhoneNumber() ?></li>
		<li><b>Cidade: </b>
			<ul>
				<li><b>C&oacute;digo IBGE: </b><?php echo $resposta->getUserInfo()->getCityAddress()->getCodigoIBGE() ?></li>
				<li><b>Nome da Cidade: </b><?php echo $resposta->getUserInfo()->getCityAddress()->getDescricao() ?></li>
				<li><b>Estado: </b>
					<ul>
						<li><b>Nome: </b><?php echo $resposta->getUserInfo()->getCityAddress()->getEstado()->getDescricao() ?></li>		
						<li><b>Sigla: </b><?php echo $resposta->getUserInfo()->getCityAddress()->getEstado()->getSigla() ?></li>		
					</ul>
				</li>
				<li><b>Nome da Cidade: </b><?php echo $resposta->getUserInfo()->getCityAddress()->getSiglaEstado() ?></li>
			</ul>
		<li><b>CNPJ: </b><?php echo $resposta->getUserInfo()->getCnpj() ?></li>
		<li><b>CPF: </b><?php echo $resposta->getUserInfo()->getCpf() ?></li>
		<li><b>Ag&ecirc;ncia Despachante: </b><?php echo $resposta->getUserInfo()->getDispatcherAgency() ?></li>
		<li><b>E-mail: </b><?php echo $resposta->getUserInfo()->getEmail() ?></li>
		<li><b>Login: </b><?php echo $resposta->getUserInfo()->getLogin() ?></li>
		<li><b>Lota&ccedil;&atilde;o: </b><?php echo $resposta->getUserInfo()->getLotacao() ?></li>
		<li><b>Nome: </b><?php echo $resposta->getUserInfo()->getName() ?></li>
		<li><b>Nacionalidade: </b><?php echo $resposta->getUserInfo()->getNationality() ?></li>
		<li><b>Naturalidade: </b>
			<ul>
				<li>
					<b>Cidade: </b>
						<ul>
							<li><b>C&oacute;digo IBGE: </b><?php echo $resposta->getUserInfo()->getNaturality()->getCodigoIBGE() ?></li>
							<li><b>Nome da Cidade: </b><?php echo $resposta->getUserInfo()->getNaturality()->getDescricao() ?></li>
							<li><b>Estado: </b>
								<ul>
									<li><b>Nome: </b><?php echo $resposta->getUserInfo()->getNaturality()->getEstado()->getDescricao() ?></li>
									<li><b>Sigla: </b><?php echo $resposta->getUserInfo()->getNaturality()->getEstado()->getSigla() ?></li>
								</ul>
							</li>
							<li><b>Nome da Cidade: </b><?php echo $resposta->getUserInfo()->getNaturality()->getSiglaEstado() ?></li>
						</ul>
				</li>
			</ul>
		</li>
		<li><b>NIS: </b><?php echo $resposta->getUserInfo()->getNis() ?></li>
		<li><b>C&oacute;digo Postal: </b><?php echo $resposta->getUserInfo()->getPostalCode() ?></li>
		<li><b>CPF do Respons&aacute;vel: </b><?php echo $resposta->getUserInfo()->getResponsibleCpf() ?></li>
		<li><b>Nome do Respons&aacute;vel: </b><?php echo $resposta->getUserInfo()->getResponsibleName() ?></li>
		<li><b>RG: </b><?php echo $resposta->getUserInfo()->getRg() ?></li>
		<li><b>Raz&atilde;o Social: </b><?php echo $resposta->getUserInfo()->getSocialReason() ?></li>
		<li><b>Estado: </b>
			<ul>
				<li><b>Nome: </b><?php echo $resposta->getUserInfo()->getStateAddress()->getDescricao() ?></li>
				<li><b>Sigla: </b><?php echo $resposta->getUserInfo()->getStateAddress()->getSigla() ?></li>
			</ul>
		</li>
		<li><b>Telefone: </b><?php echo $resposta->getUserInfo()->getTelephoneNumber() ?></li>
		<li><b>Institui&ccedil;&atilde;o de Trabalho: </b><?php echo $resposta->getUserInfo()->getWorkInstitution() ?></li>
	</ul>	

	______________________________________________________________
	
	<ul><b> INFORMACOES SOBRE AS PERMISSOES DO USUARIO </b></ul>
	
	<?php foreach ( $resposta->getUserPermissionInfo() as $eachUserPermissionInfo ): ?>
	
	<ul>
		<li><b>Justificativa da Mudan&ccedil;a de Status: </b><?php echo $eachUserPermissionInfo->getJustificationOfStatusChange() ?></li>
		<li><b>Id da permiss&atilde;o: </b><?php $eachUserPermissionInfo->getPermissionId() ?> </b></li>
		<li><b>Dados obrigat&oacute;rios preenchidos: </b>
			<?php
				if ( strcmp($eachUserPermissionInfo->getRequiredDataStatus(), "true") == 0 )
					echo ("Sim");
				else
					echo ("Nao");
			?></li>
		<li><b>Id do respons&aacute;vel pela mudan&ccedil;a de status: </b><?php echo $eachUserPermissionInfo->getResponsibleIdForStatusChange() ?></li>
		<li><b>Status da Permiss&atilde;o do Usu&aacute;rio: </b><?php echo $eachUserPermissionInfo->getUserPermissionStatus() ?></li>
	</ul>
	
	<?php endforeach; ?>

<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>TESTE DE USUARIO - CONSULTAR DADOS DE UM USUARIO</h3>
	<form method="POST">
		<label>Login do usu&aacute;rio :</label> <input type="text" name="login" /><br />
		<label>CPF ou CNPJ :</label> <input type="text" name="cpfOrCnpj" /><br />
		<label>Email do usu&aacute;rio:</label> <input type="text" name="email" /><br />
		<label>NIS do usu&aacute;rio:</label> <input type="text" name="nis" /><br />
		<!-- <label>ID da Permiss&atilde;o:</label> <input type="text" name="permissionId" value="15"/><br /> -->
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>