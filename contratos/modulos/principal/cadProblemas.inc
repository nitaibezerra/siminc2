<?php

if($_REQUEST['requisicao'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicao']();
}

if($_REQUEST['requisicaoAjax'] && $_REQUEST['classe']){
	$n = new $_REQUEST['classe'];
	$n->$_REQUEST['requisicaoAjax']();
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br/>";

$db->cria_aba( $abacod_tela, $url, $parametros);
$titulo_modulo = "Problemas na execução do contrato";
monta_titulo( $titulo_modulo, obrigatorio() . ' Indica campo obrigatório' );

if($_SESSION['ctrid']){
	montaCabecalhoContrato( $_SESSION['ctrid'] );
}

if($_POST){
	extract($_POST);
}

// Por padrão os campos vem habilitados
$desabilitado   = false;

$perfis 			= arrayPerfil();
$arPerfisBloqEdicao = array(PERFIL_CONSULTA_UNIDADE,
							PERFIL_CONSULTA_GERAL);

$arDif = array_diff($perfis, $arPerfisBloqEdicao);

// Bloqueia os campos caso o usuáro só tenha os perfis restritos
if (count($arDif) == 0) {
	$desabilitado 	= true;
}

?>
<style>
	.link{cursor:pointer}
</style>
<script type="text/javascript" src="../includes/jquery-ui-1.10.3.custom/js/jquery-1.9.1.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.10.3.custom/js/jquery-ui-1.10.3.custom.min.js"></script>
<link href="../includes/jquery-ui-1.10.3.custom/css/ui-lightness/jquery-ui-1.10.3.custom.min.css" type="text/css" rel="stylesheet"></link>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form name="formulario" id="formulario" method="post" action="">

<input type="hidden" name="classe" id="classe" value="" /> 
<input type="hidden" name="requisicao" id="requisicao" value="" /> 

<?php if (!$desabilitado): ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
		<tr>
	        <td class="SubTituloDireita" align="right" style="vertical-align: top; width: 25%;">Data da ocorrência:</td>
			<td>
				<?php echo campo_data2( 'prbdataocorr', 'S', "S", '', 'N','','','','','','prbdataocorr');?>		
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right">Título do problema:</td>
			<td>
				<?php
					$arrAtributos['name'] = "prbtitulo";
					$arrAtributos['id'] = "prbtitulo";
					$arrAtributos['obrigatorio'] = true;
					$arrAtributos['habilitado'] = true;
					$arrAtributos['size'] = 100;
					$arrAtributos['maxsize'] = 255;
					$arrAtributos['align'] = "left";
					echo campo_texto($arrAtributos);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right">Descrição do Problema:</td>
			<td>
				<?php 
					echo campo_textarea('prbdsc', 'S', 'S', '', '80', '5', null,'','','','','',$prbdsc);
	        	?>
			</td>
		</tr>
	
		<tr>
	        <td class="SubTituloCentro" colspan="2">
	        	<input type="button" value="Salvar" onclick="salvarRegistroProblemas()" />
	        	<input type="button" value="Limpar" onclick="limparCampos()" />
	        </td>
		</tr>
	</table>
<?php  endif; ?>
</form>
<?php
	$ctproblemas = new CtProblemas();
	$ctproblemas->listarProblemas($_SESSION['ctrid']);
?>
<script>

function limparCampos(){
	
	$("[name='prbdataocorr']").val("");
	$("[name='prbtitulo']").val("");
	$("[name='prbdsc']").val("");
}

function salvarRegistroProblemas() {

	if (validarCampos()) {
		$("#requisicao").val("salvarProblema");
		$("#classe").val("CtProblemas");
		$("#formulario").submit();
	}

}

function validarCampos(){

	var d = new Date();
	var month = d.getMonth()+1;
	var day = d.getDate();

    var datahoje = (day<10 ? '0' : '') + day + '/' +
	    			(month<10 ? '0' : '') + month + '/' +
	    			d.getFullYear();
     

	if(!trim($("[name='prbdataocorr']").val())){
		alert('Favor informar a Data da ocorrência.');
		return false;
	}else {

		var prbdataocorr      = $("[name='prbdataocorr']").val();

		var compara1 = parseInt(prbdataocorr.split("/")[2].toString() + prbdataocorr.split("/")[1].toString() + prbdataocorr.split("/")[0].toString());
		var compara2 = parseInt(datahoje.split("/")[2].toString() + datahoje.split("/")[1].toString() + datahoje.split("/")[0].toString());
	
		if (compara1 > compara2){
			alert('A data da ocorrência não deverá ser maior que a data atual.');
			$("[name='prbdataocorr']").val("");
			return false;
		}
	}
	
	if(!trim($("[name='prbtitulo']").val())){
		alert('Favor informar o Título do problema.');
		return false;
	}else if(!trim($("[name='prbdsc']").val())){
		alert('Favor informar a Descrição do problema.');
		return false;
	}

	return true;
	
}

$(function() {
	<?php if($_SESSION['contratos']['problema']['alert']): ?>
		alert('<?php echo $_SESSION['contratos']['problema']['alert'] ?>');
		<?php unset($_SESSION['contratos']['problema']['alert']) ?>
	<?php endif; ?>
		
    
});
</script>