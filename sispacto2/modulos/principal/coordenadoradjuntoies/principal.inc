<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Principal</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">Orientações</td>
	<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
</tr>
<tr>
	<td colspan="2">
	
	<? carregarInformes(array('pflcoddestino' => PFL_COORDENADORADJUNTOIES)); ?>
	
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="20%">&nbsp;</td>
	<td><input type="button" name="proximo" value="Próximo" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&aba=dados';"></td>
</tr>
</table>
<style>
.ui-dialog-title {
  text-align: center;
  width: 100%;
}
</style>

<div id="modalInstrumentos" style="display: none;" title="PESQUISA"></div>
<?

$sql = "SELECT iraid FROM sispacto2.instrumentosrespostasabertas WHERE iusd='".$_SESSION['sispacto2']['coordenadoradjuntoies']['iusd']."' AND (irapergunta1 IS NOT NULL OR irapergunta2 IS NOT NULL OR irapergunta3 IS NOT NULL OR irapergunta4 IS NOT NULL OR irapergunta5 IS NOT NULL)";
$iraid = $db->pegaUm($sql);

if(!$iraid) :

$html = '<form method="post" id="formulario_pesquisa" enctype="multipart/form-data"><input type="hidden" name="requisicao" value="salvarInstrumentosAbertos"><input type="hidden" name="iusd" value="'.$_SESSION['sispacto2']['coordenadoradjuntoies']['iusd'].'"><table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%"><tr><td>1. Quais as principais dificuldades na execução do processo de formação continuada no âmbito do PNAIC? Seja o mais detalhista possível.</td></tr><tr><td><textarea name=irapergunta1 id=irapergunta1 cols=70 rows=6></textarea></td></tr><tr><td>2. Quais as principais facilidades na execução do processo de formação continuada no âmbito do PNAIC? Seja o mais detalhista possível.</td></tr><tr><td><textarea name=irapergunta2 id=irapergunta2 cols=70 rows=6></textarea></td></tr><tr><td>3. Como você analisa a qualidade pedagógica do material de formação do PNAIC? Seja o mais detalhista possível.</td></tr><tr><td><textarea name=irapergunta3 id=irapergunta3 cols=70 rows=6></textarea></td></tr><tr><td>4. Quais são as barreiras e facilitadores que influem na realização das suas atribuições no âmbito do PNAIC? Seja o mais detalhista possível.</td></tr><tr><td><textarea name=irapergunta4 id=irapergunta4 cols=70 rows=6></textarea></td></tr><tr><td>5. Em sua opinião, qual é a contribuição real do PNAIC para a alfabetização das crianças? Seja o mais detalhista possível.</td></tr><tr><td><textarea name=irapergunta5 id=irapergunta5 cols=70 rows=6></textarea></td></tr><tr><td align=center><input type=button name=inserirpesquisa value="Gravar" onclick="gravarInstrumentosAbertos();"></td></tr></table></form>';

?>
<script>
	function gravarInstrumentosAbertos() {
		if(jQuery('#irapergunta1').val()=='') {
			alert('Preencha a pergunta 1.');
			return false;
		}
		if(jQuery('#irapergunta2').val()=='') {
			alert('Preencha a pergunta 2.');
			return false;
		}
		if(jQuery('#irapergunta3').val()=='') {
			alert('Preencha a pergunta 3.');
			return false;
		}
		if(jQuery('#irapergunta4').val()=='') {
			alert('Preencha a pergunta 4.');
			return false;
		}
		if(jQuery('#irapergunta5').val()=='') {
			alert('Preencha a pergunta 5.');
			return false;
		}

		jQuery('#formulario_pesquisa').submit();
	}


	jQuery("#modalInstrumentos").html('<div style=\"text-align:center;width: 95%;padding: 10px;margin: 0px;\"><?=$html ?></div>');
	
	jQuery("#modalInstrumentos").dialog({
        draggable:true,
        resizable:true,
        width: 800,
        height: 750,
        modal: true,
     	close: function(){} 
    });
</script>
<? 
endif;
?>