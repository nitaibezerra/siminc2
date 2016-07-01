<?
monta_titulo( "Síntese da dimensão 3 - Ensino e aprendizagem", "&nbsp;");

$sql = "SELECT * FROM pdeinterativo2013.respostatempoaprendizagem WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
$dadosta = $db->pegaLinha($sql);

$rtaestrategia = array();
if($dadosta['rtaestrategia']) {
	$rtaestrategia = explode(";",$dadosta['rtaestrategia']);
}
$rtaestudantepart = array();
if($dadosta['rtaestudantepart']) {
	$rtaestudantepart = explode(";",$dadosta['rtaestudantepart']);
}
$rtaatividade = array();
if($dadosta['rtaatividade']) {
	$rtaatividade = explode(";",$dadosta['rtaatividade']);
	foreach($rtaatividade as $dado) {
		$d = explode(",",$dado);
		$atv[$d[0]] = true;
		$qtd[$d[0]] = $d[1];
	}
}

$rtaporque = array();
if($dadosta['rtaporque']) {
	$rtaporque = explode(";",$dadosta['rtaporque']);
}

?>
<script>

function diagnostico_3_3_sintesedimensao3() {
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
	
	if(marcado || !existe) {
		divCarregando();
		document.getElementById('formulario').submit();
	} else {
		alert('Marque pelo menos um problema');
		return false;
	}
}

function desenvolveEducacaoIntegral(resp) {
	if(resp=='S') {
		document.getElementById('porque_espacofisico').checked=false;
		document.getElementById('porque_profissionais').checked=false;
		document.getElementById('porque_recursosmateriais').checked=false;
		document.getElementById('porque_outro').checked=false;

		document.getElementById('educ_integral_N').style.display='none';
		document.getElementById('educ_integral_S').style.display='';
	} else {
		document.getElementById('estrategia_P').checked=false;
		document.getElementById('estrategia_I').checked=false;
		document.getElementById('estrategia_O').checked=false;
		document.getElementById('qtd_propria_escola').value='';
		document.getElementById('qtd_outra_escola').value='';
		document.getElementById('carga_horaria_semanal').value='';
		document.getElementById('atv_culturais').checked=false;
		document.getElementById('atv_esportivas').checked=false;
		document.getElementById('atv_lazer').checked=false;
		document.getElementById('atv_pedagogicas').checked=false;
		document.getElementById('atv_profissionalizantes').checked=false;
		document.getElementById('atv_outras').checked=false;
		
		document.getElementById('educ_integral_S').style.display='none';
		document.getElementById('educ_integral_N').style.display='';
	}
}

function calculaNumProblemasCriticos() {
	var numCritico = jQuery("[type=checkbox][name^='critico[']").length;
	var numTa = jQuery("[type=checkbox][name^='respostatempoaprendizagem[']").length;
	var numTotal = numCritico+numTa;
	
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
		var criticoMarcados = jQuery("[name^='critico[']:checked").length;
		var tempaprenMarcados = jQuery("[name^='respostatempoaprendizagem[']:checked").length;
		var numMarcados = criticoMarcados+tempaprenMarcados;
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
	calculaNumProblemasCriticos();
});

</script>
<style>
.bordapreto {
border: 1px solid #000;
}
</style>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações</font></td>
	<td class="blue">
	<p>Chegamos à Síntese da Dimensão 3. A tabela abaixo apresenta o conjunto de problemas identificados nesta dimensão. Leia as frases e assinale os problemas considerados mais "críticos" pela escola, até o limite indicado na tabela (que corresponde a 30% do total de problemas). As frases assinaladas aparecerão na Síntese Geral do diagnóstico, depois que todas as dimensões forem respondidas. Após assinalar os problemas críticos, registre se a escola possui ou participa de algum projeto e/ou programa destinado a melhorar ou minimizar esses problemas.</p>
	<p>Lembre-se: todos os problemas são importantes, mas a escola deve concentrar esforços naqueles que ela pode resolver.</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao">
