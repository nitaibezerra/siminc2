<html>
	<head>
		<title>CLIENTE DE TESTE PHP DE SERVI&Ccedil;OS DO SSD</title>
	</head>
	<style>
		*{
			font-family:verdana;
		}
		li{
			display:block;
			padding:2px;
			font-size:12px;
		}
		a, a:hover{
			color:#3B6F14;
		}
	</style>
	<body>
		<h3>PHP CLIENT FOR SSD</h3>
		<ul>
			<li>
				<b>AUTENTICA&Ccedil;&Atilde;O</b>
				<ul>
					<li>
						<b>SIMEC</b> <a href="auth/loginUserIntoSystemByCPFOrCNPJAndPassword.php">Logar Usu&aacute;rio no Sistema por CPF/CNPJ e Senha</a>
					</li>
					<li>
						<a href="auth/authByIdWithoutApplet.php">Autentica&ccedil;&atilde;o de Usu&aacute;rio por Identificador e Senha (servlet)</a>
					</li>
					<li>
						<a href="auth/authById.php">Autentica&ccedil;&atilde;o de Usu&aacute;rio por Identificador e Senha (applet)</a>
					</li>
					<li>
						<a href="auth/authByCert.php">Autentica&ccedil;&atilde;o de Usu&aacute;rio por Certificado Digital</a>
					</li>
					<li>
						<a href="auth/authExternalUser.php">Autentica&ccedil;&atilde;o de Usu&aacute;rio <b>Externo</b> por Certificado Digital</a> <b>(NÃO FUNCIONA AINDA!)</b>
					</li>										
					<li>
						<a href="auth/authByBoth.php">Autentica&ccedil;&atilde;o de Usu&aacute;rio por Identificador ou Certificado Digital</a>
					</li>
					<li>
						<a href="auth/userAuthInfo.php">Informa&ccedil;&otilde;es de Usu&aacute;rio</a>
					</li>
					<li>
						<a href="auth/externalUserInfo.php">Buscar informa&ccedil;&otilde;es de Usu&aacute;rio <b>Externo</b></a> <b>(NÃO FUNCIONA AINDA!)</b>
					</li>					
				</ul>
			</li>
			<li>
				<b>ASSINATURA</b>
				<ul>
					<li>
						<a href="sign/uploadTmpDoc.php">Upload de Documento e Assinatura</a></b>
					</li>
					<li>
						<a href="sign/uploadTmpDocWithRestriction.php">Upload de Documento e Assinatura com Restrição</a>
					</li>					
					<li>
						<a href="sign/downloadPackByProtocol.php">Download de Pacote Assinado por Protocolo</a> 
					</li>
					<li>
						<a href="sign/downloadPackByTicket.php">Download de Pacote Assinado por Ticket</a> 
					</li>
					<li>
						<a href="sign/appletTicketForSign.php">Ticket de Applet Para Assinatura de Documento Gerado por Usu&aacute;rio</a>
					</li>
					<li>
						<a href="sign/appletTicketForSignWithRestriction.php">Ticket de Applet Para Assinatura de Documento Gerado por Usu&aacute;rio com Restri&ccedil;&atilde;o</a>
					</li>
					<li>
						<a href="sign/docSignInfoByProtocol.php">Informa&ccedil;&otilde;es de Assinatura do Documento por Protocolo</a> 
					</li>
					<li>
						<a href="sign/docSignInfoByTicket.php">Informa&ccedil;&otilde;es de Assinatura do Documento por Ticket</a> 
					</li>
				</ul>
			</li>
			<li>
				<b>MANUTEN&Ccedil;&Atilde;O DE USU&Aacute;RIO</b>
				<ul>
					<li>
						<b>SIMEC</b> <a href="user/changeUserPermissionStatusByCPFOrCNPJ.php">Alterar Status da Permiss&atilde;o do Usu&aacute;rio por CPF/CNPJ <b>(Com Respons&aacute;vel)</b></a>
					</li>
					<li>
						<b>SIMEC</b> <a href="user/includeUserPermissionByCPFOrCNPJ.php">Incluir Permiss&atilde;o do Usu&aacute;rio por CPF/CNPJ <b>(Com Respons&aacute;vel)</b></a>
					</li>					
					<li>
						<b>SIMEC</b> <a href="user/getUserInfoByCPFOrCNPJ.php">Recuperar Informa&ccedil;&otilde;es do Usu&aacute;rio por CPF/CNPJ</a>
					</li>					
					<li>
						<b>SIMEC</b> <a href="user/changeUserPasswordByCPFOrCNPJ.php">Alterar Senha do Usu&aacute;rio por CPF/CNPJ</a>
					</li>					
					<li>
						<b>SIMEC</b> <a href="user/signUpUser.php">Cadastrar Usu&aacute;rio</a>
					</li>					
					<li>
						<b>SIMEC</b> <a href="user/updateUser.php">Atualizar Usu&aacute;rio</a>
					</li>					
					<li>
						<b>SIMEC</b> <a href="user/recoveryUserPasswordByCPFOrCNPJ.php">Recuperar Senha do Usu&aacute;rio por CPF/CNPJ</a>
					</li>					
					<li>
						<b>SIMEC</b> <a href="user/recoveryUserPasswordByHashAndCode.php">Recuperar Senha do Usu&aacute;rio por Hash e C&oacute;digo</a>
					</li>					
					<li>
						<a href="user/getUserMaintenanceUrl.php">Requisitar a Url de manuten&ccedil;&atilde;o de Usu&aacute;rio</a>
					</li>				
					<li>
						<a href="user/changeUserPermissionStatus.php">Alterar status de permiss&atilde;o de usu&aacute;rio</a> 
					</li>
					<li>
						<a href="user/changeUserPermissionStatusWithResponsible.php">Alterar status de permiss&atilde;o de usu&aacute;rio <b>(com respons&aacute;vel)</b></a>
					</li>
					<li>
						<a href="user/includeUserPermission.php">Incluir permiss&atilde;o para usu&aacute;rio</a> 
					</li>
					<li>
						<a href="user/includeUserPermissionWithResponsible.php">Incluir permiss&atilde;o para usu&aacute;rio <b>(com respons&aacute;vel)</b></a>
					</li>					
					<li>
						<a href="user/changeUserPermission.php">Alterar Permiss&atilde;o de Usu&aacute;rio</a> 
					</li>
					<li>
						<a href="user/changeUserPermissionWithResponsible.php">Alterar Permiss&atilde;o de Usu&aacute;rio <b>(com respons&aacute;vel)</b></a>
					</li>					
					<li>
						<a href="user/getUserInfo.php">Buscar informa&ccedil;&otilde;es de Usu&aacute;rio</a>
					</li>
					<li>
						<a href="user/getUsersWaitingforPermissionRelease.php">Buscar Usuarios que possuem permiss&otilde;es com status "Aguardando libera&ccedil;&atilde;o"</a>
					</li>
					<li>
						<a href="user/getUserPermissionInfo.php">Buscar informa&ccedil;&otilde;es de Permiss&atilde;o de Usu&aacute;rio</a>
					</li>
					<li>
						<a href="user/getSystemPermissionsInfo.php">Consultar os Perfis do sistema</a>
					</li>
					<li>
						<a href="user/getUserAndPermissionInfo.php">Consultar dados de usuário e permissões no sistema por CPF, CNPJ, NIS, login ou email</a>
					</li>										
					<li>
						<a href="user/getTicketUserInfoByUserTicketId.php">Buscar Identificador de um Usu&aacute;rio pelo Ticket de Autentica&ccedil;&atilde;o deste Usu&aacute;rio</a> 
					</li>
					<li>
						<a href="user/getUsersByPermissionStatus.php">Buscar Usuarios pelo status de permissao</a>  <b> !NOVO! </b>
					</li>					
					<li>
						<a href="user/getUserInfoByOneOfTheIdentifiers.php">Consultar dados de usuário por por CPF, CNPJ, NIS, login ou email</a>  <b> !NOVO! </b>
					</li>
					<li>
						</ br>
					</li>	
					<li>
						</ br>
					</li>	
					<li>
						<a href="user/getMaintenanceTicketInfo.php">Gerar ticket URL do admin (1)</a>  <b> !TESTE! </b>
					</li>					
					<li>
						<a href="admin/getMaintenanceTicket.php">Decriptografar ticket (URL) admin (2)</a>  <b> !TESTE! </b>
					</li>										
					<li>
						<a href="admin/getUserMaintenanceTicketInfo.php">Gerar ticket USER do admin (3)</a>  <b> !TESTE! </b>
					</li>										
					<li>
						<a href="user/getUserMaintenanceTicket.php">Decriptografar ticket (USER) admin (4)</a>  <b> !TESTE! </b>
					</li>					
					<li>
						</ br>
					</li>	
					<li>
						<a href="user/getDateByTimestamp.php">Converter timestamp para data (no server)</a>  <b> !TESTE! </b>
					</li>

				</ul>
		</ul>
	</body>
</html>
