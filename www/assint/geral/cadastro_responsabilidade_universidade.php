<?php
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

//if($_REQUEST['req'] == 'listarIntituicao' ){
//	
// 	$cabecalho = 'Selecione a(s) Universidade(s)';
//	$sql = "SELECT 
//			  e.entid,
//			  e.entnumcpfcnpj,
//			  e.entnome,
//			  CASE WHEN rpuid is not null
//			  	THEN 'checked=\"checked\"'
//			  	ELSE ''
//			  END as ckeck
//			FROM
//				assint.entidadeassessoriainternacional eai
//			INNER JOIN 
//				entidade.entidade e ON eai.entid = e.entid
//			LEFT JOIN
//				assint.usuarioresponsabilidade ur ON ur.entid      = e.entid AND 
//													 ur.rpustatus  = 'A'  AND 
//				  									 ur.usucpf     = '{$_REQUEST['usucpf']}' AND 
//				  									 ur.pflcod     = {$_REQUEST['$pflcod']}
//			WHERE 
//				eai.entstatus = 'A' AND 
//				e.entstatus   = 'A' AND
//				eai.enttipo   = '{$_REQUEST['tipo']}'";
//	
//	$RS = @$db->carregar($sql);
//	
//	if($RS){
//		echo "<table class=\"listagem\">";
//		$nlinhas = count($RS)-1;
//		for ($i=0; $i<=$nlinhas;$i++){
//			extract($RS[$i]);
//			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
//	   			echo "  <tr bgcolor=\"{$cor}\" >
//						<td align=\"right\">
//							<input type=\"checkbox\" {$ckeck}name=\"entid\" id=\"{$entid}\" value=\"{$entid}\" onclick=\"retorna({$i});\">
//							<input type=\"Hidden\" name=\"entnome\" value=\"".$entnumcpfcnpj." - ".$entnome."\">
//						</td>
//						<td align=\"right\" style=\"color:blue;\">{$entnumcpfcnpj}</td>
//						<td>{$entnome}</td>
//						</tr>";
//		}
//		echo "</table>";
//	}else{
//		echo "<table class=\"listagem\">";
//		echo "  <tr>
//					<td align=\"center\" style=\"color: rgb(204, 0, 0);\">Não foram encontrados Registros.</td>
//				</tr>";
//		echo "</table>";
//	}
//	unset($_REQUEST['req']);
//	die();
//}

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];

/*
*** INICIO REGISTRO RESPONSABILIDADES ***
*/

