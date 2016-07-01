<?
verificarCoordenadorIESTermoCompromisso(array("uncid"=>$_SESSION['sispacto']['universidade']['uncid']));


$docidorcamento = $db->pegaUm("SELECT docidorcamento FROM sispacto.universidadecadastro WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."'");

if(!$docidorcamento) {
	$docidorcamento = wf_cadastrarDocumento(TPD_FLUXOAPROVACAO,"SISPACTO - Orçamento Execução - ".$_SESSION['sispacto']['universidade']['uncid']);
	$db->executar("UPDATE sispacto.universidadecadastro SET docidorcamento='".$docidorcamento."' WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."'");
	$db->commit();
}


$estado = wf_pegarEstadoAtual( $docidorcamento );

if($estado['esdid'] != ESD_ORCAMENTO_EM_ELABORACAO) {
	$consulta = true;
}

?>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>

<script>

function atualizarOrcamentoExecucao() {

	var diferenca = parseFloat(replaceAll(replaceAll(jQuery('#diferenca').val(),'.',''),',','.'));
	
	if(diferenca > 0) {
		var conf = confirm('A soma dos valores atualizados é maior que o saldo disponível. Confirma a gravação?');	
		if(conf) {
			jQuery('#formulario').submit();
		} else {
			return false;
		}
	
	} else {
		jQuery('#formulario').submit();
	}
	
}

function calcularOrcamentoExecucao() {

	var totalvalorexecutado  = 0;
	var totalsaldo           = 0;
	var totalvaloratualizado = 0;
	
	jQuery("[id^='valorprevisto_']").each(function() {
	
		var orcid = replaceAll(jQuery(this).attr('id'),'valorprevisto_','');
		
		var valorprevisto  = parseFloat(replaceAll(replaceAll(jQuery('#valorprevisto_'+orcid).val(),'.',''),',','.'));
		
		var valorexecutado = 0;
		if(jQuery('#valorexecutado_'+orcid).val()!='') {
			valorexecutado = parseFloat(replaceAll(replaceAll(jQuery('#valorexecutado_'+orcid).val(),'.',''),',','.'));
		}
		
		var valoratualizado = 0;
		if(jQuery('#valoratualizado_'+orcid).val()!='') {
			valoratualizado = parseFloat(replaceAll(replaceAll(jQuery('#valoratualizado_'+orcid).val(),'.',''),',','.'));
		}

		var saldo          = valorprevisto-valorexecutado;
		
		if(saldo < 0) {
			jQuery('#saldo_'+orcid).val('-'+mascaraglobal('###.###.###,##',saldo.toFixed(2)));
		} else {
			jQuery('#saldo_'+orcid).val(mascaraglobal('###.###.###,##',saldo.toFixed(2)));
		}
		
		totalvalorexecutado  += valorexecutado;
		totalsaldo           += saldo;
		totalvaloratualizado += valoratualizado;

	});
	
	jQuery('#totalvalorexecutado').val(mascaraglobal('###.###.###,##',totalvalorexecutado.toFixed(2)));
	
	jQuery('#totalvaloratualizado').val(mascaraglobal('###.###.###,##',totalvaloratualizado.toFixed(2)));
	
	var totalvalorprevisto = parseFloat(replaceAll(replaceAll(jQuery('#totalvalorprevisto').val(),'.',''),',','.'));
	
	var diferenca = totalsaldo-totalvaloratualizado;
	
	if(diferenca < 0) {
		jQuery('#diferenca').val('-'+mascaraglobal('###.###.###,##',diferenca.toFixed(2)));
	} else {
		jQuery('#diferenca').val(mascaraglobal('###.###.###,##',diferenca.toFixed(2)));
	}
	
	if(totalsaldo < 0) {
		jQuery('#totalsaldo').val('-'+mascaraglobal('###.###.###,##',totalsaldo.toFixed(2)));
	} else {
		jQuery('#totalsaldo').val(mascaraglobal('###.###.###,##',totalsaldo.toFixed(2)));
	}
	
}


<? if($consulta) : ?>
jQuery(function() {
jQuery("[id='salvar']").remove();
});
<? endif; ?>

</script>
<form method="post" id="formulario">
<input type="hidden" name="goto" id="goto" value="">
<input type="hidden" name="requisicao" value="atualizarOrcamentoExecucao">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Orçamento</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações</td>
		<td><? echo carregarOrientacao($_SERVER['REQUEST_URI']); ?></td>
	</tr>
	<tr>
		<td colspan="2">
		<table width="100%">
		<tr>
			<td width="99%" valign="top">
			<?
			carregarListaCustos(array("consulta"=>$consulta,"uncid"=>$_SESSION['sispacto']['universidade']['uncid'],"execucao" => true));
			?>
			<? if($estado['esdid'] == ESD_ORCAMENTO_APROVADO) : ?>
			<p><b>PARECER DO MEC</b></p>
			<p><?=$db->pegaUm("SELECT cmddsc FROM workflow.comentariodocumento WHERE docid='".$docidorcamento."'") ?></p>
			<? endif; ?>
			</td>
			<td width="1%" valign="top">
			<?
			/* Barra de estado atual e ações e Historico */
			wf_desenhaBarraNavegacao($docidorcamento, array('uncid' => $_SESSION['sispacto']['universidade']['uncid']) );
 			?>
			</td>
		</tr>
		</table>
		
		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">&nbsp;</td>
		<td>
			<input type="button" value="Anterior" onclick="divCarregando();window.location='sispacto.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=planoatividade';">
			<input type="button" value="Salvar" id="salvar" onclick="atualizarOrcamentoExecucao();">
			<input type="button" value="Próximo" onclick="divCarregando();window.location='sispacto.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=relatoriofinal';">
		</td>
	</tr>
</table>
</form>