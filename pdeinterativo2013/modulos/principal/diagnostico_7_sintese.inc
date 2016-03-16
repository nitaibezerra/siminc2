<script>

function diagnostico_7_sintese() {
	document.getElementById('formulario').submit();
}

function popUpDimensao2(funcao)
{
	janela('pdeinterativo2013.php?modulo=principal/popUpSinteseDimensao2&acao=A&requisicao=' + funcao ,500,300,'Turmas Críticas');
}

function exibeTurma(id)
{
	jQuery("#" + id).show();
	jQuery( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
}

function salvarParecer(abaid)
{
	var rdn_parecer = jQuery("[name='rdn_parecer[" + abaid + "]']:checked").val();
	if(!rdn_parecer){
		alert('Informe se a Dimensão foi Aprovada!');
		return false;
	}
	var parecer 	= jQuery("[name='parecer_" + abaid + "']").val();
	if(rdn_parecer == "f" && !parecer){
		alert('Informe os ajustes necessários na Dimensão!');
		return false;
	}
	
	jQuery.ajax({
	   type: "POST",
	   url: window.location,
	   data: "requisicao=salvarParecer&abaid=" + abaid + "&rdn_parecer=" + rdn_parecer + "&parecer=" + parecer,
	   success: function(resp){
	   		if(resp == "true"){
	   			alert('Parecer Salvo com Sucesso!');
	   			jQuery("[id='span_parecer_" + abaid + "']").html("<img src='../imagens/check_checklist.png' />");
	   		}else{
	   			alert('Não foi possível salvar o Parecer!');
	   		}
	   }
	 });
}

function historicoParecer(abaid)
{
	var url = "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese&requisicao=historicoParecer&abaid=" + abaid;
	janela(url,600,400,'Histórico do Parecer');
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
	<p>Parabéns! Chegamos ao final do Diagnóstico! Ao percorrer as seis dimensões, a escola analisou múltiplos aspectos da gestão escolar e identificou os seus principais desafios. O quadro abaixo exibe a conclusão deste exercício, relacionando os problemas críticos selecionados em cada dimensão, os projetos que a escola realiza e os programas que participa ou gostaria de participar.</p>
	<p>Caso o Grupo de Trabalho concorde com todos os problemas descritos, clique no botão "Salvar e continuar". Caso o GT identifique problemas que não apareceram nesta síntese, retorne à dimensão correspondente, reveja os dados e faça os ajustes.</p>
	<p>Lembre-se: ao alterar qualquer informação, novos problemas podem surgir ou deixar de ser críticos. Logo, preste muita atenção ao modificar os dados, não esqueça de salvar o conteúdo e de revisar a síntese da dimensão alterada, pois isto pode modificar esta conclusão.</p>
	</td>
</tr>
</table>

<form method="post" name="formulario" id="formulario">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_8_conclusao">
<input type="hidden" name="requisicao" value="diagnostico_7_sintese">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita bold center" width="20%">Dimensão</td>
	<td class="SubTituloDireita bold center" width="80%">Problemas Identificados</td>
</tr>
<tr>
	<td class="SubTituloDireita bold" >Indicadores e taxas</td>
	<td>
		<?php
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
		echo "<table width=100% >";
		if(count($problemasideb) > 0) {
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
			foreach($problemaspb as $indice => $ppb) {
				echo "<tr><td class=bordapreto>".$ppb."</td><input type=hidden name=respostaprovabrasil[".$indice."critico] value=FALSE></tr>";	
			}
		}

		if(count($problemasideb) == 0 && count($problemastx) == 0 && count($problemaspb) == 0) {
			echo "<tr><td class=bordapreto></td></tr>";	
		}
		echo "</table>";
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita bold" >Distorção e Aproveitamento</td>
	<td>
	<?php
		$problema_da = false;
		echo "<table width=100% >"; 
		$arrDistorcao1 = carregaDistorcaoDiagnosticoMatricula(false,"M",null);
		if($arrDistorcao){
			 echo "<tr><td class=bordapreto>Em 2012, a escola possuía ".count($arrDistorcao1)." turma(s) com nº de matrículas superior ao parâmetro do CNE.<a href=\"javascript:popUpDimensao2('exibeTurmasCNE')\" >Clique aqui para exibir a(s) turma(s).</a> </tr>";
			 $problema_da = true;
		}
		$arrDistorcao2 = carregaDistorcaoDiagnosticoMatricula(null,"D","D");
		if($arrDistorcao2){
			 echo "<tr><td class=bordapreto>Em 2012, a escola possuía ".count($arrDistorcao2)." turma(s) com taxa de distorção superior à média do Brasil.<a href=\"javascript:popUpDimensao2('exibeTurmasCriticas')\" >Clique aqui para exibir a(s) turma(s).</a> </tr>";
			 $problema_da = true;
		}
		$arrRespEscola = recuperaRespostasEscola(null,"D","D",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
		if($arrRespEscola){
			foreach($arrRespEscola as $resp){
				echo "<tr><td class=bordapreto>".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
				$problema_da = true;
			}
		}
		$arrDistorcaoReprovacao = carregaDistorcaoDiagnosticoMatricula(null,"A","R");
		if($arrDistorcaoReprovacao){
			 echo "<tr><td class=bordapreto>Em 2012, a escola possuía ".count($arrDistorcaoReprovacao)." turma(s) com taxa de reprovação superior à média do Brasil.<a href=\"javascript:popUpDimensao2('exibeTurmasCriticasReprovacao')\" >Clique aqui para exibir a(s) turma(s).</a> </tr>";
			 $problema_da = true;
		}
		$arrDistorcaoAbandono = carregaDistorcaoDiagnosticoMatricula(null,"A","A");
		if($arrDistorcaoAbandono){
			 echo "<tr><td class=bordapreto>Em 2012, a escola possuía ".count($arrDistorcaoAbandono)." turma(s) com taxa de abandono superior à média do Brasil.<a href=\"javascript:popUpDimensao2('exibeTurmasCriticasAbandono')\" >Clique aqui para exibir a(s) turma(s).</a> </tr>";
			 $problema_da = true;
		}
		$arrRespEscola2 = recuperaRespostasEscola(null,"D","A",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
		if($arrRespEscola2){
			foreach($arrRespEscola2 as $resp){
				echo "<tr><td class=bordapreto>".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
				$problema_da = true;
			}
		}
		$arrTurmas = recuperaTurmasCriticasPorEscola();
		$arrTaxaReprovacaoBrasil = carregaTaxa();
		$arrDisciplinas = retornaDisciplinasTurma();
		$arrDistorcaoTaxaReprovacaoDisciplina = carregaDistorcaoTaxaReprovacaoDisciplina();
		$arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxaEscolar();
		if($arrTurmas){
			if($arrTurmas['Ensino Fundamental']){
				foreach($arrTurmas['Ensino Fundamental'] as $em){
					if($arrDisciplinas[$em['pk_cod_turma']]){
						foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds){
							if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ){
								if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['U']){
									$arrTurmasCriticasReprovacao["U"][$ds['pk_cod_disciplina']][] = $em['pk_cod_turma'];
									$arrNomeDisciplina[$ds['pk_cod_disciplina']] = $ds['no_disciplina'];
									$arrTurmasC["U"][$ds['pk_cod_disciplina']][$em['pk_cod_turma']] = array( 
																										"disciplina" => $ds['no_disciplina'],
																										"serie" => $em['serie'],
																										"turma" => $em['turma'],
																										"horario" => $em['hrinicio']." - ".$em['hrfim'],
																										"taxaBrasil" => $arrTaxaReprovacaoBrasil['U']." %",
																										"taxa" => round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %"
																									   );
								}
							}
						}
					}
				}
			}
			if($arrTurmas['Ensino Médio']){
				foreach($arrTurmas['Ensino Médio'] as $em){
					if($arrDisciplinas[$em['pk_cod_turma']]){
						foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds){
							if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ){
								if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['M']){
									$arrTurmasCriticasReprovacao["M"][$ds['pk_cod_disciplina']][] = $em['pk_cod_turma'];
									$arrNomeDisciplina[$ds['pk_cod_disciplina']] = $ds['no_disciplina'];
									$arrTurmasC["M"][$ds['pk_cod_disciplina']][$em['pk_cod_turma']] = array( 
																										"disciplina" => $ds['no_disciplina'],
																										"serie" => $em['serie'],
																										"turma" => $em['turma'],
																										"horario" => $em['hrinicio']." - ".$em['hrfim'],
																										"taxaBrasil" => $arrTaxaReprovacaoBrasil['M']." %",
																										"taxa" => round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %"
																									   );
								}
							}
						}
					}
				}
			}
		}
		if($arrTurmasCriticasReprovacao){
			if($arrTurmasCriticasReprovacao["U"]){
				foreach($arrTurmasCriticasReprovacao["U"] as $disciplina => $arrTurmas){
					echo "<tr><td class=bordapreto>".count($arrTurmas)." turma(s) do ensino fundamental aprensentou(aram) taxa de reprovação em ".$arrNomeDisciplina[$disciplina]." superior(es) à média do Brasil. <a href=\"javascript:exibeTurma('u_$disciplina')\" >Clique aqui para exibir a(s) turma(s).</td></tr>";
					$problema_da = true;
				}
			}
			if($arrTurmasCriticasReprovacao["M"]){
				foreach($arrTurmasCriticasReprovacao["M"] as $disciplina => $arrTurmas){
					echo "<tr><td class=bordapreto>".count($arrTurmas)." turma(s) do ensino médio aprensentou(aram) taxa de reprovação em ".$arrNomeDisciplina[$disciplina]." superior(es) à média do Brasil. <a href=\"javascript:exibeTurma('m_$disciplina')\" >Clique aqui para exibir a(s) turma(s).</td></tr>";
					$problema_da = true;
				}
			}
		}
		$arrRespEscola3 = recuperaRespostasEscola(null,"D","C",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
		if($arrRespEscola3){
			foreach($arrRespEscola3 as $resp){
				echo "<tr><td class=bordapreto>".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
				$problema_da = true;
			}
		}
		if(!$problema_da) {
			echo "<tr><td class=bordapreto></td></tr>";	
		}
		echo "</table>";
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita bold" >Ensino e Aprendizagem</td>
	<td>
	<?php
	$problema_ea = false;
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and (prgdetalhe='Projeto Pedagógico - Educação Infantil' or prgdetalhe='Projeto Pedagógico')) AND
				  rp.oppid IN(5,6)";
	$pergs_projetospedagogicos = $db->carregar($sql);
	unset($isCritico);
	if($pergs_projetospedagogicos[0]) {
		foreach($pergs_projetospedagogicos as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprojpedag[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	echo "<table width=100% >";
	if(count($problemasprojpedag) > 0) {
		foreach($problemasprojpedag as $indice => $projpedag) {
			echo "<tr><td class=bordapreto>".$projpedag."</td></tr>";
			$problema_ea = true;	
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and (prgdetalhe='Currículo - Educação Infantil' or prgdetalhe='Currículo')) AND
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
		foreach($problemascurriculo as $indice => $curriculo) {
			echo "<tr><td class=bordapreto>".$curriculo."</td></tr>";
			$problema_ea = true;	
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and (prgdetalhe='Avaliações - Educação Infantil' or prgdetalhe='Avaliações')) AND
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
		foreach($problemasavaliacoes as $indice => $avaliacoes) {
			echo "<tr><td class=bordapreto>".$avaliacoes."</td></tr>";
			$problema_ea = true;	
		}
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
		foreach($problemastempaprend as $indice => $tempoaprendizagem) {
			echo "<tr><td class=bordapreto>".$tempoaprendizagem."</td></tr>";
			$problema_ea = true;	
		}
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
		foreach($problemajornadaampliada as $indice => $jornadaampliada) {
			echo "<tr><td class=bordapreto>".$jornadaampliada."</td></tr>";
			$problema_ea = true;	
		}
	}
	
	if(!$problema_ea) {
		echo "<tr><td class=bordapreto></td></tr>";	
	}
	echo "</table>";
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita bold" >Gestão</td>
	<td>
	<?php
	$sql = "SELECT tp.tpeid, tp.tpedesc, count(p.pesid) as qtd FROM pdeinterativo2013.pessoa p 
			LEFT JOIN pdeinterativo2013.pessoatipoperfil ptp ON ptp.pesid = p.pesid 
			LEFT JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = ptp.pdeid 
			LEFT JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = p.pesid 
			LEFT JOIN pdeinterativo2013.tipoperfil tp ON tp.tpeid = ptp.tpeid 
			WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND dp.tenid IN(2,3) AND tp.tpeid IN(SELECT tpeid FROM pdeinterativo2013.perfilarea WHERE apeid IN('".APE_EQUIPEPEDAGOGICA."','".APE_DIRETOR."','".APE_VICEDIRETOR."','".APE_SECRETARIA."'))
			GROUP BY tp.tpeid, tp.tpedesc";
	
	$pessoas = $db->carregar($sql);
	
	if($pessoas[0]) {
		foreach($pessoas as $pes) {
			$sql = "SELECT p.pesid, p.critico FROM pdeinterativo2013.pessoa p 
					LEFT JOIN pdeinterativo2013.pessoatipoperfil ptp ON ptp.pesid = p.pesid
					LEFT JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = ptp.pdeid 
					LEFT JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = p.pesid 
					WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND dp.tenid IN(2,3) AND ptp.tpeid='".$pes['tpeid']."'";
			
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
	
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Liderança') AND
				  rp.oppid IN(5,6)";
	
	$pergs_lideranca = $db->carregar($sql);

	if($pergs_lideranca[0]) {
		foreach($pergs_lideranca as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdirecao2[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Acompanhamento') AND
				  rp.oppid IN(5,6)";
	
	$pergs_acompanhamento = $db->carregar($sql);
	if($pergs_acompanhamento[0]) {
		foreach($pergs_acompanhamento as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdirecao2[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	echo "<table width=100%>";
	$problema_dim_4 = false;
	if(count($problemasdirecao1) > 0 || count($problemasdirecao2) > 0) {
		if($problemasdirecao1) {
			foreach($problemasdirecao1 as $indice => $prodirecao) {
				echo "<tr><td class=bordapreto>".$prodirecao."</td></tr>";
				$problema_dim_4 = true;	
			}
		}
		if($problemasdirecao2) {
			foreach($problemasdirecao2 as $indice => $prodirecao) {
				echo "<tr><td class=bordapreto>".$prodirecao."</td></tr>";
				$problema_dim_4 = true;	
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Planejamento') AND
				  rp.oppid IN(5,6)";
	
	$pergs_planejamento = $db->carregar($sql);
	
	if($pergs_planejamento[0]) {
		foreach($pergs_planejamento as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprocessos[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Rotinas') AND
				  rp.oppid IN(5,6)";
	
	$pergs_rotinas = $db->carregar($sql);
	
	if($pergs_rotinas[0]) {
		foreach($pergs_rotinas as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprocessos[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Normas e Regulamentos') AND
				  rp.oppid IN(5,6)";
	
	$pergs_normas = $db->carregar($sql);
	
	if($pergs_normas[0]) {
		foreach($pergs_normas as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprocessos[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasprocessos) > 0) {
		foreach($problemasprocessos as $indice => $processo) {
			echo "<tr><td class=bordapreto>".$processo."</td></tr>";
			$problema_dim_4 = true;	
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'F' and prgstatus = 'A' and prgdetalhe='Gestão Financeira') AND
				  rp.oppid IN(5,6)";
	
	$pergs_financeiras = $db->carregar($sql);
	
	if($pergs_financeiras[0]) {
		foreach($pergs_financeiras as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasfinancas[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasfinancas) > 0) {
		foreach($problemasfinancas as $indice => $financa) {
			echo "<tr><td class=bordapreto>".$financa."</td></tr>";
			$problema_dim_4 = true;
		}
	}
	
	if(!$problema_dim_4) {
		echo "<tr><td class=bordapreto></td></tr>";	
	}
	echo "</table>";
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita bold" >Comunidade Escolar</td>
	<td>
	<?php
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
	
	$problema_dim_5 = false;
	echo "<table width=100%>";
	if(count($problemasestudantes) > 0) {
		foreach($problemasestudantes as $indice => $proestudante) {
			echo "<tr><td class=bordapreto>".$proestudante."</td></tr>";
			$problema_dim_5 = true;	
		}
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
		if($problemasdocentes) {
			foreach($problemasdocentes as $indice => $prodocente) {
				echo "<tr><td class=bordapreto>".$prodocente."</td></tr>";
				$problema_dim_5 = true;	
			}
		}
		if($problemasdocentes2) {
			foreach($problemasdocentes2 as $indice => $prodocente2) {
				echo "<tr><td class=bordapreto>".$prodocente2."</td></tr>";
				$problema_dim_5 = true;	
			}
		}
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
		foreach($problemasdemaisprofissionais as $indice => $dprofissionais) {
			echo "<tr><td class=bordapreto>".$dprofissionais."</td></tr>";
			$problema_dim_5 = true;	
		}
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
		if(count($problemaspaisecomunidade) > 0) {
			foreach($problemaspaisecomunidade as $indice => $paisecomunidade) {
				echo "<tr><td class=bordapreto>".$paisecomunidade."</td></tr>";
				$problema_dim_5 = true;	
			}
		}
		if(count($problemaspaisecomunidade2) > 0) {
			foreach($problemaspaisecomunidade2 as $indice => $paisecomunidade) {
				echo "<tr><td class=bordapreto>".$paisecomunidade."</td></tr>";
				$problema_dim_5 = true;	
			}
		}
	}
	
	if(!$problema_dim_5) {
		echo "<tr><td class=bordapreto></td></tr>";	
	}
	echo "</table>";
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita bold" >Infraestrutura</td>
	<td>
	<?php
		$arrInstalacoesNecessarias = recuperaInstalacoesNecessarias();
		echo "<table width=100%>";
		$problema_dim_6 = false;
		if($arrInstalacoesNecessarias){
			foreach($arrInstalacoesNecessarias as $infra){
				echo "<tr><td class=bordapreto >A escola não possui {$infra['ifidesc']}.</td></tr>";
				$problema_dim_6 = true;
			}
		}
		
		$arrInstalacoesInadequadas = recuperaInstalacoesInadequadas();
		if($arrInstalacoesInadequadas){
			foreach($arrInstalacoesInadequadas as $infra){
				echo "<tr><td class=bordapreto >A escola considera que as instalações do(a) {$infra['ifidesc']} estão inadequados(as).</td></tr>";
				$problema_dim_6 = true;
			}
		}
		
		$arrRespEscola = recuperaRespostasEscola(null,"I","I",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
		if($arrRespEscola){
			foreach($arrRespEscola as $resp){
				echo "<tr><td class=bordapreto >".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
				$problema_dim_6 = true;
			}
		}
		
		$arrEquipamentosRuins = recuperaEquipamentosRuins();
		if($arrEquipamentosRuins){
			foreach($arrEquipamentosRuins as $equip){
				echo "<tr><td class=bordapreto >A escola considera que o estado de conservação de ".number_format($equip['rmeqtdruin'],"",2,".")." {$equip['tmedesc']} é/são ruim(ns).</td></tr>";
				$problema_dim_6 = true;
			}
		}
		
		$arrRespEscola = recuperaRespostasEscola(null,"I","E",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
		if($arrRespEscola){
			foreach($arrRespEscola as $resp){
				echo "<tr><td class=bordapreto >".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
				$problema_dim_6 = true;
			}
		}
		
		if(!$problema_dim_6) {
			echo "<tr><td class=bordapreto></td></tr>";	
		}
		echo "</table>";
	?>
	</td>
</tr>
</table>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita">O que deseja fazer agora?</td>
	<td>
	<?php $arrTelasPendentes = recuperaTelasSintesesPendentes() ?>
	<?php if(!$arrTelasPendentes): ?>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_3_sintese';" > 
	<input type="button" name="salvar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese';diagnostico_7_sintese();"> 
	<input type="button" name="continuar" value="Salvar e continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_orientacoes';diagnostico_7_sintese();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_orientacoes';" > 
	<?php else: ?>
		<?php foreach($arrTelasPendentes as $tela): ?>
				<p class="red bold"><img src="../imagens/atencao.png" class="img_middle"> Favor preencher a tela <?php echo $tela ?> antes de salvar a Síntese!</p>
		<?php endforeach; ?>
		<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_3_sintese';" >
		<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_orientacoes';" >
	<?php endif; ?>
	</td>
</tr>
</table>
</form>
<style>
	.PopUphidden{display:none;width:500px;height:300px;position:absolute;z-index:0;top:50%;left:50%;margin-top:-150;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;}
</style>
<?php if($arrTurmasC["U"]): ?>
	<?php foreach($arrTurmasC["U"] as $cod_disciplina => $arrTurma): ?>
		<div class="PopUphidden" id="<?php echo "u_$cod_disciplina" ?>">
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo "u_$cod_disciplina" ?>').style.display='none'" />
			</div>
			<div style="padding:5px;overflow:auto;width:95%;height:90%" >	
				<center><b>Ensino Fundamental</b></center><br />	
				<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center" >
					<thead>
						<tr>
							<td class="bold center" >Série / Ano</td>
							<td class="bold center" >Horário</td>
							<td class="bold center" >Turma(s)</td>
							<td class="bold center" >Disciplina(s)</td>
							<td class="bold center" >Taxa de Reprovação da Disciplina (em %)</td>
							<td class="bold center" >Taxa de Reprovação do Brasil (em %)</td>
						</tr>
					</thead>
				<?php $x=0;foreach($arrTurma as $turma): ?>
					<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
					<tr bgcolor="<?php echo $cor ?>" >
						<td><?php echo $turma['serie'] ?></td>
						<td><?php echo $turma['horario'] ?></td>
						<td><?php echo $turma['turma'] ?></td>
						<td><?php echo $turma['disciplina'] ?></td>
						<td><?php echo $turma['taxa'] ?></td>
						<td><?php echo $turma['taxaBrasil'] ?></td>
					</tr>
				<?php $x++;endforeach; ?>
				</table>
			</div>
		</div>
	<?php endforeach; ?>
<?php endif; ?>
<?php if($arrTurmasC["M"]): ?>
	<?php foreach($arrTurmasC["M"] as $cod_disciplina => $arrTurma): ?>
		<div class="PopUphidden" id="<?php echo "m_$cod_disciplina" ?>">
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo "m_$cod_disciplina" ?>').style.display='none'" />
			</div>
			<div style="padding:5px;overflow:auto;width:95%;height:90%" >
				<center><b>Ensino Médio</b></center><br />	
				<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center" >
					<thead>
						<tr>
							<td class="bold center" >Série / Ano</td>
							<td class="bold center" >Horário</td>
							<td class="bold center" >Turma(s)</td>
							<td class="bold center" >Disciplina(s)</td>
							<td class="bold center" >Taxa de Reprovação da Disciplina (em %)</td>
							<td class="bold center" >Taxa de Reprovação do Brasil (em %)</td>
						</tr>
					</thead>
				<?php $x=0;foreach($arrTurma as $turma): ?>
					<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
					<tr bgcolor="<?php echo $cor ?>" >
						<td><?php echo $turma['serie'] ?></td>
						<td><?php echo $turma['horario'] ?></td>
						<td><?php echo $turma['turma'] ?></td>
						<td><?php echo $turma['disciplina'] ?></td>
						<td><?php echo $turma['taxa'] ?></td>
						<td><?php echo $turma['taxaBrasil'] ?></td>
					</tr>
				<?php $x++;endforeach; ?>
				</table>
			</div>
		</div>
	<?php endforeach; ?>
<?php endif; ?>