if(is_array($_POST['entSelecionadas'])) {
	$sql = "update
			 assint.usuarioresponsabilidade 
			set
			 rpustatus = 'I' 
			where
			 usucpf = '$usucpf'  
			 and pflcod = $pflcod ";
	$db->executar($sql);
	
	if($_POST['entSelecionadas'][0]){
		foreach($_POST['entSelecionadas'] as $entid){
			$sql = "insert into assint.usuarioresponsabilidade (pflcod, usucpf,  rpustatus, rpudata_inc, entid)
						   								values ($pflcod, '$usucpf', 'A', now(), '$entid')";
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
			<script language="JavaScript">
				document.getElementById('tabela').style.visibility = "hidden";
				document.getElementById('tabela').style.display  = "none";
			</script>
			<tr>
				<td colspan="2" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3"><strong>Selecione a(s) Universidade(s)</strong>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="110px" > Tipo da Entidade:
				</td>
				<td>	
					<input type="radio" name="enttipo" id="enttipoI" value="I" onclick="mostraInstituicao(this.value);" /> Instituição
					<input type="radio" name="enttipo" id="enttipoU" value="U" onclick="mostraInstituicao(this.value);" checked="checked" /> Universidade
				</td>
			</tr> 
			<tr>
				<td colspan="2">
					<div id="divEntidadeI" style="display: none">
						<table class="listagem">
						<?
							  $cabecalho = 'Selecione a(s) Universidade(s)';
							  $sql = "SELECT 
										  e.entid,
										  e.entnumcpfcnpj,
										  e.entnome,
										  CASE WHEN rpuid is not null
										  	THEN 'checked=\"checked\"'
										  	ELSE ''
										  END as ckeck
										FROM
											assint.entidadeassessoriainternacional eai
										INNER JOIN 
											entidade.entidade e ON eai.entid = e.entid
										LEFT JOIN
											assint.usuarioresponsabilidade ur ON ur.entid 	   = e.entid AND
																				 ur.rpustatus  = 'A'  AND 
											  									 ur.usucpf     = '{$usucpf}' AND 
											  									 ur.pflcod     = {$pflcod} 
										WHERE 
											eai.entstatus = 'A' AND 
											eai.enttipo   = 'I' AND
											e.entstatus = 'A'";
							  
							  $RS = @$db->carregar($sql);
							  
							  if($RS){
								  $nlinhas = count($RS)-1;
								  for ($i=0; $i<=$nlinhas;$i++){
										extract($RS[$i]);
										if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
								   ?>
								   		
									   		<tr bgcolor="<?=$cor?>">
												<td align="right">
													<input type="checkbox" <?=$ckeck; ?>name="entid" id="ID<?=$entid?>" value="<?=$entid?>" onclick="retorna(<?=$entid?>);">
													<input type="Hidden" name="entnome" id="DSC<?=$entid?>" value="<?=$entnumcpfcnpj.' - '.$entnome?>">
												</td>
												<td align="right" style="color:blue;">
													<?=$entnumcpfcnpj?>
												</td>
												<td>
													<?=$entnome?>
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
						//xd(789);
						?>
						</table>
					</div>
					<div id="divEntidadeU">
						<table class="listagem" style="display: block">
						<?
							  $cabecalho = 'Selecione a(s) Universidade(s)';
							  $sql = "SELECT 
										  e.entid,
										  e.entnumcpfcnpj,
										  e.entnome,
										  CASE WHEN rpuid is not null
										  	THEN 'checked=\"checked\"'
										  	ELSE ''
										  END as ckeck
										FROM
											assint.entidadeassessoriainternacional eai
										INNER JOIN 
											entidade.entidade e ON eai.entid = e.entid
										LEFT JOIN
											assint.usuarioresponsabilidade ur ON ur.entid 	   = e.entid AND 
											  									 ur.rpustatus  = 'A' AND 
											  									 ur.usucpf     = '{$usucpf}' AND 
											  									 ur.pflcod     = {$pflcod}
										WHERE 
											eai.entstatus = 'A' AND 
											eai.enttipo   = 'U' AND
											e.entstatus = 'A'";
							  
							  $RS = @$db->carregar($sql);
							  
							  if($RS){
								  $nlinhas = count($RS)-1;
								  for ($i=0; $i<=$nlinhas;$i++){
										extract($RS[$i]);
										if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
								   ?>
								   		
									   		<tr bgcolor="<?=$cor?>">
												<td align="right">
													<input type="checkbox" <?=$ckeck; ?>name="entid" id="ID<?=$entid?>" value="<?=$entid?>" onclick="retorna(<?=$entid?>);">
													<input type="Hidden" name="entnome" id="DSC<?=$entid?>" value="<?=$entnumcpfcnpj.' - '.$entnome?>">
												</td>
												<td align="right" style="color:blue;">
													<?=$entnumcpfcnpj?>
												</td>
												<td>
													<?=$entnome?>
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
						//xd(789);
						?>
						</table>
					</div>
				</td>
			</tr>
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
			  e.entid, 
			  e.entnumcpfcnpj as codigo, 
			  e.entnome as descricao 
			FROM 
			  assint.usuarioresponsabilidade ur	INNER JOIN entidade.entidade e 
			  ON (ur.entid = e.entid)
			WHERE 
			  ur.rpustatus  = 'A' 
			  AND ur.usucpf = '$usucpf' 
			  AND ur.pflcod = $pflcod";
	
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

//if (campoSelect.options[0].value != ''){
//	for(var i=0; i<campoSelect.options.length; i++){
//		document.getElementById(campoSelect.options[i].value).checked = true;
//	}
//}

//function abreconteudo(objeto) {
//if (document.getElementById('img'+objeto).name=='+')
//	{
//	document.getElementById('img'+objeto).name='-';
//    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
//	document.getElementById(objeto).style.visibility = "visible";
//	document.getElementById(objeto).style.display  = "";
//	}
//	else
//	{
//	document.getElementById('img'+objeto).name='+';
//    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
//	document.getElementById(objeto).style.visibility = "hidden";
//	document.getElementById(objeto).style.display  = "none";
//	}
//}

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
//
//function retorna(objeto)
//{
//	//alert(objeto);
//	tamanho = campoSelect.options.length;
//	if (campoSelect.options[0].value=='') {
//		tamanho--;
//	}
//	//alert(tamanho);
//	
//	var arEntid   = document.getElementsByName( 'entid' );
//	var arEntnome = document.getElementsByName( 'entnome' );
//	
//	
//	
//	if (arEntid[objeto].checked == true){
//		campoSelect.options[tamanho] = new Option(arEntnome[objeto].value, arEntid[objeto].value, false, false);
//		sortSelect(campoSelect);
//	}
//	else {
//		for(var i=0; i<=campoSelect.length-1; i++){
//			if (arEntid[objeto].value == campoSelect.options[i].value)
//				{campoSelect.options[i] = null;}
//			}
//			if (!campoSelect.options[0]){campoSelect.options[0] = new Option('Clique na Entidade.', '', false, false);}
//			sortSelect(campoSelect);
//	}
//}

function moveto(obj) {

	/*if (obj.options[0].value != '') {
		if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
			abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
		}
		document.getElementById(obj.value).focus();
	}*/
}

//function carregaInstituicao( tipo ){
//	
//	var div = $('divEntidade');
//
//	
//	return new Ajax.Request(window.location.href,{
//		method: 'post',
//		parameters: '&req=listarIntituicao&tipo=' + tipo,
//		onComplete: function(res){
//			div.innerHTML = res.responseText;
//		}
//	});	
//}

function mostraInstituicao( tipo ){

	var divI = $('divEntidadeI');
	var divU = $('divEntidadeU');

	if( tipo == 'I' ){
		
		divI.style.display = 'block';
		divU.style.display = 'none';
		
	}else{
		
		divI.style.display = 'none';
		divU.style.display = 'block';
		
	}	
}
</script>