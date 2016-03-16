<?
monta_titulo( "Síntese da dimensão 4 - Gestão", "&nbsp;");
?>
<script>

function diagnostico_4_4_sintesedimensao4() {
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
	
	if(jQuery("[name^='exibeprojeto']:checked").length == 0) {
		alert('Clique se a escola desenvolve algum PROJETO');
		return false;
	}
	
	if(jQuery("[name^='exibeprograma']:checked").length == 0) {
		alert('Clique se a escola desenvolve algum PROGRAMA');
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

	if(marcado || !existe) {
		divCarregando();
		document.getElementById('formulario').submit();
	} else {
		alert('Marque pelo menos um problema');
		return false;
	}
}

function calculaNumProblemasCriticos() {
	var numCritico = jQuery("[type=checkbox][name^='critico[']").length;
	var numPe = jQuery("[type=checkbox][name^='pessoas[']").length;
	var numTotal = numCritico+numPe;
	
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
		var pessoasMarcados = jQuery("[name^='pessoas[']:checked").length;
		var numMarcados = criticoMarcados+pessoasMarcados;
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

<? $existePrograma = existePrograma(array('rppmodulo'=>'G')); ?>
<? if($existePrograma=="t") : ?>
document.getElementById('programa_label').style.display='';
carregarProgramas('G');
document.getElementById('exibeprograma_S').checked=true;
<? elseif($existePrograma=="f"): ?>
document.getElementById('exibeprograma_N').checked=true;
<? endif; ?>

<? $existeProjeto = existeProjeto(array('rppmodulo'=>'G')); ?>
<? if($existeProjeto=="t") : ?>
document.getElementById('projeto_label').style.display='';
carregarProjetos('G');
document.getElementById('exibeprojeto_S').checked=true;
<? elseif($existeProjeto=="f"): ?>
document.getElementById('exibeprojeto_N').checked=true;
<? endif; ?>

});

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações:</font></td>
	<td class="blue">
	<p>Chegamos à Síntese da Dimensão 4. A tabela abaixo apresenta o conjunto de problemas identificados nesta dimensão. Leia as frases e assinale os problemas considerados mais "críticos" pela escola, até o limite indicado na tabela (que corresponde a 30% do total de problemas). As frases assinaladas aparecerão na Síntese Geral do diagnóstico, depois que todas as dimensões forem respondidas. Após assinalar os problemas críticos, registre se a escola possui ou participa de algum projeto e/ou programa destinado a melhorar ou minimizar esses problemas.</p>
	<p>Lembre-se: todos os problemas são importantes, mas a escola deve concentrar esforços naqueles que ela pode resolver.</p>
	</td>
