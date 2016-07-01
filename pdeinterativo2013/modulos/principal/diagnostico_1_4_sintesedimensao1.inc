<?
monta_titulo( "Síntese da dimensão 1 - Indicadores e Taxas", "&nbsp;");
?>
<script>

function diagnostico_1_4_sintesedimensao1() {
	var node_list = document.getElementsByTagName('input');
	var marcado = false;
	var existe = false;
	for (var i = 0; i < node_list.length; i++) {
	    var node = node_list[i];
	    if (node.getAttribute('type') == 'checkbox') {
	    	existe=true;
	    	if(node.checked==true) {
	    		marcado=true;
	    	}
	    }
	}
	/*
	if(jQuery("[name^='exibeprojeto']:checked").length == 0) {
		alert('Informe se a escola desenvolve algum Projeto');
		return false;
	}
	
	if(jQuery("[name^='exibeprograma']:checked").length == 0) {
		alert('Informe se a escola participa ou deseja participar de algum Programa');
		return false;
	}
	
	if(document.getElementById('exibeprojeto_S').checked) {
		if(document.getElementById('projeto_label').childNodes[1].rows[0].cells.length == 1) {
			alert('Insira um projeto');
			return false;
		}
	}
	
	if(document.getElementById('exibeprograma_S').checked) {
		if(document.getElementById('programa_label').childNodes[1].rows[0].cells.length == 1) {
			alert('Insira um programa');
			return false;
		}
	}
	*/
	if(marcado || !existe) {
		divCarregando();
		document.getElementById('formulario').submit();
	} else {
		alert('Marque pelo menos um problema');
		return false;
	}

}

function calculaNumProblemasCriticos() {
	var numIDEB = jQuery("[type='checkbox'][name^='respostaideb[']").length;
	var numTx = jQuery("[type='checkbox'][name^='respostataxarendimento[']").length;
	var numPb = jQuery("[type='checkbox'][name^='respostaprovabrasil[']").length;
	var numTotal = numIDEB+numTx+numPb;
	if(numTotal > 0){
		var num = numTotal*0.3;
		num = num.toFixed(1);
		num +='';
		if(num.search(".") >= 0){
			var NewNum = num.split(".");
			if( ((NewNum[1])*1) >= 5 ){
				num = (NewNum[0]*1);
				num = num + 1;
			}else{
				num = (NewNum[0]*1);
			}
		}
		if(num == 0){
			num = 1;
		}
		jQuery("#num_problemas_possiveis").html(num);
	}
}

function verificaCheckBox(obj) {
	if(obj.checked == true) {
		var num = jQuery("#num_problemas_possiveis").html();
		var idebMarcados = jQuery("[type=checkbox][name^='respostaideb[']:checked").length;
		var txMarcados = jQuery("[type=checkbox][name^='respostataxarendimento[']:checked").length;
		var pbMarcados = jQuery("[type=checkbox][name^='respostaprovabrasil[']:checked").length;
		var numMarcados = idebMarcados+txMarcados+pbMarcados;
		num = num*1;
		numMarcados = numMarcados*1;
		if(numMarcados > num)
		{
			jQuery("[name='" + obj.name + "']").attr("checked","");
			alert('Selecione no máximo ' + num + ' problema(s) como crítico(s)!');
		}
	}
}


jQuery(document).ready(function() {

calculaNumProblemasCriticos()

<? $existePrograma = existePrograma(array('rppmodulo'=>'I')); ?>
<? if($existePrograma=="t") : ?>
document.getElementById('programa_label').style.display='';
carregarProgramas('I');
document.getElementById('exibeprograma_S').checked=true;
<? elseif($existePrograma=="f"): ?>
document.getElementById('exibeprograma_N').checked=true;
<? endif; ?>

<? $existeProjeto = existeProjeto(array('rppmodulo'=>'I')); ?>
<? if($existeProjeto=="t") : ?>
document.getElementById('projeto_label').style.display='';
carregarProjetos('I');
document.getElementById('exibeprojeto_S').checked=true;
<? elseif($existeProjeto=="f"): ?>
document.getElementById('exibeprojeto_N').checked=true;
<? endif; ?>

});

</script>
<style>
.bordapreto {
border: 1px solid #000;
}
</style>
<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento">
<input type="hidden" name="requisicao" value="diagnostico_1_4_sintesedimensao1">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>Chegamos à Síntese da Dimensão 1. A tabela abaixo apresenta o conjunto de problemas identificados nesta dimensão. Leia as frases e assinale os problemas considerados mais "críticos" pela escola, até o limite indicado na tabela (que corresponde a 30% do total de problemas). As frases assinaladas aparecerão na Síntese Geral do diagnóstico, depois que todas as dimensões forem respondidas. Após assinalar os problemas críticos, registre se a escola possui ou participa de algum projeto e/ou programa destinado a melhorar ou minimizar esses problemas.</p>
	<p>Lembre-se: todos os problemas são importantes, mas a escola deve concentrar esforços naqueles que ela pode resolver.</p>
	</td>
