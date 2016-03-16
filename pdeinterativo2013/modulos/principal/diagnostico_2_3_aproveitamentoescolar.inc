<?
monta_titulo( "Aproveitamento escolar - Distorção e aproveitamento", "&nbsp;");

$arrDistorcaoReprovacao = carregaDistorcaoDiagnosticoMatricula(null,"A","R");
$arrDistorcaoAbandono = carregaDistorcaoDiagnosticoMatricula(null,"A","A");
$arrDistorcao = array();
$arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxaEscolar();
$display = count($arrDistorcaoTaxa) == 0 && count($arrDistorcaoReprovacao) == 0 && count($arrDistorcaoAbandono) == 0 ? "none" : "";

?>
<script>
	function salvarDistorcaoAproveitamentoEscolar(local)
	{
	
		jQuery("[name^='num_abandono[']").each(function(k, v){v.value = mascaraglobal('[.###]',v.value);});
		jQuery("[name^='num_reprovacao[']").each(function(k, v){v.value = mascaraglobal('[.###]',v.value);});
	
		var erro = 0;
		
		var n = 0;
		var arrT = new Array();
		jQuery("[name^='chk_turma_abandono[']").each(function(){
	 			var id = jQuery(this).attr("name");
	 			id = id.replace("chk_turma_abandono[","");
	 			id = id.replace("]","");
	 			var taxaBrasil = jQuery("[name='taxa_abandono_brasil[" + id + "]']").val();
	 			var taxa = jQuery("#taxa_abandono_" + id).html();
	 			if(taxa.search("%") >= 0){
					taxa = trim(taxa);
					taxa = taxa.replace("%","");
					taxa = taxa*1;
					if(taxaBrasil){
						taxaBrasil = taxaBrasil*1;
						if(taxa > taxaBrasil && jQuery("[name='chk_turma_abandono[" + id + "]']").attr("checked") == false){
							arrT[n] = id;
		 					n++;
						}
					}
				}
			});
			
		if(arrT.length){
			alert("Há " + arrT.length + " turma(s) cuja taxa de abandono é superior à média do Brasil.\nSelecione todas as turmas nesta situação.");
			erro = 1;
			return false;
		}
		
		var n = 0;
		var arrT = new Array();
		jQuery("[name^='chk_turma_reprovacao[']").each(function(){
	 			var id = jQuery(this).attr("name");
	 			id = id.replace("chk_turma_reprovacao[","");
	 			id = id.replace("]","");
	 			var taxaBrasil = jQuery("[name='taxa_reprovacao_brasil[" + id + "]']").val();
	 			var taxa = jQuery("#taxa_reprovacao_" + id).html();
	 			if(taxa.search("%") >= 0){
					taxa = trim(taxa);
					taxa = taxa.replace("%","");
					taxa = taxa*1;
					if(taxaBrasil){
						taxaBrasil = taxaBrasil*1;
						if(taxa > taxaBrasil && jQuery("[name='chk_turma_reprovacao[" + id + "]']").attr("checked") == false){
							arrT[n] = id;
		 					n++;
						}
					}
				}
			});
			
		if(arrT.length){
			alert("Há " + arrT.length + " turma(s) cuja taxa de reprovação é superior à média do Brasil.\nSelecione todas as turmas nesta situação.");
			erro = 1;
			return false;
		}
		
		if(!verificaRespostasPerguntas()){
			alert('Favor responder todas as perguntas!');
			erro = 1;
			return false;
		}
		
		var numAbandono = jQuery("[name^='num_abandono[']").length;
		var numAbandonoOK = jQuery("[name^='num_abandono['][value!='']").length;
		if(numAbandono != numAbandonoOK){
			alert('Favor informar o número de abandono para todas as turmas!');
			erro = 1;
			return false;
		}
		
		var numReprovacao = jQuery("[name^='num_reprovacao[']").length;
		var numReprovacaoOK = jQuery("[name^='num_reprovacao['][value!='']").length;
		if(numReprovacao != numReprovacaoOK){
			alert('Favor informar o número de reprovação para todas as turmas!');
			erro = 1;
			return false;
		}
		
		if(erro == 0){
			jQuery("[name='hdn_redirect']").val(local);
			jQuery("[name=form_distorcao_aproveitamento_escolar]").submit();
		}
	}
	
	function calculataxaAbandono(CodTurma,numAbandono,numMatricula)
	{
		if(numMatricula && numAbandono)
		{
			numAbandono = numAbandono.replace(".","");
			numAbandono = numAbandono.replace(".","");
			numAbandono = numAbandono.replace(".","");
			numAbandono = numAbandono*1;
			
			numMatricula = numMatricula.replace(".","");
			numMatricula = numMatricula.replace(".","");
			numMatricula = numMatricula.replace(".","");
			numMatricula = numMatricula*1;
			
			if(numAbandono > numMatricula){
				numAbandono = numMatricula;
				jQuery("[name='num_abandono[" + CodTurma + "]']").val( mascaraglobal('[.###]',numAbandono) );
			}
			
			var taxa = ( numAbandono / numMatricula )*100;
			
			jQuery("#taxa_abandono_" + CodTurma).html( Math.round(taxa) + " %");
		}else{
			jQuery("#taxa_abandono_" + CodTurma).html("N/A");
		}
	}
	
	function calculataxaReprovacao(CodTurma,numReprovacao,numMatricula)
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
			
			if(numReprovacao > numMatricula){
				numReprovacao = numMatricula;
				jQuery("[name='num_reprovacao[" + CodTurma + "]']").val( mascaraglobal('[.###]',numReprovacao) );
			}
			
			var taxa = ( numReprovacao / numMatricula )*100;
			
			jQuery("#taxa_reprovacao_" + CodTurma).html( Math.round(taxa) + " %");
		}else{
			jQuery("#taxa_reprovacao_" + CodTurma).html("N/A");
		}
	}
	
	function verificaTaxaReprovacao(CodTurma,taxaBrasil)
	{
		var taxa = jQuery("#taxa_reprovacao_" + CodTurma).html();
		if(taxa.search("%") >= 0){
			taxa = trim(taxa);
			taxa = taxa.replace("%","");
			taxa = taxa*1;
			if(!taxaBrasil){
				return true;
			}
			taxaBrasil = taxaBrasil*1;
			if(taxa <= taxaBrasil && jQuery("[name='chk_turma_reprovacao[" + CodTurma + "]']").attr("checked") == true){
				if(confirm("A turma selecionada possui taxa de reprovação inferior ou igual à média do Brasil.\nTem certeza de que deseja selecionar esta turma?")){
					return true;
				}else{
					jQuery("[name='chk_turma_reprovacao[" + CodTurma + "]']").attr("checked","");
					return false;
				}
			}else{
				return true;
			}
		}else{
			jQuery("[name='chk_turma_reprovacao[" + CodTurma + "]']").attr("checked","");
			alert('Esta turma não possui taxa de reprovação!');
		}
	}
	
	function verificaTaxaAbandono(CodTurma,taxaBrasil)
	{
		var taxa = jQuery("#taxa_abandono_" + CodTurma).html();
		if(taxa.search("%") >= 0){
			taxa = trim(taxa);
			taxa = taxa.replace("%","");
			taxa = taxa*1;
			if(!taxaBrasil){
				return true;
			}
			taxaBrasil = taxaBrasil*1;
			if(taxa <= taxaBrasil && jQuery("[name='chk_turma_abandono[" + CodTurma + "]']").attr("checked") == true){
				if(confirm("A turma selecionada possui taxa de abandono inferior ou igual à média do Brasil.\nTem certeza de que deseja selecionar esta turma?")){
					return true;
				}else{
					jQuery("[name='chk_turma_abandono[" + CodTurma + "]']").attr("checked","");
					return false;
				}
			}else{
				return true;
			}
		}else{
			jQuery("[name='chk_turma_abandono[" + CodTurma + "]']").attr("checked","");
			alert('Esta turma não possui taxa de abandono!');
		}
	}
	
