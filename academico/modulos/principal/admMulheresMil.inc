<?php

if($_REQUEST['requisicao'] == 'salvaDados'){
	
	$mmaano 					= $_REQUEST['mmaano'] ? $_REQUEST['mmaano'] : 'null';
	$mmaaberturapreenchimento 	= $_REQUEST['mmaaberturapreenchimento'] ? formata_data_sql($_REQUEST['mmaaberturapreenchimento']) : '';
	$mmafechamentopreenchimento = $_REQUEST['mmafechamentopreenchimento'] ? formata_data_sql($_REQUEST['mmafechamentopreenchimento']) : '';  
	$mmaaberturaanalise			= $_REQUEST['mmaaberturaanalise'] ? formata_data_sql($_REQUEST['mmaaberturaanalise']) : ''; 
	$mmafechamentoanalise		= $_REQUEST['mmafechamentoanalise'] ? formata_data_sql($_REQUEST['mmafechamentoanalise']) : '';
	$mmaaberturarecurso			= $_REQUEST['mmaaberturarecurso'] ? formata_data_sql($_REQUEST['mmaaberturarecurso']) : '';
	$mmafechamentorecurso		= $_REQUEST['mmafechamentorecurso'] ? formata_data_sql($_REQUEST['mmafechamentorecurso']) : '';
	
	if(empty($_REQUEST['mmaid'])){
	
		$sql = "insert into academico.mulheresmiladministracao
				(
					mmaano, 
					mmaaberturapreenchimento, 
					mmafechamentopreenchimento, 
					mmaaberturaanalise, 
					mmafechamentoanalise, 
					mmaaberturarecurso, 
					mmafechamentorecurso
				)
				values
				(
					{$mmaano},
					'{$mmaaberturapreenchimento}',
					'{$mmafechamentopreenchimento}',
					'{$mmaaberturaanalise}',
					'{$mmafechamentoanalise}',
					'{$mmaaberturarecurso}',
					'{$mmafechamentorecurso}'
				)";
		
	}else{
		
		$sql = "update academico.mulheresmiladministracao
				set
					mmaano 						= {$mmaano}, 
					mmaaberturapreenchimento 	= '{$mmaaberturapreenchimento}', 
					mmafechamentopreenchimento 	= '{$mmafechamentopreenchimento}', 
					mmaaberturaanalise 			= '{$mmaaberturaanalise}', 
					mmafechamentoanalise 		= '{$mmafechamentoanalise}', 
					mmaaberturarecurso 			= '{$mmaaberturarecurso}', 
					mmafechamentorecurso 		= '{$mmafechamentorecurso}'
				where 
					mmaid = ".$_REQUEST['mmaid'];
		
	}
	
	$db->executar($sql);
	$db->commit();
	//$db->sucesso('principal/admMulheresMil');

echo "<script>
		alert('Operação realizada com sucesso');
	</script >";	
}


include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

monta_titulo("Mulheres Mil", "Administração");

$mmaano = $_REQUEST['mmaano'];
if ($mmaano == ''){
	$mmaano = '2012';
}



$sql = "select * from academico.mulheresmiladministracao  where mmaano = ".$mmaano;
$rsDados = $db->pegaLinha($sql);

if($rsDados){
	extract($rsDados);
}

?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script> 
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script>
jQuery.noConflict();

function mudaano()
{
	jQuery('[name=requisicao]').val('');
	jQuery("form#formulario").submit();
}

jQuery(document).ready(function(){
	 jQuery('#mmaano').change(function (){
      	mudaano();
     }); 
});

function salvarDados()
{
	if(jQuery('[name=mmaano]').val() == ''){
		alert('O campo Ano é obrigatório!');
		jQuery('[name=mmaano]').focus();
		return false;
	}
	if(jQuery('[name=mmaaberturapreenchimento]').val() == ''){
		alert('O campo Data Abertura Preenchimento é obrigatório!');
		jQuery('[name=mmaaberturapreenchimento]').focus();
		return false;
	}
	if(jQuery('[name=mmafechamentopreenchimento]').val() == ''){
		alert('O campo Data Fechamento Preenchimento é obrigatório!');
		jQuery('[name=mmafechamentopreenchimento]').focus();
		return false;
	}
	if(jQuery('[name=mmaaberturaanalise]').val() == ''){
		alert('O campo Data Abertura Análise é obrigatório!');
		jQuery('[name=mmaaberturaanalise]').focus();
		return false;
	}
	if(jQuery('[name=mmafechamentoanalise]').val() == ''){
		alert('O campo Data Fechamento Análise é obrigatório!');
		jQuery('[name=mmafechamentoanalise]').focus();
		return false;
	}
	if(jQuery('[name=mmaaberturarecurso]').val() == ''){
		alert('O campo Data Abertura Recurso é obrigatório!');
		jQuery('[name=mmaaberturarecurso]').focus();
		return false;
	}
	if(jQuery('[name=mmafechamentorecurso]').val() == ''){
		alert('O campo Data Fechamento Recurso é obrigatório!');
		jQuery('[name=mmafechamentorecurso]').focus();
		return false;
	}
	document.formulario.submit();
}
</script>
<form method="post" name="formulario" id="formulario"  action="">
	<input type="hidden" id="requisicao" name="requisicao" value="salvaDados"/>
	<input type="hidden" id="mmaid" name="mmaid" value="<?php echo $rsDados['mmaid'] ? $rsDados['mmaid'] : '' ?>"/>
	<table class="tabela" height="95%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >		
		<tr>
			<td class="subtituloDireita">Ano:</td>
			<td>
				<select class="CampoEstilo obrigatorio" name="mmaano" id = "mmaano" >
					<option value="">Selecione...</option>
					<option <?php echo( $mmaano=="2012" ? "selected": ""); ?> value="2012">2012</option>
					<option <?php echo( $mmaano=="2013" ? "selected": ""); ?> value="2013">2013</option>
				</select>
				<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Data Abertura Preenchimento:</td>
			<td>
				<?php echo campo_data2( 'mmaaberturapreenchimento', 'S', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Data Fechamento Preenchimento:</td>
			<td>
				<?php echo campo_data2( 'mmafechamentopreenchimento', 'S', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Data Abertura Análise:</td>
			<td>
				<?php echo campo_data2( 'mmaaberturaanalise', 'S', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Data Fechamento Análise:</td>
			<td>
				<?php echo campo_data2( 'mmafechamentoanalise', 'S', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Data Abertura Recurso:</td>
			<td>
				<?php echo campo_data2( 'mmaaberturarecurso', 'S', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Data Fechamento Recurso:</td>
			<td>
				<?php echo campo_data2( 'mmafechamentorecurso', 'S', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita"></td>
			<td>
				<input type="button" value="Salvar" onclick="salvarDados()" />
			</td>
		</tr>
	</table>
</form>