</tr>
</table>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Problemas identificados</td>
	<td>
	<table width="100%">
	<tr>
		<td class="SubTituloCentro" width="20%">Tema</td>
		<td class="SubTituloCentro">Problema(s) Identificado(s)</td>
	</tr>
	<?
	// Avaliado os problemas do IDEB
	
	$sql = "SELECT * FROM pdeinterativo2013.respostaideb WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$dadosideb = $db->pegaLinha($sql);
	
	if($dadosideb['ridinicialum']=="N") {
		$problemasideb['ridinicialum'] = "O IDEB dos anos iniciais da escola não melhorou nas duas últimas medições disponíveis";
	}
	if($dadosideb['ridinicialdois']=="N") {
		$problemasideb['ridinicialdois'] = "Não há evidências de que a escola alcançará a próxima meta do IDEB, para os Anos Iniciais";
	}
	if($dadosideb['ridfinalum']=="N") {
		$problemasideb['ridfinalum'] = "O IDEB dos anos finais da escola não melhorou nas duas últimas medições disponíveis";
	}
	if($dadosideb['ridfinaldois']=="N") {
		$problemasideb['ridfinaldois'] = "Não há evidências de que a escola alcançará a próxima meta do IDEB, para os Anos Finais";
	}

	if(count($problemasideb) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemasideb)+1)." class=SubTituloDireita><b>IDEB</b></td></tr>";
		foreach($problemasideb as $indice => $pideb) {
			echo "<tr><td class=bordapreto>".$pideb."</td><input type=hidden name=respostaideb[".$indice."critico] value=FALSE></tr>";	
		}
	}
	
	// Avaliando os problemas das taxas de rendimento
	
	$sql = "SELECT * FROM pdeinterativo2013.respostataxarendimento WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$dadostx = $db->pegaLinha($sql);
	
	if($dadostx['rtrfunaprova']=="N") {
		$problemastx['rtrfunaprova'] = "A taxa de aprovação da escola, no ensino fundamental, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrfunreprova']=="N") {
		$problemastx['rtrfunreprova'] = "A taxa de reprovação da escola, no ensino fundamental, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrfunabandono']=="N") {
		$problemastx['rtrfunabandono'] = "A taxa de abandono da escola, no ensino fundamental, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrmedaprova']=="N") {
		$problemastx['rtrmedaprova'] = "A taxa de aprovação da escola, no ensino médio, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrmedreprova']=="N") {
		$problemastx['rtrmedreprova'] = "A taxa de reprovação da escola, no ensino médio, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrmedabandono']=="N") {
		$problemastx['rtrmedabandono'] = "A taxa de abandono da escola, no ensino médio, não melhorou nos últimos dois anos";
	}

	
	
	if(count($problemastx) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemastx)+1)." class=SubTituloDireita><b>Taxas de rendimento</b></td></tr>";
		foreach($problemastx as $indice => $ptx) {
			echo "<tr><td class=bordapreto>".$ptx."</td><input type=hidden name=respostataxarendimento[".$indice."critico] value=FALSE></tr>";	
		}
	}
	
	// Avaliando os problemas da prova brasil
	
	$sql = "SELECT * FROM pdeinterativo2013.respostaprovabrasil WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$dadospb = $db->pegaLinha($sql);
	
	if($dadospb['rpbinicialport']=="N") {
		$problemaspb['rpbinicialport'] = "Os resultados de Língua Portuguesa na Prova Brasil, nos anos iniciais, não demonstram evolução nas duas últimas medições";
	}
	if($dadospb['rpbinicialmat']=="N") {
		$problemaspb['rpbinicialmat'] = "Os resultados de Matemática  na Prova Brasil, nos anos iniciais, não demonstram evolução nas duas últimas medições";
	}
	if($dadospb['rpbfinalport']=="N") {
		$problemaspb['rpbfinalport'] = "Os resultados de Língua Portuguesa na Prova Brasil, nos anos finais, não demonstram evolução nas duas últimas medições";
	}
	if($dadospb['rpbfinalmat']=="N") {
		$problemaspb['rpbfinalmat'] = "Os resultados de Matemática  na Prova Brasil, nos anos finais, não demonstram evolução nas duas últimas medições";
	}

	if(count($problemaspb) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemaspb)+1)." class=SubTituloDireita><b>Prova Brasil</b></td></tr>";
		foreach($problemaspb as $indice => $ppb) {
			echo "<tr><td class=bordapreto>".$ppb."</td><input type=hidden name=respostaprovabrasil[".$indice."critico] value=FALSE></tr>";	
		}
	}
	if(count($problemasideb) == 0 && count($problemastx) == 0 && count($problemaspb) == 0) {
		echo "<tr><td colspan=2 align=right class=SubTituloCentro>A evolução desse(s) indicador(es) não apresentou problemas.</td></tr>";
	}
	?>
	</table>
	</td>
</tr>



</table>
</form>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita">O que deseja fazer agora?</td>
	<td>
	<?php $arrTelasPendentes = recuperaTelasPendentes("diagnostico_1_indicadoresetaxas") ?>
	<?php if(!$arrTelasPendentes): ?>
		<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_3_provabrasil';" >
		<input type="button" name="salvar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_4_sintesedimensao1';diagnostico_1_4_sintesedimensao1();"> 
		<input type="button" name="continuar" value="Salvar e continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento';diagnostico_1_4_sintesedimensao1();">
		<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento';" >
	<?php else: ?>
		<?php foreach($arrTelasPendentes as $tela): ?>
				<p class="red bold"><img src="../imagens/atencao.png" class="img_middle"> Favor preencher a tela <?php echo $tela ?> antes de salvar a Síntese!</p>
		<?php endforeach; ?>
		<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_1_indicadoresetaxas&aba1=diagnostico_1_3_provabrasil';" >
		<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento';" >
	<?php endif; ?>
	</td>
</tr>
</table>