</tr>
</table>
<style>
.bordapreto {
border: 1px solid #000;
}
</style>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar">
<input type="hidden" name="requisicao" value="diagnostico_4_4_sintesedimensao4">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%">Problemas identificados</td>
	<td>
	<table width="100%">
	<tr>
		<td class="SubTituloCentro" width="20%">Tema</td>
		<td class="SubTituloCentro">Problema(s) Identificado(s)</td>
		<td class="SubTituloCentro">Problema(s) Críticos<br/>(Máximo <span id="num_problemas_possiveis">X</span>)</td>
	</tr>
	<? 
	$sql = "SELECT tp.tpeid, tp.tpedesc, count(p.pesid) as qtd FROM pdeinterativo.pessoa p 
			LEFT JOIN pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = p.pesid 
			LEFT JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = ptp.pdeid 
			LEFT JOIN pdeinterativo.detalhepessoa dp ON dp.pesid = p.pesid 
			LEFT JOIN pdeinterativo.tipoperfil tp ON tp.tpeid = ptp.tpeid 
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND dp.tenid IN(2,3) AND tp.tpeid IN(SELECT tpeid FROM pdeinterativo.perfilarea WHERE apeid IN('".APE_EQUIPEPEDAGOGICA."','".APE_DIRETOR."','".APE_VICEDIRETOR."','".APE_SECRETARIA."'))
			GROUP BY tp.tpeid, tp.tpedesc";
	
	$pessoas = $db->carregar($sql);
	
	if($pessoas[0]) {
		foreach($pessoas as $pes) {
			$sql = "SELECT p.pesid, p.critico FROM pdeinterativo.pessoa p 
					LEFT JOIN pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = p.pesid
					LEFT JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = ptp.pdeid 
					LEFT JOIN pdeinterativo.detalhepessoa dp ON dp.pesid = p.pesid 
					WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND dp.tenid IN(2,3) AND ptp.tpeid='".$pes['tpeid']."'";
			
			$p = $db->carregar($sql);
			unset($pesids);
			if($p[0]) {
				foreach($p as $po) {
					$pesids[] = $po['pesid'];
				}
			}
			$isCriticoP[implode(",",$pesids)] = (($po['critico']=="t")?TRUE:FALSE);
			$problemasdirecao1[implode(",",$pesids)] = "Há ".$pes['qtd']." ".$pes['tpedesc']." que não possui(em) graduação."; 
		}
	}
	
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Liderança') AND
				  rp.oppid IN(5,6)";
	
	$pergs_lideranca = $db->carregar($sql);

	if($pergs_lideranca[0]) {
		foreach($pergs_lideranca as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdirecao2[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Acompanhamento') AND
				  rp.oppid IN(5,6)";
	
	$pergs_acompanhamento = $db->carregar($sql);
	if($pergs_acompanhamento[0]) {
		foreach($pergs_acompanhamento as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdirecao2[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasdirecao1) > 0 || count($problemasdirecao2) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemasdirecao1)+count($problemasdirecao2)+1)." class=SubTituloDireita><b>Direção</b></td></tr>";
		if($problemasdirecao1) {
			foreach($problemasdirecao1 as $indice => $prodirecao) {
				echo "<tr><td class=bordapreto>".$prodirecao."</td><td align=center class=bordapreto><input type=hidden name=pessoas[".$indice."] value=FALSE><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCriticoP[$indice]==true)?"checked":"")." name=pessoas[".$indice."] value=TRUE></td></tr>";	
			}
		}
		if($problemasdirecao2) {
			foreach($problemasdirecao2 as $indice => $prodirecao) {
				echo "<tr><td class=bordapreto>".$prodirecao."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=FALSE><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=TRUE></td></tr>";	
			}
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Direção</b></td><td colspan=2 class=bordapreto>Não existem problemas na Direção</td></tr>";
	}
	
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Planejamento') AND
				  rp.oppid IN(5,6)";
	
	$pergs_planejamento = $db->carregar($sql);
	
	if($pergs_planejamento[0]) {
		foreach($pergs_planejamento as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprocessos[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Rotinas') AND
				  rp.oppid IN(5,6)";
	
	$pergs_rotinas = $db->carregar($sql);
	
	if($pergs_rotinas[0]) {
		foreach($pergs_rotinas as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprocessos[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Normas e Regulamentos') AND
				  rp.oppid IN(5,6)";
	
	$pergs_normas = $db->carregar($sql);
	
	if($pergs_normas[0]) {
		foreach($pergs_normas as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprocessos[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasprocessos) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemasprocessos)+1)." class=SubTituloDireita><b>Processos</b></td></tr>";
		foreach($problemasprocessos as $indice => $processo) {
			echo "<tr><td class=bordapreto>".$processo."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=FALSE><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=TRUE></td></tr>";	
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Processos</b></td><td colspan=2 class=bordapreto>Não existem problemas nos processos</td></tr>";
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo.respostapergunta rp 
			INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'F' and prgstatus = 'A' and prgdetalhe='Gestão Financeira') AND
				  rp.oppid IN(5,6)";
	
	$pergs_financeiras = $db->carregar($sql);
	
	if($pergs_financeiras[0]) {
		foreach($pergs_financeiras as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasfinancas[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasfinancas) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemasfinancas)+1)." class=SubTituloDireita><b>Finanças</b></td></tr>";
		foreach($problemasfinancas as $indice => $financa) {
			echo "<tr><td class=bordapreto>".$financa."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=FALSE><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=TRUE></td></tr>";	
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Finanças</b></td><td colspan=2 class=bordapreto>Não existem problemas nas finanças</td></tr>";
	}
	?>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Projetos e Programas</td>
	<td>
	<p>A escola desenvolve algum PROJETO destinado a melhorar a Gestão? <input type="radio" name="exibeprojeto" id="exibeprojeto_S" onclick="divCarregando();if(this.checked){gravarRespostaProgramasProjetos('G','J',true);document.getElementById('projeto_label').style.display='';carregarProjetos('G');}else{document.getElementById('projeto_label').style.display='none';}divCarregado();"> Sim <input type="radio" id="exibeprojeto_N" name="exibeprojeto" onclick="divCarregando();if(this.checked){if(gravarRespostaProgramasProjetos('G','J',false)){document.getElementById('projeto_label').style.display='none';}else{document.getElementById('exibeprojeto_S').checked=true;}}else{document.getElementById('projeto_label').style.display='';}divCarregado();"> Não </p>
	<div id="projeto_label" style="display:none"></div>
	
	<p>A escola participa ou gostaria de participar de algum PROGRAMA, do Governo Federal ou da secretaria, que auxilie na melhoria da Gestão? <input type="radio" name="exibeprograma" id="exibeprograma_S" onclick="divCarregando();if(this.checked){gravarRespostaProgramasProjetos('G','G',true);document.getElementById('programa_label').style.display='';carregarProgramas('G');}else{document.getElementById('programa_label').style.display='none';}"> Sim <input type="radio" id="exibeprograma_N" name="exibeprograma" onclick="divCarregando();if(this.checked){if(gravarRespostaProgramasProjetos('G','G',false)){document.getElementById('programa_label').style.display='none';}else{document.getElementById('exibeprograma_S').checked=true;}}else{document.getElementById('programa_label').style.display='';}divCarregado();"> Não </p>
	<div id="programa_label" style="display:none"></div>
	</td>
</tr>

</table>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
	<?php $arrTelasPendentes = recuperaTelasPendentes("diagnostico_4_gestao") ?>
	<?php if(!$arrTelasPendentes): ?>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_3_financas';" >
	<input type="button" name="salvar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_4_sintesedimensao4';diagnostico_4_4_sintesedimensao4();"> 
	<input type="button" name="continuar" value="Salvar e continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar';diagnostico_4_4_sintesedimensao4();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar';" >
	<?php else: ?>
		<?php foreach($arrTelasPendentes as $tela): ?>
				<p class="red bold"><img src="../imagens/atencao.png" class="img_middle"> Favor preencher a tela <?php echo $tela ?> antes de salvar a Síntese!</p>
		<?php endforeach; ?>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_4_gestao&aba1=diagnostico_4_3_financas';" >
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar';" >
	<?php endif; ?>
	</td>
</tr>
</table>
</form>