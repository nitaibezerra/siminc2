<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:seleciona_aca.php
   
   */
	require_once "config.inc";
	  include APPRAIZ . "includes/classes_simec.inc";
      include APPRAIZ . "includes/funcoes.inc";
	  $db = new cls_banco();
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Programas e Ações</title>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript">var campoSelect = window.opener.document.getElementById("<?=$_REQUEST['campo']?>");</script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>
</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?flush();?>
<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<form name="formulario">
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Clique no Programa para selecionar as Ações</strong></td>
</tr>
<tr>
<?
//	  $sql = "select a.prgcod, p.prgdsc, a.acacod, a.unicod, a.acadsc, u.unidsc, p.prgid from acao a inner join programa p on a.prgid = p.prgid inner join unidade u on a.unicod=u.unicod where acasnrap='f' group by a.prgcod, p.prgdsc, a.acacod, a.unicod, a.acadsc, u.unidsc, p.prgid order by a.prgcod, a.acacod, a.unicod, a.acadsc";
	$sql = "
		select a.prgcod, p.prgdsc, a.acacod, a.unicod, a.acadsc, u.unidsc, p.prgid
		from elabrev.ppaacao_proposta a
		inner join elabrev.ppaprograma_proposta p on a.prgid = p.prgid and p.prgstatus = 'A'
		inner join unidade u on a.unicod=u.unicod
		group by a.prgcod, p.prgdsc, a.acacod, a.unicod, a.acadsc, u.unidsc, p.prgid
		order by a.prgcod, a.acacod, a.unicod, a.acadsc
	";
	
	  //dbg( $sql, 1 );
	  $RS = $db->carregar($sql);
	  $nlinhas = count($RS)-1;
	  for ($i=0; $i<=$nlinhas;$i++)
		 {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
			if ($v_prgid<>$prgid) {
				if ($corp == '#e0e0e0') $corp = '#f4f4f4' ; else $corp='#e0e0e0';
				if ($v_prgid) {?>
			 </table>
	  			 </td></tr>
			   <script language="JavaScript">
				   document.getElementById('<?=$v_prgid?>').style.visibility = "hidden";
				   document.getElementById('<?=$v_prgid?>').style.display  = "none";
			   </script>
				<?}?>
	   		<tr bgcolor="<?=$corp?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$corp?>';">
				<td align="left" onclick="abreconteudo('<?=$prgid?>');"><img src="/imagens/mais.gif" border="0" width="9" height="9" align="absmiddle" vspace="3" id="img<?=$prgid?>" name="+">&nbsp;&nbsp;<font color="#0000ff"><?=$prgcod?></font> - <?=$prgdsc?></td>
			</tr>
			<tr id="<?=$prgid?>"><td>
			   <table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" >
	   <?$v_prgid=$prgid;}?>
				<!-- <tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';"><td align="left" nowrap style="color:#006666;"> <input type="Checkbox" name="prgid" id="<?=$prgid.'.'.$acacod.'.'.$unicod?>" value="<?=$prgid.'.'.$acacod.'.'.$unicod?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="prgdsc" value="<?=$prgcod.'.'.$acacod.'.'.$unicod?> - <?=$acadsc?>"><?=$acacod.'.'.$unicod?></td><td style="color:#666666;"><font color="#333333"><?=$acadsc?></font> (<?=$unidsc?>)</td></tr>-->
					 <tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';">
						 <td align="left" nowrap style="color:#006666;"> 
							 <input type="Checkbox" name="prgid" id="<?=$prgid.'.'.$acacod.'.'.$unicod?>" value="<?=$acacod?>" onclick="retorna(<?=$i?>);">
							 <input type="Hidden" name="prgdsc" value="<?=$prgcod.'.'.$acacod.'.'.$unicod?> - <?=$acadsc?>"><?=$acacod.'.'.$unicod?></td>
						 <td style="color:#666666;"><font color="#333333"><?=$acadsc?></font> (<?=$unidsc?>)
						 </td>
					 </tr><?}?>
<script language="JavaScript">
				   document.getElementById('<?=$v_prgid?>').style.visibility = "hidden";
				   document.getElementById('<?=$v_prgid?>').style.display  = "none";
</script>
 </table>
</td></tr>
<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="self.close();">
</td></tr>
</form>
</table>
<script language="JavaScript">
if (campoSelect.options[0].value != ''){
	v_prg=0;
	for(var i=0; i<campoSelect.options.length; i++)
		{ 	document.getElementById(campoSelect.options[i].value).checked = true;
			
			if (v_prg!=campoSelect.options[i].value.slice(0,campoSelect.options[i].value.indexOf('.')))
				{ abreconteudo(campoSelect.options[i].value.slice(0,campoSelect.options[i].value.indexOf('.')));
					v_prg = campoSelect.options[i].value.slice(0,campoSelect.options[i].value.indexOf('.'));
				}
		}
}

document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";

function abreconteudo(objeto)
{
if (document.getElementById('img'+objeto).name=='+')
	{
	document.getElementById('img'+objeto).name='-';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
	document.getElementById(objeto).style.visibility = "visible";
	document.getElementById(objeto).style.display  = "";
	}
	else
	{
	document.getElementById('img'+objeto).name='+';
    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
	document.getElementById(objeto).style.visibility = "hidden";
	document.getElementById(objeto).style.display  = "none";
	}
}

function retorna(objeto)
{
window.opener.retorna(objeto,'A');
}
</script>
