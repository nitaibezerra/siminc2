<?php
header('content-type: text/html; charset=ISO-8859-1');

if($_POST['requisicao'] == 'salvarDocumenta') {
	extract($_POST);
	
	$prpdocumenta = str_replace("/","", $prpdocumenta);
	$prpdocumenta = str_replace("-","", $prpdocumenta);
	
	$sql = "UPDATE par.processopar SET prpdocumenta = '$prpdocumenta' WHERE prpnumeroprocesso = '$prpnumeroprocesso'";
	$db->executar( $sql );
	if( $db->commit() ){
		echo $prpdocumenta;
	} else {
		echo 'erro';
	}
	exit();
}

$sql = "SELECT 
			pp.prpnumeroprocesso,
		    mun.mundescricao||'/'||mun.estuf as municipios,
		    pp.prpdocumenta 
		FROM
			par.processopar pp
		    inner join territorios.municipio mun on mun.muncod = pp.muncod
		WHERE
			pp.prpnumeroprocesso = '".$_REQUEST['processo']."'
			and pp.prpstatus = 'A'
			";

$arrProc = $db->pegaLinha( $sql );

?>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form method="post" name="formularioDoc" id="formularioDoc">
<input type="hidden" name="pagina" id="pagina" value="<?=$_GET['pagina']; ?>">
<table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td class="SubTitulocentro" colspan="2">Informe o número da documenta para continuar</td>		
	</tr>
	<tr>
		<td class="SubTituloDireita">Número de Processo:</td>
		<td>
		<?php
			$prpnumeroprocesso = substr($arrProc['prpnumeroprocesso'],0,5) . "." .
								 substr($arrProc['prpnumeroprocesso'],5,6) . "/" .
								 substr($arrProc['prpnumeroprocesso'],11,4) . "-" .
								 substr($arrProc['prpnumeroprocesso'],15,2);
			echo simec_htmlentities( $prpnumeroprocesso );
		?>
		<input type="hidden" name="prpnumeroprocesso" value="<?=$arrProc['prpnumeroprocesso']; ?>">
		</td>
		
	</tr>
	<tr>
		<td class="SubTituloDireita" >Município:</td>
		<td>
		<?php 
			echo simec_htmlentities( $arrProc['municipios'] ); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Nº do Documento:</td>
		<td>
		<?php
			if( !empty($arrProc['prpdocumenta']) ){
				$prpdocumenta = substr($arrProc['prpdocumenta'],0,7) . "/" .
								substr($arrProc['prpdocumenta'],7,4) . "-" .
								substr($arrProc['prpdocumenta'],11,2);
			}
							
			echo campo_texto('prpdocumenta', 'N', 'S', '', 30, 30, '', '', '', '', 0, 'id="prpdocumenta" onkeypress="return somenteNumeros(event);"', "this.value=mascaraglobal('#######/####-#',this.value);" );
		?>
		</td>
	</tr>
	<tr>
		<td align="center" style="background-color:#c0c0c0;" colspan="2">
			<input type="button" value="Salvar" name="btnSalvar" id="btnSalvar">
			<input type="button" value="Fechar" name="btnVoltar" id="btnVoltar" onclick="javascript: closeMessage();">
		</td>
	</tr>
</table>
</form>
<div id="debug"></div>
<script type="text/javascript">
jQuery(document).ready(function(){
	jQuery('#btnSalvar').click(function(){			
		var prpdocumenta = jQuery('#prpdocumenta').val();
		var pagina = jQuery('#pagina').val();
		
		if( prpdocumenta == '' ){
			alert('É obrigatório informar o número do documento!');
			jQuery('#prpdocumenta').focus();
			return false;
		}
		
		divCarregando();
		var dados = jQuery('#formularioDoc').serialize();
		jQuery.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/popupCadastraDocumenta&acao=A",
	   		data: "requisicao=salvarDocumenta&"+dados,
	   		async: true,
	   		success: function(msg){
	   			
	   			if(msg != 'erro'){
	   				window.location.href = 'par.php?modulo=principal/'+pagina+'&acao=A';
	   				closeMessage();
	   				tramitar(msg);
	   			}else{
	   				alert('Falha na operação');
	   			}
	   			//document.getElementById('debug').innerHTML = msg;
	   			//alert(msg);
	   		}
		});	
		divCarregado();
	});
});
</script>