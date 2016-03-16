<?php 
require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
monta_titulo( $titulo_modulo, '' );

if( $_REQUEST['req'] == 'enviar' && $_REQUEST['esdid'] ){
	
	$esdid = $_REQUEST['esdid'];
	if( $_REQUEST['tpmid'] ){
		$grupo = implode(",",$_REQUEST['tpmid']);
	}else{
		$grupo = 0;
	}
	
	switch ($esdid) {
	    case WF_TIPO_EM_CORRECAO:
			reenviaEmailDiligenciaProinfancia( $grupo );
	        break;
	    default:
	    	echo "<script>alert('Nada foi feito');</script>";
	} 
}
?>
<form id="reenviaForm" method="post">
	<input type="hidden" id="req" name="req" value=""/>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="subtituloDireita" width="20%">
				Estado do Documento: 
			</td>
			<td>
				<select style="width: auto;" class="CampoEstilo" name="esdid" id="esdid">
					<option value="0">Escolha uma fase de tramitação</option>
					<option value="<?=WF_TIPO_EM_CORRECAO ?>">Em diligência</option>
				</select>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="20%">
				Grupo do Municipio: 
			</td>
			<td>
				<?php
					$sql = "SELECT 
								tpmid as codigo, 
								tpmdsc as descricao
							FROM 
								territorios.tipomunicipio
							WHERE
								tpmstatus = 'A' 
								AND gtmid = 7
							ORDER BY
								descricao  ";
					$db-> monta_checkbox('tpmid[]', $sql, $_POST['tpmid']);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="20%">
			</td>
			<td>
				<input type="button" value="Enviar E-mails" onclick="validaForm()"/>
			</td>
		</tr>
	</table>
</form>
<script>
function validaForm(){
	var form = document.getElementById('reenviaForm');
	var req  = document.getElementById('req');
	var esdid  = document.getElementById('esdid');
	if( esdid.value == 0 ){
		alert('Escolha uma fase de tramitação');
		esdid.focus();
		return false;
	}
	if(confirm('Realmente deseja enviar para os municipios enquadrados nas especificações informadas?')){
		req.value = 'enviar';
		form.submit();
	}
}
</script>