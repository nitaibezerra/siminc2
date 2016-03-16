<?
monta_titulo( "Distorção idade-série - Distorção e aproveitamento", "&nbsp;");

$arrDistorcao = carregaDistorcaoDiagnosticoMatricula(null,"D","D");
$arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxa();
$display = $arrDistorcaoTaxa ? "" : "none";

?>
<script>
	function salvarDistorcaoIdadeSerie(local)
	{
		var erro = 0;
		
		var n = 0;
		var arrT = new Array();
		jQuery("[name^='chk_turma[']").each(function(){
	 			var id = jQuery(this).attr("name");
	 			id = id.replace("chk_turma[","");
	 			id = id.replace("]","");
	 			var taxaBrasil = jQuery("[name='taxa_brasil[" + id + "]']").val();
	 			var taxa = jQuery("#taxa_distorcao_" + id).html();
	 			if(taxa.search("%") >= 0){
					taxa = trim(taxa);
					taxa = taxa.replace("%","");
					taxa = taxa*1;
					if(taxaBrasil){
						taxaBrasil = taxaBrasil*1;
						if(taxa > taxaBrasil && jQuery("[name='chk_turma[" + id + "]']").attr("checked") == false){
							arrT[n] = id;
		 					n++;
						}
					}
				}
			});
		if(arrT.length){
			alert("Há " + arrT.length + " turma(s) cuja taxa de distorção idade-série é superior à média do Brasil.\nSelecione todas as turmas nesta situação.");
			erro = 1;
			return false;
		}
		
		var numDist = jQuery("[name^='num_distorcao[']").length;
		var numDistResp = jQuery("[name^='num_distorcao['][value!='']").length;
		
		if(numDist != numDistResp){
			alert('Favor informar todas as distorções!');
			erro = 1;
			return false;
		}
		
		if(!verificaRespostasPerguntas()){
			alert('Favor responder todas as perguntas!');
			erro = 1;
			return false;
		}	
		
		if(erro == 0){
			jQuery("[name='hdn_redirect']").val(local);
			jQuery("[name=form_distorcao_idade_serie]").submit();
		}
	}
	
	function calculataxaDistorcao(CodTurma,numDistorcao,numMatricula)
	{
		if(numDistorcao && numMatricula)
		{
			numDistorcao = numDistorcao.replace(".","");
			numDistorcao = numDistorcao.replace(".","");
			numDistorcao = numDistorcao.replace(".","");
			numDistorcao = numDistorcao*1;
			
			numMatricula = numMatricula.replace(".","");
			numMatricula = numMatricula.replace(".","");
			numMatricula = numMatricula.replace(".","");
			numMatricula = numMatricula*1;
			
			if(numDistorcao > numMatricula){
				numDistorcao = numMatricula;
				jQuery("[name='num_distorcao[" + CodTurma + "]']").val( mascaraglobal('[.###]',numDistorcao) );
			}
			
			var taxa = ( numDistorcao / numMatricula )*100;
			
			jQuery("#taxa_distorcao_" + CodTurma).html( Math.round(taxa) + " %");
		}else{
			jQuery("#taxa_distorcao_" + CodTurma).html("N/A");
		}
	}
	
	function verificaTaxaDistorcao(CodTurma,taxaBrasil)
	{

		var taxa = jQuery("#taxa_distorcao_" + CodTurma).html();
		if(taxa.search("%") >= 0){
			taxa = trim(taxa);
			taxa = taxa.replace("%","");
			taxa = taxa*1;
			if(!taxaBrasil){
				return true;
			}
			taxaBrasil = taxaBrasil*1;
			if(taxa <= taxaBrasil && jQuery("[name='chk_turma[" + CodTurma + "]']").attr("checked") == true){
				if(confirm("A turma selecionada possui taxa de distorção idade-série inferior ou igual à média do Brasil.\nTem certeza de que deseja selecionar esta  turma?")){
					return true;
				}else{
					jQuery("[name='chk_turma[" + CodTurma + "]']").attr("checked","");
					return false;
				}
			}else{
				return true;
			}
		}else{
			jQuery("[name='chk_turma[" + CodTurma + "]']").attr("checked","");
			alert('Esta turma não possui taxa de distorção idade-série!');
		}
	}
	
