<?
monta_titulo( "Síntese da dimensão 5 - Comunidade escolar", "&nbsp;");
?>
<script>

function diagnostico_5_5_sintesedimensao5() {
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

function calculaNumProblemasCriticos() {
	var numCritico = jQuery("[type=checkbox][name^='critico[']").length;
	var numPc = jQuery("[type=checkbox][name^='respostapaiscomunidade[']").length;
	var numTotal = numCritico+numPc;
	
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
		var pcMarcados = jQuery("[name^='respostapaiscomunidade[']:checked").length;
		var numMarcados = criticoMarcados+pcMarcados;
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

function exibeTurma(id)
{
	jQuery("#" + id).show();
	jQuery( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
}

</script>
<style>
.bordapreto {
border: 1px solid #000;
}
</style>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações:</font></td>
	<td class="blue">
	<p>Chegamos à Síntese da Dimensão 5. A tabela abaixo apresenta o conjunto de problemas identificados nesta dimensão. Leia as frases e assinale os problemas considerados mais "críticos" pela escola, até o limite indicado na tabela (que corresponde a 30% do total de problemas). As frases assinaladas aparecerão na Síntese Geral do diagnóstico, depois que todas as dimensões forem respondidas. Após assinalar os problemas críticos, registre se a escola possui ou participa de algum projeto e/ou programa destinado a melhorar ou minimizar esses problemas.</p>
	<p>Lembre-se: todos os problemas são importantes, mas a escola deve concentrar esforços naqueles que ela pode resolver.</p>
	</td>
</tr>
</table>

<form method="post" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura">
<input type="hidden" name="requisicao" value="diagnostico_5_5_sintesedimensao5">
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
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and (prgdetalhe='Compromisso' or prgdetalhe='Compromisso - Educação Infantil')) AND
				  rp.oppid IN(5,6)";
	
	$pergs_compromisso = $db->carregar($sql);
	unset($isCritico);
	if($pergs_compromisso[0]) {
		foreach($pergs_compromisso as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasestudantes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and (prgdetalhe='Protagonismo e Participação' or prgdetalhe='Protagonismo e Participação - Educação Infantil')) AND
				  rp.oppid IN(5,6)";
	
	$pergs_protagonismo = $db->carregar($sql);
	if($pergs_protagonismo[0]) {
		foreach($pergs_protagonismo as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasestudantes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and (prgdetalhe='Saúde e Bem-estar' or prgdetalhe='Saúde e Bem-estar - Educação Infantil')) AND
				  rp.oppid IN(5,6)";
	
	$pergs_saude = $db->carregar($sql);
	if($pergs_saude[0]) {
		foreach($pergs_saude as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasestudantes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Segurança - Educação Infantil') AND
				  rp.oppid IN(8)";
	
	$pergs_saude = $db->carregar($sql);
	if($pergs_saude[0]) {
		foreach($pergs_saude as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasestudantes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasestudantes) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemasestudantes)+1)." class=SubTituloDireita><b>Estudantes</b></td></tr>";
		foreach($problemasestudantes as $indice => $proestudante) {
			echo "<tr><td class=bordapreto>".$proestudante."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=\"FALSE\"></td></tr>";	
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Estudantes</b></td><td class=bordapreto>Não existem problemas na Estudantes</td></tr>";
	}
	
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Práticas') AND
				  rp.oppid IN(5,6)";
	
	$pergs_praticas = $db->carregar($sql);
	
	unset($isCritico);
	if($pergs_praticas[0]) {
		foreach($pergs_praticas as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdocentes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Experiencia e Auto-Confiança') AND
				  rp.oppid IN(5,6)";
	
	$pergs_experiencia = $db->carregar($sql);
	
	if($pergs_experiencia[0]) {
		foreach($pergs_experiencia as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdocentes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rd.rdoid FROM pdeinterativo2013.respostadocente rd 
			INNER JOIN pdeinterativo2013.respostadocenteformacao rf ON rf.rdoid = rd.rdoid 
			INNER JOIN educacenso_2010.tab_docente td ON td.pk_cod_docente = rd.pk_cod_docente 
			WHERE rd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rdodeseja=FALSE AND rdostatus='A' 
			GROUP BY rd.rdoid";
	
	$docentesformacao = $db->carregarColuna($sql);
	
	if($docentesformacao) {
		$num = $db->pegaUm("SELECT COUNT(rdoid) as num FROM pdeinterativo2013.respostadocente WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rdocritico=TRUE AND rdostatus='A'");
		if($num == count($docentesformacao)) {
			$isCritico[implode(",",$docentesformacao)] = true;
		}
		$problemasdocentes2[implode(",",$docentesformacao)] = count($docentesformacao)." docentes consideram que sua formação não é apropriada para ministrar a área de conhecimento/ disciplina que atuam. <a href=\"javascript:onclick=exibeTurma('docentes2');\" style=\"cursor:pointer;font-weight:bold\" >Clique aqui</a> para ver os nomes dos docentes.";
	}
	
	if(count($problemasdocentes) > 0 || count($problemasdocentes2) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemasdocentes)+count($problemasdocentes2)+1)." class=SubTituloDireita><b>Docentes</b></td></tr>";
		if($problemasdocentes) {
			foreach($problemasdocentes as $indice => $prodocente) {
				echo "<tr><td class=bordapreto>".$prodocente."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=\"FALSE\"></td></tr>";	
			}
		}
		if($problemasdocentes2) {
			foreach($problemasdocentes2 as $indice => $prodocente2) {
				echo "<tr><td class=bordapreto>".$prodocente2."</td><td align=center class=bordapreto><input type=hidden name=respostadocente[".$indice."] value=\"FALSE\"></td></tr>";	
			}
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Docentes</b></td><td class=bordapreto>Não existem problemas com docentes</td></tr>";
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Cooperação e Respeito') AND
				  rp.oppid IN(5,6)";
	
	$pergs_cooperacao = $db->carregar($sql);
	
	unset($isCritico);
	if($pergs_cooperacao[0]) {
		foreach($pergs_cooperacao as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdemaisprofissionais[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Motivação') AND
				  rp.oppid IN(5,6)";
	
	$pergs_motivacao = $db->carregar($sql);
	
	if($pergs_motivacao[0]) {
		foreach($pergs_motivacao as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdemaisprofissionais[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	
	if(count($problemasdemaisprofissionais) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemasdemaisprofissionais)+1)." class=SubTituloDireita><b>Demais profissionais</b></td></tr>";
		foreach($problemasdemaisprofissionais as $indice => $dprofissionais) {
			echo "<tr><td class=bordapreto>".$dprofissionais."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=\"FALSE\"></td></tr>";	
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Demais profissionais</b></td><td class=bordapreto>Não existem problemas com Demais profissionais</td></tr>";
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Comunicação') AND
				  rp.oppid IN(5,6)";
	
	$pergs_comunicacao = $db->carregar($sql);
	
	if($pergs_comunicacao[0]) {
		foreach($pergs_comunicacao as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemaspaisecomunidade[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Participação') AND
				  rp.oppid IN(5,6)";
	
	$pergs_participacao = $db->carregar($sql);
	
	if($pergs_participacao[0]) {
		foreach($pergs_participacao as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemaspaisecomunidade[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT * FROM pdeinterativo2013.respostapaiscomunidade WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$respostapaiscomunidade = $db->pegaLinha($sql);
	
	if($respostapaiscomunidade) {
		if($respostapaiscomunidade['rpcpossuiconcelho']=="f") {
			$problemaspaisecomunidade2['rpcpossuiconcelho'] = "A escola não possui Conselho Escolar";
		}
	}
	
	if(count($problemaspaisecomunidade) > 0 || count($problemaspaisecomunidade2) > 0) {
		echo "<tr><td align=right rowspan=".(count($problemaspaisecomunidade)+count($problemaspaisecomunidade2)+1)." class=SubTituloDireita><b>Pais e comunidade</b></td></tr>";
		if(count($problemaspaisecomunidade) > 0) {
			foreach($problemaspaisecomunidade as $indice => $paisecomunidade) {
				echo "<tr><td class=bordapreto>".$paisecomunidade."</td><td align=center class=bordapreto><input type=hidden name=critico[".$indice."] value=\"FALSE\"></td></tr>";	
			}
		}
		if(count($problemaspaisecomunidade2) > 0) {
			foreach($problemaspaisecomunidade2 as $indice => $paisecomunidade) {
				echo "<tr><td class=bordapreto>".$paisecomunidade."</td><td align=center class=bordapreto><input type=hidden name=respostapaiscomunidade[".$indice."critico] value=\"FALSE\"></td></tr>";	
			}
		}
	} else {
		echo "<tr><td align=right class=SubTituloDireita><b>Pais e comunidade</b></td><td class=bordapreto>Não existem problemas com Pais e comunidade</td></tr>";
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
	<?php $arrTelasPendentes = recuperaTelasPendentes("diagnostico_5_comunidadeescolar") ?>
	<?php if(!$arrTelasPendentes): ?>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_4_paisecomunidade';" >
	<input type="button" name="salvar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_5_comunidadeescolar&aba1=diagnostico_5_5_sintesedimensao5';diagnostico_5_5_sintesedimensao5();"> 
	<input type="button" name="continuar" value="Salvar e continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura';diagnostico_5_5_sintesedimensao5();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura';" >
	<?php else: ?>
		<?php foreach($arrTelasPendentes as $tela): ?>
				<p class="red bold"><img src="../imagens/atencao.png" class="img_middle"> Favor preencher a tela <?php echo $tela ?> antes de salvar a Síntese!</p>
		<?php endforeach; ?>
	<?php endif; ?>
	</td>
</tr>
</table>
</form>
<style>
	.PopUphidden{display:none;width:500px;height:300px;position:absolute;z-index:0;top:50%;left:50%;margin-top:-150;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;}
</style>
<?

if(count($problemasdocentes2) > 0) {

	$sql = "SELECT td.pk_cod_docente, td.no_docente FROM pdeinterativo2013.respostadocente rd 
			INNER JOIN pdeinterativo2013.respostadocenteformacao rf ON rf.rdoid = rd.rdoid 
			INNER JOIN educacenso_2010.tab_docente td ON td.pk_cod_docente = rd.pk_cod_docente 
			WHERE rd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rdodeseja=FALSE 
			GROUP BY td.pk_cod_docente, td.no_docente";
	
	$docentes = $db->carregar($sql);
	
?>
<div class="PopUphidden" id="docentes2">
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('docentes2').style.display='none'" />
	</div>
	<div style="padding:5px;overflow:auto;height:250px;" >	
		<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center" >
			<thead>
				<tr>
					<td class="bold center" >Nome</td>
				</tr>
			</thead>
		<?php $x=0;foreach($docentes as $docente): ?>
			<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
			<tr bgcolor="<?php echo $cor ?>" >
				<td><?php echo $docente['no_docente'] ?></td>
			</tr>
		<?php $x++;endforeach; ?>
		</table>
	</div>
</div>
<?		
}
?>