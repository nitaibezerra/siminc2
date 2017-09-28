<?php 

if( $_GET['grtid'] )
{
	$grtconcluido = $db->pegaUm("SELECT grtconcluido FROM pdeinterativo.grupotrabalho WHERE grtid = ".$_GET['grtid']);
	
	salvarAbaResposta("primeiros_passos_passo_3");
	 
	if( $grtconcluido == 'f' )
	{
		$db->executar("UPDATE pdeinterativo.grupotrabalho SET grtconcluido = 't' WHERE grtid = ".$_GET['grtid']);
		$db->commit();
	}
}

?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
		<script type="text/javascript">

		$(document).ready(function()
		{
			$(window).bind("beforeunload", function()
			{
				window.opener.location.href = "pdeinterativo.php?modulo=principal/diagnostico&acao=A";
			});
		});
		
		function fecharJanela()
		{
			$(window).unbind("beforeunload");
			window.opener.location.href = "pdeinterativo.php?modulo=principal/diagnostico&acao=A";
			self.close();
		}
		
		</script>
	</head>
	<body>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="10" cellPadding="10" align="center">
			<tr>
				<td style="text-align:center">
					Muito bem!
					<br />
					A escola concluiu a etapa de Identificação e os Primeiros Passos!
					<br /><br />
					Agora, vamos elaborar o diagnóstico da escola. Leia atentamente as orientações que antecedem cada passo.
				</td>
			</tr>
			<tr>
				<td align="center">
					<input type="button" value="Fechar" onclick="fecharJanela();" />
				</td>
			</tr>
		</table>
	</body>
</html>