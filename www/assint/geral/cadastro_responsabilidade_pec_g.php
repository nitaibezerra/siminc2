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

if(is_array($_POST['entSelecionadas'])) {
	$sql = "UPDATE
				assint.usuarioresponsabilidade 
			SET
				rpustatus = 'I' 
			WHERE
				usucpf = '$usucpf'  
				AND pflcod = $pflcod ";
	$db->executar($sql);
	
	if($_POST['entSelecionadas'][0]){
		foreach($_POST['entSelecionadas'] as $co_ies){
			$sql = "INSERT INTO assint.usuarioresponsabilidade(pflcod, usucpf,  rpustatus, rpudata_inc, co_ies)
						   								VALUES($pflcod, '$usucpf', 'A', now(), '$co_ies')";
			$db->executar($sql);
		}		
	}
	$db->commit();
?>
	<script>
		window.parent.opener.location.reload();
		self.close();
	</script>
<?
	exit();
}

/*
*** FIM REGISTRO RESPONSABILIDADES ***
*/
?>
<html>
<head>
	<META http-equiv="Pragma" content="no-cache">
	<title>Universidade</title>
	<script language="JavaScript" src="/includes/funcoes.js"></script>
	<script src="../../includes/prototype.js"></script>
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
	<link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde">
	<img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> 
	<font color=blue size="2">Aguarde! Carregando Dados...</font>
</div>
<div style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
	<form name="formulario">
		<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
			<tr>
				<td colspan="2" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3">
					<strong>Selecione a(s) IES(s)</strong>
				</td>
			</tr>
			<?
				$cabecalho = 'Selecione a(s) Universidade(s)';
				$sql = "SELECT DISTINCT
							ies.co_ies as codigo, 
							ies.no_ies as descricao,
							CASE WHEN ur.co_ies is not null
								THEN 'checked=\"checked\"'
								ELSE ''
							END as ckeck
						FROM 
							emec.ies ies
						LEFT JOIN assint.usuarioresponsabilidade ur ON ur.co_ies = ies.co_ies AND
																	   ur.usucpf  = '{$usucpf}' AND 
  							 										   ur.pflcod  = {$pflcod}
  						ORDER BY
  							2";
				  $RS = @$db->carregar($sql);
				  
				  if($RS){
					  $nlinhas = count($RS)-1;
					  for ($i=0; $i<=$nlinhas;$i++){
							extract($RS[$i]);
							if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
					   ?>
					   		
						   		<tr bgcolor="<?=$cor?>">
									<td align="right">
										<input type="checkbox" <?=$ckeck; ?>name="entid" id="ID<?=$codigo?>" value="<?=$codigo?>" onclick="retorna(<?=$codigo?>);">
										<input type="Hidden" name="entnome" id="DSC<?=$codigo?>" value="<?=$descricao?>">
									</td>
									<td>
										<?=$descricao?>
									</td>
								</tr>
					   
					   <?
					  }
				  }else{
				  	?>
				  		<tr>
							<td align="center" style="color: rgb(204, 0, 0);">Não foram encontrados Registros.</td>
						</tr>
				  	<?php
				  }
			?>
			<tr>
				<td colspan="2">
				</td>
			</tr>
		</table>
	</form>
</div>
<form name="formassocia" style="margin:0px;" method="POST">
	<input type="hidden" name="usucpf" value="<?=$usucpf?>">
	<input type="hidden" name="pflcod" value="<?=$pflcod?>">
	<select multiple size="8" name="entSelecionadas[]" id="entSelecionadas" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
	<?
	$sql = "SELECT DISTINCT
				ies.co_ies as codigo, 
				ies.no_ies as descricao
			FROM 
				emec.ies ies
			INNER JOIN assint.usuarioresponsabilidade ur ON ur.co_ies = ies.co_ies 
			WHERE 
  				 ur.usucpf  = '{$usucpf}'
            AND
                 rpustatus = 'A'
            AND
  				 ur.pflcod  = {$pflcod} ";
	
	$RS = @$db->carregar($sql);
	if(is_array($RS)) {
		$nlinhas = count($RS)-1;
		if ($nlinhas>=0) {
			for ($i=0; $i<=$nlinhas;$i++) {
				foreach($RS[$i] as $k=>$v) ${$k}=$v;
	    		print " <option value=\"$entid\">$codigo - $descricao</option>";		
			}
		}
	} else {?>
	<option value="">Clique na Entidade.</option>
	<?
	}
	?>
	</select>
</form>
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding:3px;" colspan="3">
			<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
		</td>
	</tr>
</table>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";

var campoSelect = document.getElementById("entSelecionadas");

function retorna(objeto) {
	
	tamanho = campoSelect.options.length;
	if (campoSelect.options[0].value=='') {
		tamanho--;
	}
	
	var ID   = document.getElementById( 'ID'+objeto );
	var DSC  = document.getElementById( 'DSC'+objeto );
	
	if ( ID.checked == true ){
		campoSelect.options[tamanho] = new Option(DSC.value, ID.value, false, false);
		sortSelect(campoSelect);
	}
	else {
		for( var i=0; i<=campoSelect.length-1; i++ ){
			if ( ID.value == campoSelect.options[i].value ){
				{campoSelect.options[i] = null;}
			}
		}
		if ( !campoSelect.options[0] ){
			campoSelect.options[0] = new Option('Clique na Entidade.', '', false, false);
		}
		sortSelect(campoSelect);
	}
}

function moveto(obj) {

	/*if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();
	}*/
}
</script>