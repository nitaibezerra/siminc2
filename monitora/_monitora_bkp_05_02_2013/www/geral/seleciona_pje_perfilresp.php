<?

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/ 	

if(is_array($_POST['usupjeresp']) && @count($_POST['usupjeresp'])>0) {
	$txtpjeComCoordenador = "";
	$confirmarpje = 0;
	$concluido = 0; // -1 erro, 0 nao concluido, 1 sucesso
	$pjeConfirmadas = $_REQUEST["pjeConfirmadas"];
	$sqlSelResp = "SELECT distinct ur.rpuid, ur.usucpf, ur.rpustatus, u.pjedsc, u.pjecod, u.pjeid FROM monitora.usuarioresponsabilidade ur 
		INNER JOIN monitora.projetoespecial u ON u.unitpocod='U' and u.pjecod = ur.pjecod AND u.pjeid = %s			
		WHERE ur.rpustatus = 'A' AND ur.usucpf <> '" . $usucpf . "'";
	

	$sqlSelpje	= "SELECT u.pjecod, u.pjeid FROM monitora.projetoespecial u WHERE u.pjeid = %s ";
	$sqlInsRpu = "INSERT INTO monitora.usuarioresponsabilidade (pjeid, usucpf, rpustatus, rpudata_inc, pflcod,prsano) VALUES ('%s', '%s', '%s', '%s', '%s','".$_SESSION['exercicio']."')";
	$sqlUpdRpu = "UPDATE monitora.usuarioresponsabilidade SET rpustatus = 'I' WHERE rpuid = '%s'";

	//
	// verificar quais projetos especiais possuem outro coordenador ativo
	foreach ($_POST['usupjeresp'] as $respcod) {
		$sql='';
		$sql = vsprintf($sqlSelResp, $respcod);

//dbg('wwww'.$linhasRpu.$sql);
		if ($sql<>"" && ($linhasRpu = @$db->carregar($sql))) {
			foreach ($linhasRpu as $rpu) {
				$confirmarpje = true;
				$txtpjeComCoordenador .= $rpu["pjecod"] . " - " . $rpu["pjedsc"] . " CPF: " . $rpu["usucpf"] . "\\n";
			}
		}
	}
		print 'Sucesso';
	//
	// caso nao existam outros coordenadores de planejamento, registrar os itens selecionados
	if(!$confirmarpje) {
	$sql = "delete from monitora.usuarioresponsabilidade where pjeid is not null and usucpf='$usucpf'";
		$db->executar($sql);
		$concluido = 1;
		foreach ($_POST['usupjeresp'] as $respcod) {
		   $sql = "";
		   if ($respcod>0){
		   $sql = vsprintf($sqlSelpje, $respcod);
           $linha = $db->carregar($sql);
			if(is_array($linha) && count($linha)>=1) {
				foreach ($linha as $pje) {
					$pjeid = $pje["pjeid"]; 		
					$dados = array($pjeid, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod); 			
					$sql = vsprintf($sqlInsRpu, $dados);
					$db->executar($sql);
				}
				$concluido = 1;
			}}
		}
	}
	//
	// verificar se foi confirmado a substituição do coordenador atual pelo
	// usuario que está sendo liberado e/ou alterado
	else
	if($pjeConfirmadas) {
		if (is_array($linhasRpu)){
		foreach ($linhasRpu as $rpu) {
			$sql = sprintf($sqlUpdRpu, $rpu["rpuid"]);
			$db->executar($sql);
			$dados = array($rpu["pjeid"], $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod);
			$sql = vsprintf($sqlInsRpu, $dados);
			//dbg('222'.$sql);
			$db->executar($sql);
		}
		$concluido = 1;	
	}	
	}
	//
	// exibir a tela de aviso dos itens que já possuem coordenador e confirmar
	// a substituição pelo usuario que está sendo liberado e/ou alterado
	else {
		$msg = "Existem usuários ativos com o perfil selecionado para estes Projetos:\\n\\n";
		$msg .= $txtpjeComCoordenador;
		$msg .= "\\nDeseja sobrescrevê-los?\\n\\n";
		$msg .= "Ao confirmar, o perfil dos usuários atuais (listados acima) será desativado.";
		
		?><form name="formassocia" style="margin:0px;" method="POST">
		<input type="hidden" name="usucpf" value="<?=$usucpf?>">
		<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		<input type="hidden" name="pjeConfirmadas" value="1">
		<?
			foreach ($_POST['usupjeresp'] as $respcod) {
				?><input type="hidden" name="usupjeresp[]" value="<?=$respcod?>"><?
			}
		?>
		<script>
			if (confirm("<?=$msg?>")) {		
				document.formassocia.submit();
			} else
			{
				document.formassocia.pjeConfirmadas.value=0;
				document.formassocia.submit();
			}
		</script>

		<?
		exit(0);
	}

	if ($concluido>0) {
		$db->commit();
		?>
		
	<script language="javascript">
	alert("Operação realizada com sucesso!");
	window.opener.reload;
	self.close();
	</script>
		<?
		exit(0);
	}
}
/*
*** FIM REGISTRO RESPONSABILIDADES ***
*/
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Projetos Especiais</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="../imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?flush();?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<form name="formulario">
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione o(s) Projeto(s) Especial(is)</strong></td>
</tr>
<tr>
<?

	  $cabecalho = 'Selecione o(s) Projeto(s) Especial (is)';
	  $sql = "select pjeid, pjecod, pjedsc from monitora.projetoespecial where pjestatus='A'  order by pjecod,pjedsc";

	  $RS = @$db->carregar($sql);
	  $nlinhas = count($RS)-1;
	  for ($i=0; $i<=$nlinhas;$i++)
		 {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
	   ?>
	   		
		   		<tr bgcolor="<?=$cor?>">
				<td align="right"><input type="Checkbox" name="pjeid" id="<?=$pjeid?>" value="<?=$pjeid?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="pjedsc" value="<?=$pjecod.' - '.$pjedsc?>"></td>
				<td align="right" style="color:blue;"><?=$pjecod?></td>
				<td><?=$pjedsc?></td>
				</tr>
	   
	   <?}
?>
</form>
</table>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="usupjeresp[]" id="usupjeresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?
$sql = "select distinct u.pjeid as codigo, u.pjecod||' - '||u.pjedsc as descricao from monitora.usuarioresponsabilidade ur inner join monitora.projetoespecial u on ur.pjeid=u.pjeid where ur.rpustatus='A' and ur.usucpf = '$usucpf' and ur.pflcod=$pflcod";
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
else{
	$sql = "select distinct u.pjecod as codigo, u.pjedsc as descricao from monitora.projetoespecial u inner join monitora.pjeproposto up on u.pjeid=up.pjeid up.usucpf='".$usucpf."' where up.pjeid is not null";
	$RS = @$db->carregar($sql);
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
				print " <option value=\"$codigo\">$codigo - $descricao</option>";
			}
		}
} else {?>
<option value="">Clique no Projeto Especial para selecionar.</option>
<?	}
}?>
</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
<tr bgcolor="#c0c0c0">
<td align="right" style="padding:3px;" colspan="3">
<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
</td></tr>
</table>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";


var campoSelect = document.getElementById("usupjeresp");


if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
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
	if (document.formulario.pjeid[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.pjedsc[objeto].value, document.formulario.pjeid[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.pjeid[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique no Projeto Especial.', '', false, false);}
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



</script>