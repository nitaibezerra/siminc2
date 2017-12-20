<?php
	
	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	
	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();
	
	// grava informações na sessão para que os registros de auditoria sejam persistidos
	$_SESSION['usucpf'] = '';
	$_SESSION['usucpforigem'] = '';
	
?>
<html>
	<head>
		<title>Simec - Ministério da Educação</title>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"/>
	</head>
	<body>
		<?php
			
			// compila os dados da mensagem
			$remetente = '';
			$destinatarios = array( '', '' );
			$assunto = 'assunto qualquer';
			$conteudo = 'conteudo qualquer';
			$anexos = array(
				APPRAIZ . 'www/imagens/logo_brasil.gif',
				APPRAIZ . 'remanejamento.rtf'
			);
			
			// carrega a classe e envia a mensagem
			include_once APPRAIZ . "includes/Email.php";
			$mensagem = new Email();
			if($mensagem->enviar( $destinatarios, $assunto, $conteudo, $anexos, false )){
				echo 'Enviou 1';
			}
			
			$email = array($_SESSION['email_sistema']);			
			
			$remetente = array('nome'=>'Programação Orçamentária - Descetralização de Crédito', 'email'=>$_SESSION['email_sistema']);
			
			$assunto  = "O termo de cooperação 123 necessita de ajustes.";
			
			$conteudo = "<p>teste@teste.com</p><p>O termo de cooperação 123 necessita de ajustes.</p>";
			
			
			enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco );
			if(enviar_email($remetente, $email, $assunto, $conteudo, $cc, $cco )){
				echo "email 3";
			}
			
			// persiste os dados no banco
// 			$db->commit();
			
			
			//Estou a receber o formulário, componho o corpo
			$corpo = "Formulário enviado\n";
			$corpo .= "Nome: teste\n";
			$corpo .= "Email: teste@teste.com\n";
			$corpo .= "Comentários: teste de comentario\n";
			
			//envio o correio...
			mail($_SESSION['email_sistema'],"Formulário recebido teste",$corpo);
			
// 			if(mail("maykelsb@gmail.com","Formulário recebido teste",$corpo)){
			if(mail($_SESSION['email_sistema'],"Formulário recebido teste",$corpo)){
				echo "enviou 2";	
			}
			
		?>
	</body>
</html>