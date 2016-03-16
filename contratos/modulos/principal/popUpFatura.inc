<?php
// if ( isset($_REQUEST['ftcid']) && empty($_REQUEST['ftcid']) ){
// 	die("<script>
// 			alert('Não existe fatura lançada para esta ordem bancária.'); 
// 			window.close();
// 		 </script>");
// }

if($_REQUEST['requisicaoAjax'])
{
	$_REQUEST['requisicaoAjax']();
	die;
}

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
$habilitado = true;

?>
<html>
<head>
	<meta http-equiv="Cache-Control" content="no-cache">
	<meta http-equiv="Pragma" content="no-cache">
	<meta http-equiv="Connection" content="Keep-Alive">
	<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
	<title><?= $titulo ?></title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script	type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
	<script type="text/javascript">
		this._closeWindows = false;

		function downloadAnexo(arqid)
		{
			window.location = '?modulo=principal/addNotaContratos&acao=A&requisicao=downloadAnexo&classe=AnexoFatura&arqid='+arqid;
		}
	</script>
</head>
<body style="margin:0px; padding:0; background-color: #fff;">
<?php
if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
	//if($_REQUEST['requisicao'] != 'popUpEmpenho')
		//popUpEmpenho();
}
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align="center">
        	<input type="button" value="Voltar" name="btn_voltar" onclick="history.go(-1)" />
        	<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
        </td>
	</tr>
</table>
</body>
</html>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>