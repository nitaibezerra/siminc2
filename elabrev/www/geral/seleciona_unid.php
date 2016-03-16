<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:seleciona_unid.php
  
   */
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Unidades</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<script language="JavaScript">var campoSelect = window.opener.document.getElementById("<?=$_REQUEST['campo']?>");</script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<form name="formulario">
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Unidade(s)</strong></td>
</tr>
<tr>
<?php

	include "config.inc";	  
	include APPRAIZ."includes/classes_simec.inc";
	include APPRAIZ."includes/funcoes.inc";
	
	$db = new cls_banco();
	
	$cabecalho = 'Selecione a(s) Unidade(s)';
	$sql = "select uniid, unicod, unidsc from unidade where unistatus='A' and orgcod='26000' order by unidsc";
	$RS = $db->carregar($sql);
	$nlinhas = count($RS)-1;
	
	for ($i=0; $i<=$nlinhas;$i++){
		foreach($RS[$i] as $k=>$v) ${$k}=$v;
		if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
	   ?>
		<tr bgcolor="<?=$cor?>">
		<td align="right">
			<input type="Checkbox" name="prgid" id="<?=$uniid?>" value="<?=$unicod?>" onclick="retorna(<?=$i?>);">
			<input type="Hidden" name="prgdsc" value="<?=$unicod.' - '.$unidsc?>">
		</td>
		<td align="right" style="color:blue;"><?=$unicod?></td>
		<td><?=$unidsc?></td>
		</tr>
	<?}?>

<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="self.close();">
</td></tr>
</form>
</table>
<script language="JavaScript">
var campoSelect = window.opener.document.getElementById("usuuniproposto");
if (campoSelect.options[0].value != ''){
for(var i=0; i<campoSelect.options.length; i++)
	{document.getElementById(campoSelect.options[i].value).checked = true;}
}

function retorna(objeto)
{
window.opener.retorna(objeto,'U');
}
</script>
