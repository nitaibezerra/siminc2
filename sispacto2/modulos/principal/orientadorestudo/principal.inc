<script>

function exibirRelatoExperiencia(reeid) {
	ajaxatualizar('requisicao=exibirRelatoExperiencia&reeid='+reeid,'modalDiv');
	
	jQuery("#modalDiv").dialog({
	                        draggable:true,
	                        resizable:true,
	                        width: 1300,
	                        height: 600,
	                        modal: true,
	                     	close: function(){} 
	                    });

}

function gravarCorrecoes() {

	if(jQuery( "input:radio[name=correcao1]:checked" ).length==0) {
		alert('Selecione opção : Concisão, clareza e objetividade na descrição do resumo da atividade');
		return false;
	}

	if(jQuery( "input:radio[name=correcao2]:checked" ).length==0) {
		alert('Selecione opção : Adequação do espaço e dos materiais utilizados');
		return false;
	}

	if(jQuery( "input:radio[name=correcao3]:checked" ).length==0) {
		alert('Selecione opção : Adequação da atividade em relação à turma/ faixa etária');
		return false;
	}

	if(jQuery( "input:radio[name=correcao4]:checked" ).length==0) {
		alert('Selecione opção : Potencial da atividade para a melhoria da proficiência na escrita');
		return false;
	}

	if(jQuery( "input:radio[name=correcao5]:checked" ).length==0) {
		alert('Selecione opção : Criatividade e inovação');
		return false;
	}

	var pa='&reeid='+jQuery('#reeid').val()+'&correcao1='+jQuery( "input:radio[name=correcao1]:checked" ).val()+'&correcao2='+jQuery( "input:radio[name=correcao2]:checked" ).val()+'&correcao3='+jQuery( "input:radio[name=correcao3]:checked" ).val()+'&correcao4='+jQuery( "input:radio[name=correcao4]:checked" ).val()+'&correcao5='+jQuery( "input:radio[name=correcao5]:checked" ).val();
	
	ajaxatualizar('requisicao=atualizarCorrecaoRelatoExperiencia'+pa,'dv_listaatv6');
	ajaxatualizar('requisicao=listaProfessoresRelatoExperiencia&iusd=<?=$_SESSION['sispacto2']['orientadorestudo']['iusd'] ?>','dv_listaatv6');

	jQuery("#modalDiv").dialog("close");
}



// FUNÇÕES DOS INSTRUMENTOS

function exibirInstrumentos() {
	ajaxatualizar('requisicao=carregarInstrumentos&iusd=<?=$_SESSION['sispacto2']['orientadorestudo']['iusd'] ?>','modalInstrumentos');

	jQuery("#modalInstrumentos").dialog('open');
	
}

function fecharInstrumentos() {
	jQuery("#modalInstrumentos").dialog('close');
}

function gravarRespostaInstrumento(p) {
	ajaxatualizar('requisicao=gravarRespostaInstrumento&iusd=<?=$_SESSION['sispacto2']['orientadorestudo']['iusd'] ?>'+p,'');
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

	ajaxatualizar('requisicao=carregarInstrumentos&iusd=<?=$_SESSION['sispacto2']['orientadorestudo']['iusd'] ?>','modalInstrumentos');
	if(jQuery("#modalInstrumentos").html()=='') {
		jQuery("#modalInstrumentos").html('<p>Muito obrigada por sua participação! Ela será de grande importância para a avaliação do processo de formação do PNAIC. Se desejar encaminhar comentários e sugestões à pesquisa, envie uma mensagem para '. $_SESSION['email_sistema']. '.</p><p align="center"><input type="button" name="fim" value="Fechar" onclick="fecharInstrumentos();"></p>');
		jQuery("#modalInstrumentos").dialog('open');
	} else {
		jQuery("#modalInstrumentos").dialog('open');
	}
	
}

function verResposta(inrid) {
	if(inrid=='') {
		ajaxatualizar('requisicao=carregarInstrumentos&iusd=<?=$_SESSION['sispacto2']['orientadorestudo']['iusd'] ?>','modalInstrumentos');
	} else {
		ajaxatualizar('requisicao=carregarInstrumentos&iusd=<?=$_SESSION['sispacto2']['orientadorestudo']['iusd'] ?>&inrid='+inrid,'modalInstrumentos');
	}
	jQuery("#modalInstrumentos").dialog('open');
}

<?

$intline = verificarInstrumentosPendentes(array('iusd'=>$_SESSION['sispacto2']['orientadorestudo']['iusd']));

if($intline) :
?>
jQuery(document).ready(function() {

	jQuery("#modalInstrumentos").html('<div style=\"text-align:center;width: 80%;padding: 10px;border: 3px solid black;margin: 0px;\"><p>Caro Professor Alfabetizador,<p> <p>Convidamos você a participar da <b>Avaliação das Ações de Formação Continuada do PNAIC</b>. A sua opinião é de extrema importância para a melhoria do processo de formação, pois ela produzirá informações que buscarão representar a realidade do processo de formação no seu município, estado e região.<p> <p>Sinta-se totalmente à vontade para dizer exatamente o que você pensa pois esta avaliação é totalmente sigilosa.<p> <p>Desde já agradecemos sua colaboração.</p><p align="center"><input type="button" name="iniciar" value="Iniciar" onclick="exibirInstrumentos();"></p></div>');
	
	jQuery("#modalInstrumentos").dialog({
        draggable:true,
        resizable:true,
        width: 800,
        height: 600,
        modal: true,
     	close: function(){} 
    });


});
<? 
endif;
?>

</script>

<style>
.ui-dialog-title {
  text-align: center;
  width: 100%;
}
</style>

<div id="modalInstrumentos" style="display: none;" title="PESQUISA"></div>
<div id="modalDiv" style="display:none;"></div>

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
	<? carregarInformes(array('pflcoddestino' => PFL_ORIENTADORESTUDO)); ?>
	</td>
</tr>
<tr>
	<td colspan="2">
	<table width="100%">
	<tr>
	<td width="50%">
	<p align=center style="font-size:x-small;"><b>Validação da atividade 6 - relatos de experiência dos professores alfabetizadores</b><br>Somente na 6º parcela estará disponível. Clicar no ícone, e ao final da experiência é fundamental selecionar as opções para categorizar a atividade.</p>
	<div style="height:100px;overflow:auto;" id="dv_listaatv6">
	<?
	listaProfessoresRelatoExperiencia(array('iusd'=>$_SESSION['sispacto2']['orientadorestudo']['iusd']));
	?>
	</div>
	
	</td>
	<td width="50%">&nbsp;</td>
	</tr>
	</table>
	</td>
</tr>
<? $perfis = pegaPerfilGeral(); ?>
<? if($_SESSION['sispacto2']['orientadorestudo']['iusdesativado']!='t') : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=dados';">
		</td>
	</tr>
<? elseif($db->testa_superuser() || in_array(PFL_CONSULTAMEC,$perfis)) : ?>
	<tr>
		<td colspan="2">
			<p>Os perfis de Adminstrador e Consulta MEC podem acessar as informações dos orientadores clicando</p>
			<input type="button" value="Acessar Orientador Desabilitado" onclick="divCarregando();window.location='sispacto2.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&requisicao=visualizarDesabilitado&vis=orientadorestudo';">
		</td>
	</tr>
<? endif; ?>
</table>