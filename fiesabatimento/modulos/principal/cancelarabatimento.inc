<?

/*
if( !in_array($_SESSION['usucpf'], $cpf_liberados) ){
	echo '<script>
 			location.href = "fiesabatimento.php?modulo=inicio&acao=C";
 		  </script>';
	die;
}
*/

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);

$idoid = $db->pegaUm("SELECT idoid FROM fiesabatimento.identificacaodocente WHERE idocpf='".$_SESSION['fiesabatimento_var']['cpfusuario']."'");
if($idoid){
	$docid = pegaDocidSolicitacao($idoid);
	$sbaid = $db->pegaUm("SELECT sbaid FROM fiesabatimento.solicitacaoabatimento WHERE idoid = $idoid AND sbarenovacao = false AND sbastatus = 'A'");
	if($docid){
		$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid = $docid");
	}	
}


?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
$(document).ready(function(){
	$('#btn_solicitar_canc').click(function(){
		<?php if( $sbaid && $esdid != WF_FIES1_CANCELADA ){?>
		if( $('.cancelarTR').css('display') == 'none' ){
			$('.cancelarTR').show();
		}else{
			$('.cancelarTR').hide();
		}
		<?php }elseif( $sbaid && $esdid == WF_FIES1_CANCELADA ){?>
		alert('Solicitante já cancelou o abatimento.');
		<?php }else{?>
		alert('Solicitante não possui a solicitação do beneficio do abatimento.');
		<?php }?>
	});
});
</script>
<form name="formulario" method="post" id="formulario">
<input type="hidden" name="requisicao" value="confirmarSolicitacaoCancela">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%"><b>Orientações :</b></td>
		<td>
			<p>O atendimento à solicitação de cancelamento implicará no cancelamento da suspensão da cobrança do pagamento das prestações do financiamento, devendo ser retomado o pagamento das prestações remanescentes a partir do mês imediatamente subsequente ao mês do cancelamento pelo financiado.</p>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" id="btn_solicitar_canc" value="Solicitar Cancelamento do Abatimento" >
		</td>
	</tr>
	<tr class="cancelarTR" style="display:none">
		<td class="SubTituloDireita" >Informe as condições motivadoras para o cancelamento do abatimento</td>
		<td>
			<?php 
			
			$sql = "SELECT
						mccid as codigo,
						mccdesc as descricao
					FROM
						fiesabatimento.motivocancelamento
					WHERE
						mccstatus = 'A'
						and mccid not in(7)
					order by 1";
			$arrMotivos = $db->carregar($sql);
			?>
			<table>
			<?php foreach($arrMotivos as $motivo): ?>
				<tr>
					<td><input type="checkbox" name="mccid[]" id="mccid" value="<?=$motivo['codigo'] ?>" /></td>
					<td><?=$motivo['descricao'] ?></td>
				</tr>
			<?php endforeach; ?>
				<tr>
					<td><input type="checkbox" name="mccid[]" id="mccid" value="7" /></td>
					<td>Outro(s) motivo(s): <?php echo campo_texto("sacoutrosmotivos","N","S","",30,255,"","") ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<tr class="cancelarTR" style="display:none">
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="btn_confirmar" value="Confirmar Solicitação de Cancelamento" onclick="cancelaAbat();">
			<input type="button" name="btn_voltar" value="Voltar" onclick="history.back(-1)" />
		</td>
	</tr>
</table>
</form>
<script>
function cancelaAbat(){
	
	var obj = document.getElementsByName('mccid[]');
	var ct = 0;
	var ctoutros = 0;
	
	for(i=0; i<obj.length; i++){
		if(obj[i].checked == true){
			ct = 1;
			if(obj[i].value == 7){
				ctoutros = 1;
			}
		}
	}
	
	if(ct == 0){
		alert("Você deve selecionar ao menos um motivo de cancelamento para prosseguir com o cancelamento do abatimento.");
		return false;
	}
	
	if(ctoutros == 1 && document.formulario.sacoutrosmotivos.value == ''){
		alert("Favor informar o motivo.");
		document.formulario.sacoutrosmotivos.focus();
		return false;
	}
	
	if( confirm('Deseja prosseguir com o cancelamento da solicitação do abatimento?') ){
		document.formulario.submit();
	}
	
}
</script>