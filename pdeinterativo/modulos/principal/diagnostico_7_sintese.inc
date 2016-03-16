<script>

function diagnostico_7_sintese() {
	document.getElementById('formulario').submit();
}

function popUpDimensao2(funcao)
{
	janela('pdeinterativo.php?modulo=principal/popUpSinteseDimensao2&acao=A&requisicao=' + funcao ,500,300,'Turmas Críticas');
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
	var url = "pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese&requisicao=historicoParecer&abaid=" + abaid;
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
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_8_conclusao">
<input type="hidden" name="requisicao" value="diagnostico_7_sintese">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloCentro" width="10%">Dimensão</td>
	<td class="SubTituloCentro" width="30%">Problemas</td>
	<td class="SubTituloCentro" width="25%">Projetos e programas</td>
</tr>
<?

/* DIMENSAO 1 - Indicadores e Taxas */

$sql = "SELECT * FROM pdeinterativo.respostaideb WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
$dadosideb = $db->pegaLinha($sql);

if($dadosideb['ridinicialumcritico']=="t") {
	$problemasindicadorestaxas['ridinicialum'] = "O IDEB dos anos iniciais da escola não melhorou nas duas últimas medições disponíveis";
}
if($dadosideb['ridinicialdoiscritico']=="t") {
	$problemasindicadorestaxas['ridinicialdois'] = "Não há evidências de que a escola alcançará a próxima meta do IDEB, para os Anos Iniciais";
}
if($dadosideb['ridfinalumcritico']=="t") {
	$problemasindicadorestaxas['ridfinalum'] = "O IDEB dos anos finais da escola não melhorou nas duas últimas medições disponíveis";
}
if($dadosideb['ridfinaldoiscritico']=="t") {
	$problemasindicadorestaxas['ridfinaldois'] = "Não há evidências de que a escola alcançará a próxima meta do IDEB, para os Anos Finais";
}

$sql = "SELECT * FROM pdeinterativo.respostataxarendimento WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
$dadostx = $db->pegaLinha($sql);

if($dadostx['rtrfunaprovacritico']=="t") {
	$problemasindicadorestaxas['rtrfunaprova'] = "A taxa de aprovação da escola, no ensino fundamental, não melhorou nos últimos dois anos";
}
if($dadostx['rtrfunreprovacritico']=="t") {
	$problemasindicadorestaxas['rtrfunreprova'] = "A taxa de reprovação da escola, no ensino fundamental, não melhorou nos últimos dois anos";
}
if($dadostx['rtrfunabandonocritico']=="t") {
	$problemasindicadorestaxas['rtrfunabandono'] = "A taxa de abandono da escola, no ensino fundamental, não melhorou nos últimos dois anos";
}
if($dadostx['rtrmedaprovacritico']=="t") {
	$problemasindicadorestaxas['rtrmedaprova'] = "A taxa de aprovação da escola, no ensino médio, não melhorou nos últimos dois anos";
}
if($dadostx['rtrmedreprovacritico']=="t") {
	$problemasindicadorestaxas['rtrmedreprova'] = "A taxa de reprovação da escola, no ensino médio, não melhorou nos últimos dois anos";
}
if($dadostx['rtrmedabandonocritico']=="t") {
	$problemasindicadorestaxas['rtrmedabandono'] = "A taxa de abandono da escola, no ensino médio, não melhorou nos últimos dois anos";
}

$sql = "SELECT * FROM pdeinterativo.respostaprovabrasil WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
$dadospb = $db->pegaLinha($sql);

if($dadospb['rpbinicialportcritico']=="t") {
	$problemasindicadorestaxas['rpbinicialport'] = "Os resultados de Língua Portuguesa na Prova Brasil, nos anos iniciais, não demonstram evolução nas duas últimas medições";
}
if($dadospb['rpbinicialmatcritico']=="t") {
	$problemasindicadorestaxas['rpbinicialmat'] = "Os resultados de Matemática  na Prova Brasil, nos anos iniciais, não demonstram evolução nas duas últimas medições";
}
if($dadospb['rpbfinalportcritico']=="t") {
	$problemasindicadorestaxas['rpbfinalport'] = "Os resultados de Língua Portuguesa na Prova Brasil, nos anos finais, não demonstram evolução nas duas últimas medições";
}
if($dadospb['rpbfinalmatcritico']=="t") {
	$problemasindicadorestaxas['rpbfinalmat'] = "Os resultados de Matemática  na Prova Brasil, nos anos finais, não demonstram evolução nas duas últimas medições";
}

$sql = "SELECT  CASE WHEN spodesc IS NULL THEN '- '||prodesc
				 ELSE '- '||spodesc END as col FROM pdeinterativo.sinteseprograma s 
	LEFT JOIN pdeinterativo.programa p ON p.proid = s.proid 
	WHERE s.sprmodulo='I' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND spostatus='A'";

$programas = $db->carregarColuna($sql);

$sql = "SELECT '- '||sprdesc as col FROM pdeinterativo.sinteseprojeto WHERE sprmodulo='I' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND sprstatus='A'";

$projetos = $db->carregarColuna($sql);

if(count($problemasindicadorestaxas) > 0) {
	
	echo "<tr>
			<td rowspan=".(count($problemasindicadorestaxas))." class=SubTituloDireita><b>Indicadores e Taxas</b></td>
			<td class=bordapreto>".current($problemasindicadorestaxas)."</td>
			<td class=bordapreto rowspan=".(count($problemasindicadorestaxas))." valign=top><b>Programas:</b><br/>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."<br/></td>
		  </tr>";
	
	unset($problemasindicadorestaxas[current(array_keys($problemasindicadorestaxas))]);
	
	if($problemasindicadorestaxas) {
		foreach($problemasindicadorestaxas as $indice => $problema) {
			echo "<tr>
					<td class=bordapreto>".$problema."</td>
				  </tr>";
			$i++;
		}
	}

} else {
	echo "<tr>
			<td align=right class=SubTituloDireita><b>Indicadores e Taxas</b></td>
			<td class=bordapreto>Não existem problemas nos Indicadores e taxas</td>
			<td class=bordapreto valign=top><b>Programas:</b><br/>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."<br/></td>
			</tr>";
}

exibeParecerDimensao(3);

/* FIM DIMENSAO 1 - Indicadores e Taxas */

/* DIMENSAO 2 - Distorção e Aproveitamento */
?>
<tr>
	<?php $arrDistorcao = carregaDistorcaoDiagnosticoMatricula(false,"M",null); ?>
	<?php $arrRespEscola = array() ?>
	<?php if( $arrDistorcao && verificaTurmasCNE(array_unique($arrDistorcao)) ): ?>
			<?php $arrProblemas['descricao'][] = "Em 2010, a escola possuía ".count($arrDistorcao)." turma(s) com nº de matrículas superior ao parâmetro do CNE. <a href=\"javascript:popUpDimensao2('exibeTurmasCNE')\" >Clique aqui para exibir a(s) turma(s).</a>" ?>			
	<?php endif; ?>
	
	<?php $arrDistorcao = carregaDistorcaoDiagnosticoMatricula(null,"D","D"); ?>
	<?php $arrRespEscola = array() ?>
	<?php $arrRespEscola = recuperaRespostasEscola(null,"D","D",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
	<?php if( $arrDistorcao && verificaCheckBoxTaxa("distorcao",array_unique($arrDistorcao)) ): ?>
			<?php $arrProblemas['descricao'][] = "Em 2010, a escola possuía ".count($arrDistorcao)." turma(s) com taxa de distorção superior à média do Brasil. <a href=\"javascript:popUpDimensao2('exibeTurmasCriticas')\" >Clique aqui para exibir a(s) turma(s).</a>" ?>
			<?php $arrProblemas['radio']['critico']['sim'][] = "taxadistorcao[critico][".implode(",",$arrDistorcao)."]" ?>
			<?php $arrProblemas['radio']['critico']['nao'][] = "taxadistorcao[critico][".implode(",",$arrDistorcao)."]" ?>
			<?php $critico = verificaTaxaDimensao7("distorcao",$arrDistorcao,"diacritico2") ?>
			<?php $arrProblemas['radio']['critico']['resposta'][] = $critico; ?>
			<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "taxadistorcao[suficiente][".implode(",",$arrDistorcao)."]" ?>
			<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "taxadistorcao[suficiente][".implode(",",$arrDistorcao)."]" ?>
			<?php $programaprojeto = verificaTaxaDimensao7("distorcao",$arrDistorcao,"disuficiente") ?>
			<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = $programaprojeto ?>
			<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "taxadistorcao[resolvidoescola][".implode(",",$arrDistorcao)."]" ?>
			<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "taxadistorcao[resolvidoescola][".implode(",",$arrDistorcao)."]" ?>
			<?php $resolvidoescola = verificaTaxaDimensao7("distorcao",$arrDistorcao,"diaresolvidoescola") ?>
			<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = $resolvidoescola ?>
	<?php endif; ?>
	<?php if($arrRespEscola): ?>
		<?php foreach($arrRespEscola as $resp): ?>
			<?php if( verificaCheckBoxPergunta($resp['repid']) ): ?>
				<?php $arrProblemas['descricao'][] = str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?>
				<?php $arrProblemas['radio']['critico']['sim'][] = "critico2[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['critico']['nao'][] = "critico2[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['critico']['resposta'][] = $resp['critico2'] ?>
				<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "suficiente[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "suficiente[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = $resp['suficiente'] ?>
				<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "resolvidoescola[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "resolvidoescola[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = $resp['resolvidoescola'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	
	<?php $arrDistorcaoReprovacao = carregaDistorcaoDiagnosticoMatricula(null,"A","R"); ?>
	<?php $arrDistorcaoAbandono = carregaDistorcaoDiagnosticoMatricula(null,"A","A"); ?>
	<?php $arrRespEscola = array() ?>
	<?php $arrRespEscola = recuperaRespostasEscola(null,"D","A",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
	<?php if( $arrDistorcaoReprovacao && verificaCheckBoxTaxa("reprovacao",array_unique($arrDistorcaoReprovacao)) ): ?>
			<?php $arrProblemas['descricao'][] = "Em 2010, a escola possuía ".count($arrDistorcaoReprovacao)." turma(s) com taxa de reprovação superior à média do Brasil. <a href=\"javascript:popUpDimensao2('exibeTurmasCriticasReprovacao')\" >Clique aqui para exibir a(s) turma(s).</a>" ?>
			<?php $arrProblemas['radio']['critico']['sim'][] = "taxareprovacao[critico][".implode(",",$arrDistorcaoReprovacao)."]" ?>
			<?php $arrProblemas['radio']['critico']['nao'][] = "taxareprovacao[critico][".implode(",",$arrDistorcaoReprovacao)."]" ?>
			<?php $arrProblemas['radio']['critico']['resposta'][] = verificaTaxaDimensao7("reprovacao",$arrDistorcaoReprovacao,"diacritico2") ?>
			<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "taxareprovacao[suficiente][".implode(",",$arrDistorcaoReprovacao)."]" ?>
			<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "taxareprovacao[suficiente][".implode(",",$arrDistorcaoReprovacao)."]" ?>
			<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = verificaTaxaDimensao7("reprovacao",$arrDistorcaoReprovacao,"disuficiente") ?>
			<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "taxareprovacao[resolvidoescola][".implode(",",$arrDistorcaoReprovacao)."]" ?>
			<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "taxareprovacao[resolvidoescola][".implode(",",$arrDistorcaoReprovacao)."]" ?>
			<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = verificaTaxaDimensao7("reprovacao",$arrDistorcaoReprovacao,"diaresolvidoescola") ?>
	<?php endif; ?>
	<?php if( $arrDistorcaoAbandono && verificaCheckBoxTaxa("abandono",array_unique($arrDistorcaoAbandono)) ): ?>
			<?php $arrProblemas['descricao'][] = "Em 2010, a escola possuía ".count($arrDistorcaoAbandono)." turma(s) com taxa de abandono superior à média do Brasil. <a href=\"javascript:popUpDimensao2('exibeTurmasCriticasAbandono')\" >Clique aqui para exibir a(s) turma(s).</a>" ?>
			<?php $arrProblemas['radio']['critico']['sim'][] = "taxaabandono[critico][".implode(",",$arrDistorcaoAbandono)."]" ?>
			<?php $arrProblemas['radio']['critico']['nao'][] = "taxaabandono[critico][".implode(",",$arrDistorcaoAbandono)."]" ?>
			<?php $arrProblemas['radio']['critico']['resposta'][] = verificaTaxaDimensao7("abandono",$arrDistorcaoAbandono,"diacritico2") ?>
			<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "taxaabandono[suficiente][".implode(",",$arrDistorcaoAbandono)."]" ?>
			<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "taxaabandono[suficiente][".implode(",",$arrDistorcaoAbandono)."]" ?>
			<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = verificaTaxaDimensao7("abandono",$arrDistorcaoAbandono,"disuficiente") ?>
			<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "taxaabandono[resolvidoescola][".implode(",",$arrDistorcaoAbandono)."]" ?>
			<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "taxaabandono[resolvidoescola][".implode(",",$arrDistorcaoAbandono)."]" ?>
			<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = verificaTaxaDimensao7("abandono",$arrDistorcaoAbandono,"diaresolvidoescola") ?>
	<?php endif; ?>
	<?php if($arrRespEscola): ?>
		<?php foreach($arrRespEscola as $resp): ?>
			<?php if( verificaCheckBoxPergunta($resp['repid']) ): ?>
				<?php $arrProblemas['descricao'][] = str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?>
				<?php $arrProblemas['radio']['critico']['sim'][] = "critico2[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['critico']['nao'][] = "critico2[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['critico']['resposta'][] = $resp['critico2'] ?>
				<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "suficiente[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "suficiente[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = $resp['suficiente'] ?>
				<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "resolvidoescola[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "resolvidoescola[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = $resp['resolvidoescola'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	
	<?php $arrTurmas = recuperaTurmasCriticasPorEscola();?>
	<?php $arrTaxaReprovacaoBrasil = carregaTaxa(); ?>
	<?php $arrDisciplinas = retornaDisciplinasTurma(); ?>
	<?php $arrDistorcaoTaxaReprovacaoDisciplina = carregaDistorcaoTaxaReprovacaoDisciplina(); ?>
	<?php $arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxaEscolar(); ?>
	<?php $arrRespEscola = array() ?>
	<?php $arrRespEscola = recuperaRespostasEscola(null,"D","C",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
	<?php if($arrTurmas): ?>
		<?php if($arrTurmas['Ensino Fundamental']): ?>
			<?php foreach($arrTurmas['Ensino Fundamental'] as $em): ?>
				<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
					<?php foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds): ?>
						<?php if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ): ?>
							<?php if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['U']): ?>
								<?php $arrTurmasCriticasReprovacao["U"][$ds['pk_cod_disciplina']][] = $em['pk_cod_turma'] ?>
								<?php $arrNomeDisciplina[$ds['pk_cod_disciplina']] = $ds['no_disciplina'] ?>
								<?php $arrTurmasC["U"][$ds['pk_cod_disciplina']][$em['pk_cod_turma']] = array( 
																				"disciplina" => $ds['no_disciplina'],
																				"serie" => $em['serie'],
																				"turma" => $em['turma'],
																				"horario" => $em['hrinicio']." - ".$em['hrfim'],
																				"taxaBrasil" => $arrTaxaReprovacaoBrasil['U']." %",
																				"taxa" => round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %"
																			   ) ?>
							<?php endif; ?>
						<?php endif; ?>
					<?php endforeach; ?>
				<?php endif; ?>
			<?php endforeach; ?>
		<?php endif; ?>
		<?php if($arrTurmas['Ensino Médio']): ?>
			<?php foreach($arrTurmas['Ensino Médio'] as $em): ?>
				<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
					<?php foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds): ?>
						<?php if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ): ?>
							<?php if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['M']): ?>
								<?php $arrTurmasCriticasReprovacao["M"][$ds['pk_cod_disciplina']][] = $em['pk_cod_turma'] ?>
								<?php $arrNomeDisciplina[$ds['pk_cod_disciplina']] = $ds['no_disciplina'] ?>
								<?php $arrTurmasC["M"][$ds['pk_cod_disciplina']][$em['pk_cod_turma']] = array( 
																				"disciplina" => $ds['no_disciplina'],
																				"serie" => $em['serie'],
																				"turma" => $em['turma'],
																				"horario" => $em['hrinicio']." - ".$em['hrfim'],
																				"taxaBrasil" => $arrTaxaReprovacaoBrasil['M']." %",
																				"taxa" => round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %"
																			   ) ?>
							<?php endif; ?>
						<?php endif; ?>
					<?php endforeach; ?>
				<?php endif; ?>
			<?php endforeach; ?>
		<?php endif; ?>
	<?php endif; ?>
	<?php if($arrTurmasCriticasReprovacao["U"]): ?>
		<?php foreach($arrTurmasCriticasReprovacao["U"] as $disciplina => $arrTurmas): ?>
			<?php if( verificaCheckBoxDisciplina($disciplina,$arrTurmas) ): ?>
				<?php $arrProblemas['descricao'][] = count($arrTurmas)." turma(s) do ensino fundamental aprensentou(aram) taxa de reprovação em ".$arrNomeDisciplina[$disciplina]." superior(es) à média do Brasil. <a href=\"javascript:exibeTurma('u_$disciplina')\" >Clique aqui para exibir a(s) turma(s).</a>" ?>
				<?php $arrProblemas['radio']['critico']['sim'][] = "disciplina[critico][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['critico']['nao'][] = "disciplina[critico][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['critico']['resposta'][] = verificaDisciplinaDimensao7($disciplina,$arrTurmas,"dtdcritico2") ?>
				<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "disciplina[suficiente][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "disciplina[suficiente][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = verificaDisciplinaDimensao7($disciplina,$arrTurmas,"dtdsuficiente") ?>
				<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "disciplina[resolvidoescola][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "disciplina[resolvidoescola][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = verificaDisciplinaDimensao7($disciplina,$arrTurmas,"dtdresolvidoescola") ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	<?php if($arrTurmasCriticasReprovacao["M"]): ?>
		<?php foreach($arrTurmasCriticasReprovacao["M"] as $disciplina => $arrTurmas): ?>
			<?php if( verificaCheckBoxDisciplina($disciplina,$arrTurmas) ): ?>
				<?php $arrProblemas['descricao'][] = count($arrTurmas)." turma(s) do ensino médio aprensentou(aram) taxa de reprovação em ".$arrNomeDisciplina[$disciplina]." superior(es) à média do Brasil. <a href=\"javascript:exibeTurma('m_$disciplina')\" >Clique aqui para exibir a(s) turma(s).</a>" ?>
				<?php $arrProblemas['radio']['critico']['sim'][] = "disciplina[critico][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['critico']['nao'][] = "disciplina[critico][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['critico']['resposta'][] = verificaDisciplinaDimensao7($disciplina,$arrTurmas,"dtdcritico2") ?>
				<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "disciplina[suficiente][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "disciplina[suficiente][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = verificaDisciplinaDimensao7($disciplina,$arrTurmas,"dtdsuficiente") ?>
				<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "disciplina[resolvidoescola][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "disciplina[resolvidoescola][$disciplina][".implode(",",$arrTurmas)."]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = verificaDisciplinaDimensao7($disciplina,$arrTurmas,"dtdresolvidoescola") ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	<?php if($arrRespEscola): ?>
		<?php foreach($arrRespEscola as $resp): ?>
			<?php if( verificaCheckBoxPergunta($resp['repid']) ): ?>
				<?php $arrProblemas['descricao'][] = str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?>
				<?php $arrProblemas['radio']['critico']['sim'][] = "critico2[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['critico']['nao'][] = "critico2[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['critico']['resposta'][] = $resp['critico2'] ?>
				<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "suficiente[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "suficiente[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = $resp['suficiente'] ?>
				<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "resolvidoescola[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "resolvidoescola[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = $resp['resolvidoescola'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	
	<?php
	$sql = "SELECT  CASE WHEN spodesc IS NULL THEN '- '||prodesc
					 ELSE '- '||spodesc END as col FROM pdeinterativo.sinteseprograma s 
		LEFT JOIN pdeinterativo.programa p ON p.proid = s.proid 
		WHERE s.sprmodulo='D' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND spostatus='A'";
	
	$programas = $db->carregarColuna($sql);
	
	$sql = "SELECT '- '||sprdesc as col FROM pdeinterativo.sinteseprojeto WHERE sprmodulo='D' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND sprstatus='A'";
	
	$projetos = $db->carregarColuna($sql);
	
	 ?>
	
	<td class="SubTituloDireita bold" <?php echo $arrProblemas ? " rowspan='".count($arrProblemas['descricao'])."'' " : "" ?> >Distorção e Aproveitamento</td>
	<?php if($arrProblemas['descricao']): ?>
		<?php $x=0;foreach($arrProblemas['descricao'] as $chave => $prob): ?>
			<?php if($x != 0): ?>
				<tr>
			<?php endif; ?>
					<td class=bordapreto><?php echo $prob ?></td>
			<?php if($x == 0): ?>
					<td valign="top" class=bordapreto <?php echo $arrProblemas ? " rowspan='".count($arrProblemas['descricao'])."'' " : "" ?> ><?php echo "<b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."" ?></td>
				</tr>
			<?php else: ?>
				</tr>
			<?php endif; ?>
		<?php $x++;$i++;endforeach; ?>
	<?php else: ?>
		<td class=bordapreto >Não foram encontrados problemas de distorção e aproveitamento.</td>
		<td class=bordapreto valign="top" <?php echo $arrProblemas ? " rowspan='".count($arrProblemas['descricao'])."'' " : "" ?> ><?php echo "<b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."" ?></td>
	<?php $i++;endif; ?>
</tr>
<?php

exibeParecerDimensao(4);

/* FIM DIMENSAO 2 - Distorção e Aproveitamento */

/* DIMENSAO 3 - Ensino e Aprendizagem */

$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Projeto Pedagógico') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_projetospedagogicos = $db->carregar($sql);
if($pergs_projetospedagogicos[0]) {
	foreach($pergs_projetospedagogicos as $pergs) {
		$problemasensinoaprendizagem[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Currículo') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_curriculo = $db->carregar($sql);
if($pergs_curriculo[0]) {
	foreach($pergs_curriculo as $pergs) {
		$problemasensinoaprendizagem[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Avaliações') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_avaliacoes = $db->carregar($sql);
if($pergs_avaliacoes[0]) {
	foreach($pergs_avaliacoes as $pergs) {
		$problemasensinoaprendizagem[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE rp.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'E' and prgsubmodulo = 'T' and prgstatus = 'A' and prgdetalhe='Tempo de Aprendizagem') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_tempoaprendizagem = $db->carregar($sql);
if($pergs_tempoaprendizagem[0]) {
	foreach($pergs_tempoaprendizagem as $pergs) {
		$problemasensinoaprendizagem[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT * FROM pdeinterativo.respostatempoaprendizagem WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
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
		$problemasensinoaprendizagem['rtaporque'] = "A escola não desenvolve ações de educação integral porque: ".implode(", ",$probl);
	}
}

$sql = "SELECT  CASE WHEN spodesc IS NULL THEN '- '||prodesc
					 ELSE '- '||spodesc END as col FROM pdeinterativo.sinteseprograma s 
		LEFT JOIN pdeinterativo.programa p ON p.proid = s.proid 
		WHERE s.sprmodulo='E' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND spostatus='A'";
$programas = $db->carregarColuna($sql);
$sql = "SELECT '- '||sprdesc as col FROM pdeinterativo.sinteseprojeto WHERE sprmodulo='E' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND sprstatus='A'";
$projetos = $db->carregarColuna($sql);

if(count($problemasensinoaprendizagem) > 0) {
	
	echo "<tr>
			<td rowspan=".(count($problemasensinoaprendizagem))." class=SubTituloDireita><b>Ensino e Aprendizagem</b></td>
			<td class=bordapreto >".current($problemasensinoaprendizagem)."</td>
			<td class=bordapreto rowspan=".(count($problemasensinoaprendizagem))." valign=top><b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."</td>
		  </tr>";
	
	unset($problemasensinoaprendizagem[current(array_keys($problemasensinoaprendizagem))]);
	
	// problemas ensino a aprendizagem
	if($problemasensinoaprendizagem) {
		foreach($problemasensinoaprendizagem as $indice => $problema) {
			echo "<tr style=background-color:".$_CORES[($i%2)].">
					<td class=bordapreto>".$problema."</td>
				  </tr>";
			$i++;
		}
	}

} else {
	echo "<tr>
			<td align=right class=SubTituloDireita><b>Ensino e Aprendizagem</b></td>
			<td class=bordapreto>Não existem problemas no Ensino e Aprendizagem</td>
			<td class=bordapreto valign=top><b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."</td>
			</tr>";
}

exibeParecerDimensao(5);

/* FIM DIMENSAO 3 - Ensino e Aprendizagem */

/* DIMENSAO 4 - Gestão */

$sql = "SELECT tp.tpeid, tp.tpedesc, count(p.pesid) as qtd FROM pdeinterativo.pessoa p 
		INNER JOIN pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = p.pesid
		LEFT JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = ptp.pdeid 
		LEFT JOIN pdeinterativo.detalhepessoa dp ON dp.pesid = p.pesid 
		LEFT JOIN pdeinterativo.tipoperfil tp ON tp.tpeid = ptp.tpeid 
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND dp.tenid IN(2,3) AND tp.tpeid IN(SELECT tpeid FROM pdeinterativo.perfilarea WHERE apeid IN('".APE_EQUIPEPEDAGOGICA."','".APE_DIRETOR."','".APE_VICEDIRETOR."','".APE_SECRETARIA."')) 
		AND p.critico=true
		GROUP BY tp.tpeid, tp.tpedesc";

$pessoas = $db->carregar($sql);

if($pessoas[0]) {
	foreach($pessoas as $pes) {
		$sql = "SELECT p.pesid FROM pdeinterativo.pessoa p 
				INNER JOIN pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = p.pesid
				LEFT JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = ptp.pdeid 
				LEFT JOIN pdeinterativo.detalhepessoa dp ON dp.pesid = p.pesid 
				WHERE p.critico=true AND pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND dp.tenid IN(2,3) AND ptp.tpeid='".$pes['tpeid']."'";
		
		$p = $db->carregar($sql);
		unset($pesids);
		if($p[0]) {
			foreach($p as $po) {
				$pesids[] = $po['pesid'];
			}
		}
		$problemasgestao[implode(",",$pesids)] = "Há ".$pes['qtd']." ".$pes['tpedesc']." que não possui(em) graduação."; 
	}
}


$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Liderança') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_lideranca = $db->carregar($sql);
if($pergs_lideranca[0]) {
	foreach($pergs_lideranca as $pergs) {
		$problemasgestao[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Acompanhamento') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_acompanhamento = $db->carregar($sql);
if($pergs_acompanhamento[0]) {
	foreach($pergs_acompanhamento as $pergs) {
		$problemasgestao[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Planejamento') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_planejamento = $db->carregar($sql);

if($pergs_planejamento[0]) {
	foreach($pergs_planejamento as $pergs) {
		$problemasgestao[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Rotinas') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_rotinas = $db->carregar($sql);

if($pergs_rotinas[0]) {
	foreach($pergs_rotinas as $pergs) {
		$problemasgestao[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Normas e Regulamentos') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_normas = $db->carregar($sql);

if($pergs_normas[0]) {
	foreach($pergs_normas as $pergs) {
		$problemasgestao[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'G' and prgsubmodulo = 'F' and prgstatus = 'A' and prgdetalhe='Gestão Financeira') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_financeiras = $db->carregar($sql);

if($pergs_financeiras[0]) {
	foreach($pergs_financeiras as $pergs) {
		$problemasgestao[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT  CASE WHEN spodesc IS NULL THEN '- '||prodesc
					 ELSE '<li>'||spodesc END as col FROM pdeinterativo.sinteseprograma s 
		LEFT JOIN pdeinterativo.programa p ON p.proid = s.proid 
		WHERE s.sprmodulo='G' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND spostatus='A'";

$programas = $db->carregarColuna($sql);

$sql = "SELECT '- '||sprdesc as col FROM pdeinterativo.sinteseprojeto WHERE sprmodulo='G' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND sprstatus='A'";

$projetos = $db->carregarColuna($sql);

if(count($problemasgestao) > 0) {
	
	echo "<tr>
			<td rowspan=".(count($problemasgestao))." class=SubTituloDireita><b>Gestão</b></td>
			<td class=bordapreto>".current($problemasgestao)."</td>
			<td class=bordapreto rowspan=".(count($problemasgestao))." valign=top><b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."</td>
		  </tr>";
	
	unset($problemasgestao[current(array_keys($problemasgestao))]);
	
	if($problemasgestao) {
		foreach($problemasgestao as $indice => $problemas) {
			echo "<tr>
					<td class=bordapreto>".$problemas."</td>
				  </tr>";
			$i++;
		}
	}

} else {
	echo "<tr>
			<td align=right class=SubTituloDireita><b>Gestão</b></td>
			<td class=bordapreto>Não existem problemas para gestão</td>
			<td class=bordapreto valign=top><b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."</td>
			</tr>";
}

exibeParecerDimensao(6);

/* FIM DIMENSAO 4 - Gestão */


/* DIMENSAO 5 - Comunidade escolar */
	
$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Compromisso') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_compromisso = $db->carregar($sql);
if($pergs_compromisso[0]) {
	foreach($pergs_compromisso as $pergs) {
		$problemascomunidadeescolar[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Protagonismo e Participação') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_protagonismo = $db->carregar($sql);
if($pergs_protagonismo[0]) {
	foreach($pergs_protagonismo as $pergs) {
		$problemascomunidadeescolar[$pergs['repid']] = str_replace("(*)", $pergs['oppdesc'], $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Saúde e Bem-estar') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_saude = $db->carregar($sql);
if($pergs_saude[0]) {
	foreach($pergs_saude as $pergs) {
		$problemascomunidadeescolar[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Práticas') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_praticas = $db->carregar($sql);

if($pergs_praticas[0]) {
	foreach($pergs_praticas as $pergs) {
		$problemascomunidadeescolar[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Experiencia e Auto-Confiança') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_experiencia = $db->carregar($sql);

if($pergs_experiencia[0]) {
	foreach($pergs_experiencia as $pergs) {
		$problemascomunidadeescolar[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Cooperação e Respeito') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_cooperacao = $db->carregar($sql);
if($pergs_cooperacao[0]) {
	foreach($pergs_cooperacao as $pergs) {
		$problemascomunidadeescolar[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Motivação') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_motivacao = $db->carregar($sql);

if($pergs_motivacao[0]) {
	foreach($pergs_motivacao as $pergs) {
		$problemascomunidadeescolar[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Comunicação') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_comunicacao = $db->carregar($sql);

if($pergs_comunicacao[0]) {
	foreach($pergs_comunicacao as $pergs) {
		$problemaspaisecomunidade[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc FROM pdeinterativo.respostapergunta rp 
		INNER JOIN pdeinterativo.pdinterativo pd ON pd.pdeid = rp.pdeid 
		INNER JOIN pdeinterativo.pergunta pe ON pe.prgid = rp.prgid 
		INNER JOIN pdeinterativo.opcaopergunta op ON op.oppid = rp.oppid
		WHERE pd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND 
			  rp.prgid IN(select prgid from pdeinterativo.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Participação') AND
			  rp.oppid IN(5,6) AND rp.critico=true ORDER BY rp.repid";

$pergs_participacao = $db->carregar($sql);

if($pergs_participacao[0]) {
	foreach($pergs_participacao as $pergs) {
		$problemascomunidadeescolar[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
	}
}

$sql = "SELECT rd.rdoid FROM pdeinterativo.respostadocente rd 
		INNER JOIN pdeinterativo.respostadocenteformacao rf ON rf.rdoid = rd.rdoid 
		INNER JOIN educacenso_2010.tab_docente td ON td.pk_cod_docente = rd.pk_cod_docente 
		WHERE rd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND rd.rdocritico=TRUE AND rdostatus='A'
		GROUP BY rd.rdoid";

$docentesformacao = $db->carregarColuna($sql);

if($docentesformacao) {
	$problemascomunidadeescolar[implode(",",$docentesformacao)] = count($docentesformacao)." docentes consideram que sua formação não é apropriada para ministrar a área de conhecimento/ disciplina que atuam. <span style=cursor:pointer; onclick=exibeTurma('docentes2');>Clique aqui para ver os nomes dos docentes</span>.";
}

$sql = "SELECT * FROM pdeinterativo.respostapaiscomunidade WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
$respostapaiscomunidade = $db->pegaLinha($sql);

if($respostapaiscomunidade) {
	if($respostapaiscomunidade['rpcpossuiconcelhocritico']=="t" && $respostapaiscomunidade['rpcpossuiconcelho']=="f") {
		$problemascomunidadeescolar['rpcpossuiconcelho'] = "A escola não possui Conselho Escolar";
	}
}

$sql = "SELECT  CASE WHEN spodesc IS NULL THEN '- '||prodesc
					 ELSE '- '||spodesc END as col FROM pdeinterativo.sinteseprograma s 
		LEFT JOIN pdeinterativo.programa p ON p.proid = s.proid 
		WHERE s.sprmodulo='C' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND spostatus='A'";
$programas = $db->carregarColuna($sql);

$sql = "SELECT '- '||sprdesc as col FROM pdeinterativo.sinteseprojeto WHERE sprmodulo='C' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND sprstatus='A'";
$projetos = $db->carregarColuna($sql);


if(count($problemascomunidadeescolar) > 0) {
	
	echo "<tr>
			<td rowspan=".(count($problemascomunidadeescolar))." class=SubTituloDireita><b>Comunidade escolar</b></td>
			<td class=bordapreto>".current($problemascomunidadeescolar)."</td>
			<td  class=bordapreto rowspan=".(count($problemascomunidadeescolar))." valign=top><b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."</td>
		  </tr>";
	
	unset($problemascomunidadeescolar[current(array_keys($problemascomunidadeescolar))]);
	
	if($problemascomunidadeescolar) {
		foreach($problemascomunidadeescolar as $indice => $problemas) {
			echo "<tr>
					<td class=bordapreto>".$problemas."</td>
				  </tr>";
			$i++;
		}
	}

} else {
	echo "<tr>
			  <td align=right class=SubTituloDireita><b>Comunidade escolar</b></td>
			  <td class=bordapreto>Não existem problemas com Comunidade escolar</td>
			  <td class=bordapreto valign=top><b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."</td>
		  </tr>";
}

exibeParecerDimensao(7);

/* FIM DIMENSAO 5 - Comunidade escolar */


/* DIMENSAO 6 - Infraestrutura */
?>
<tr>
	<?php
	$sql = "SELECT  CASE WHEN spodesc IS NULL THEN '- '||prodesc
					 ELSE '- '||spodesc END as col FROM pdeinterativo.sinteseprograma s 
		LEFT JOIN pdeinterativo.programa p ON p.proid = s.proid 
		WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND spostatus='A' and s.sprmodulo='F'";
	
	$programas = $db->carregarColuna($sql);
	
	$sql = "SELECT '- '||sprdesc as col FROM pdeinterativo.sinteseprojeto WHERE sprmodulo='F' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND sprstatus='A'";
	
	$projetos = $db->carregarColuna($sql);
	
	 ?>
	 <?php $arrProblemas = false; ?>
	 <?php $arrInstalacoesNecessarias = recuperaInstalacoesNecessarias(); ?>
	 <?php $arrInstalacoesInadequadas = recuperaInstalacoesInadequadas(); ?>
	 <?php $arrEquipamentosRuins = recuperaEquipamentosRuins() ?>
	 <?php $arrRespEscola = false; ?>
	 <?php $arrRespEscola = recuperaRespostasEscola(null,"I","I",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
	<?php if($arrInstalacoesInadequadas): ?>
		<?php foreach($arrInstalacoesInadequadas as $infra):?>
			<?php if( $infra['rifcritico'] == "t" ): ?>
				<?php $arrProblemas['descricao'][] = "A escola possui ". $infra['rifqtdinadequado'] ." ". $infra['ifidesc'] ." inadequados(as)." ?>
				<?php $arrProblemas['radio']['critico']['sim'][] = "intalacao[critico][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['critico']['nao'][] = "intalacao[critico][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['critico']['resposta'][] = $infra['rifcritico2'] ?>
				<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "intalacao[suficiente][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "intalacao[suficiente][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = $infra['rifsuficiente'] ?>
				<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "intalacao[resolvidoescola][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "intalacao[resolvidoescola][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = $infra['rifresolvidoescola'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	<?php if($arrInstalacoesNecessarias): ?>
		<?php foreach($arrInstalacoesNecessarias as $infra):?>
			<?php if( $infra['rifcritico'] == "t" ): ?>
				<?php $arrProblemas['descricao'][] = "A escola não possui ".$infra['ifidesc']."." ?>
				<?php $arrProblemas['radio']['critico']['sim'][] = "intalacao[critico][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['critico']['nao'][] = "intalacao[critico][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['critico']['resposta'][] = $infra['rifcritico2'] ?>
				<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "intalacao[suficiente][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "intalacao[suficiente][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = $infra['rifsuficiente'] ?>
				<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "intalacao[resolvidoescola][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "intalacao[resolvidoescola][{$infra['ifiid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = $infra['rifresolvidoescola'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	<?php if($arrEquipamentosRuins): ?>
		<?php foreach($arrEquipamentosRuins as $equip): ?>
			<?php if( $equip['remcritico'] == "t" ): ?>
				<?php $arrProblemas['descricao'][] = "A escola avalia que o estado de conservação de ".number_format($equip['rmeqtdruin'],"",2,".")." ".$equip['tmedesc']." é ruim." ?>
				<?php $arrProblemas['radio']['critico']['sim'][] = "equipamento[critico][{$equip['tmeid']}]" ?>
				<?php $arrProblemas['radio']['critico']['nao'][] = "equipamento[critico][{$equip['tmeid']}]" ?>
				<?php $arrProblemas['radio']['critico']['resposta'][] = $equip['remcritico2'] ?>
				<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "equipamento[suficiente][{$equip['tmeid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "equipamento[suficiente][{$equip['tmeid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = $equip['remsuficiente'] ?>
				<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "equipamento[resolvidoescola][{$equip['tmeid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "equipamento[resolvidoescola][{$equip['tmeid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = $equip['remresolvidoescola'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	 <?php if($arrRespEscola): ?>
		<?php foreach($arrRespEscola as $resp): ?>
			<?php if( verificaCheckBoxPergunta($resp['repid']) ): ?>
				<?php $arrProblemas['descricao'][] = str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?>
				<?php $arrProblemas['radio']['critico']['sim'][] = "critico2[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['critico']['nao'][] = "critico2[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['critico']['resposta'][] = $resp['critico2'] ?>
				<?php $arrProblemas['radio']['programaprojeto']['sim'][] = "suficiente[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['nao'][] = "suficiente[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['programaprojeto']['resposta'][] = $resp['suficiente'] ?>
				<?php $arrProblemas['radio']['resolvidoescola']['sim'][] = "resolvidoescola[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['nao'][] = "resolvidoescola[{$resp['repid']}]" ?>
				<?php $arrProblemas['radio']['resolvidoescola']['resposta'][] = $resp['resolvidoescola'] ?>
			<?php endif; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	
	<td class="SubTituloDireita bold" <?php echo $arrProblemas ? " rowspan='".count($arrProblemas['descricao'])."'' " : "" ?> >Infraestrutura</td>
	<?php if($arrProblemas['descricao']): ?>
		<?php $x=0;foreach($arrProblemas['descricao'] as $chave => $prob): ?>
			<?php if($x != 0): ?>
				<tr>
			<?php endif; ?>
					<td class=bordapreto ><?php echo $prob ?></td>
			<?php if($x == 0): ?>
					<td class=bordapreto <?php echo $arrProblemas ? " rowspan='".count($arrProblemas['descricao'])."'' " : "" ?> ><?php echo "<b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."" ?></td>
				</tr>
			<?php else: ?>
				</tr>
			<?php endif; ?>
		<?php $x++;$i++;endforeach; ?>
	<?php else: ?>
		<td class=bordapreto >Não foram encontrados problemas de infraestrutura.</td>
		<td class=bordapreto valign="top" <?php echo $arrProblemas ? " rowspan='".count($arrProblemas['descricao'])."'' " : "" ?> ><?php echo "<b>Programas:</b><br>".(($programas)?implode("<br>",$programas):"- Não existem programas cadastrados")."<br/><b>Projetos:</b><br>".(($projetos)?implode("<br>",$projetos):"- Não existem projetos cadastrados")."" ?></td>
	<?php $i++;endif; ?>
</tr>
<?php

exibeParecerDimensao(8);

/* FIM DIMENSAO 6 - Infraestrutura */


?>
</table>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita">O que deseja fazer agora?</td>
	<td>
	<?php $arrTelasPendentes = recuperaTelasSintesesPendentes() ?>
	<?php if(!$arrTelasPendentes): ?>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_3_sintese';" >
	<input type="button" name="salvar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_7_sintese';diagnostico_7_sintese();"> 
	<input type="button" name="continuar" value="Salvar e continuar" onclick="document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_orientacoes';diagnostico_7_sintese();">
	<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_orientacoes';" > 
	<?php else: ?>
		<?php foreach($arrTelasPendentes as $tela): ?>
				<p class="red bold"><img src="../imagens/atencao.png" class="img_middle"> Favor preencher a tela <?php echo $tela ?> antes de salvar a Síntese!</p>
		<?php endforeach; ?>
		<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_6_infraestrutura&aba1=diagnostico_6_3_sintese';" >
		<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_orientacoes';" >
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
			<div style="padding:5px;overflow:auto;" >	
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
			<div style="padding:5px;overflow:auto;" >
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

<?

if(count($docentesformacao) > 0) {

	$sql = "SELECT td.pk_cod_docente, td.no_docente FROM pdeinterativo.respostadocente rd 
			INNER JOIN pdeinterativo.respostadocenteformacao rf ON rf.rdoid = rd.rdoid 
			INNER JOIN educacenso_2010.tab_docente td ON td.pk_cod_docente = rd.pk_cod_docente 
			WHERE rd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND rd.rdocritico=TRUE 
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