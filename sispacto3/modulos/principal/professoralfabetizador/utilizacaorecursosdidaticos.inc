<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function gravarUsoMateriaisDidaticos() {
	
	var opcoesmarcadas    = jQuery("[name^='usomaterialdidatico[']:enabled:checked").length;
	
	if(opcoesmarcadas!=<?=$db->pegaUm("SELECT count(*) FROM sispacto3.aprendizagemconhecimento WHERE cattipo='R'") ?>) {
		alert('Marque todas a opções');
		return false;
	}
	
	divCarregando();
	
	jQuery('#salvar').attr('disabled','disabled');
	jQuery('#inserirfoto').attr('disabled','disabled');
	
	document.getElementById('formulario').submit();
}


jQuery(document).ready(function() {});


</script>

<form method="post" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="gerenciarUsoMateriaisDidaticos">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Uso dos Materiais Didáticos</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao("/sispacto3/sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&rfuparcela=".$rfuparcela); ?></td>
	</tr>
	
	<tr>
		<td colspan="2">
		<p><b>Com que frequência você tem utilizado os recursos didáticos sugeridos pelo Pacto?</b></p>
		<?php 
		
		$sql = "SELECT catdsc,
						   '<center><input type=\"radio\" name=\"usomaterialdidatico['||catid||']\" value=\"F\" '||COALESCE((SELECT 'checked'::text FROM sispacto3.usomateriaisdidaticos c WHERE c.catid=ac.catid AND iusd=".$_SESSION['sispacto3']['professoralfabetizador']['iusd']." AND c.umdopcao='F'),'')||' id=\"usomaterialdidatico_'||catid||'_F\"</center>' as comfrequencia,
						   '<center><input type=\"radio\" name=\"usomaterialdidatico['||catid||']\" value=\"R\" '||COALESCE((SELECT 'checked'::text FROM sispacto3.usomateriaisdidaticos c WHERE c.catid=ac.catid AND iusd=".$_SESSION['sispacto3']['professoralfabetizador']['iusd']." AND c.umdopcao='R'),'')||' id=\"usomaterialdidatico_'||catid||'_R\"</center>' as raramente,
						   '<center><input type=\"radio\" name=\"usomaterialdidatico['||catid||']\" value=\"N\" '||COALESCE((SELECT 'checked'::text FROM sispacto3.usomateriaisdidaticos c WHERE c.catid=ac.catid AND iusd=".$_SESSION['sispacto3']['professoralfabetizador']['iusd']." AND c.umdopcao='N'),'')||' id=\"usomaterialdidatico_'||catid||'_N\"</center>' as nunca 
				FROM sispacto3.aprendizagemconhecimento ac WHERE cattipo='R'";
		
	   $cabecalho = array("&nbsp;",
						  "<b>Com frequência</b><br><span style=font-size:xx-small;>(Mínimo 1x por semana)</span>",
						  "<b>Raramente</b><br><span style=font-size:xx-small;>(Mínimo 1x por mês)</span>",
						  "<b>Nunca</b>");

	   $db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
		
		
		?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Salvar" id="salvar" onclick="gravarUsoMateriaisDidaticos();"> 
		</td>
	</tr>
</table>
</form>
