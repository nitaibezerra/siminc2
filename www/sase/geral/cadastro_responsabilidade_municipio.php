<?

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
include_once "../_constantes.php";
$db = new cls_banco();

include APPRAIZ."includes/funcoes_espelhoperfil.php";

$usucpf = $_REQUEST['usucpf'];
$pflcod = (int)$_REQUEST['pflcod'];

if( $_POST['requisicao'] == 'municipio' ){
	$estuf = $_POST['estado'];
	$sql = "select muncod||'#'||mundescricao from territorios.municipio where estuf = '$estuf' order by mundescricao";
	$municipios = $db->carregarColuna($sql);
	$muncod = implode('|', $municipios);
	echo $muncod;
	exit();
}

/**
 * Regras Server-Side do negócio de responsabilidade
 */
function validaRegrasNegocio( $pflcod ){

	$numeroPermitidoParaMunicipiosSupervisor = 120;
	$numeroPermitidoParaMunicipiosTecnico = 30;

	switch ( $pflcod ) {
		case PFLCOD_SASE_SUPERVISOR:
			if( count($_POST['usuunidresp']) > $numeroPermitidoParaMunicipiosSupervisor ){
				echo "<script>alert('Perfil executivo permite somente {$numeroPermitidoParaMunicipiosSupervisor} municípios para responsabilidade.');</script>";
				$_POST['usuunidresp'] = array($_POST['usuunidresp'][0]);
			}
			break;
        case PFLCOD_SASE_TECNICO_DIVAPE:
		case PFLCOD_SASE_TECNICO:
			if( count($_POST['usuunidresp']) > $numeroPermitidoParaMunicipiosTecnico ){
				echo "<script>alert('Perfil executivo permite somente {$numeroPermitidoParaMunicipiosTecnico} municípios para responsabilidade.');</script>";
				$_POST['usuunidresp'] = array($_POST['usuunidresp'][0]);
			}
			break;
	}

}

/*
 *** INICIO REGISTRO RESPONSABILIDADES ***
 */

if(isset($_REQUEST['enviar'])) {
	
	// desativa todos os elementos  da responsabilidade dessse usuario
	$sql = "UPDATE
			 sase.usuarioresponsabilidade 
			set
			 rpustatus = 'I' 
			where
			 usucpf = '$usucpf'  
			 and pflcod = $pflcod ";
	
	$db->executar($sql);
	
	if($_POST['usuunidresp'][0]){

		// aplica regras de negócio server side
		validaRegrasNegocio( $pflcod );

		foreach($_POST['usuunidresp'] as $muncod){
			$sql = "INSERT INTO sase.usuarioresponsabilidade (muncod, usucpf, rpustatus, rpudata_inc, pflcod) 
					VALUES ('$muncod', '$usucpf', 'A',  now(), '$pflcod')";
			$db->executar($sql);
		}		
	}
	
	atualizarResponsabilidadesSlaves($usucpf,$pflcod);
	
	$db->commit();

	?>
<script>
	
	window.parent.opener.location.reload();self.close();
	
</script>
	<?
	exit(0);
}

/*
 *** FIM REGISTRO RESPONSABILIDADES ***
 */
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Estados e Municípios</title>
<script language="JavaScript" src="../../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<link rel='stylesheet' type='text/css'
	href='../../includes/listagem.css'>

</head>
<body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0"
	MARGINHEIGHT="0" BGCOLOR="#ffffff">
<div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif"
	border="0" align="absmiddle"> <font color=blue size="2">Aguarde!
Carregando Dados...</font></div>
<?/*flush();*/?>
<DIV
	style="OVERFLOW: AUTO; WIDTH: 496px; HEIGHT: 350px; BORDER: 2px SOLID #ECECEC; background-color: White;">
<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
	<script language="JavaScript">
document.getElementById('tabela').style.visibility = "hidden";
document.getElementById('tabela').style.display  = "none";
</script>
	<form name="formulario" method="post" action="">
	
	
	<thead>
		<tr>
			<td valign="top" class="title"
				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
				colspan="3"><strong>Selecione o(s) estado(s)</strong></td>

		</tr>
		<tr>
		<?

		$cabecalho = 'Selecione o(s) estado(s)';
		$sql = "
			select
				estuf, estuf AS codigo, estdescricao, estdescricao AS descricao
			from territorios.estado
			order by estuf, estdescricao
