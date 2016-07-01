<?
if($_REQUEST['exec_function']) {
	$_REQUEST['exec_function']($_REQUEST);
}
?>
<html>
	<head>
	<title>Inserir Cursos</title>		
	<script language="JavaScript" src="../../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script language="JavaScript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="./js/sig.js"></script>
	<script>
	function crtobrasinauguradas(chk,param) {
		if(chk.checked) {
			ajaxatualizar('exec_function=salvarObrasInauguradas&param='+param,'');
			window.opener.ajaxatualizar('exec_function=carregarlistaobras&buscar=serinaugurada','td_serinaugurada');
			window.opener.ajaxatualizar('exec_function=carregarlistaobras&buscar=outras','td_outras');
		} else {
			var conf = confirm("A obra deixará de ser inaugurada. Deseja continuar?");
			if(conf) {
				ajaxatualizar('exec_function=removerObrasInauguradas&param='+param,'');
				window.opener.ajaxatualizar('exec_function=carregarlistaobras&buscar=serinaugurada','td_serinaugurada');
				window.opener.ajaxatualizar('exec_function=carregarlistaobras&buscar=outras','td_outras');
			}
		}
	}
	</script>
	</head>
	<body>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
	<td id="td_listageral"></td>
	</tr>
	<tr>
	<td align="right"><input type="button" value="Ok" onclick="window.close();"></td>
	</tr>
	</table>
	<script>
	ajaxatualizar('exec_function=carregarlistaobras','td_listageral');
	</script>
	
	</body>
</html>