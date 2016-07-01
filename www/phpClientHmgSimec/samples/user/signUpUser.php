<?php
	if (isset($_POST['submit'])) {
		require_once("../connector.php");
		require("../debug.php");
		header("Content-Type: text/html; charset=utf-8");
		ob_start();
		
		try {
			msgOutput("[SIMEC] CADASTRAR USU&Aacute;RIO");
			$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
			
			msgOutput("Conectando...");
			if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
				$SSDWs->useProductionSSDServices();
				msgOutput("Servidor de PRODU&Ccedil;&Atilde;O conectado. WSDL baixada.");
			} else {
				$SSDWs->useHomologationSSDServices();
				msgOutput("Servidor de HOMOLOGA&Ccedil;&Atilde;O conectado. WSDL baixada.");
			}
			
			$senha = @utf8_encode(base64_encode($_POST["senha"]));
			$tipo_pessoa = @utf8_encode($_POST["tipo_pessoa"]);
			$nome = @utf8_encode($_POST["nome"]);
			$cnpj = @utf8_encode($_POST["cnpj"]);
			$nome_responsavel = @utf8_encode($_POST["nome_responsavel"]);
			$cpf_responsavel = @utf8_encode($_POST["cpf_responsavel"]);
			$nome_mae = @utf8_encode($_POST["nome_mae"]);
			$cpf = @utf8_encode($_POST["cpf"]);
			$rg = @utf8_encode($_POST["rg"]);
			$sigla_orgao_expedidor = @utf8_encode($_POST["sigla_orgao_expedidor"]);
			$orgao_expedidor = @utf8_encode($_POST["orgao_expedidor"]);
			$nis = @utf8_encode($_POST["nis"]);
			$data_nascimento = @utf8_encode($_POST["data_nascimento"]);
			$codigo_municipio_naturalidade = @utf8_encode($_POST["codigo_municipio_naturalidade"]);
			$codigo_nacionalidade = @utf8_encode($_POST["codigo_nacionalidade"]);
			$email = @utf8_encode($_POST["email"]);
			$email_alternativo = @utf8_encode($_POST["email_alternativo"]);
			$cep = @utf8_encode($_POST["cep"]);
			$endereco = @utf8_encode($_POST["endereco"]);
			$sigla_uf_cep = @utf8_encode($_POST["sigla_uf_cep"]);
			$localidade = @utf8_encode($_POST["localidade"]);
			$bairro = @utf8_encode($_POST["bairro"]);
			$complemento = @utf8_encode($_POST["complemento"]);
			$numero_endereco = @utf8_encode($_POST["numero_endereco"]);
			$ddd_telefone = @utf8_encode($_POST["ddd_telefone"]);
			$telefone = @utf8_encode($_POST["telefone"]);
			$ddd_telefone_alternativo = @utf8_encode($_POST["ddd_telefone_alternativo"]);
			$telefone_alternativo = @utf8_encode($_POST["telefone_alternativo"]);
			$ddd_celular = @utf8_encode($_POST["ddd_celular"]);
			$celular = @utf8_encode($_POST["celular"]);
			$instituicao_trabalho = @utf8_encode($_POST["instituicao_trabalho"]);
			$lotacao = @utf8_encode($_POST["lotacao"]);
			if ($tipo_pessoa === "F") {
				$userInfo = "$senha||$tipo_pessoa||$nome||$nome_mae||$cpf||$rg||$sigla_orgao_expedidor||$orgao_expedidor||$nis||" .
							"$data_nascimento||$codigo_municipio_naturalidade||$codigo_nacionalidade||$email||$email_alternativo||" .
							"$cep||$endereco||$sigla_uf_cep||$localidade||$bairro||$complemento||$endereco||$ddd_telefone||$telefone||" .
							"$ddd_telefone_alternativo||$telefone_alternativo||$ddd_celular||$celular||$instituicao_trabalho||$lotacao||ssd";
			} else if ($tipo_pessoa === "J") {
				$userInfo = "$senha||$tipo_pessoa||$nome||$cnpj||$nome_responsavel||$cpf_responsavel||$email||$email_alternativo||" .
							"$cep||$endereco||$sigla_uf_cep||$localidade||$bairro||$complemento||$endereco||$ddd_telefone||$telefone||" .
							"$ddd_telefone_alternativo||$telefone_alternativo||$ddd_celular||$celular||ssd";
			}
			
			echo "<pre>";
			print_r($userInfo);
			echo "</pre>";
			
			$resposta = $SSDWs->signUpUser($userInfo);
			echo "<pre>";
			print_r($resposta);
			echo "</pre>";
			
		} catch (Exception $e) {
			$erro = $e->getMessage();
			echo $erro;
			exit();
		}
