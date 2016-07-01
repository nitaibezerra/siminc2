<?
monta_titulo( "Matrícula - Distorção e aproveitamento", "&nbsp;");

$arrDistorcao = carregaDistorcaoDiagnosticoMatricula(false,"M",null);
$display = $arrDistorcao ? "" : "none";

?>
<script type="text/javascript">
	
	function salvarMatricula(local)
	{
		var numTurmas 	 = jQuery("[name='chk_turma[]']").length;
		var numTurmasSel = jQuery("[name='chk_turma[]']:checked").length;
		if(numTurmasSel != numTurmas){
			alert('Favor selecionar todas as escolas com parâmetro superior aos do CNE!');
		}else{
			jQuery("[name='hdn_redirect']").val(local);
			jQuery("[name='fomr_matricula']").submit();
		}
	}
	
</script>
<form id="fomr_matricula" name="fomr_matricula" method="post" action="" >
	<input type="hidden" name="requisicao" value="salvarDistorcaoDiagnosticoMatricula" />
	<input type="hidden" name="hdn_redirect" value="" />
	<table class="tabela" bgcolor="#f5f5f5"  cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloDireita blue" width="20%">Orientações</td>
			<td class="SubTituloEsquerda blue normal" >
				<p>Vamos iniciar a análise desta dimensão visualizando alguns dados informados no Censo Escolar, com destaque para as turmas e o número de estudantes matriculados em cada turma. Clique no botão abaixo para importar os dados do Censo.</p>
				<p>Em seguida, analise se o tamanho das turmas é o mais adequado para cada etapa oferecida ou se há excesso de estudantes numa mesma turma. Para auxiliar nesta tarefa, utilizaremos como referência os parâmetros sugeridos pelo Conselho Nacional de Educação (CNE), indicados na tabela abaixo. Esses parâmetros não são rígidos, mas representam uma referência importante para dimensionar e organizar as turmas.</p>
				<p>Para continuar o diagnóstico, selecione as turmas que possuem número de estudantes matriculados superior aos parâmetros do CNE. E lembre-se: os valores constantes nesta tabela não poderão ser alterados e os seus conteúdos serão utilizados nas próximas análises. Caso discorde das informações, verifique os dados enviados pela escola para o INEP.</p>
				<?php if(!$arrDistorcao): ?>
					<p><input type="button" name="btncenso" class="blue bold" value="CLIQUE AQUI para visualizar os dados do Censo Escolar" onclick="exibeTRs()"></p>
				<?php endif; ?>	
			</td>
		</tr>
		<?php $arrCNE = recuperaParametroCNE() ?>
		<?php $arrTurmas = recuperaTurmasPorEscola() ?>
		<?php if($arrTurmas['Ensino Fundamental']): ?>
			<tr id="tr_censo_ef" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Ensino Fundamental</td>
				<td>
					<table width="100%" >
						<tr valign="top" >
							<td width="50%">
								<table class="listagem" cellSpacing="1" cellPadding="3" align="center">
									<thead>
										<tr>
											<td class="bold center" colspan="5">Ano de Referência: 2010</td>
										</tr>
										<tr>
											<td class="bold center" >Selecione as turmas com <br />nº de matrículas superior <br />ao parâmetro do CNE</td>
											<td class="bold center" >Série / Ano</td>
											<td class="bold center" >Turma(s)</td>
											<td class="bold center" >Horário(s)</td>
											<td class="bold center" >Nº de matrículas</td>
										</tr>
									</thead>				
									<?php $n=1;foreach($arrTurmas['Ensino Fundamental'] as $em): ?>
										<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
										<tr bgcolor="<?php echo $cor ?>" >
											<td class="center" >
												<? $parametroCNE = retornaparametroCNE($em['ensino'], $em['serie'],$em['nummatricula'],$em['localizacao']); ?>
												<?php if($parametroCNE == true && is_bool($parametroCNE)): ?>
													<input type="checkbox" name="chk_turma[]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcao) ? "checked=checked" : "" ?>  value="<?php echo $em['pk_cod_turma'] ?>" />
												<?php else: ?>
													<?php  $msg = !is_bool($parametroCNE) ? $parametroCNE : "A quantidade de estudantes desta turma é inferior ou igual ao parâmetro do CNE." ?>
													<div style="width:100%" onMouseOver="return escape('<?php echo $msg ?>')" class="link center" ><input type="checkbox" disabled="disabled" name="chk_turma_off[]" /></div>
												<?php endif; ?>
											</td>
											<td class="center" ><?php echo $em['serie'] ?></td>
											<td class="center" ><?php echo $em['turma'] ?></td>
											<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
											<td class="center" ><?php echo $em['nummatricula'] ?></td>
										</tr>
									<?php $n++;endforeach; ?>
								</table>
							</td>
							<td valign="top" >
								<?php if($arrCNE): ?>
									<table class="listagem" cellSpacing="1" cellPadding="3" align="center">
										<thead>
											<tr>
												<td class="bold center" colspan="3">Parâmetros CNE</td>
											</tr>
											<tr>
												<td class="bold center" >Localização</td>
												<td class="bold center" >Etapa</td>
												<td class="bold center" >Estudantes por <br />turma</td>
											</tr>
										</thead>				
										<?php $n=1;foreach($arrCNE as $cne): ?>
											<?php if(strstr($cne['cnedesc'],"Escola do campo")){
												$localizacao="Rural";
												$corDestaque = "#228B22;font-weight:bold";
											}else{
												$localizacao="Urbana";
												$corDestaque = "";
											}
											?>

											<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
											<tr bgcolor="<?php echo $cor ?>" >
												<td><span style="color:<?php echo $corDestaque ?>" ><?php echo $localizacao ?></span></td>
												<td><span style="color:<?php echo $corDestaque ?>" ><?php echo $cne['cnedesc'] ?></span></td>
												<td><span style="color:<?php echo $corDestaque ?>" ><?php echo $cne['cnenumestturm'] ?></span></td>
											</tr>
										<?php $n++;endforeach; ?>
									</table>
								<?php endif; ?>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		<?php endif; ?>
		<?php if($arrTurmas['Ensino Médio']): ?>
			<tr id="tr_censo_em" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Ensino Médio</td>
				<td>
					<table width="100%" >
						<tr valign="top" >
							<td width="50%">
								<table class="listagem" cellSpacing="1" cellPadding="3" align="center">
									<thead>
										<tr>
											<td class="bold center" colspan="5">Ano de Referência: 2010</td>
										</tr>
										<tr>
											<td class="bold center" >Selecione as turmas com <br />nº de matrículas superior <br />ao parâmetro do CNE</td>
											<td class="bold center" >Série / Ano</td>
											<td class="bold center" >Turma(s)</td>
											<td class="bold center" >Horário(s)</td>
											<td class="bold center" >Nº de matrículas</td>
										</tr>
									</thead>				
									<?php $n=1;foreach($arrTurmas['Ensino Médio'] as $em): ?>
										<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
										<tr bgcolor="<?php echo $cor ?>" >
											<td class="center" >
												<? $parametroCNE = retornaparametroCNE($em['ensino'], $em['serie'],$em['nummatricula'],$em['localizacao']) ?>
												<?php if($parametroCNE == true && is_bool($parametroCNE)): ?>
													<input type="checkbox" name="chk_turma[]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcao) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
												<?php else: ?>
													<?php  $msg = !is_bool($parametroCNE) ? $parametroCNE : "A quantidade de estudantes desta turma é inferior ou igual ao parâmetro do CNE." ?>
													<div style="width:100%" onMouseOver="return escape('<?php echo $msg ?>')" class="link center" ><input type="checkbox" disabled="disabled" name="chk_turma_off[]" /></div>
												<?php endif; ?>
											</td>
											<td class="center" ><?php echo $em['serie'] ?></td>
											<td class="center" ><?php echo $em['turma'] ?></td>
											<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
											<td class="center" ><?php echo $em['nummatricula'] ?></td>
										</tr>
									<?php $n++;endforeach; ?>
								</table>
							</td>
							<td valign="top" >
								<?php if($arrCNE): ?>
									<table class="listagem" cellSpacing="1" cellPadding="3" align="center">
										<thead>
											<tr>
												<td class="bold center" colspan="2">Parâmetros CNE</td>
											</tr>
											<tr>
												<td class="bold center" >Localização</td>
												<td class="bold center" >Etapa</td>
												<td class="bold center" >Estudantes por <br />turma</td>
											</tr>
										</thead>				
										<?php $n=1;foreach($arrCNE as $cne): ?>
										
											<?php if(strstr($cne['cnedesc'],"Escola do campo")){
												$localizacao="Rural";
												$corDestaque = "#228B22;font-weight:bold";
											}else{
												$localizacao="Urbana";
												$corDestaque = "";
											}
											?>
											<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
											<tr bgcolor="<?php echo $cor ?>" >
												<td><span style="color:<?php echo $corDestaque ?>" ><?php echo $localizacao ?></span></td>
												<td><span style="color:<?php echo $corDestaque ?>" ><?php echo $cne['cnedesc'] ?></span></td>
												<td><span style="color:<?php echo $corDestaque ?>" ><?php echo $cne['cnenumestturm'] ?></span></td>
											</tr>
										<?php $n++;endforeach; ?>
									</table>
								<?php endif; ?>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		<?php endif; ?>
		<?php if($arrTurmas['Educação Infantil']): ?>
			<tr id="tr_censo_ei" style="display:<?php echo $display ?>" >
				<td class="SubTituloDireita" width="20%">Educação Infantil</td>
				<td>
					<table width="100%" >
						<tr valign="top" >
							<td width="50%">
								<table class="listagem" cellSpacing="1" cellPadding="3" align="center">
									<thead>
										<tr>
											<td class="bold center" colspan="5">Ano de Referência: 2010</td>
										</tr>
										<tr>
											<td class="bold center" >Selecione as turmas com <br />nº de matrículas superior <br />ao parâmetro do CNE</td>
											<td class="bold center" >Série / Ano</td>
											<td class="bold center" >Turma(s)</td>
											<td class="bold center" >Horário(s)</td>
											<td class="bold center" >Nº de matrículas</td>
										</tr>
									</thead>				
									<?php $n=1;foreach($arrTurmas['Educação Infantil'] as $em): ?>
										<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
										<tr bgcolor="<?php echo $cor ?>" >
											<td class="center" >
												<? $parametroCNE = retornaparametroCNE($em['ensino'], $em['serie'],$em['nummatricula'],$em['localizacao']) ?>
												<?php if($parametroCNE == true && is_bool($parametroCNE)): ?>
													<input type="checkbox" name="chk_turma[]" <?php echo in_array($em['pk_cod_turma'],$arrDistorcao) ? "checked=checked" : "" ?> value="<?php echo $em['pk_cod_turma'] ?>" />
												<?php else: ?>
													<?php  $msg = !is_bool($parametroCNE) ? $parametroCNE : "A quantidade de estudantes desta turma é inferior ou igual ao parâmetro do CNE." ?>
													<div style="width:100%" onMouseOver="return escape('<?php echo $msg ?>')" class="link center" ><input type="checkbox" disabled="disabled" name="chk_turma_off[]" /></div>
												<?php endif; ?>
											</td>
											<td class="center" ><?php echo $em['serie'] ?></td>
											<td class="center" ><?php echo $em['turma'] ?></td>
											<td class="center" ><span style="white-space:nowrap;" ><?php echo $em['hrinicio'] ?> - <?php echo $em['hrfim'] ?></span></td>
											<td class="center" ><?php echo $em['nummatricula'] ?></td>
										</tr>
									<?php $n++;endforeach; ?>
								</table>
							</td>
							<td valign="top" >
								<?php if($arrCNE): ?>
									<table class="listagem" cellSpacing="1" cellPadding="3" align="center">
										<thead>
											<tr>
												<td class="bold center" colspan="3">Parâmetros CNE</td>
											</tr>
											<tr>
												<td class="bold center" >Localização</td>
												<td class="bold center" >Etapa</td>
												<td class="bold center" >Estudantes por <br />turma</td>
											</tr>
										</thead>				
										<?php $n=1;foreach($arrCNE as $cne): ?>
										
											<?php if(strstr($cne['cnedesc'],"Escola do campo")){
												$localizacao="Rural";
												$corDestaque = "#228B22;font-weight:bold";
											}else{
												$localizacao="Urbana";
												$corDestaque = "";
											}
											?>

											<?php $cor = $n%2==0 ? "#FFFFFF" : "" ?>
											<tr bgcolor="<?php echo $cor ?>" >
												<td><span style="color:<?php echo $corDestaque ?>" ><?php echo $localizacao ?></span></td>
												<td><span style="color:<?php echo $corDestaque ?>" ><?php echo $cne['cnedesc'] ?></span></td>
												<td><span style="color:<?php echo $corDestaque ?>" ><?php echo $cne['cnenumestturm'] ?></span></td>
											</tr>
										<?php $n++;endforeach; ?>
									</table>
								<?php endif; ?>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		<?php endif; ?>
		<tr id="tr_navegacao" style="display:<?php echo $display ?>" >
			<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
			<td>
				<input type="button" name="btn_anterior" value="Anterior" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_0_orientacoes'" >
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarMatricula();">
				<input type="button" name="btn_salvar_continuar" value="Salvar e Continuar" onclick="salvarMatricula('C');">
				<input type="button" name="btn_proximo" value="Próximo" onclick="window.location='pdeinterativo.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_2_distorcaoeaproveitamento&aba1=diagnostico_2_2_distorcaoidadeserie'" >
			</td>
		</tr>
	</table>
</form>