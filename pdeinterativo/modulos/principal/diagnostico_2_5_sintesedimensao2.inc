<?
monta_titulo( "Síntese da Dimesão 2 - Distorção e aproveitamento", "&nbsp;");

?>
<script>
	function salvarSinsteseDimensao2(local)
	{
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
	
		if(jQuery("[name^='chk_problemas[']:checked").length > 0){
			jQuery("[name='hdn_redirect']").val(local);
			jQuery("[name=form_distorcao_sintese_dimensao]").submit();
		}else{
			alert('Selecione pelo menos 1 problema como crítico!');
		}
	}
	
	function exibePrograma(obj)
	{
		if(obj.value == "Sim"){
			jQuery("#programa_label").show();
		}else{
			jQuery("#programa_label").hide();
		}
	}
	
	function exibeProjeto(obj)
	{
		if(obj.value == "Sim"){
			jQuery("#projeto_label").show();
		}else{
			jQuery("#projeto_label").hide();
		}
	}
	
	function addProjeto()
	{
		janela('pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarProjetos&sprmodulo=D',500,300,'Projetos');
	}
	
	function addPrograma()
	{
		janela('pdeinterativo.php?modulo=principal/diagnostico&acao=A&requisicao=gerenciarProgramas&sprmodulo=D',500,300,'Projetos');
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
	
	function calculaNumProblemasCriticos()
	{
		var numTotal = jQuery("[name^='chk_problemas[']").length;
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
		
	function verificaCheckBox(obj)
	{
		
		if(jQuery("[name='" + obj.name + "']").attr("checked") == true)
		{
			var num = jQuery("#num_problemas_possiveis").html();
			var numMarcados = jQuery("[name^='chk_problemas[']:checked").length;
			num = num*1;
			numMarcados = numMarcados*1;
			if(numMarcados > num)
			{
				jQuery("[name='" + obj.name + "']").attr("checked","");
				alert('Selecione no máximo ' + num + ' problema(s) como crítico(s)!');
			}
		}
	}
	
	jQuery(function(){
		calculaNumProblemasCriticos();
		
		
	<? $existePrograma = existePrograma(array('rppmodulo'=>'D')); ?>
	<? if($existePrograma=="t") : ?>
	document.getElementById('programa_label').style.display='';
	carregarProgramas('D');
	document.getElementById('exibeprograma_S').checked=true;
	<? elseif($existePrograma=="f"): ?>
	document.getElementById('exibeprograma_N').checked=true;
	<? endif; ?>
	
	<? $existeProjeto = existeProjeto(array('rppmodulo'=>'D')); ?>
	<? if($existeProjeto=="t") : ?>
	document.getElementById('projeto_label').style.display='';
	carregarProjetos('D');
	document.getElementById('exibeprojeto_S').checked=true;
	<? elseif($existeProjeto=="f"): ?>
	document.getElementById('exibeprojeto_N').checked=true;
	<? endif; ?>
		
	});	
</script>
<form name="form_distorcao_sintese_dimensao" action="" method="post" >
	<input type="hidden" name="requisicao" value="salvarSinsteseDimensao2" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5"  cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita blue" width="20%">Orientações</td>
			<td class="SubTituloEsquerda blue normal" >
				<p>Chegamos à Síntese da Dimensão 2. A tabela abaixo apresenta o conjunto de problemas identificados nesta dimensão. Leia as frases e assinale os problemas considerados mais "críticos" pela escola, até o limite indicado na tabela (que corresponde a 30% do total de problemas). As frases assinaladas aparecerão na Síntese Geral do diagnóstico, depois que todas as dimensões forem respondidas. Após assinalar os problemas críticos, registre se a escola possui ou participa de algum projeto e/ou programa destinado a melhorar ou minimizar esses problemas.</p>
				<p>Lembre-se: todos os problemas são importantes, mas a escola deve concentrar esforços naqueles que ela pode resolver.</p>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Problemas identificados</td>
			<td>
				<table width="100%" class="listagem" cellSpacing="1" cellPadding="3"  >
				<tr>
					<td class="SubTituloCentro bold" width="20%">Tema</td>
					<td class="SubTituloCentro bold">
						<table cellSpacing="1" cellPadding="3" width="100%" >
							<tr>
								<td  class="SubTituloCentro bold" >Problema(s) Identificado(s)</td>
								<td  class="SubTituloCentro bold" width="100" >Problema(s) Crítico(s)<br/>(Máximo <span id="num_problemas_possiveis">X</span>)</td>
							</tr>
						</table>	
					</td>
				</tr>
				<tr>
					<?php $arrDistorcao = carregaDistorcaoDiagnosticoMatricula(false,"M",null); ?>
					<?php $arrRespEscola = array() ?>
					<td class="bold" width="20%">Matrícula</td>
					<td valign="top" >
						<table cellSpacing="1" cellPadding="3" width="100%" >	
							<?php $x=1;if($arrDistorcao): ?>
								<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
								<tr bgcolor="<?php echo $cor ?>" >
									<td>Em 2010, a escola possuía <?php echo count($arrDistorcao) ?> turma(s) com nº de matrículas superior ao parâmetro do CNE. <a href="javascript:popUpDimensao2('exibeTurmasCNE')" >Clique aqui para exibir a(s) turma(s).</a></td>
									<td width="100" class="center" >
										<input type="checkbox" <?php echo verificaTurmasCNE(array_unique($arrDistorcao)) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[matricula]" value="<?php echo implode(",",$arrDistorcao) ?>" />
										<input type="hidden" name="arrTurmasMatricula[]" value="<?php echo implode(",",$arrDistorcao) ?>" />
									</td>
								</tr>
							<?php $x++;endif; ?>
							<?php if($arrRespEscola): ?>
								<?php foreach($arrRespEscola as $resp): ?>
								<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
								<tr bgcolor="<?php echo $cor ?>" >
										<td><?php echo str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?></td>
										<td class="center" >
											<input type="checkbox" <?php echo verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[opcao][<?php echo $resp['repid'] ?>]" value="1" />
											<input type="hidden" name="arrRepid[<?php echo $resp['repid'] ?>]" value="<?php echo $resp['repid'] ?>" />
										</td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
							<?php if(!$arrDistorcao && !$arrRespEscola): ?>
								<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
								<tr bgcolor="<?php echo $cor ?>" >
									<td>Não foram identificados problemas na distorção idade-série.</td>
								</tr>
							<?php $x++;endif; ?>
						</table>
					</td>
				</tr>
				<tr bgcolor="#FFFFFF" >
					<?php $arrDistorcao = carregaDistorcaoDiagnosticoMatricula(null,"D","D"); ?>
					<?php $arrRespEscola = array() ?>
					<?php $arrRespEscola = recuperaRespostasEscola(null,"D","D",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
					<td class="bold" width="20%">Distorção idade-série</td>
					<td valign="top" >
						<table cellSpacing="1" cellPadding="3" width="100%" >	
							<?php $x=1;if($arrDistorcao): ?>
								<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
								<tr bgcolor="<?php echo $cor ?>" >
									<td>Em 2010, a escola possuía <?php echo count($arrDistorcao) ?> turma(s) com taxa de distorção superior à média do Brasil. <a href="javascript:popUpDimensao2('exibeTurmasCriticas')" >Clique aqui para exibir a(s) turma(s).</a></td>
									<td width="100" class="center" >
										<input type="checkbox" <?php echo verificaCheckBoxTaxa("distorcao",array_unique($arrDistorcao)) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[taxa][distorcao][<?php echo count($arrDistorcao) ?>]" value="<?php echo implode(",",$arrDistorcao) ?>" />
										<input type="hidden" name="arrTurmasTaxa[]" value="<?php echo implode(",",$arrDistorcao) ?>" />
									</td>
								</tr>
							<?php $x++;endif; ?>
							<?php if($arrRespEscola): ?>
								<?php foreach($arrRespEscola as $resp): ?>
								<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
								<tr bgcolor="<?php echo $cor ?>" >
										<td><?php echo str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?></td>
										<td class="center" >
											<input type="checkbox" <?php echo verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[opcao][<?php echo $resp['repid'] ?>]" value="1" />
											<input type="hidden" name="arrRepid[<?php echo $resp['repid'] ?>]" value="<?php echo $resp['repid'] ?>" />
										</td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
							<?php if(!$arrDistorcao && !$arrRespEscola): ?>
								<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
								<tr bgcolor="<?php echo $cor ?>" >
									<td>Não foram identificados problemas na distorção idade-série.</td>
								</tr>
							<?php $x++;endif; ?>
						</table>
					</td>
				</tr>
				<tr>
					<?php $arrDistorcaoReprovacao = carregaDistorcaoDiagnosticoMatricula(null,"A","R"); ?>
					<?php $arrDistorcaoAbandono = carregaDistorcaoDiagnosticoMatricula(null,"A","A"); ?>
					<?php $arrRespEscola = array() ?>
					<?php $arrRespEscola = recuperaRespostasEscola(null,"D","A",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
					<td class="bold" width="20%">Aproveitamento escolar</td>
					<td valign="top" >
						<table cellSpacing="1" cellPadding="3" width="100%" >	
							<?php $x=0;if($arrDistorcaoReprovacao): ?>
								<?php $cor = $x%2==1 ? "#FFFFFF" : "#F5F5F5" ?>
								<tr bgcolor="<?php echo $cor ?>" >
									<td>Em 2010, a escola possuía <?php echo count($arrDistorcaoReprovacao) ?> turma(s) com taxa de reprovação superior à média do Brasil. <a href="javascript:popUpDimensao2('exibeTurmasCriticasReprovacao')" >Clique aqui para exibir a(s) turma(s).</a></td>
									<td width="100" class="center" >
										<input type="checkbox" <?php echo verificaCheckBoxTaxa("reprovacao",array_unique($arrDistorcaoReprovacao)) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[taxa][reprovacao][<?php echo count($arrDistorcaoReprovacao) ?>]" value="<?php echo implode(",",$arrDistorcaoReprovacao) ?>" />
										<input type="hidden" name="arrTurmasTaxa[]" value="<?php echo implode(",",$arrDistorcaoReprovacao) ?>" />
									</td>
								</tr>
							<?php $x++;endif; ?>
							<?php if($arrDistorcaoAbandono): ?>
								<?php $cor = $x%2==1 ? "#FFFFFF" : "#F5F5F5" ?>
								<tr bgcolor="<?php echo $cor ?>" >
									<td>Em 2010, a escola possuía <?php echo count($arrDistorcaoAbandono) ?> turma(s) com taxa de abandono superior à média do Brasil. <a href="javascript:popUpDimensao2('exibeTurmasCriticasAbandono')" >Clique aqui para exibir a(s) turma(s).</a></td>
									<td class="center" >
										<input type="checkbox" <?php echo verificaCheckBoxTaxa("abandono",array_unique($arrDistorcaoAbandono)) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[taxa][abandono][<?php echo count($arrDistorcaoAbandono) ?>]" value="<?php echo implode(",",$arrDistorcaoAbandono) ?>" />
										<input type="hidden" name="arrTurmasTaxa[]" value="<?php echo implode(",",$arrDistorcaoAbandono) ?>" />
									</td>
								</tr>
							<?php $x++;endif; ?>
							<?php if($arrRespEscola): ?>
								<?php foreach($arrRespEscola as $resp): ?>
									<?php $cor = $x%2==1 ? "#FFFFFF" : "#F5F5F5" ?>
									<tr bgcolor="<?php echo $cor ?>" >
										<td><?php echo str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?></td>
										<td class="center" >
											<input type="checkbox" <?php echo verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[opcao][<?php echo $resp['repid'] ?>]" value="1" />
											<input type="hidden" name="arrRepid[<?php echo $resp['repid'] ?>]" value="<?php echo $resp['repid'] ?>" />
										</td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
							<?php if(!$arrDistorcaoAbandono && !$arrDistorcaoAbandono && !$arrRespEscola): ?>
								<tr>
									<td>Não foram identificados problemas no aproveitamento escolar.</td>
								</tr>
							<?php endif; ?>
						</table>
					</td>
				</tr>
				<tr bgcolor="#FFFFFF" >
					<?php $arrTurmas = recuperaTurmasCriticasPorEscola();?>
					<?php $arrTaxaReprovacaoBrasil = carregaTaxa(); ?>
					<?php $arrDisciplinas = retornaDisciplinasTurma(); ?>
					<?php $arrDistorcaoTaxaReprovacaoDisciplina = carregaDistorcaoTaxaReprovacaoDisciplina(); ?>
					<?php $arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxaEscolar(); ?>
					<td class="bold" width="20%">Áreas de conhecimento</td>
					<td valign="top" >
						<table cellSpacing="1" cellPadding="3" width="100%" >	
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
							<?php $x=1;if($arrTurmasCriticasReprovacao): ?>
								<?php if($arrTurmasCriticasReprovacao["U"]): ?>
									<?php foreach($arrTurmasCriticasReprovacao["U"] as $disciplina => $arrTurmas): ?>
										<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
										<tr bgcolor="<?php echo $cor ?>" >
											<td><?php echo count($arrTurmas) ?> turma(s) do ensino fundamental aprensentou(aram) taxa de reprovação em <?php echo $arrNomeDisciplina[$disciplina] ?> superior(es) à média do Brasil. <a href="javascript:exibeTurma('u_<?php echo $disciplina ?>')" >Clique aqui para exibir a(s) turma(s).</a></td>
											<td width="100" class="center" >
												<input type="checkbox" <?php echo verificaCheckBoxDisciplina($disciplina,$arrTurmas) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[disciplina][<?php echo $disciplina ?>][<?php echo count($arrTurmas) ?>]" value="<?php echo implode(",",$arrTurmas) ?>" />
												<input type="hidden" name="arrTurmasDisciplina[]" value="<?php echo implode(",",$arrTurmas) ?>" />
											</td>
										</tr>
									<?php $x++;endforeach; ?>
								<?php endif; ?>	
								<?php if($arrTurmasCriticasReprovacao["M"]): ?>
									<?php foreach($arrTurmasCriticasReprovacao["M"] as $disciplina => $arrTurmas): ?>
										<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
										<tr bgcolor="<?php echo $cor ?>" >
											<td><?php echo count($arrTurmas) ?> turma(s) do ensino médio aprensentou(aram) taxa de reprovação em <?php echo $arrNomeDisciplina[$disciplina] ?> superior(es) à média do Brasil. <a href="javascript:exibeTurma('m_<?php echo $disciplina ?>')" >Clique aqui para exibir a(s) turma(s).</a></td>
											<td class="center" width="100" >
												<input type="checkbox" <?php echo verificaCheckBoxDisciplina($disciplina,$arrTurmas) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[disciplina][<?php echo $disciplina ?>][<?php echo count($arrTurmas) ?>]" value="<?php echo implode(",",$arrTurmas) ?>" />
												<input type="hidden" name="arrTurmasDisciplina[]" value="<?php echo implode(",",$arrTurmas) ?>" />
											</td>
										</tr>
									<?php $x++;endforeach; ?>
								<?php endif; ?>
							<?php endif; ?>
							<?php $arrRespEscola = array() ?>
							<?php $arrRespEscola = recuperaRespostasEscola(null,"D","C",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')")); ?>
							<?php if($arrRespEscola): ?>
								<?php foreach($arrRespEscola as $resp): ?>
									<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
									<tr bgcolor="<?php echo $cor ?>" >
										<td><?php echo str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc']) ?></td>
										<td class="center" width="100" >
											<input type="checkbox" <?php echo verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[opcao][<?php echo $resp['repid'] ?>]" value="1" />
											<input type="hidden" name="arrRepid[<?php echo $resp['repid'] ?>]" value="<?php echo $resp['repid'] ?>" />
											</td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
							<?php if(!$arrTurmasCriticasReprovacao && !$arrRespEscola): ?>
								<tr>
									<td>Não foram identificados problemas no aproveitamento escolar.</td>
								</tr>
							<?php endif; ?>
						</table>
					</td>
				</tr>
				</table>
			</td>
		</tr>
		<tr id="tr_projetos_programas" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita">Projetos e Programas</td>
			<td>
			<p>A escola desenvolve algum PROJETO destinado a melhorar os resultados relacionados à Distorção e Aproveitamento? <input type="radio" name="exibeprojeto" id="exibeprojeto_S" onclick="divCarregando();if(this.checked){gravarRespostaProgramasProjetos('D','J',true);document.getElementById('projeto_label').style.display='';carregarProjetos('D');}else{document.getElementById('projeto_label').style.display='none';}divCarregado();"> Sim <input type="radio" id="exibeprojeto_N" name="exibeprojeto" onclick="divCarregando();if(this.checked){if(gravarRespostaProgramasProjetos('D','J',false)){document.getElementById('projeto_label').style.display='none';}else{document.getElementById('exibeprojeto_S').checked=true;}}else{document.getElementById('projeto_label').style.display='';}divCarregado();"> Não </p>
			<div id="projeto_label" style="display:none"></div>
			
			<p>A escola participa ou gostaria de participar de algum PROGRAMA, do Governo Federal ou da secretaria, que auxilie na melhoria os resultados relacionados à Distorção e Aprendizagem? <input type="radio" name="exibeprograma" id="exibeprograma_S" onclick="divCarregando();if(this.checked){gravarRespostaProgramasProjetos('D','G',true);document.getElementById('programa_label').style.display='';carregarProgramas('D');}else{document.getElementById('programa_label').style.display='none';}divCarregado();"> Sim <input type="radio" id="exibeprograma_N" name="exibeprograma" onclick="divCarregando();if(this.checked){if(gravarRespostaProgramasProjetos('D','G',false)){document.getElementById('programa_label').style.display='none';}else{document.getElementById('exibeprograma_S').checked=true;}}else{document.getElementById('programa_label').style.display='';}divCarregado();"> Não </p>
			<div id="programa_label" style="display:none"></div>
			</td>
		</tr>
		<tr id="tr_navegacao" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
			<td>
				<?php $arrTelasPendentes = recuperaTelasPendentes("diagnostico_2_distorcaoeaproveitamento") ?>
				<?php if(!$arrTelasPendentes): ?>
					<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_4_areasdeconhecimento'" >
					<input type="button" name="btn_salvar" value="Salvar" onclick="salvarSinsteseDimensao2()">
					<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="salvarSinsteseDimensao2('C');">
					<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_0_orientacoes'" >
				<?php else: ?>
					<?php foreach($arrTelasPendentes as $tela): ?>
							<p class="red bold"><img src="../imagens/atencao.png" class="img_middle"> Favor preencher a tela <?php echo $tela ?> antes de salvar a Síntese!</p>
					<?php endforeach; ?>
					<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_4_areasdeconhecimento'" >
					<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_0_orientacoes'" >
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