?>
	<a href="javascript:history.back()">Voltar</a> | 
<?
	} else {
?>
	<h3>[SIMEC] CADASTRAR USU&Aacute;RIO</h3>
	<script type="text/javascript">
		function mudarDiv(select) {
			if (select.selectedIndex == 0) {
				document.getElementById("juridica").style.display = "none";
				document.getElementById("fisica_1").style.display = "block";
				document.getElementById("fisica_2").style.display = "block";
			} else {
				document.getElementById("fisica_1").style.display = "none";
				document.getElementById("fisica_2").style.display = "none";
				document.getElementById("juridica").style.display = "block";
			}
		}
		window.onload = function(e) {
			mudarDiv(document.getElementById("tipo_pessoa"));
		}
	</script>
	<form method="POST">
		Senha:
		<input type="password" name="senha"/><br/>
		Tipo de Pessoa:
		<select id="tipo_pessoa" name="tipo_pessoa" onchange="javascript:mudarDiv(this);">
			<option value="F">F&iacute;sica</option>
			<option value="J">Jur&iacute;dica</option>
		</select><br/>
		Nome:
		<input type="text" name="nome"/><br/>
		<div id="juridica">
			CNPJ:
			<input type="text" name="cnpj"/><br/>
			Nome do Respons&aacute;vel:
			<input type="text" name="nome_responsavel"/><br/>
			CPF do Respons&aacute;vel:
			<input type="text" name="cpf_responsavel"/><br/>
		</div>
		<div id="fisica_1">
			Nome da m&atilde;e:
			<input type="text" name="nome_mae"/><br/>
			CPF:
			<input type="text" name="cpf"/><br/>
			RG:
			<input type="text" name="rg"/><br/>
			Sigla UF do Org&atilde;o Expedidor:
			<input type="text" name="sigla_orgao_expedidor"/><br/>
			Org&atilde;o Expedidor:
			<input type="text" name="orgao_expedidor"/><br/>
			NIS:
			<input type="text" name="nis"/><br/>
			Data de Nascimento (formato: AAAA-MM-DD):
			<input type="text" name="data_nascimento"/><br/>
			C&oacute;digo Munic&iacute;pio Naturalidade:
			<input type="text" name="codigo_municipio_naturalidade"/><br/>
			C&oacute;digo Nacionalidade:
			<input type="text" name="codigo_nacionalidade"/><br/>
		</div>
		Email:
		<input type="text" name="email"/><br/>
		Email Alternativo:
		<input type="text" name="email_alternativo"/><br/>
		CEP:
		<input type="text" name="cep"/><br/>
		Endere&ccedil;o:
		<input type="text" name="endereco"/><br/>
		Sigla UF CEP:
		<input type="text" name="sigla_uf_cep"/><br/>
		Localidade:
		<input type="text" name="localidade"/><br/>
		Bairro:
		<input type="text" name="bairro"/><br/>
		Complemento:
		<input type="text" name="complemento"/><br/>
		N&uacute;mero Endere&ccedil;o:
		<input type="text" name="numero_endereco"/><br/>
		DDD Telefone:
		<input type="text" name="ddd_telefone"/><br/>
		Telefone:
		<input type="text" name="telefone"/><br/>
		DDD Telefone Alternativo:
		<input type="text" name="ddd_telefone_alternativo"/><br/>
		Telefone Alternativo:
		<input type="text" name="telefone_alternativo"/><br/>
		DDD Celular:
		<input type="text" name="ddd_celular"/><br/>
		Celular:
		<input type="text" name="celular"/><br/>
		<div id="fisica_2">
			Institui&ccedil;&atilde;o de Trabalho:
			<input type="text" name="instituicao_trabalho"/><br/>
			Lota&ccedil;&atilde;o:
			<input type="text" name="lotacao"/><br/>
		</div>
		<input type="submit" value="Enviar" name="submit"/>
	</form>
<?php
	}
?>
<a href="../index.php">Menu Principal</a>
