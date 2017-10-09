<?php

//instancia o objeto do módulo
$academico = new academico();
pegaSessoes(1);
$prtid 			= $_SESSION["academico"]["prtid"];

// Cadastra as unidades selecionadas
if ( $_REQUEST['requisicao'] ){
	$academico->cadastraunidades( $_REQUEST['entid'], $prtid);
}

// cria o titulo da tela
$titulo_modulo = "Selecione as unidades desejadas";
monta_titulo( $titulo_modulo, "");


?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

	</head>
	<body>
		<form id="formulario" method="post" action="">
			<input type="hidden" name="requisicao" value="1"/>
			<table align="center" border="0" cellpadding="5" cellspacing="1" class="tabela" cellpadding="0" cellspacing="0">
				<tr>
					<td colspan="2" bgcolor="#CCCCCC">
						<input style="cursor:pointer;" type="submit" value="Salvar"/>
					</td>
				</tr>				
				<?php echo $academico->listaunidadeselecao( $_SESSION["academico"]['orgid'] ); ?>
				<tr>
					<td colspan="2" bgcolor="#CCCCCC">
						<input style="cursor:pointer;" type="submit" value="Salvar"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>