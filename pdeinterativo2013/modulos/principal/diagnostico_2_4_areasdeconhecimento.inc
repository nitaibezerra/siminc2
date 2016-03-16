<?
monta_titulo( "Áreas de conhecimento - Distorção e aproveitamento", "&nbsp;");
$arrDistorcaoReprovacao = carregaDistorcaoDiagnosticoMatricula(null,"A","R");
$arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxaEscolar();
$arrDisciplinas = retornaDisciplinasTurma();
$arrDistorcaoTaxaReprovacaoDisciplina = carregaDistorcaoTaxaReprovacaoDisciplina();
?>
<script>
	function salvarDistorcaoAreasConhecimento(local)
	{
		var erro = 0;
		
		if(!verificaRespostasPerguntas()){
			alert('Favor responder todas as perguntas!');
			erro = 1;
			return false;
		}
		
		var numDisc = jQuery("[name^='num_reprovacao_disciplina[']").length;
		var numDiscOK = jQuery("[name^='num_reprovacao_disciplina['][value!='']").length;
		if(numDisc != numDiscOK){
			alert('Favor informar todos os reprovados por disciplina!');
			erro = 1;
			return false;
		}
		
		var valorPorTurma = new Array();
		
//		jQuery("[id^='turma_reprovacao_']").each(function(){
//			var id = jQuery(this).attr("id");
// 			id = id.replace("turma_reprovacao_","");
// 			var valorTotal = jQuery(this).html() * 1;
// 			valorPorTurma[id] = 0;
// 			jQuery("[name^='num_reprovacao_disciplina[" + id + "][']").each(function(){
// 				valorPorTurma[id] += ( jQuery(this).val()*1 );
// 			});
// 			
// 			if(valorTotal > valorPorTurma[id]){
// 				erro = 1;
// 				alert('Existe(m) turma(s) com o número de reprovações maior que o especificado na(s) disciplina(s)!');
// 				return false;
// 			}
// 			
//		});
		
		if(erro == 0){
			jQuery("[name='hdn_redirect']").val(local);
			jQuery("[name=form_distorcao_areas_conhecimento]").submit();
		}
	}
	
	function fechaDisciplina(obj,turma)
	{

		if(obj.src.search("../imagens/menos.gif") >= 0){
			jQuery("[id^='tr_disciplina_" + turma + "'],[id='thead_disciplina_" + turma + "']").hide();
			obj.src = "../imagens/mais.gif";
		}else{
			jQuery("[id^='tr_disciplina_" + turma + "'],[id='thead_disciplina_" + turma + "']").show();
			obj.src = "../imagens/menos.gif";
		}
	}
	
	function calculataxaReprovacaoDisciplina(CodTurma,CodDisciplina,numReprovacao,numMatricula,numTotalReprovacao)
	{
		
		if(numMatricula && numReprovacao)
		{
			numReprovacao = numReprovacao.replace(".","");
			numReprovacao = numReprovacao.replace(".","");
			numReprovacao = numReprovacao.replace(".","");
			numReprovacao = numReprovacao*1;
			
			numMatricula = numMatricula.replace(".","");
			numMatricula = numMatricula.replace(".","");
			numMatricula = numMatricula.replace(".","");
			numMatricula = numMatricula*1;
			
			if(numReprovacao > numTotalReprovacao){
				jQuery("[name='num_reprovacao_disciplina[" + CodTurma + "][" + CodDisciplina + "]']").val(numTotalReprovacao);
				numReprovacao = numTotalReprovacao;
			}
			
			var taxa = ( numReprovacao / numMatricula )*100;
			
			jQuery("#taxa_reprovacao_turma_" + CodTurma + "_disciplina_" + CodDisciplina).html( Math.round(taxa) + " %");
		}else{
			jQuery("#taxa_reprovacao_turma_" + CodTurma + "_disciplina_" + CodDisciplina).html("N/A");
		}
	}
	