";
		?>
		<tr>
			<td colspan="2">
				<?php
					$db->monta_combo("estuf",$sql,"S","Selecione...","mostraEsconde(this.value)","","","","N","estuf","");
				?>
				&nbsp;&nbsp;
				<button title="Desvincular os municípios ao técnico" class="btn btn-success" type="button" id="btnDesvincular" onclick="desvincularEstados()"><span
					class="glyphicon glyphicon-search"></span> Desvincular
				</button>				
			</td>
		</tr>
</table>
		<?php 
		$RS = @$db->carregar($sql);
		$nlinhas = count($RS)-1;
		$j = 0 ;
		for ($i=0; $i<=$nlinhas;$i++)
		{
			foreach($RS[$i] as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $cor = '#f4f4f4' ; else $cor='#e0e0e0';
			?>
		
		
		<!-- tr bgcolor="<?=$cor?>">
			<td width="20" align="right"><img src="/imagens/mais.gif"
				id="<?=$estuf."_img" ?>" onclick="mostraEsconde('<?=$estuf?>')">&nbsp;</td>
			<td align="left" style="color: blue;"><?=$estuf . ' - ' . $estdescricao?></td>
		</tr>
		<tr>
			<td style="height: 0" colspan="2" -->
		
		<input type="hidden" id="hidEstUf" name="hidEstUf" value="" />
		<div id="<?=$estuf?>" class="divMun" style="display: none;">
			<table width="100%" cellpadding="0" cellspacing="0" border="0">
				<tr bgcolor="#f4f4f4">
					<td align="left" style="border: 0">
						<input type="checkbox" name="muncod" id="<?=$estuf?>" value="<?=$estuf?>" onclick="selecionaTodos( this, '<?=$estuf?>' );" />Todos</td>
				</tr>

			<?
			$sql = "select distinct m.mundescricao, m.muncod, ur.usucpf from territorios.municipio m left join sase.usuarioresponsabilidade ur on m.muncod = ur.muncod and ur.pflcod = ".$pflcod." and ur.rpustatus = 'A' where m.estuf = '$estuf' order by mundescricao";
			$municipios = $db->carregar($sql);
			
			foreach ($municipios as $municipio) {
				if ($cor2 == '#e0e0e0') $cor2 = '#f4f4f4' ; else $cor2='#e0e0e0';
				?>
				<tr bgcolor="<?=$cor2?>" <?php echo $municipio['usucpf'] != '' ? 'title="Este município já está vinculado à um técnico."' : '' ?>>
					<td align="left" style="border: 0"><input type="checkbox"
						<?php echo $municipio['usucpf'] != '' && $municipio['usucpf'] != $usucpf ? 'disabled' : '' ?>
						name="muncod" class="muncod" id="<?=$municipio['muncod']?>"
						value="<?=$municipio['muncod']?>"
						onclick="retorna( this, '<?= $municipio['muncod'] ?>', '<?= $estuf.' - '. addslashes( $municipio['mundescricao'] ) ?>', $('#estuf').val() );" />
						<?=$municipio['mundescricao']?></td>
				</tr>
				<?
}
?>
			</table>
		</div>
			<!-- /td>
		</tr -->
		<?}
		?>
		</form>

<!-- /table -->
</div>
<form name="formassocia" style="margin: 0px;" method="POST"><input
	type="hidden" name="usucpf" value="<?=$usucpf?>"> <input type="hidden"
	name="pflcod" value="<?=$pflcod?>"> <input type="hidden" name="enviar"
	value=""> <select multiple size="8" onclick="mostraMunicipio(this);"
	name="usuunidresp[]" id="usuunidresp" style="width: 500px;"
	class="CampoEstilo">
	<?
	$sql = "
			select 
				distinct m.muncod as codigo, m.estuf||' - '||m.mundescricao as descricao 
			from 
				sase.usuarioresponsabilidade ur inner join territorios.municipio m on ur.muncod = m.muncod
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
</select></form>
<div id="erro"></div>
<table width="100%" align="center" border="0" cellspacing="0"
	cellpadding="2" class="listagem">
	<tr bgcolor="#c0c0c0">
		<td align="right" style="padding: 3px;" colspan="3"><input
			type="Button" name="ok" value="OK"
			onclick="selectAllOptions(campoSelect);enviarFormulario();" id="ok"></td>
	</tr>
</table>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="JavaScript">
var validadeAcrescimo = true;

document.getElementById('aguarde').style.visibility = "hidden";
document.getElementById('aguarde').style.display  = "none";
document.getElementById('tabela').style.visibility = "visible";
document.getElementById('tabela').style.display  = "";

//selecionaTodos

function selecionaTodos(check, estado){
	
	$.ajax({
		type: "POST",
		url: window.location,
		data: "requisicao=municipio&estado="+estado,
		success: function(msg){
			var arrMuncod = msg.split('|');			
			
			/*var muncod 		 = arrMuncod[0];
			var mundescricao = arrMuncod[1];*/
			
			for(i=0; i<arrMuncod.length; i++){
				
				if( validadeAcrescimo ){

					var arrMunicipio = arrMuncod[i].split('#');
					
					if(check.checked == true){
						if( document.getElementById(arrMunicipio[0]).checked == false && !document.getElementById(arrMunicipio[0]).disabled){
							document.getElementById(arrMunicipio[0]).checked = true;
							ultimoChecado = document.getElementById(arrMunicipio[0]);
							retorna( check, arrMunicipio[0], estado+' - '+arrMunicipio[1], estado );
						}
					} else {
						document.getElementById(arrMunicipio[0]).checked = false;
						retorna( check, arrMunicipio[0], estado+' - '+arrMunicipio[1], estado);
					}
				}else{
					ultimoChecado.checked = false;
				}
			}
		}
	});
}

function mostraEsconde(estado){
	var estadoAtual = document.getElementById(estado).style.display;
	//var objImagem = document.getElementById(estado+'_img');
	$('.divMun').attr('style', 'display: none;');
	if(estadoAtual == 'none'){
		document.getElementById(estado).style.display = 'block';
		//objImagem.src = '/imagens/menos.gif';
		
	}else{
		document.getElementById(estado).style.display = 'none';
		//objImagem.src = '/imagens/mais.gif';
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
	document.getElementById('estuf').value = estado;
	document.getElementById('hidEstUf').value = estado;
	var estadoAtual = document.getElementById(estado).style.display;
	if(estadoAtual != 'block'){
		 mostraEsconde(estado);
	}
	document.getElementById(objSelect.value).focus();
		
}

function retorna( check, muncod, mundescricao, estuf )
{
	console.log(validadeAcrescimo);
	if (document.getElementById('hidEstUf').value == ''){
		console.log(estuf);
		document.getElementById('hidEstUf').value = estuf;
	} else {
		if (estuf != document.getElementById('hidEstUf').value) {
			alert('Não é possível selecionar municípios de estados diferentes.');
			check.checked = false;
			return false;
		}
	}
	
	if ( check.checked ){

		validaRegrasNegocio( 
			{
				'input': check,
				'muncod': muncod,
				'mundescricao': mundescricao
			} );

		// põe
		// todo: ajustar para nao avisar novamente aqui
		if( validadeAcrescimo ){
			campoSelect.options[campoSelect.options.length] = new Option( mundescricao, muncod, false, false );
		}else{
			// alert( 'Só são válidos até 100 municípios para perfil Supervisor.' );
			check.checked = false;
		}
	}else if( validadeAcrescimo ){

		// tira
		for( var i = 0; i < campoSelect.options.length; i++ )
		{
			if ( campoSelect.options[i].value == muncod )
			{
				campoSelect.options[i] = null;
			}
		}

		if (campoSelect.options.length == 0){
			document.getElementById('hidEstUf').value = '';
		}
	}
	sortSelect( campoSelect );
}

/**
 * Valida Regras de Negócio Client-Side
 */
 function validaRegrasNegocio( objeto ){

	<?php if( $pflcod == PFLCOD_SASE_SUPERVISOR ){ ?>
 		// console.log(objeto);

		// tratamento para perfil Supervisor
		if( campoSelect.options.length > 119 ){
			if( validadeAcrescimo == true ){
				alert( 'Só são válidos até 120 municípios para perfil Supervisor.' );
			}
			validadeAcrescimo = false;
		}else{
			validadeAcrescimo = true;
		}
	
	<?php }else if( $pflcod == PFLCOD_SASE_TECNICO || $pflcod == PFLCOD_SASE_TECNICO_DIVAPE ){ ?>

		// tratamento para perfil Supervisor
		if( campoSelect.options.length > 29 ){
			if( validadeAcrescimo == true ){
				alert( 'Só são válidos até 30 municípios para perfil Técnico.' );
			}
			validadeAcrescimo = false;
		}else{
			validadeAcrescimo = true;
		}

	<?php } ?>
}

function desvincularEstados(){
	$('.muncod').checked = false;
	enviarFormulario();	
}

</script>