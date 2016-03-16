<?php

	/**
	 * Sistema Integrado de Monitoramento do Minist�rio da Educa��o
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Programadores: Ren� de Lima Barbosa <renedelima@gmail.com>
	 * M�dulo: Seguran�a
	 * Finalidade: Permite que o usu�rio solicite uma nova senha.
	 * �ltima modifica��o: 26/08/2006
	 */

	function erro(){
		global $db;
		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'] = func_get_args();
		header( "Location: ". $_SERVER['PHP_SELF'] );
		exit();
	}

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conex�o com o servidor de banco de dados
	$db = new cls_banco();

	// executa a rotina de recupera��o de senha quando o formul�rio for submetido
	if ( $_POST['formulario'] ) {
		
		// verifica se a conta est� ativa
		$sql = sprintf(
			"SELECT u.* FROM seguranca.usuario u WHERE u.usucpf = '%s'",
			corrige_cpf( $_REQUEST['usucpf'] )
		);
		$usuario = (object) $db->pegaLinha( $sql );
		if ( $usuario->suscod != 'A' ) {
			erro( "A conta n�o est� ativa." );
		}
		
		$_SESSION['mnuid'] = 10;
		$_SESSION['sisid'] = 4;
		$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
		$_SESSION['usucpf'] = $usuario->usucpf;
		$_SESSION['usucpforigem'] = $usuario->usucpf;
		
		// cria uma nova senha
	    //$senha = $db->gerar_senha();
	    $senha = strtoupper(senha());
		$sql = sprintf(
			"UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 'f' WHERE usucpf = '%s'",
			md5_encrypt_senha( $senha, '' ),
			$usuario->usucpf
		);
		$db->executar( $sql );
		
		// envia email de confirma��o
		$sql = "select ittemail from public.instituicao where ittstatus = 'A'";
		$remetente = $db->pegaUm( $sql );
		$destinatario = $usuario->usuemail;
		$assunto = "Simec - Recupera��o de Senha";
	    $conteudo = sprintf(
	    	"%s %s<br/>Sua senha foi alterada para %s<br>Ao se conectar, altere esta senha para a sua senha preferida.",
	    	$usuario->ususexo == 'F' ?  'Prezada Sra.': 'Prezado Sr.',
	    	$usuario->usunome,
	    	$senha
	    );
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		
		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'][] = "Recupera��o de senha conclu�da. Em breve voc� receber� uma nova senha por email.";
		header( "Location: /demandas/login.php" );
		exit();
	}

	if ( $_REQUEST['expirou'] ) {
		$_SESSION['MSG_AVISO'][] = "Sua conex�o expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
	}

?>
<html>
	<head>
		<title>Simec - Minist�rio da Educa��o</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<style type=text/css>
			form {
				margin: 0px;
			}
		</style>
	</head>
	<body bgcolor=#ffffff vlink=#666666 bottommargin="0" topmargin="0" marginheight="0" marginwidth="0" rightmargin="0" leftmargin="0">
		<?php include "cabecalho.php"; ?>
		<br/>
		<?php
			$mensagens = '<p style="align: center; color: red; font-size: 12px">'. implode( '<br/>', (array) $_SESSION['MSG_AVISO'] ) . '</p>';
			$_SESSION['MSG_AVISO'] = null;
			$titulo_modulo = 'Recupera��o de Senha';
			$subtitulo_modulo = 'Digite seu CPF e pressione o bot�o "Lembrar Senha".<br/>O Sistema enviar� um e-mail para voc� contendo uma nova senha de acesso.<br/>'. obrigatorio() .' Indica Campo Obrigat�rio.'. $mensagens;
			monta_titulo( $titulo_modulo, $subtitulo_modulo );
		?>
		<form method="POST" name="formulario">
			<input type=hidden name="formulario" value="1"/>
			<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
			<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
				<tr bgcolor="#F2F2F2">
					<td align = 'right' class="subtitulodireita" width="150px">CPF:</td>
					<td>
						<input type="text" name="usucpf" value="" size="20" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal" onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);">
						<?= obrigatorio(); ?>
					</td>
			 	</tr>
				<tr bgcolor="#C0C0C0">
					<td>&nbsp;</td>
					<td>
						<input type="button" name="btinserir" value="Lembrar Senha" onclick="enviar_formulario()"/>
						&nbsp;&nbsp;&nbsp;
						<input type="Button" value="Voltar" onclick="location.href='./login.php'"/>
					</td>
				</tr>
			</table>
		</form>
		<br/>
		<?php include "./rodape.php"; ?>
	</body>
</html>
<script language="javascript">

	document.formulario.usucpf.focus();

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( !validar_cpf( document.formulario.usucpf.value ) ) {
			mensagem += '\nO cpf informado n�o � v�lido.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

</script>