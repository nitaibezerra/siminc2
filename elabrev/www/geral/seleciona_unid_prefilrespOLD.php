<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:sql_siof-simec.php
   
   */
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Unidades</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
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
<?

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

	  $db = new cls_banco();
	  $cabecalho = 'Selecione a(s) Unidade(s)';
	  $sql = "select uniid, unicod, unidsc from unidade where unistatus='A' and orgcod='".$_SESSION['ittorgao']."' order by unidsc";

	  $RS = $db->carregar($sql);
	  $nlinhas = count($RS)-1;
	  for ($i=0; $i<=$nlinhas;$i++)
		 {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
	   ?>
	   		
		   		<tr bgcolor="<?=$cor?>">
				<td align="right"><input type="Checkbox" name="uniid" id="<?=$uniid?>" value="<?=$uniid?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="unidsc" value="<?=$unicod.' - '.$unidsc?>"></td>
				<td align="right" style="color:blue;"><?=$unicod?></td>
				<td><?=$unidsc?></td>
				</tr>
	   
	   <?}
?>

<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="self.close();">
</td></tr>
</table>
</form>
<form name="formulario2">
<select multiple size="5" name="usuuniproposto[]" id="usuuniproposto" style="width:500px;" class="CampoEstilo">
  <option value="">Selecione a(s) Unidade(s)</option>
</select>
</form>
<script language="JavaScript">
var campoSelect = document.getElementById("usuuniproposto");
if (campoSelect.options[0].value != ''){
for(var i=0; i<campoSelect.options.length; i++)
	{document.getElementById(campoSelect.options[i].value).checked = true;}
}

function retorna(objeto,uniaca)
{
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (document.formulario.uniid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.unidsc[objeto].value, document.formulario.uniid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.uniid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){if (uniaca=="uni") {campoSelect.options[0] = new Option('Clique Aqui para Selecionar a(s) Unidade(s)', '', false, false);} else {campoSelect.options[0] = new Option('Clique Aqui para Selecionar a(s) Ação(ões)', '', false, false);}}
			sortSelect(campoSelect);
	}
}	

</script>
