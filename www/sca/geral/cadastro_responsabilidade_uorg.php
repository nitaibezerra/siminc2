<?
/*
 Sistema Simec
 Setor responsável: SPO-MEC
 Desenvolvedor: Equipe Consultores Simec
 Analista: Cristiano Cabral
 Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
 Módulo:seleciona_unid_perfilresp.php

 */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

/*
 *** INICIO REGISTRO RESPONSABILIDADES ***
 */

if(isset($_POST['ok'])) {

	$sql = "update
			 sca.usuarioresponsabilidade 
			set
			 rpustatus = 'I' 
			where
			 usucpf = '$usucpf'  
			 and pflcod = $pflcod ";
	
	$db->executar($sql);
	
        $co_interno_uorg = $_POST['co_interno_uorg'];
        
	if($_POST['co_interno_uorg']){
		
			$sql = "INSERT INTO sca.usuarioresponsabilidade (co_uorg, usucpf, rpustatus, rpudata_inc, pflcod) 
				VALUES ($co_interno_uorg, '$usucpf', 'A',  now(), '$pflcod')";
			$db->executar($sql);
		
	}
	$db->commit();

	?>
<script>
	
	window.parent.opener.location.reload();self.close();

function filtro2(phrase, _id){
	var words = phrase.value.toLowerCase().split(" ");
	var table = document.getElementById(_id);
	var ele;
	for (var r = 1; r < table.rows.length; r++){
		ele = table.rows[r].innerHTML.replace(/<[^>]+>/g,"");
	        var displayStyle = 'none';
	        for (var i = 0; i < words.length; i++) {
		    if (ele.toLowerCase().indexOf(words[i])>=0)
			displayStyle = '';
		    else {
			displayStyle = 'none';
			break;
		    }
	        }
		table.rows[r].style.display = displayStyle;
	}
}
	
</script>


	<? 	exit(0); } /*  *** FIM REGISTRO RESPONSABILIDADES ***  */ ?>
	
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Estados e Municípios</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' 	href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>

<?/*flush();*/?>
<DIV style="OVERFLOW: AUTO; WIDTH: 496px; HEIGHT: 350px; BORDER: 2px SOLID #ECECEC; background-color: White;">

	
<script language="JavaScript">

/*
*  Filtro de Tabela
*/
function filtro2(phrase, _id){
	var words = phrase.value.toLowerCase().split(" ");
	var table = document.getElementById(_id);
	var ele;
	for (var r = 1; r < table.rows.length; r++){
		ele = table.rows[r].innerHTML.replace(/<[^>]+>/g,"");
	        var displayStyle = 'none';
	        for (var i = 0; i < words.length; i++) {
		    if (ele.toLowerCase().indexOf(words[i])>=0)
			displayStyle = '';
		    else {
			displayStyle = 'none';
			break;
		    }
	        }
		table.rows[r].style.display = displayStyle;
	}
}


    document.getElementById('tabela').style.visibility = "hidden";
    document.getElementById('tabela').style.display  = "none";
</script>

    <!-- FILTRO -->
    <form>
	    <input name="filter" onkeyup="filtro2(this, 'tabela');" type="text" size="80">
    </form>
    <!-- /FILTRO -->

	<form name="formulario" method="post" action="">
    <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem filterable" id="tabela">
	<thead>
		<tr>
			<td valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				colspan="3"><strong>Selecione a(s) estado(s)</strong></td>

		</tr>
		<tr>
		<?

		$cabecalho = 'Selecione a(s) Uorg(s)';
		$sql = "
			select
				co_interno_uorg, sg_unidade_org, no_unidade_org
			from sca.vwunidadeorganizacional
			order by no_unidade_org";
                
		$RS = @$db->carregar($sql);
		$nlinhas = count($RS)-1;
		$j = 0 ;
		for ($i=0; $i<=$nlinhas;$i++)
		{
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			    if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
			?>
		
		
		<tr bgcolor="<?=$cor?>">
			<td align="left" style="color: blue;">
                <input type="radio" name="co_interno_uorg" id="<?=$co_interno_uorg?>" value="<?=$co_interno_uorg?>" />
                <?php echo $co_interno_uorg, ' - ',  $sg_unidade_org, ' - ', $no_unidade_org?>
			</td>
		</tr>
		
		<?php } ?>
</table>
</div>
        <input 	type="hidden" name="usucpf" value="<?=$usucpf?>"> <input type="hidden" 	name="pflcod" value="<?=$pflcod?>"> 
        <input type="hidden" name="enviar" 	value=""> <select multiple size="8" onclick="mostraMunicipio(this);" name="usuunidresp[]" id="usuunidresp" style="width: 500px;" class="CampoEstilo">


<?php
	$sql = "
			select 
				distinct uorg.co_interno_uorg as codigo, uorg.co_interno_uorg || ' - ' || uorg.sg_unidade_org || ' - ' || uorg.no_unidade_org as descricao 
			from 
				sca.usuarioresponsabilidade ur inner join sca.vwunidadeorganizacional uorg on ur.co_uorg = uorg.co_interno_uorg
	 		where ur.rpustatus='A' and ur.usucpf = '$usucpf' and ur.pflcod=$pflcod";
	$RS = @$db->carregar($sql);
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
				print " <option value=\"$codigo\">$descricao</option>";
			}
		}
	}

	?>
</select>
<table width="100%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding: 3px;" colspan="3">
                    <input type="submit" name="ok" value="OK" id="ok">
                </td>
	</tr>
</table>
</form>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";


function mostraEsconde(estado){
	var estadoAtual = document.getElementById(estado).style.display;
	var objImagem = document.getElementById(estado+'_img');
	if(estadoAtual == 'none'){
		document.getElementById(estado).style.display = 'block';
		
		objImagem.src = '/imagens/menos.gif';
		
	}else{
		document.getElementById(estado).style.display = 'none';
		objImagem.src = '/imagens/mais.gif';
	}
	
}


var campoSelect = document.getElementById("usuunidresp");


if (campoSelect.options[0] && campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
}


function enviarFormulario(){
	document.formassocia.enviar.value=1;
	document.formassocia.submit();

}


function mostraMunicipio(objSelect){
	for( var i = 0; i < objSelect.options.length; i++ )
	{
		if ( objSelect.options[i].value == objSelect.value )
		{
			var estado = objSelect.options[i].innerHTML.substring(0,2);
			break;
		}
	}
	var estadoAtual = document.getElementById(estado).style.display;
	if(estadoAtual != 'block'){
		 mostraEsconde(estado);
	}
	document.getElementById(objSelect.value).focus();
		
}
/*
function retorna( check, muncod, mundescricao )
{
	if ( check.checked )
	{
		// põe
		campoSelect.options[campoSelect.options.length] = new Option( mundescricao, muncod, false, false );
	}
	else
	{
		// tira
		for( var i = 0; i < campoSelect.options.length; i++ )
		{
			if ( campoSelect.options[i].value == muncod )
			{
				campoSelect.options[i] = null;
			}
		}
	}
	sortSelect( campoSelect );
}
*/
</script>
