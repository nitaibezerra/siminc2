<?
function salvar(){
	
	global $db;
	
	extract($_POST);
	
	$sql = "UPDATE fiesabatimento.parametrosabatimento SET pdastatus = 'I'";
	
	$db->executar( $sql );
	$db->commit();
	
	$pdadatainiciosolicitacao 	= formata_data_sql($pdadatainiciosolicitacao);
	$pdadatafimsolicitacao 		= formata_data_sql($pdadatafimsolicitacao);

	$pdadatainiciorenovacao 	= formata_data_sql($pdadatainiciorenovacao);
	$pdadatafimrenovacao 		= formata_data_sql($pdadatafimrenovacao);

	$pdadatainicioaprovacao 	= formata_data_sql($pdadatainicioaprovacao);
	$pdadatafimaprovacao 		= formata_data_sql($pdadatafimaprovacao);
	
	$sql = "INSERT INTO fiesabatimento.parametrosabatimento(
	
	            --pdaprazoaprovacao, 
	            pdaprazoreabertura, 
	
	            pdadatainiciosolicitacao, 
	            pdadatafimsolicitacao,

	            pdadatainiciorenovacao,
	            pdadatafimrenovacao,

				pdaano,
	
	            pdadatainicioaprovacao, 
	            pdadatafimaprovacao, 
	
	            usucpf)
			VALUES (
				--$pdaprazoaprovacao,
				$pdaprazoreabertura,
	
				'$pdadatainiciosolicitacao',
				'$pdadatafimsolicitacao',

				'$pdadatainiciorenovacao',
				'$pdadatafimrenovacao',

				'$pdaano',
	
				'$pdadatainicioaprovacao',
				'$pdadatafimaprovacao',
	
				'".$_SESSION['usucpf']."')";
	
	$db->executar( $sql );
	$db->commit();
	
	echo "	<script>
				alert('Dados alterados.');
				window.location.href = window.location.href;
			</script>";
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$sql = "SELECT
			pdaprazoaprovacao, 
            pdaprazoreabertura, 

            to_char(pdadatainiciosolicitacao,'DD/MM/YYYY') as pdadatainiciosolicitacao, 
            to_char(pdadatafimsolicitacao,'DD/MM/YYYY') as pdadatafimsolicitacao,

            to_char(pdadatainiciorenovacao,'DD/MM/YYYY') as pdadatainiciorenovacao,
            to_char(pdadatafimrenovacao,'DD/MM/YYYY') as pdadatafimrenovacao,

            pdaano as pdaano,

            to_char(pdadatainicioaprovacao,'DD/MM/YYYY') as pdadatainicioaprovacao, 
            to_char(pdadatafimaprovacao,'DD/MM/YYYY') as pdadatafimaprovacao
		FROM
			fiesabatimento.parametrosabatimento
		WHERE
			pdastatus = 'A'";

$param = $db->pegaLinha( $sql );

extract($param)

?>
<script src="/includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>
	$(document).ready(function(){

		$('.salvar').click(function(){
			$('#req').val('salvar');
			$('#formulario_param').submit();
		});
	});
</script>
<div id="loader-container" style="position: absolute; background-color: white; 
	 opacity: .6; width:110%; height:2000%; margin-top: -200px; 
	 margin-left: -20px; Z-index:22; display:none;" >
</div>
<form id="formulario_param" name="formulario_param" action="" method="post" >
	<input type="hidden" id="req" name="req" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita" width="30%"><b>Orientações :</b></td>
			<td>
			<p>
				Esta área tem por finalidade a parametrização de prazos, datas limites e outras variáveis de sistema.
			</p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">Parâmetros de Sistema</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">SOLICITAÇÃO</td>
		</tr>
<!-- 		<tr> -->
<!-- 			<td class="SubTituloDireita">Prazo para 'Rejeição por decurso de prazo' :</td> -->
<!-- 			<td>	 
				<?=campo_texto("pdaprazoaprovacao", "S", "S", "Prazo para \'Rejeição por decurso de prazo\' ", 4, 3, "[#]", "", "", "", 0, "", "", $pdaprazoaprovacao); ?>
 				Dias -->
<!-- 			</td> -->
<!-- 		</tr> -->
		<tr>
			<td class="SubTituloDireita">Prazo regularização de Solicitação Reaberta :</td>
			<td>	
				<?=campo_texto("pdaprazoreabertura", "S", "S", "Prazo regularização de Solicitação Reaberta ", 4, 3, "[#]", "", "", "", 0, "", "", $pdaprazoreabertura); ?>
				Dias
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Início para Solicitar o Abatimento :</td>
			<td>	
				<?=campo_data2('pdadatainiciosolicitacao','S','S','','##/##/####','','', $pdadatainiciosolicitacao,'', '', 'pdadatainiciosolicitacao' );?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Término para Solicitar o Abatimento :</td>
			<td>	
				<?=campo_data2('pdadatafimsolicitacao','S','S','','##/##/####','','', $pdadatafimsolicitacao,'', '', 'pdadatafimsolicitacao' );?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Início para o Secretário Aprovar o Abatimento :</td>
			<td>	
				<?=campo_data2('pdadatainicioaprovacao','S','S','','##/##/####','','', $pdadatainicioaprovacao,'', '', 'pdadatainicioaprovacao' );?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Término para o Secretário Aprovar o Abatimento :</td>
			<td>	
				<?=campo_data2('pdadatafimaprovacao','S','S','','##/##/####','','', $pdadatafimaprovacao,'', '', 'pdadatafimaprovacao' );?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">RENOVAÇÃO</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Ano Referência Término para Renovação :</td>
			<td>
				<?=campo_texto("pdaano", "S", "S", "Ano Referência para Renovação", 4, 4, "[#]", "", "", "", 0, "", "", $pdaano); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Início para Renovar o Abatimento :</td>
			<td>
				<?=campo_data2('pdadatainiciorenovacao','S','S','','##/##/####','','', $pdadatainiciorenovacao,'', '', 'pdadatainiciorenovacao' );?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de Término para Renovar o Abatimento :</td>
			<td>
				<?=campo_data2('pdadatafimrenovacao','S','S','','##/##/####','','', $pdadatafimrenovacao,'', '', 'pdadatafimrenovacao' );?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2"><center><input type="button" class="salvar" value="Salvar"/></center></td>
		</tr>
	</table>
</form>