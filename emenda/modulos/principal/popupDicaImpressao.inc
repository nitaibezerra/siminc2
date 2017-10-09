<html>
<head>
	<title><?php echo NOME_SISTEMA; ?></title>
	<script type="text/javascript" src="../includes/prototype.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<body>
	<table class="tabela" align="center" cellspacing="1" cellpadding="4" >
		<tr>
			<td style="text-align: center;">				
				<b>Ministério da Educação</b> <br/>
				FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO <br/>
				<?php echo NOME_SISTEMA; ?>
			</td>
		</tr>
	</table>		
	<br/>
	<table class="tabela" align="center" cellspacing="1" cellpadding="4" >
		<tr>
			<td>
				Para realizar a impressão corretamente, favor configurar o seu browser conforme as instruções abaixo.
			</td>
		</tr>
		<tr>
			<th><b>Configuração para impressão</b></th>
		</tr>
		<tr>
			<td> Configurar a página removendo os dados de cabeçalho e rodapé.<br/><br/>
				<b>No browser Internet Explorer:</b><br/>
				<font style="color: red;"> Acessar no menu <b>Arquivo (File)</b> a opção <b>Configurar Página (Page Setup)</b>.<br/>
				Na opção cabeçalho e rodapé altere as seleções de todas as opções para <b>"-Vazio-"</b>.<br/>
				Após a conclusão clique no botão <b>OK</b>. </font><br/><br/>
				
				<b>No browser Mozilla Firefox:</b><br/>
				<font style="color: red;"> Acessar no menu <b>Arquivo (File)</b> a opção <b>Configurar Página (Page Setup)</b> e clique na aba <b>Margens</b>.<br/>
				Na opção cabeçalho e rodapé altere as seleções de todas as opções para <b>"--em branco--"</b>.<br/>
				Após a conclusão clique no botão <b>OK</b>. </font><br/><br/>				
				Após estes procedimentos o seu browser estará configurado para as impressões necessárias. </td>
		</tr>
	</table>
	<br/>		
	<table  align="center" cellspacing="1" cellpadding="4" >
		<tr style="text-align: center;">
			<td><input type="button" name="btnRelatorio" id="btnRelatorio" value="Ir para Impressão" onclick="imprimir(<?=$_REQUEST['parametro']; ?>,'<?=$_REQUEST['tipo']; ?>');">
				<input type="button" name="btnFechar" id="btnFechar" value="Fechar" onclick="window.close();">
			</td>
		</tr>
	</table>		
</body>
<script type="text/javascript">
function imprimir(parametro, tipo){
	if( tipo == 'minuta' ){
		var janela = window.open( 'emenda.php?modulo=principal/popImprimeMinutaPTA&acao=A&tipo=M&ptrid='+parametro, 'relatorio', 'width=450,height=400,status=0,menubar=1,toolbar=0,scrollbars=1,resizable=0' );
		janela.focus();
	} else if(tipo == 'M'){
		var janela = window.open( 'emenda.php?modulo=principal/popImprimeMinutaPTA&acao=A&tipo=M&pmcid='+parametro, 'relatorio', 'width=450,height=400,status=0,menubar=1,toolbar=0,scrollbars=1,resizable=0' );
		janela.focus();
	}else if( tipo == 'T' ){
		var janela = window.open( 'emenda.php?modulo=principal/popImprimeMinutaPTA&acao=A&tipo=T&refid='+parametro, 'relatorio', 'width=450,height=400,status=0,menubar=1,toolbar=0,scrollbars=1,resizable=0' );
		janela.focus();
	}else{
		var janela = window.open("emenda.php?modulo=principal/ptaConsolidado&acao=A&ptrid=" + parametro,"relatorio", "scrollbars=yes,height=600,width=800");
		janela.focus();
	}
}
</script>
</html>