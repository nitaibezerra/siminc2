<?php
if ($_POST['sbaid']){
	$elementInsert = implode('),(',$_POST['sbaid']);
	$sql = "INSERT INTO cte.subacaoconvenio (sbaid) VALUES ({$elementInsert})";	
	$db->executar($sql);
	$db->commit();
	die ("<script>
			alert('Operação executada com sucesso!');
			//window.opener.location.replace(window.opener.location);
			window.close();
		  </script>");
}
?>
<html>
  <head>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript">
	/*
	* Function, check/ñ check todos os itens da lista
	*/
	function allSelect(obj) {
		element = document.getElementsByName('sbaid[]');	
		param = obj.checked ? obj.checked : obj.checked;

		for (i = 0; i < element.length; i++) {
			element[i].checked = param;
		} 
	}
	/*
	* Function, Tira o checked do botão selecionar todos
	*/
	function notCheckButton (obj){
		if (!obj.checked)
			document.getElementById('selectAll').checked = false;
	}
	</script>
  </head>	
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( 'Enviar Subação para FNDE', '&nbsp;'  );
?>
<form action="" method="post" name="formulario">
<?php 
$sql = sprintf("SELECT
				 DISTINCT
				 '<INPUT TYPE=\'checkbox\' NAME=\'sbaid[]\' VALUE=\'' || sa.sbaid || '\' onclick=\'notCheckButton(this)\'>' as check,
				 sbadsc		 
				FROM
				 cte.pontuacao p
				 INNER JOIN cte.acaoindicador ai ON p.ptoid = ai.ptoid
				 INNER JOIN cte.subacaoindicador sa ON sa.aciid = ai.aciid 
				 INNER JOIN cte.subacaoparecertecnico spt ON spt.sbaid = sa.sbaid  AND spt.ssuid = 3 AND spt.sptano = to_char(now(),'yyyy')
				WHERE
				 p.inuid =%d AND
				 p.ptostatus = 'A' AND
				 sa.sbaid not in (select sbaid from cte.subacaoconvenio);",
				$_SESSION['inuid']);
#echo "<pre>$sql";
$cabecalho = array('<INPUT TYPE=\'checkbox\' NAME=\'selectAll\' ID=\'selectAll\' VALUE=\'\' onclick=\'allSelect(this);\'>','Subação');
$db->monta_lista_simples($sql,$cabecalho, 1000, 1000, 'N', '95%');
?>
<table border="0" width="95%" cellspacing="0" cellpadding="2" align="center">
	<tr bgcolor="#cdcdcd">
		<td width="1%">&nbsp;</td>
		<td>&nbsp<input type="submit" value="Enviar FNDE" style="margin-left:55px;" /></td>
	</tr>
</table>
</form>
</body>
</html>