<input type="hidden" name="requisicao" value="diagnostico_3_3_sintesedimensao3">
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
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and ( prgdetalhe='Projeto Pedagógico - Educação Infantil' or prgdetalhe='Projeto Pedagógico' )) AND
				  rp.oppid IN(5,6)";
	
	$pergs_projetospedagogicos = $db->carregar($sql);
	unset($isCritico);
	if($pergs_projetospedagogicos[0]) {
		foreach($pergs_projetospedagogicos as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprojpedag[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasprojpedag) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemasprojpedag)+1)." class=SubTituloDireita><b>Projeto Pedagógico</b></td></tr>";
		foreach($problemasprojpedag as $indice => $projpedag) {
			echo "<tr><td class=bordapreto>".$projpedag."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=\"FALSE\"></td></tr>";	
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Projeto Pedagógico</b></td><td class=bordapreto>Não existem problemas no Projeto Pedagógico</td></tr>";
	}
	
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and (prgdetalhe='Currículo - Educação Infantil' or prgdetalhe='Currículo') ) AND
				  rp.oppid IN(5,6)";
	
	$pergs_curriculo = $db->carregar($sql);
	
	unset($isCritico);
	if($pergs_curriculo[0]) {
		foreach($pergs_curriculo as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemascurriculo[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemascurriculo) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemascurriculo)+1)." class=SubTituloDireita><b>Currículo</b></td></tr>";
		foreach($problemascurriculo as $indice => $curriculo) {
			echo "<tr><td class=bordapreto>".$curriculo."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=\"FALSE\"></td></tr>";	
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Currículo</b></td><td class=bordapreto>Não existem problemas no Currículo</td></tr>";
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and (prgdetalhe='Avaliações - Educação Infantil' or prgdetalhe='Avaliações') ) AND
				  rp.oppid IN(5,6)";
	
	$pergs_avaliacoes = $db->carregar($sql);
	
	unset($isCritico);
	if($pergs_avaliacoes[0]) {
		foreach($pergs_avaliacoes as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasavaliacoes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasavaliacoes) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemasavaliacoes)+1)." class=SubTituloDireita><b>Avaliações</b></td></tr>";
		foreach($problemasavaliacoes as $indice => $avaliacoes) {
			echo "<tr><td class=bordapreto>".$avaliacoes."</td><td align=center class=bordapreto></td></tr>";	
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Avaliações</b></td><td class=bordapreto>Não existem problemas no Avaliações</td></tr>";
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'T' and prgstatus = 'A' and prgdetalhe='Tempo de Aprendizagem') AND
				  rp.oppid IN(5,6)";
	
	$pergs_tempoaprendizagem = $db->carregar($sql);
	
	unset($isCritico);
	if($pergs_tempoaprendizagem[0]) {
		foreach($pergs_tempoaprendizagem as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemastempaprend[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemastempaprend) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemastempaprend)+1)." class=SubTituloDireita><b>Tempo de Aprendizagem</b></td></tr>";
		foreach($problemastempaprend as $indice => $tempoaprendizagem) {
			echo "<tr><td class=bordapreto>".$tempoaprendizagem."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=\"FALSE\"></td></tr>";	
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Tempo de Aprendizagem</b></td><td class=bordapreto>Não existem problemas no Tempo de Aprendizagem</td></tr>";
	}
	
	$sql = "SELECT * FROM pdeinterativo2013.respostatempoaprendizagem WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$respostatempoaprendizagem = $db->pegaLinha($sql);
	
	$_PORQUE = array("espacofisico" => "Não possui espaço físico",
					 "profissionais" => "Não dispoe de profissionais para coordenar as atividades",
					 "recursosmateriais" => "Não dispoe de recursos materiais",
					 "outro" => "Outro");
	if($respostatempoaprendizagem['rtacaso']=="N") {
		$rtaporque = explode(";",$respostatempoaprendizagem['rtaporque']);
		if($rtaporque) {
			
			foreach($rtaporque as $pq) {
				$probl[] = $_PORQUE[$pq];	
			}
			$problemajornadaampliada['rtaporque'] = "A escola não desenvolve ações de educação integral porque: ".implode(", ",$probl);
		}
	}
	
	if(count($problemajornadaampliada) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemajornadaampliada)+1)." class=SubTituloDireita><b>Jornada Ampliada</b></td></tr>";
		foreach($problemajornadaampliada as $indice => $jornadaampliada) {
			echo "<tr><td class=bordapreto>".$jornadaampliada."</td><td align=center class=bordapreto><input type=hidden name=respostatempoaprendizagem[".$indice."critico] value=\"FALSE\"></td></tr>";	
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Jornada Ampliada</b></td><td class=bordapreto>Não existem problemas no Jornada Ampliada</td></tr>";
	}
	?>
	</table>
	</td>
</tr>
</table>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
	<?php $arrTelasPendentes = recuperaTelasPendentes("diagnostico_3_ensinoeaprendizagem") ?>
	<?php if(!$arrTelasPendentes): ?>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_2_tempodeaprendizagem';" >
	<input type="button" name="salvar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_3_sintesedimensao3';diagnostico_3_3_sintesedimensao3();">	
	<input type="button" name="continuar" value="Salvar e continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_0_orientacoes';diagnostico_3_3_sintesedimensao3();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_0_orientacoes';" >
	<?php else: ?>
		<?php foreach($arrTelasPendentes as $tela): ?>
				<p class="red bold"><img src="../imagens/atencao.png" class="img_middle"> Favor preencher a tela <?php echo $tela ?> antes de salvar a Síntese!</p>
		<?php endforeach; ?>
		<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_2_tempodeaprendizagem';" >
		<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_0_orientacoes';" >
	<?php endif; ?>
	</td>
</tr>
</table>
</form>