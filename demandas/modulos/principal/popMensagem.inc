<html>
<head>
		<title>Mensagem de Aviso</title>
		<style type="text/css">
			
			@media print {.notprint { display: none }}

	
		</style>

		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		
</head>
<body>
<table width="100%" border="0" cellpadding="0" cellspacing="0" align="center" >
		<tr bgcolor="#ffffff">
			<td width="4%" style="border-bottom: solid 0px black;">
				<img src="../imagens/brasao.gif" width="45" height="45" border="0">
			</td>
			<td style="border-bottom: solid 0px black;">
				<?php echo NOME_SISTEMA; ?><br/>
				Mensagem de Aviso<br/>
			</td>

			<td align="right" style="border-bottom: solid 0px black;">
				Usuário: <? echo $_SESSION['usunome']; ?><br/>
				Data: 04/06/2009<?// echo date("d/m/Y  G:i:s"); ?>
			</td>
		</tr>
		<tr>
			<td colspan="3" align="center">
				<b><font style="font-size:14px;">AVISO</font></b>
			</td>
		</tr>
	</table> 
	


<table width="100%" align="center" border="0"  cellpadding="3" cellspacing="0">
	<tr>
		<td style="padding:15px; background-color:#fafafa; color:#404040; border-top: 1px solid black;" >
			<li>
				Por motivos de segurança, informamos que solicitações destinadas à rede MEC (criação de conta de novo usuário, acesso a diretórios em servidores, etc..) devem partir do responsável pelo setor, repassando as seguintes informações:
			</li>
			
			<br><br>
			
			<div style="padding-left:15px;">
				<b>Criação de conta de usuário:</b>
				<br>
				<li>Nome completo e localização (sala e ramal) do usuário;</li>
				<li>Período de utilização da conta;</li>
				<br>
				Ex.: Solicito criação de conta de e-mail e login para a funcionária: Carla da Silva Mota (SESU – 3 º andar – Sala 00 – ramal: 5555), a mesma é estagiária e seu contrato expira em 06 meses.
			</div>
			
			<br>

			<div style="padding-left:15px;">
				<b>Criação de e-mail institucional:</b>
				<br>
				<li>Qual o nome do e-mail a ser criado;</li>
				<li>Definir qual a utilização do mesmo, ou seja, o motivador;</li>
				<li>Nome completo e localização (sala e ramal) dos usuários que deverão ter acesso ao e-mail;</li>
				<li>Período de utilização do e-mail;</li>
				<br>
				Ex.: Solicito a criação do e-mail institucional <?php echo $_SESSION['email_sistema']; ?>, devido ao Programa de Educação, que será utilizado até o dia 31/12/2009. Segue a lista dos usuários que deverão ter acesso ao mesmo: Carla da Silva Mota (SESU – 3 º andar – Sala 00 – ramal: 5555) e Evandro Silva Torres (SESU – 3 º andar – Sala 00 – ramal: 5555).
			</div>
			
			<br>

			<div style="padding-left:15px;">
				<b>Acesso a diretórios:</b>
				<br>
				<li>Nome completo e localização (sala e ramal) do usuário;</li>
				<li>Caminho completo do diretório;</li>
				<li>O tipo de acesso de cada usuário (somente leitura ou acesso total);</li>
				<br>
				Ex.: Solicito acesso de leitura à pasta: \\mecsrv02\dti\programas\testes para a usuária Carla da Silva Mota (SESU – 3 º andar – Sala 00 – ramal: 5555).
				<br><br>
				Obs.: Os solicitantes de cada demanda serão responsáveis por todos os serviços e acessos de rede solicitados.
				<br>
				<li>Informamos que as demandas que não seguirem essa padronização serão desconsideradas, porém, o solicitante será informado por telefone ou e-mail.</li>
				<li>Esse tipo de iniciativa, informando todos os dados solicitados pela Rede, não só agiliza o atendimento de cada chamado, mas ajuda a manter um controle mínimo com relação à Segurança de Rede.</li>
			</div>
		</td>
	</tr>
	<tr>
		<td style="background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px; border-bottom: 1px solid black;" >
			&nbsp;
		</td>
	</tr>
</table>
		
</body>
</html>		