</script>
<form name="form_distorcao_idade_serie" action="" method="post" >
	<input type="hidden" name="requisicao" value="salvarDistorcaoIdadeSerie" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5"  cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita blue" width="20%">Orientações</td>
			<td class="SubTituloEsquerda blue normal" >
				<p>O indicador de distorção idade-série informa o percentual de estudantes que se encontram fora da série/ano considerada(o) adequada(o) para a sua faixa etária.</p>
				<p>A tabela e o gráfico abaixo apresentam as taxas de distorção idade-série do Brasil, Estado, Município e da escola. Indique ao lado de cada turma o número de alunos em distorção no ano de referência indicado na tabela e o sistema calculará a taxa de distorção idade-série.</p>
				<p>Em seguida, analise os resultados de cada turma e assinale aquela(s) cuja taxa de distorção idade-série é(são) superior(es) à média do Brasil. Não esqueça de responder as perguntas no final da tela.</p>
				<?php if(!$arrDistorcaoTaxa): ?>
					<p>Clique no botão abaixo para visualizar as taxas de distorção idade-série.</p>
					<p><input type="button" name="btntaxas" class="blue bold" value="CLIQUE AQUI para visualizar as Taxas" onclick="exibeTRs()"></p>
				<?php endif; ?>
			</td>
		</tr>
		<tr id="tr_parametros" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita" width="20%">Parâmetros</td>
			<td>
			<table width="100%">
			<tr>
				<td align="center"><? $dados_distorcao = montaTabelaDistorcao(); echo $dados_distorcao['html']; ?></td>
				<td align="center"><img src="pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&requisicao=montaGraficoDistorcao"></td>
			</tr>
			</table>
			</td>
		</tr>
		<?php $arrTurmas = recuperaTurmasPorEscola();?>
		<?php if($arrTurmas['Ensino Fundamental']): ?>
			<tr id="tr_censo_ef" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Ensino Fundamental</td>
				<td>
					<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2012</td>
							</tr>
							<tr>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de distorções</td>
								<td class="bold center" >Taxa de distorção idade-série (em %)</td>
								<td class="bold center" >Selecione as turmas cuja taxa seja<br /> superior à média do Brasil (<?php echo $dados_distorcao['taxaDistorcaoBrasil']['Ensino Fundamental'] ?>%)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Ensino Fundamental'] as $em): ?>
							<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" ><?php echo campo_texto("num_distorcao[".$em['pk_cod_turma']."]","S","S","Número de Distorção","10","20","[.###]","","","","","","calculataxaDistorcao('{$em['pk_cod_turma']}',this.value,'{$em['nummatricula']}')",($arrDistorcaoTaxa[$em['pk_cod_turma']]['distorcao'] ? $arrDistorcaoTaxa[$em['pk_cod_turma']]['distorcao'] : "0") ) ?></td>
								<td class="center" id="taxa_distorcao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa[$em['pk_cod_turma']]['taxa'] ? $arrDistorcaoTaxa[$em['pk_cod_turma']]['taxa']." %" : "0%" ?></td>
								<td class="center" >
									<input type="checkbox" onclick="verificaTaxaDistorcao('<?php echo $em['pk_cod_turma'] ?>','<?php echo $dados_distorcao['taxaDistorcaoBrasil']['Ensino Fundamental'] ?>')" name="chk_turma[<?php echo $em['pk_cod_turma'] ?>]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcao) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
									<input type="hidden" name="taxa_brasil[<?php echo $em['pk_cod_turma'] ?>]" value="<?php echo $dados_distorcao['taxaDistorcaoBrasil']['Ensino Fundamental'] ?>" /> 
								</td>
							</tr>
						<?php $n++;endforeach; ?>
					</table>	
				</td>
			</tr>
		<?php endif; ?>
		<?php if($arrTurmas['Ensino Médio']): ?>
			<tr id="tr_censo_em" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Ensino Médio</td>
				<td>
					<table class="listagem" width="100%" cellSpacing="1" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2012</td>
							</tr>
							<tr>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de distorções</td>
								<td class="bold center" >Taxa de distorção idade-série (em %)</td>
								<td class="bold center" >Selecione as turmas cuja taxa seja<br /> superior à média do Brasil (<?php echo $dados_distorcao['taxaDistorcaoBrasil']['Ensino Médio'] ?>%)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Ensino Médio'] as $em): ?>
							<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" ><?php echo campo_texto("num_distorcao[".$em['pk_cod_turma']."]","S","S","Número de Distorção","10","20","[.###]","","","","","","calculataxaDistorcao('{$em['pk_cod_turma']}',this.value,'{$em['nummatricula']}')",$arrDistorcaoTaxa[$em['pk_cod_turma']]['distorcao']) ?></td>
								<td class="center" id="taxa_distorcao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa[$em['pk_cod_turma']]['taxa'] ? $arrDistorcaoTaxa[$em['pk_cod_turma']]['taxa']." %" : "" ?></td>
								<td class="center" >
									<input type="checkbox" onclick="verificaTaxaDistorcao('<?php echo $em['pk_cod_turma'] ?>','<?php echo $dados_distorcao['taxaDistorcaoBrasil']['Ensino Médio'] ?>')" name="chk_turma[<?php echo $em['pk_cod_turma'] ?>]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcao) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
								</td>
							</tr>
						<?php $n++;endforeach; ?>
					</table>
				</td>
			</tr>
		<?php endif; ?>
		<?php if($arrTurmas['Educação Infantil'] && 1==2): ?>
			<tr id="tr_censo_ei" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Educação Infantil</td>
				<td>
					<table width="100%" class="listagem" cellSpacing="1" cellPadding="3" align="center">
						<thead>
							<tr>
								<td class="bold center" colspan="7">Ano de Referência: 2012</td>
							</tr>
							<tr>
								<td class="bold center" >Série / Ano</td>
								<td class="bold center" >Turma(s)</td>
								<td class="bold center" >Horário(s)</td>
								<td class="bold center" >Nº de matrículas</td>
								<td class="bold center" >Nº de distorções</td>
								<td class="bold center" >Taxa de distorção idade-série (em %)</td>
								<td class="bold center" >Selecione as turmas cuja taxa seja<br /> superior à média do Brasil (<?php echo $dados_distorcao['taxaDistorcaoBrasil']['Educação Infantil'] ?>%)</td>
							</tr>
						</thead>				
						<?php $n=1;foreach($arrTurmas['Educação Infantil'] as $em): ?>
							<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
							<tr bgcolor="<?php echo $cor ?>" >
								<td class="center" ><?php echo $em['serie'] ?></td>
								<td class="center" ><?php echo $em['turma'] ?></td>
								<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
								<td class="center" ><?php echo $em['nummatricula'] ?></td>
								<td class="center" ><?php echo campo_texto("num_distorcao[".$em['pk_cod_turma']."]","S","S","Número de Distorção","10","20","[.###]","","","","","","calculataxaDistorcao('{$em['pk_cod_turma']}',this.value,'{$em['nummatricula']}')",$arrDistorcaoTaxa[$em['pk_cod_turma']]['distorcao']) ?></td>
								<td class="center" id="taxa_distorcao_<?php echo $em['pk_cod_turma'] ?>" ><?php echo $arrDistorcaoTaxa[$em['pk_cod_turma']]['taxa'] ? $arrDistorcaoTaxa[$em['pk_cod_turma']]['taxa']." %" : "" ?></td>
								<td class="center" >
									<input type="checkbox" onclick="verificaTaxaDistorcao('<?php echo $em['pk_cod_turma'] ?>','<?php echo $dados_distorcao['taxaDistorcaoBrasil']['Educação Infantil'] ?>')" name="chk_turma[<?php echo $em['pk_cod_turma'] ?>]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcao) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
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
				<?php $sql = "select * from pdeinterativo2013.pergunta where prgmodulo = 'D' and prgsubmodulo = 'D' and prgstatus = 'A'"; ?>
				<?php quadroPerguntas($sql) ?>
			</td>
		</tr>
		<tr id="tr_navegacao" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
			<td>
				<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_1_matriculas'" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarDistorcaoIdadeSerie();">
				<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="salvarDistorcaoIdadeSerie('C');">
				<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_3_aproveitamentoescolar'" >
			</td>
		</tr>
	</table>
</form>