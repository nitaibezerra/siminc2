<?php

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

/*define('PERFIL_GESTORUNIDPLANEJAM', 112);
define('PERFIL_EQUIPAPOIOGESTORUP', 113);
define('PERFIL_MONITORA_GESTORUNIDORCAMENTO', 410);
define('PERFIL_MONITORA_EQAGESTORUNIDORCAMENTO', 411);
define('PERFIL_MONITORA_GESTORUNIOBRIGATORIA', 412);
define('PERFIL_MONITORA_ESQUIPEGESTORUNIOBRIGATORIA', 413);
define('PERFIL_MONITORA_GESTORSUBACOES', 414)*/;

$arPerfis = array(112,113,410,411,412,413,414);

if(is_array($_POST['usuuniresp']) && @count($_POST['usuuniresp'])>0) {
	$txtunidadesComCoordenador = "";
	$confirmarunidades = 0;
	$concluido = 0; // -1 erro, 0 nao concluido, 1 sucesso
	$unidadesConfirmados = $_REQUEST["unidadesConfirmados"];
	
	
	//SQL para verificar as responsabilidades
	$sqlSelResp = "
		SELECT 
			distinct ur.rpuid, 
			ur.usucpf, 
			ur.rpustatus, 
			uni.ungdsc, 
			uni.ungcod
		FROM 
			monitora.usuarioresponsabilidade ur 
			INNER JOIN public.unidadegestora uni on
				uni.ungcod = ur.ungcod and
				uni.ungcod = '%s' and
				uni.ungstatus = 'A'
			inner join seguranca.perfil pfl on
				pfl.pflcod = ur.pflcod and
				pfl.pflcod = '" . $pflcod . "'
		where
			ur.prsano='".$_SESSION['exercicio']."' AND
			ur.rpustatus = 'A' and
			ur.usucpf <> '" . $usucpf . "'
	";

	$sqlSelUnidade	= "SELECT uni.ungdsc, uni.ungcod FROM public.unidadegestora uni WHERE uni.ungcod = '%s' ";
	
	$sqlInsRpu = "INSERT INTO monitora.usuarioresponsabilidade (ungcod, usucpf, rpustatus, rpudata_inc, pflcod, prsano) VALUES ('%s', '%s', '%s', '%s', '%s','".$_SESSION['exercicio']."')";

	$sqlUpdRpu = "UPDATE monitora.usuarioresponsabilidade SET rpustatus = 'I' WHERE ungcod = '%s' AND prsano='".$_SESSION['exercicio']."' AND pflcod = ".$pflcod;

	$sqlUpdRpuUsu = "UPDATE monitora.usuarioresponsabilidade SET rpustatus = 'I' WHERE ungcod IS NOT NULL AND ungcod != '' AND prsano='".$_SESSION['exercicio']."' AND usucpf = '".$usucpf."' AND pflcod = ".$pflcod;

	//
	// verificar quais Unidades possuem outro com o mesmo perfil
	if ( is_array( $_POST['usuuniresp'] ) && $_POST['usuuniresp'][0] != "" )
	{
		foreach ( $_POST['usuuniresp'] as $respcod )
		{
			$sql='';
			
			$sql = vsprintf($sqlSelResp, $respcod);
			if ($sql<>"" && ($linhasRpu = @$db->carregar($sql))) {
				foreach ($linhasRpu as $rpu) {
					$confirmarunidades = true;
					$txtunidadesComCoordenador .= $rpu["ungcod"] . ' - ' . $rpu["ungdsc"] . ' CPF: ' . $rpu["usucpf"] . '\n ';
				}
			}
		}
	}
	// caso nao existam outros coordenadores de planejamento, registrar os itens selecionados
	
	// Foi incluído uma exceção para o perfil 113, já que usuários com este perfil podem possuir unidades
	// que já estejam viculadas à outros usuários do módulo.
//	if(!$confirmarunidades || $_REQUEST["pflcod"] == "113") {
	if(!$confirmarunidades || in_array($_REQUEST["pflcod"],$arPerfis)) {
		
		$sql = $sqlUpdRpuUsu;
		$db->executar($sql);
		foreach ($_POST['usuuniresp'] as $respcod) {
		   $sql = "";
		   if ($respcod>0){
		   $sql = vsprintf($sqlSelUnidade, $respcod);
           $linha = $db->carregar($sql);
			if(is_array($linha) && count($linha)>=1) {
				foreach ($linha as $unidade) {
					$ungcod = $unidade["ungcod"]; 		
					$dados = array($ungcod, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod);
					$sql = vsprintf($sqlInsRpu, $dados);
					$db->executar($sql);
				}
				
			}}
		}
		$concluido = 1;
	}
	//
	// verificar se foi confirmado a substituição do coordenador atual pelo
	// usuario que está sendo liberado e/ou alterado
	else if($unidadesConfirmados) {
		if (is_array($_REQUEST['usuuniresp'])){
		
			foreach ($_REQUEST['usuuniresp'] as $rpu) {
				$sql = sprintf($sqlUpdRpu, $rpu);
				$db->executar($sql);
				$dados = array($rpu, $usucpf, 'A', date("Y-m-d H:i:s"), $pflcod);
				$sql = vsprintf($sqlInsRpu, $dados);
				$db->executar($sql);
			}
			$concluido = 1;	
		}
	}
	//
	// exibir a tela de aviso dos itens que já possuem coordenador e confirmar
	// a substituição pelo usuario que está sendo liberado e/ou alterado
	else {
		//ver(123,$unidadesConfirmados,d);
		$msg = 'Existem usuários ativos com o perfil selecionado para este Unidade:\n\n';
		$msg .= $txtunidadesComCoordenador;
		$msg .= '\nDeseja sobrescrevê-los?\n\n';
		$msg .= 'Ao confirmar, o perfil dos usuários atuais (listados acima) será desativado.';
		?>
		<html>
		<body>
		<form name="formassocia" style="margin:0px;" method="POST">
		<input type="hidden" name="usucpf" value="<?=$usucpf?>">
		<input type="hidden" name="pflcod" value="<?=$pflcod?>">
		<input type="hidden" name="unidadesConfirmados" value="1">
		<?
			foreach ($_POST['usuuniresp'] as $respcod) {
				?><input type="hidden" name="usuuniresp[]" value="<?=$respcod?>"><?
			}
		?>
		</form>
		</body>
		</html>
		
		<script>
			if (confirm("<?=$msg?>")) {		
				document.formassocia.submit();
			} else
			{
				self.close();		
			}
		</script>

		<?
		exit(0);
	}
	
	//ver('fim',d);

	if ($concluido>0) {
		$db->commit();
		?>
	<script>
	window.parent.opener.location.reload();self.close();
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
<title>Unidades</title>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font></div>
<?flush();?>
<DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
<form name="formulario">
<thead><tr>
<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Unidades(s) Gestora(s)</strong></td>
</tr>
<tr>
<?

	  $cabecalho = 'Selecione a(s) Unidade(s) Gestora(s)';
	  $sql = "
		select 
			ungcod, 
			ungdsc
		from 
			public.unidadegestora
		where
			ungcod NOT IN ('152004','152005') AND
			ungstatus = 'A' 
		group by 
			ungcod, 
			ungdsc
		order by 
			ungcod;
	";

	  $RS = @$db->carregar($sql);
	  $nlinhas = count($RS)-1;
	  for ($i=0; $i<=$nlinhas;$i++)
		 {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
	   ?>
	   		
		   		<tr bgcolor="<?=$cor?>">
				<td align="right"><input type="Checkbox" name="ungcod" id="<?=$ungcod?>" value="<?=$ungcod?>" onclick="retorna(<?=$i?>);"><input type="Hidden" name="ungdsc" value="<?=$ungcod.' - '.$ungdsc?>"></td>
				<td align="right" style="color:blue;"><?=$ungcod?></td>
				<td><?=$ungdsc?></td>
				</tr>
	   
	   <?}
?>
</form>
</table>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
<input type="hidden" name="usucpf" value="<?=$usucpf?>">
<input type="hidden" name="pflcod" value="<?=$pflcod?>">
<select multiple size="8" name="usuuniresp[]" id="usuuniresp" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?php

$sql = "
	select distinct p.ungcod as codigo, p.ungcod || ' - ' || p.ungdsc as descricao
	from monitora.usuarioresponsabilidade ur
		inner join public.unidadegestora p on
			ur.ungcod = p.ungcod
	where
		ur.rpustatus = 'A' and
		ur.usucpf = '$usucpf' and
		ur.pflcod = $pflcod and
		ur.prsano = '".$_SESSION['exercicio']."'
	order by 2
";

$RS = @$db->carregar($sql);
if(is_array($RS)) {
	$nlinhas = count($RS)-1;
	if ($nlinhas>=0) {
		for ($i=0; $i<=$nlinhas;$i++) {
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
    		print " <option value=\"$codigo\">$descricao</option>";		
		}
	}
} else {
//	$sql ='';
	$sql = "
		select distinct p.ungcod as codigo, p.ungdsc as descricao
		from public.unidadegestora p
			inner join monitora.progacaoproposto pp on
				p.ungcod = pp.ungcod and pp.usucpf = '" . $usucpf . "'
		where pp.ungcod is not null
	";
//	$RS = @$db->carregar($sql);
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
				print " <option value=\"$codigo\">$codigo - $descricao</option>";
			}
		}
	} else {?>
		<option value="">Selecione a(s) Unidade(s) Gestora(s).</option>
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


var campoSelect = document.getElementById("usuuniresp");


if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++)
		{document.getElementById(campoSelect.options[i].value).checked = true;}
}



function retorna(objeto)
{

	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {tamanho--;}
	if (document.formulario.ungcod[objeto].checked == true){
		campoSelect.options[tamanho] = new Option(document.formulario.ungdsc[objeto].value, document.formulario.ungcod[objeto].value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for(var i=0; i<=campoSelect.length-1; i++){
			if (document.formulario.ungcod[objeto].value == campoSelect.options[i].value)
				{campoSelect.options[i] = null;}
			}
			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Selecione a(s) Unidade(s) Gestora(s).', '', false, false);}
			sortSelect(campoSelect);
	}
}

function moveto(obj) {
	if (obj.options[0].value != '') {
		document.getElementById(obj.value).focus();}
}




</script>
