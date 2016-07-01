<?php
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

include APPRAIZ."includes/funcoes_espelhoperfil.php";

$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

if( $_POST['requisicao'] == 'programa' ){
	$sql_prg = "SELECT prg.prgid||'#'||prg.prgdsc  
				FROM par.programa prg  
				INNER JOIN par.pfadesao pfa ON prg.prgid = pfa.prgid 
				ORDER BY prg.prgdsc";
	$programas = $db->carregarColuna($sql_prg);
	$prgid = implode('|', $programas);
	echo $prgid;
	exit();
}

/*** INICIO REGISTRO RESPONSABILIDADES ***/

if(isset($_REQUEST['enviar'])) {
	$sql = "UPDATE par.usuarioresponsabilidade 
			SET rpustatus = 'I' 
			WHERE usucpf = '$usucpf' AND pflcod = $pflcod ";
	$db->executar($sql);
	
	if($_POST['usuunidresp'][0]){
		foreach($_POST['usuunidresp'] as $prgid){
			$sql = "INSERT INTO par.usuarioresponsabilidade (prgid, usucpf, rpustatus, rpudata_inc, pflcod) VALUES ($prgid, '$usucpf', 'A',  now(), '$pflcod')";
			$db->executar($sql);
		}		
	}
	atualizarResponsabilidadesSlaves($usucpf,$pflcod);
	$db->commit(); 
	echo "<script>window.parent.opener.location.reload();self.close();</script>";
	exit(0);
}

/*** FIM REGISTRO RESPONSABILIDADES ***/
?>
<html>
<head>
	<META http-equiv="Pragma" content="no-cache">
	<title>Programas</title>
	<script language="javaScript" src="../../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
	<link rel='stylesheet' type='text/css'	href='../../includes/listagem.css'>
</head>
<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">

<div align=center id="aguarde">
	<img src="/imagens/icon-aguarde.gif" border="0" align="middle"> 
	<font color=blue size="2">Aguarde! Carregando Dados...</font>
</div>

<div style="OVERFLOW: AUTO; width: 496px; HEIGHT: 350px; border: 2px SOLID #ECECEC; background-color: White;">
<script language="javascript" type="text/javascript">
	document.getElementById('tabela').style.visibility = "hidden";
	document.getElementById('tabela').style.display  = "none";
</script>

<?php 
$cabecalho = 'Selecione o(s) programa(s)';
$sql_programa = "SELECT prg.prgid, prg.prgdsc  
				FROM par.programa prg   
				INNER JOIN par.pfadesao pfa ON prg.prgid = pfa.prgid
				ORDER BY prg.prgdsc";
$rs_programa = $db->carregar($sql_programa);
$nlinhas = count($rs_programa)-1; ?>

<form name="formulario" method="post" action="">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
	<tr bgcolor="e0e0e0">
		<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3">
			<strong>Selecione o(s) programa(s)</strong>
		</td>
	</tr>
	<tr bgcolor="#f4f4f4">
		<td align="left" style="border: 0">
		<input type="checkbox" name="prgid" id="todos" value="todos" onclick="selecionaTodos( this, 'todos' );" />Todos
		</td>
	</tr>
	<?php foreach($rs_programa as $programa){ 
	if (fmod($nlinhas,2) == 1) $cor = '#f4f4f4' ; else $cor='#e0e0e0'; ?>
	<tr bgcolor="<?=$cor?>">
		<td align="left" style="border: 0">
		<input type="checkbox" name="prgid" id="<?php echo $programa['prgid']; ?>" value="<?php echo $programa['prgid'];?>" onclick="retorna( this, '<?= $programa['prgid'] ?>', '<?= addslashes( $programa['prgdsc'] ) ?>' );" /><?=$programa['prgdsc'];?></td>
	</tr>
	<?php $nlinhas = $nlinhas - 1; } ?>
</table>
</form>
</div>
<form name="formassocia" style="margin: 0px;" method="POST">
	<input type="hidden" name="usucpf" value="<?php echo $usucpf?>"> 
	<input type="hidden" name="pflcod" value="<?php echo $pflcod?>"> <input type="hidden" name="enviar" value=""> 
	<select multiple size="8" name="usuunidresp[]" id="usuunidresp" style="width: 500px;" class="CampoEstilo">
		<?php $sql_resp = "SELECT DISTINCT prg.prgid as codigo, prg.prgdsc as descricao 
				   		   FROM par.usuarioresponsabilidade usu
				   		   INNER JOIN par.programa prg ON prg.prgid = usu.prgid
				   		   INNER JOIN par.pfadesao pfa ON pfa.prgid = prg.prgid
		 		   		   WHERE usu.rpustatus='A' AND usu.usucpf = '$usucpf' AND usu.pflcod=$pflcod";
			$rs_resp = $db->carregar($sql_resp);
			if(is_array($rs_resp)) {
				$nlinhas = count($rs_resp)-1;
				if ($nlinhas>=0) {
					for ($i=0; $i<=$nlinhas;$i++) {
						foreach($rs_resp[$i] as $k=>$v) ${$k}=$v;
						print " <option value=\"$codigo\">$descricao</option>";
					}
				}
			}?>
	</select>
</form>

<div id="erro"></div>

<table width="100%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding: 3px;" colspan="3">
		<input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);enviarFormulario();" id="ok"></td>
	</tr>
</table>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="JavaScript">
document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";

//SelecionaTodos
function selecionaTodos(check, estado){
	$.ajax({
		type: "POST",
		url: window.location,
		data: "requisicao=programa",
		success: function(msg){
			var arrProg = msg.split('|');			
			for(i=0; i<arrProg.length; i++){
				var arrPrograma = arrProg[i].split('#');
				if(check.checked == true){
					if( document.getElementById(arrPrograma[0]).checked == false ){
						document.getElementById(arrPrograma[0]).checked = true;
						retorna( check, arrPrograma[0], arrPrograma[1] );
					}
				} else {
					document.getElementById(arrPrograma[0]).checked = false;
					retorna( check, arrPrograma[0], arrPrograma[1]);
				}
			}
		}
	});
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

function retorna( check, prgid, prgdsc ){
	if ( check.checked ){
		//Põe
		campoSelect.options[campoSelect.options.length] = new Option( prgdsc, prgid, false, false );
	} else {
		// Tira
		for( var i = 0; i < campoSelect.options.length; i++ ){
			if ( campoSelect.options[i].value == prgid ){
				campoSelect.options[i] = null;
			}
		}
	}
	sortSelect( campoSelect );
}
</script>