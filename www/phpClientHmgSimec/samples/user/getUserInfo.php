<?php
	//if (isset($_REQUEST['appletAuthTicketId'])) {
	if (isset($_POST['submit'])) {		
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("TESTE DE USUARIO - BUSCAR INFORMACOES DE USUARIO");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODUCAO conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de homologacao conectado. WSDL baixada.");
				//msgOutput("WSDL INICIO");
				//debug($SSDWs->wsdl);
				//msgOutput("WSDL FIM");
			}
			
			msgOutput("buscando informacoes de usuario");
			
			$userId = $_POST['userId'];
			$userId =  (integer) $userId;

			$resposta = $SSDWs->getUserInfo($userId);
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
	<ul>
		<li><b>Endere&ccedil;o: </b><?php echo $resposta->getAddress() ?></li>
		<li><b>E-mail Alternativo: </b><?php echo $resposta->getAlternativeEmail() ?></li>
		<li><b>Anivers&aacute;rio: </b><?php echo $resposta->getBirthDate() ?></li>
		<li><b>Celular: </b><?php echo $resposta->getCellPhoneNumber() ?></li>
		<li><b>Cidade: </b>
			<ul>
				<li><b>C&oacute;digo IBGE: </b><?php echo $resposta->getCityAddress()->getCodigoIBGE() ?></li>
				<li><b>Nome da Cidade: </b><?php echo $resposta->getCityAddress()->getDescricao() ?></li>
				<li><b>Estado: </b>
					<ul>
						<li><b>Nome: </b><?php echo $resposta->getCityAddress()->getEstado()->getDescricao() ?></li>		
						<li><b>Sigla: </b><?php echo $resposta->getCityAddress()->getEstado()->getSigla() ?></li>		
					</ul>
				</li>
				<li><b>Nome da Cidade: </b><?php echo $resposta->getCityAddress()->getSiglaEstado() ?></li>
			</ul>
		<li><b>CNPJ: </b><?php echo $resposta->getCnpj() ?></li>
		<li><b>CPF: </b><?php echo $resposta->getCpf() ?></li>
		<li><b>Ag&ecirc;ncia Despachante: </b><?php echo $resposta->getDispatcherAgency() ?></li>
		<li><b>E-mail: </b><?php echo $resposta->getEmail() ?></li>
		<li><b>Login: </b><?php echo $resposta->getLogin() ?></li>
		<li><b>Lota&ccedil;&atilde;o: </b><?php echo $resposta->getLotacao() ?></li>
		<li><b>Nome: </b><?php echo $resposta->getName() ?></li>
		<li><b>Nacionalidade: </b><?php echo $resposta->getNationality() ?></li>
		<li><b>Naturalidade: </b>
			<ul>
				<li>
					<b>Cidade: </b>
						<ul>
							<li><b>C&oacute;digo IBGE: </b><?php echo $resposta->getNaturality()->getCodigoIBGE() ?></li>
							<li><b>Nome da Cidade: </b><?php echo $resposta->getNaturality()->getDescricao() ?></li>
							<li><b>Estado: </b>
								<ul>
									<li><b>Nome: </b><?php echo $resposta->getNaturality()->getEstado()->getDescricao() ?></li>
									<li><b>Sigla: </b><?php echo $resposta->getNaturality()->getEstado()->getSigla() ?></li>
								</ul>
							</li>
							<li><b>Nome da Cidade: </b><?php echo $resposta->getNaturality()->getSiglaEstado() ?></li>
						</ul>
				</li>
			</ul>
		</li>
		<li><b>NIS: </b><?php echo $resposta->getNis() ?></li>
		<li><b>C&oacute;digo Postal: </b><?php echo $resposta->getPostalCode() ?></li>
		<li><b>CPF do Respons&aacute;vel: </b><?php echo $resposta->getResponsibleCpf() ?></li>
		<li><b>Nome do Respons&aacute;vel: </b><?php echo $resposta->getResponsibleName() ?></li>
		<li><b>RG: </b><?php echo $resposta->getRg() ?></li>
		<li><b>Raz&atilde;o Social: </b><?php echo $resposta->getSocialReason() ?></li>
		<li><b>Estado: </b>
			<ul>
				<li><b>Nome: </b><?php echo $resposta->getStateAddress()->getDescricao() ?></li>
				<li><b>Sigla: </b><?php echo $resposta->getStateAddress()->getSigla() ?></li>
			</ul>
		</li>
		<li><b>Telefone: </b><?php echo $resposta->getTelephoneNumber() ?></li>
		<li><b>Institui&ccedil;&atilde;o de Trabalho: </b><?php echo $resposta->getWorkInstitution() ?></li>
	</ul>
	
	<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>TESTE DE USUARIO - BUSCAR INFORMACOES DO USUARIO</h3>
	<form method="POST">
		<label>ID do Usu&aacute;rio:</label> <input type="text" name="userId" value="58"/><br />
		<input type="submit" value="Enviar" name="submit">
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>
