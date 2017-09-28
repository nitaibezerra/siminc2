<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script>
			function exibeTurma(id)
			{
				jQuery("#" + id).show();
				jQuery( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
			}
		</script>
		<style>
			.PopUphidden{display:none;width:500px;height:300px;position:absolute;z-index:0;top:50%;left:50%;margin-top:-150;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;}
		</style>
	</head>
	<body>
		<form method="post" name="frmCursos" id="frmCursos">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="10" cellPadding="10" align="center">
				<tr bgcolor="#FFFFFF" >
					<?php $arrTurmas = recuperaTurmasCriticasPorEscola();?>
					<?php $arrTaxaReprovacaoBrasil = carregaTaxa(); ?>
					<?php $arrDisciplinas = retornaDisciplinasTurma(); ?>
					<?php $arrDistorcaoTaxaReprovacaoDisciplina = carregaDistorcaoTaxaReprovacaoDisciplina(); ?>
					<?php $arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxaEscolar(); ?>
					<td valign="top" >
						<table cellSpacing="1" cellPadding="3" width="100%" >
							<tr>
								<td><b>Áreas de conhecimento</b></td>
								<td><b>Problema(s) Crítico(s) Selecionado(s) na Aba '2.5 Sintese da Dimensão' do Diagnóstico</b></td>
							</tr>	
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
												<input disabled type="checkbox" <?php echo verificaCheckBoxDisciplina($disciplina,$arrTurmas) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[disciplina][<?php echo $disciplina ?>][<?php echo count($arrTurmas) ?>]" value="<?php echo implode(",",$arrTurmas) ?>" />
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
												<input disabled type="checkbox" <?php echo verificaCheckBoxDisciplina($disciplina,$arrTurmas) ? "checked='checked'" : "" ?> onclick="verificaCheckBox(this)" name="chk_problemas[disciplina][<?php echo $disciplina ?>][<?php echo count($arrTurmas) ?>]" value="<?php echo implode(",",$arrTurmas) ?>" />
												<input type="hidden" name="arrTurmasDisciplina[]" value="<?php echo implode(",",$arrTurmas) ?>" />
											</td>
										</tr>
									<?php $x++;endforeach; ?>
								<?php endif; ?>
							<?php endif; ?>
						</table>
					</td>
				</tr>
			</table>
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
		</form>
	</body>
</html>