</script>
<form name="form_distorcao_areas_conhecimento" action="" method="post" >
	<input type="hidden" name="requisicao" value="salvarDistorcaoAreasConhecimento" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5"  cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita blue" width="20%">Orientações</td>
			<td class="SubTituloEsquerda blue normal" >
			<p>Esta etapa tem como principal objetivo identificar as áreas de conhecimento/ disciplinas críticas. Para auxiliar nesta tarefa, o sistema copiou apenas as séries e turmas nas quais as taxas de reprovação calculadas foram superiores à média de reprovação do Brasil (conforme tela "Aproveitamento escolar").</p>
			<p>Vamos calcular agora a taxa de reprovação por área de conhecimento/ disciplina nestas séries/ anos e turmas críticas. Para tanto, preencha o campo que indica o número de estudantes reprovados em cada disciplina, no ano de referência. O sistema calculará a taxa de reprovação e, na síntese da dimensão, exibirá as disciplinas com taxas superiores à média de reprovação do Brasil.</p>
			<p>Esta tela aponta, a rigor, quais as áreas de conhecimento/ disciplinas críticas nas turmas críticas. Ou seja, onde os estudantes estão apresentando os piores resultados e que afetam os resultados de toda a escola. Esta é, portanto, uma boa oportunidade para a equipe escolar debater as possíveis causas que explicam as dificuldades de ensino e aprendizagem e definir ações que ajudem a superá-las. 
			</td>
		</tr>
		<?php $arrTurmas = recuperaTurmasCriticasPorEscola();?>
		<?php if($arrTurmas['Ensino Fundamental']): ?>
			<tr id="tr_censo_ef" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Ensino Fundamental</td>
				<td>
					<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Turmas Críticas</td>
							</tr>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2012</td>
							</tr>
							<tr>
								<td class="bold center" >Ação</td>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de reprovação</td>
								<td class="bold center" >Taxa de reprovação (em %)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Ensino Fundamental'] as $em): ?>
							<?php $cor = $n%2==0 ? "#F9F9F9" : "#C5C5C5" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" >
									<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
										<img class="link" onclick="fechaDisciplina(this,<?php echo $em['pk_cod_turma'] ?>)" src="../imagens/menos.gif" />
									<?php else: ?>
									-
									<?php endif; ?>
								</td>
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td id="turma_reprovacao_<?php echo $em['pk_cod_turma'] ?>" class="center" ><?php echo $arrDistorcaoTaxa['reprovacao'][$em['pk_cod_turma']] ?></td>
								<td class="center" id="taxa_reprovacao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']] ? $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']]." %" : "" ?></td>
							</tr>
							<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
								<thead>
									<tr id="thead_disciplina_<?php echo $em['pk_cod_turma'] ?>" >
										<td class="bold center" ><img src="../imagens/seta_filho.gif" /></td>
										<td colspan="4" class="bold left" >Área de Conhecimento / Disciplina</td>
										<td class="bold center" >Nº de reprovação</td>
										<td class="bold center" >Taxa de reprovação da disciplina (em %)</td>
									</tr>
								</thead>
								<?php $x=1;foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds): ?>
									<?php $cor = $x%2==0 ? "#FFFFFF" : "" ?>
									<tr id="tr_disciplina_<?php echo $em['pk_cod_turma'] ?>_<?php echo $x ?>" bgcolor="<?php echo $cor ?>" >
										<td class="bold center" ></td>
										<td class="center" ><img src="../imagens/seta_filho.gif" /></td>
										<td colspan="3" class="left" ><?php echo $ds['no_disciplina'] ?></td>
										<td class="center" ><?php echo campo_texto("num_reprovacao_disciplina[".$em['pk_cod_turma']."][".$ds['pk_cod_disciplina']."]","S","S","Número de Reprovados na Disciplina","10","20","[.###]","","","","","","calculataxaReprovacaoDisciplina('{$em['pk_cod_turma']}','{$ds['pk_cod_disciplina']}',this.value,'{$em['nummatricula']}','{$arrDistorcaoTaxa['reprovacao'][$em['pk_cod_turma']]}')",$arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]) ?></td>
										<td id="taxa_reprovacao_turma_<?php echo $em['pk_cod_turma'] ?>_disciplina_<?php echo $ds['pk_cod_disciplina'] ?>" class="center" ><?php echo $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ? round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %" : "" ?></td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
						<?php $n++;endforeach; ?>
					</table>
				</td>
			</tr>
		<?php endif; ?>
		<?php if($arrTurmas['Ensino Médio']): ?>
			<tr id="tr_censo_em" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Ensino Médio</td>
				<td>
					<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Turmas Críticas</td>
							</tr>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2012</td>
							</tr>
							<tr>
								<td class="bold center" >Ação</td>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de reprovação</td>
								<td class="bold center" >Taxa de reprovação (em %)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Ensino Médio'] as $em): ?>
							<?php $cor = $n%2==0 ? "#F9F9F9" : "#C5C5C5" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" >
									<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
										<img class="link" onclick="fechaDisciplina(this,<?php echo $em['pk_cod_turma'] ?>)" src="../imagens/menos.gif" />
									<?php else: ?>
									-
									<?php endif; ?>
								</td>
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" id="turma_reprovacao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['reprovacao'][$em['pk_cod_turma']] ?></td>
								<td class="center" id="taxa_reprovacao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']] ? $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']]." %" : "" ?></td>
							</tr>
							<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
								<thead>
									<tr id="thead_disciplina_<?php echo $em['pk_cod_turma'] ?>" >
										<td class="bold center" ><img src="../imagens/seta_filho.gif" /></td>
										<td colspan="4" class="bold left" >Área de Conhecimento / Disciplina</td>
										<td class="bold center" >Nº de reprovação</td>
										<td class="bold center" >Taxa de reprovação da disciplina (em %)</td>
									</tr>
								</thead>
								<?php $x=1;foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds): ?>
									<?php $cor = $x%2==0 ? "#FFFFFF" : "" ?>
									<tr id="tr_disciplina_<?php echo $em['pk_cod_turma'] ?>_<?php echo $x ?>" bgcolor="<?php echo $cor ?>" >
										<td class="bold center" ></td>
										<td class="center" ><img src="../imagens/seta_filho.gif" /></td>
										<td colspan="3" class="left" ><?php echo $ds['no_disciplina'] ?></td>
										<td class="center" ><?php echo campo_texto("num_reprovacao_disciplina[".$em['pk_cod_turma']."][".$ds['pk_cod_disciplina']."]","S","S","Número de Reprovados na Disciplina","10","20","[.###]","","","","","","calculataxaReprovacaoDisciplina('{$em['pk_cod_turma']}','{$ds['pk_cod_disciplina']}',this.value,'{$em['nummatricula']}','{$arrDistorcaoTaxa['reprovacao'][$em['pk_cod_turma']]}')",$arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]) ?></td>
										<td id="taxa_reprovacao_turma_<?php echo $em['pk_cod_turma'] ?>_disciplina_<?php echo $ds['pk_cod_disciplina'] ?>" class="center" ><?php echo $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ? round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %" : "" ?></td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
						<?php $n++;endforeach; ?>
					</table>
				</td>
			</tr>
		<?php endif; ?>
		<?php if($arrTurmas['Educação Infantil']): ?>
			<tr id="tr_censo_ei" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Educação Infantil</td>
				<td>
					<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Turmas Críticas</td>
							</tr>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2012</td>
							</tr>
							<tr>
								<td class="bold center" >Ação</td>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de reprovação</td>
								<td class="bold center" >Taxa de reprovação (em %)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Educação Infantil'] as $em): ?>
							<?php $cor = $n%2==0 ? "#F9F9F9" : "#C5C5C5" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" >
									<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
										<img class="link" onclick="fechaDisciplina(this,<?php echo $em['pk_cod_turma'] ?>)" src="../imagens/menos.gif" />
									<?php else: ?>
									-
									<?php endif; ?>
								</td>
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" id="turma_reprovacao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['reprovacao'][$em['pk_cod_turma']] ?></td>
								<td class="center" id="taxa_reprovacao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']] ? $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']]." %" : "" ?></td>
							</tr>
							<?php if($arrDisciplinas[$em['pk_cod_turma']]): ?>
								<thead>
									<tr id="thead_disciplina_<?php echo $em['pk_cod_turma'] ?>" >
										<td class="bold center" ><img src="../imagens/seta_filho.gif" /></td>
										<td colspan="4" class="bold left" >Área de Conhecimento / Disciplina</td>
										<td class="bold center" >Nº de reprovação</td>
										<td class="bold center" >Taxa de reprovação da disciplina (em %)</td>
									</tr>
								</thead>
								<?php $x=1;foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds): ?>
									<?php $cor = $x%2==0 ? "#FFFFFF" : "" ?>
									<tr id="tr_disciplina_<?php echo $em['pk_cod_turma'] ?>_<?php echo $x ?>" bgcolor="<?php echo $cor ?>" >
										<td class="bold center" ></td>
										<td class="center" ><img src="../imagens/seta_filho.gif" /></td>
										<td colspan="3" class="left" ><?php echo $ds['no_disciplina'] ?></td>
										<td class="center" ><?php echo campo_texto("num_reprovacao_disciplina[".$em['pk_cod_turma']."][".$ds['pk_cod_disciplina']."]","S","S","Número de Reprovados na Disciplina","10","20","[.###]","","","","","","calculataxaReprovacaoDisciplina('{$em['pk_cod_turma']}','{$ds['pk_cod_disciplina']}',this.value,'{$em['nummatricula']}','{$arrDistorcaoTaxa['reprovacao'][$em['pk_cod_turma']]}')",$arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]) ?></td>
										<td id="taxa_reprovacao_turma_<?php echo $em['pk_cod_turma'] ?>_disciplina_<?php echo $ds['pk_cod_disciplina'] ?>" class="center" ><?php echo $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ? round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %" : "" ?></td>
									</tr>
								<?php $x++;endforeach; ?>
							<?php endif; ?>
						<?php $n++;endforeach; ?>
					</table>
				</td>
			</tr>
		<?php endif; ?>
		<tr id="tr_perguntas" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita" width="20%">Perguntas</td>
			<td>
				<?php $sql = "select * from pdeinterativo2013.pergunta where prgmodulo = 'D' and prgsubmodulo = 'C' and prgstatus = 'A'"; ?>
				<?php quadroPerguntas($sql) ?>
			</td>
		</tr>
		<tr id="tr_navegacao" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
			<td>
				<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_3_aproveitamentoescolar'" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarDistorcaoAreasConhecimento();">
				<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="salvarDistorcaoAreasConhecimento('C');">
				<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_5_sintesedimensao2'" >
			</td>
		</tr>
	</table>
</form>