</script>
<form name="form_distorcao_aproveitamento_escolar" action="" method="post" >
	<input type="hidden" name="requisicao" value="salvarDistorcaoAproveitamentoEscolar" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5"  cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita blue" width="20%">Orientações</td>
			<td class="SubTituloEsquerda blue normal" >
				<p>Na Dimensão 1, analisamos as taxas de rendimento do Brasil, do Estado, do Município e da Escola. Agora, vamos aprofundar a análise dos indicadores e conhecer os resultados de reprovação e abandono de cada turma.</p>
				<p>Enfatizamos essas duas taxas porque sabemos que elas sinalizam situações não desejadas, caso estejam muito elevadas. E, em todo caso, é importante conhecer quais as turmas que estão impactando mais nos resultados da escola, a fim de propor ações mais focalizadas.</p>
				<p>Indique ao lado de cada turma o número de alunos reprovados ou que abandonaram a escola no ano de referência indicado na tabela e o sistema calculará as taxas automaticamente.</p>
				<p>Em seguida, analise os resultados de cada turma e assinale na última coluna aquela(s) cujas taxas de reprovação e abandono são superiores à média do Brasil (valor indicado no título da coluna). Lembre-se que as turmas assinaladas serão consideradas críticas na síntese desta dimensão. E não esqueça de responder às perguntas no final da tela.</p>
				
				<?php if($display == "none"): ?>
					<p>Clique no botão abaixo para visualizar as taxas de rendimento.</p>
				<?php endif; ?>
				<?php if($display == "none"): ?>
					<p><input type="button" name="btntaxas" class="blue bold" value="CLIQUE AQUI para visualizar as Taxas" onclick="exibeTRs()"></p>
				<?php endif; ?>
			</td>
		</tr>
		<?php $arrTurmas = recuperaTurmasPorEscola();?>
		<tr id="tr_parametros" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita" width="20%">Parâmetros</td>
			<td>
			<table width="100%">
			<?php if($arrTurmas['Ensino Fundamental']): ?>
				<tr>
					<td align="center" >
						<? $dados_distorcaoU = montaTabelaAproveitamentoEstudantes("U"); echo $dados_distorcaoU['html'];?>
					</td>
					<td align="center">
						<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoAproveitamentoEstudantes&ensino=U">
					</td>
				</tr>
			<?php endif; ?>
			<?php if($arrTurmas['Ensino Médio']): ?>
				<tr>
					<td align="center" >
						<? $dados_distorcaoM = montaTabelaAproveitamentoEstudantes("M"); echo $dados_distorcaoM['html'];?>
					</td>
					<td align="center">
						<img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoAproveitamentoEstudantes&ensino=M">
					</td>
				</tr>
			<?php endif; ?>
			</table>
			</td>
		</tr>
		<?php if($arrTurmas['Ensino Fundamental']): ?>
			<tr id="tr_censo_ef" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Ensino Fundamental</td>
				<td>
					<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Abandono</td>
							</tr>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2010</td>
							</tr>
							<tr>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de abandono</td>
								<td class="bold center" >Taxa de abandono (em %)</td>
								<td class="bold center" >Selecione as turmas cuja taxa de abandono seja</br> superior à média do Brasil (<?php echo $dados_distorcaoU['taxaAbandonoBrasil']['Ensino Fundamental'] ?>%)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Ensino Fundamental'] as $em): ?>
							<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" ><?php echo campo_texto("num_abandono[".$em['pk_cod_turma']."]","S","S","Número de Abandonos","10","20","[.###]","","","","","","calculataxaAbandono('{$em['pk_cod_turma']}',this.value,'{$em['nummatricula']}')",$arrDistorcaoTaxa['abandono'][$em['pk_cod_turma']]) ?></td>
								<td class="center" id="taxa_abandono_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['taxa']['abandono'][$em['pk_cod_turma']] != "" ? $arrDistorcaoTaxa['taxa']['abandono'][$em['pk_cod_turma']]." %" : "" ?></td>
								<td class="center" >
									<input type="checkbox" onclick="verificaTaxaAbandono('<?php echo $em['pk_cod_turma'] ?>','<?php echo $dados_distorcaoU['taxaAbandonoBrasil']['Ensino Fundamental'] ?>')" name="chk_turma_abandono[<?php echo $em['pk_cod_turma'] ?>]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcaoAbandono) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
									<input type="hidden" name="taxa_abandono_brasil[<?php echo $em['pk_cod_turma'] ?>]" value="<?php echo $dados_distorcaoU['taxaAbandonoBrasil']['Ensino Fundamental'] ?>" />
								</td>
							</tr>
						<?php $n++;endforeach; ?>
					</table>
					<br />
					<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Reprovação</td>
							</tr>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2010</td>
							</tr>
							<tr>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de reprovação</td>
								<td class="bold center" >Taxa de reprovação (em %)</td>
								<td class="bold center" >Selecione as turmas cuja taxa de reprovação seja</br> superior à média do Brasil (<?php echo $dados_distorcaoU['taxaReprovacaoBrasil']['Ensino Fundamental'] ?>%)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Ensino Fundamental'] as $em): ?>
							<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" ><?php echo campo_texto("num_reprovacao[".$em['pk_cod_turma']."]","S","S","Número de Reprovados","10","20","[.###]","","","","","","calculataxaReprovacao('{$em['pk_cod_turma']}',this.value,'{$em['nummatricula']}')",$arrDistorcaoTaxa['reprovacao'][$em['pk_cod_turma']]) ?></td>
								<td class="center" id="taxa_reprovacao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']] != "" ? $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']]." %" : "" ?></td>
								<td class="center" >
									<input type="checkbox" onclick="verificaTaxaReprovacao('<?php echo $em['pk_cod_turma'] ?>','<?php echo $dados_distorcaoU['taxaReprovacaoBrasil']['Ensino Fundamental'] ?>')" name="chk_turma_reprovacao[<?php echo $em['pk_cod_turma'] ?>]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcaoReprovacao) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
									<input type="hidden" name="taxa_reprovacao_brasil[<?php echo $em['pk_cod_turma'] ?>]" value="<?php echo $dados_distorcaoU['taxaReprovacaoBrasil']['Ensino Fundamental'] ?>" />
								</td>
							</tr>
						<?php $n++;endforeach; ?>
					</table>
				</td>
			</tr>
		<?php endif; ?>
		<?php if($arrTurmas['Ensino Médio']): ?>
			<tr id="tr_censo_ef" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Ensino Médio</td>
				<td>
					<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Abandono</td>
							</tr>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2010</td>
							</tr>
							<tr>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de abandono</td>
								<td class="bold center" >Taxa de abandono (em %)</td>
								<td class="bold center" >Selecione as turmas cuja taxa de abandono seja</br> superior à média do Brasil (<?php echo $dados_distorcaoM['taxaAbandonoBrasil']['Ensino Médio'] ?>%)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Ensino Médio'] as $em): ?>
							<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" ><?php echo campo_texto("num_abandono[".$em['pk_cod_turma']."]","S","S","Número de Abandonos","10","20","[.###]","","","","","","calculataxaAbandono('{$em['pk_cod_turma']}',this.value,'{$em['nummatricula']}')",$arrDistorcaoTaxa['abandono'][$em['pk_cod_turma']]) ?></td>
								<td class="center" id="taxa_abandono_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['taxa']['abandono'][$em['pk_cod_turma']] != "" ? $arrDistorcaoTaxa['taxa']['abandono'][$em['pk_cod_turma']]." %" : "" ?></td>
								<td class="center" >
									<input type="checkbox" onclick="verificaTaxaAbandono('<?php echo $em['pk_cod_turma'] ?>','<?php echo $dados_distorcaoM['taxaAbandonoBrasil']['Ensino Médio'] ?>')" name="chk_turma_abandono[<?php echo $em['pk_cod_turma'] ?>]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcaoAbandono) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
									<input type="hidden" name="taxa_abandono_brasil[<?php echo $em['pk_cod_turma'] ?>]" value="<?php echo $dados_distorcaoM['taxaAbandonoBrasil']['Ensino Médio'] ?>" />
								</td>
							</tr>
						<?php $n++;endforeach; ?>
					</table>
					<br />
					<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Reprovação</td>
							</tr>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2010</td>
							</tr>
							<tr>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de reprovação</td>
								<td class="bold center" >Taxa de reprovação (em %)</td>
								<td class="bold center" >Selecione as turmas cuja taxa de reprovação seja</br> superior à média do Brasil (<?php echo $dados_distorcaoM['taxaReprovacaoBrasil']['Ensino Médio'] ?>%)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Ensino Médio'] as $em): ?>
							<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" ><?php echo campo_texto("num_reprovacao[".$em['pk_cod_turma']."]","S","S","Número de Reprovados","10","20","[.###]","","","","","","calculataxaReprovacao('{$em['pk_cod_turma']}',this.value,'{$em['nummatricula']}')",$arrDistorcaoTaxa['reprovacao'][$em['pk_cod_turma']]) ?></td>
								<td class="center" id="taxa_reprovacao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']] != "" ? $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']]." %" : "" ?></td>
								<td class="center" >
									<input type="checkbox" onclick="verificaTaxaReprovacao('<?php echo $em['pk_cod_turma'] ?>','<?php echo $dados_distorcaoM['taxaReprovacaoBrasil']['Ensino Médio'] ?>')" name="chk_turma_reprovacao[<?php echo $em['pk_cod_turma'] ?>]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcaoReprovacao) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
									<input type="hidden" name="taxa_reprovacao_brasil[<?php echo $em['pk_cod_turma'] ?>]" value="<?php echo $dados_distorcaoM['taxaReprovacaoBrasil']['Ensino Médio'] ?>" />
								</td>
							</tr>
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
								<td class="bold center" colspan="7">Abandono</td>
							</tr>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2010</td>
							</tr>
							<tr>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de abandono</td>
								<td class="bold center" >Taxa de abandono (em %)</td>
								<td class="bold center" >Selecione as turmas cuja taxa de abandono seja</br> superior à média do Brasil (<?php echo $dados_distorcaoI['taxaAbandonoBrasil']['Educação Infantil'] ?>%)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Educação Infantil'] as $em): ?>
							<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" ><?php echo campo_texto("num_abandono[".$em['pk_cod_turma']."]","S","S","Número de Abandonos","10","20","[.###]","","","","","","calculataxaAbandono('{$em['pk_cod_turma']}',this.value,'{$em['nummatricula']}')",$arrDistorcaoTaxa['abandono'][$em['pk_cod_turma']]) ?></td>
								<td class="center" id="taxa_abandono_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['taxa']['abandono'][$em['pk_cod_turma']] != "" ? $arrDistorcaoTaxa['taxa']['abandono'][$em['pk_cod_turma']]." %" : "" ?></td>
								<td class="center" >
									<input type="checkbox" onclick="verificaTaxaAbandono('<?php echo $em['pk_cod_turma'] ?>','<?php echo $dados_distorcaoI['taxaAbandonoBrasil']['Educação Infantil'] ?>')" name="chk_turma_abandono[<?php echo $em['pk_cod_turma'] ?>]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcaoAbandono) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
									<input type="hidden" name="taxa_abandono_brasil[<?php echo $em['pk_cod_turma'] ?>]" value="<?php echo $dados_distorcaoI['taxaAbandonoBrasil']['Educação Infantil'] ?>" />
								</td>
							</tr>
						<?php $n++;endforeach; ?>
					</table>
					<br />
					<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Reprovação</td>
							</tr>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2010</td>
							</tr>
							<tr>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de reprovação</td>
								<td class="bold center" >Taxa de reprovação (em %)</td>
								<td class="bold center" >Selecione as turmas cuja taxa de reprovação seja</br> superior à média do Brasil (<?php echo $dados_distorcaoI['taxaReprovacaoBrasil']['Educação Infantil'] ?>%)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Educação Infantil'] as $em): ?>
							<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" ><?php echo campo_texto("num_reprovacao[".$em['pk_cod_turma']."]","S","S","Número de Reprovados","10","20","[.###]","","","","","","calculataxaReprovacao('{$em['pk_cod_turma']}',this.value,'{$em['nummatricula']}')",$arrDistorcaoTaxa['reprovacao'][$em['pk_cod_turma']]) ?></td>
								<td class="center" id="taxa_reprovacao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']] != "" ? $arrDistorcaoTaxa['taxa']['reprovacao'][$em['pk_cod_turma']]." %" : "" ?></td>
								<td class="center" >
									<input type="checkbox" onclick="verificaTaxaReprovacao('<?php echo $em['pk_cod_turma'] ?>','<?php echo $dados_distorcaoI['taxaReprovacaoBrasil']['Educação Infantil'] ?>')" name="chk_turma_reprovacao[<?php echo $em['pk_cod_turma'] ?>]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcaoReprovacao) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
									<input type="hidden" name="taxa_reprovacao_brasil[<?php echo $em['pk_cod_turma'] ?>]" value="<?php echo $dados_distorcaoI['taxaReprovacaoBrasil']['Educação Infantil'] ?>" />
								</td>
							</tr>
						<?php $n++;endforeach; ?>
					</table>
				</td>
			</tr>
		<?php endif; ?>
		<tr id="tr_perguntas" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita" width="20%">Perguntas</td>
			<td>
				<?php $sql = "select * from pdeinterativo2013.pergunta where prgmodulo = 'D' and prgsubmodulo = 'A' and prgstatus = 'A'"; ?>
				<?php quadroPerguntas($sql) ?>
			</td>
		</tr>
		<tr id="tr_navegacao" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
			<td>
				<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_2_distorcaoidadeserie'" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarDistorcaoAproveitamentoEscolar();">
				<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="salvarDistorcaoAproveitamentoEscolar('C');">
				<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_4_areasdeconhecimento'" >
			</td>
		</tr>
	</table>
</form>