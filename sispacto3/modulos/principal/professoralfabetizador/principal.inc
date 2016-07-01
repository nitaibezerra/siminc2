<script>
function exibirInstrumentos() {
	ajaxatualizar('requisicao=carregarInstrumentos&iusd=<?=$_SESSION['sispacto3']['professoralfabetizador']['iusd'] ?>','modalInstrumentos');

	jQuery("#modalInstrumentos").dialog('open');
	
}

function fecharInstrumentos() {
	jQuery("#modalInstrumentos").dialog('close');
}

function gravarRespostaInstrumento(p) {
	ajaxatualizar('requisicao=gravarRespostaInstrumento&iusd=<?=$_SESSION['sispacto3']['professoralfabetizador']['iusd'] ?>'+p,'');
}

function avancarInstrumento() {
	var problemas = false;
	jQuery("[id^='inoid_']").each(function() {
		if(jQuery(this).val()=='') {
			problemas = true;
		}
	});

	if(problemas==true) {
		alert('Todas questões devem estar preenchidas');
		return false;
	}

	ajaxatualizar('requisicao=carregarInstrumentos&iusd=<?=$_SESSION['sispacto3']['professoralfabetizador']['iusd'] ?>','modalInstrumentos');
	if(jQuery("#modalInstrumentos").html()=='') {
		jQuery("#modalInstrumentos").html('<p>Muito obrigada por sua participação! Ela será de grande importância para a avaliação do processo de formação do PNAIC. Se desejar encaminhar comentários e sugestões à pesquisa, envie uma mensagem para '. $_SESSION['email_sistema']. '.</p><p align="center"><input type="button" name="fim" value="Fechar" onclick="fecharInstrumentos();"></p>');
		jQuery("#modalInstrumentos").dialog('open');
	} else {
		jQuery("#modalInstrumentos").dialog('open');
	}
	
}

function verResposta(inrid) {
	if(inrid=='') {
		ajaxatualizar('requisicao=carregarInstrumentos&iusd=<?=$_SESSION['sispacto3']['professoralfabetizador']['iusd'] ?>','modalInstrumentos');
	} else {
		ajaxatualizar('requisicao=carregarInstrumentos&iusd=<?=$_SESSION['sispacto3']['professoralfabetizador']['iusd'] ?>&inrid='+inrid,'modalInstrumentos');
	}
	jQuery("#modalInstrumentos").dialog('open');
}

</script>
<style>
.ui-dialog-title {
  text-align: center;
  width: 100%;
}
</style>

<div id="modalInstrumentos" style="display: none;" title="PESQUISA"></div>

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
	<? carregarInformes(array('pflcoddestino' => PFL_PROFESSORALFABETIZADOR)); ?>
	</td>
</tr>
<? $perfis = pegaPerfilGeral(); ?>
<? if($_SESSION['sispacto3']['professoralfabetizador']['iusdesativado']!='t') : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=dados';">
		</td>
	</tr>
<? elseif($db->testa_superuser() || in_array(PFL_CONSULTAMEC,$perfis)) : ?>
	<tr>
		<td colspan="2">
			<p>Os perfis de Adminstrador e Consulta MEC podem acessar as informações dos professores clicando</p>
			<input type="button" value="Acessar Professor Desabilitado" onclick="divCarregando();window.location='sispacto.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&requisicao=visualizarDesabilitado&vis=professoralfabetizador';">
		</td>
	</tr>
<? endif; ?>
</table>