<?php
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include_once "../_constantes.php";
$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

// ver($pflcod,d);

/**
 * Regras Server-Side do negócio de responsabilidade
 */
function validaRegrasNegocio( $pflcod ){

	$numeroPermitidoParaEstadosExecutivo = 1;

	switch ( $pflcod ) {
		case PFLCOD_SASE_EXECUTIVO:
			if( count($_POST['usuunidresp']) > $numeroPermitidoParaEstadosExecutivo ){
				echo "<script>alert('Perfil executivo permite somente 1 estado para responsabilidade. Somente foi liberado estado {$_POST['usuunidresp'][0]}');</script>";
				$_POST['usuunidresp'] = array($_POST['usuunidresp'][0]);
			}
			break;
	}

}


/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/
if(is_array($_POST['usuunidresp'])) {
	
	// desativa todos os elementos  da responsabilidade dessse usuario
	$sql = "UPDATE
				sase.usuarioresponsabilidade 
			set
				rpustatus = 'I' 
			where
			 	usucpf = '$usucpf'  
			 	and pflcod = $pflcod ";
	$db->executar($sql);
	
	// insere responsabilidades ativas nesse momento
	if($_POST['usuunidresp'][0]){

		// aplica regras de negócio server side
		validaRegrasNegocio( $pflcod );

		foreach($_POST['usuunidresp'] as $estuf){

			$sql = "INSERT INTO sase.usuarioresponsabilidade (estuf, usucpf, rpustatus, rpudata_inc, pflcod) 
					VALUES ('$estuf', '$usucpf', 'A',  now(), '$pflcod')";
			$db->executar($sql);
		}		
	}
	$db->commit(); ?>
	
	<script> window.parent.opener.location.reload();self.close(); </script>

	<?php
	exit();
}

/*
*** FIM REGISTRO RESPONSABILIDADES ***
*/
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Instituição</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?
//flush();
?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<form name="formulario">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o(s) estado(s)</strong></td>
</tr>
<tr>
<?php
	// ver($_POST,$_SESSION,d);


	$cabecalho = 'Selecione o(s) estado(s)';
	$sql = "select
				estuf, estdescricao
			from territorios.estado
			order by estuf, estdescricao";
	$RS = @$db->carregar($sql);
	$nlinhas = count($RS)-1;
	for ($i=0; $i<=$nlinhas;$i++)
	{
		extract($RS[$i]);
		if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
		?>	
			<tr bgcolor="<?=$cor?>">
				<td align="right" width="5px"><input type="Checkbox" name="estuf" id="<?=$estuf?>" value="<?=$estuf?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="estdescricao" value="<?=$estuf.' - '.$estdescricao?>"></td>
				<td align="right" width="10px" style="color:blue;"><?=$estuf?></td>
				<td><?=$estdescricao?></td>
			</tr>
	   
	   	<?php } 
?>
</table>
</form>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<?php 
$sql = "select distinct u.estuf as codigo, 
			u.estuf||' - '||u.estdescricao as descricao 
		from sase.usuarioresponsabilidade ur 
		inner join territorios.estado u on ur.estuf = u.estuf 
		where 
			ur.rpustatus='A' 
			and ur.usucpf = '$usucpf' 
			and ur.pflcod=$pflcod ";
// ver($sql,d);
$RS = @$db->carregar($sql);
?>
<select multiple size="8" name="usuunidresp[]" id="usuunidresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?php 
if(is_array($RS)) {
	$nlinhas = count($RS)-1;
	if ($nlinhas>=0) {
		for ($i=0; $i<=$nlinhas;$i++) {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    		print " <option value=\"$codigo\">$codigo - $descricao</option>";		
		}
	}
} else {?>
<option value="">Clique no estado.</option>
<?
}
?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
</td></tr>
</table>
<script language="JavaScript">
var tamanho = 0;

document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";

var campoSelect = document.getElementById("usuunidresp");

if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++){
		document.getElementById(campoSelect.options[i].value).checked = true;
	}
}

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
	tamanho = campoSelect.options.length;

	if (campoSelect.options[0].value=='') {tamanho--;}

	if (document.formulario.estuf[objeto].checked == true){

		validaRegrasNegocio( objeto );

		campoSelect.options[tamanho] = new Option(document.formulario.estdescricao[objeto].value, document.formulario.estuf[objeto].value, false, false);
		sortSelect(campoSelect);
	
	}else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.estuf[objeto].value == campoSelect.options[i].value){
				campoSelect.options[i] = null;
			}
		}
		
		if (!campoSelect.options[0]){
			campoSelect.options[0] = new Option('Clique no estado.', '', false, false);
		}

		sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();}
}

/**
 * Valida Regras de Negócio Client-Side
 */
function validaRegrasNegocio( objeto ){

	<?php if( $pflcod == PFLCOD_SASE_EXECUTIVO ){ ?>

		// tratamento para perfil EXECUTIVO
		if( campoSelect.options.length > 0 ){
			// deschecka todos que não forem esse elemento
			for(var i=0;i<document.formulario.estuf.length;i++){ 
				if( document.formulario.estuf[i] !=  document.formulario.estuf[objeto] ){
					document.formulario.estuf[i].checked = false;
				}
			}

			// retira todos options
			campoSelect.options.length = 0

			tamanho = 0;
		}
	
	<?php } ?>
}
</script>