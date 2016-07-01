<?
monta_titulo( "Visualização do Plano de Formação Continuada", "&nbsp;");
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<script>
function imprimirCurso( curid ){
	if( parseInt(curid) > 0 ){
		return window.open('pdeinterativo.php?modulo=principal/popUpImpressaoCursoFormacao&acao=A&curid='+curid, 
						   'modelo', 
						   "height=600,width=950,scrollbars=yes,top=50,left=200" );
	}
}

function abreDetalhe( pfdid ){
	return window.open('pdeinterativo.php?modulo=principal/popUpDetalheDocente&acao=A&pfdid='+pfdid, 
					   'modelo', 
					   "height=400,width=950,scrollbars=yes,top=50,left=200" );
}


</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="10" align="center">
	<tr>
		<td class="blue subtituloDireita" width="10%">Orientações</td>
		<td class="blue" colspan="2">
			Parabéns! Você concluiu o Plano de Formação da sua escola e pode visualizar, nesta aba, o resumo de seu planejamento. Releia com atenção os profissionais indicados, cursos e períodos. Agora  você pode enviar o Plano de Formação para a Secretaria de Educação.
			<br><br>ATENÇÃO: Após o envio, o Plano de Formação não pode ser revisto e nem devolvido pela Secretaria. 
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita" rowspan="8"></td>
		<td colspan="2" class="SubTituloCentro">Proposta da Escola</td>
	</tr>
	<tr>
		<td colspan="2" >
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td style="text-align:center" colspan="10"><b>Relação de Profissionais</b></td>
				</tr>
				<tr>
					<td class="subtituloDireita" style="text-align:center" ><b>Diretoria e Equipe Pedagógica</b></td>
				</tr>
				<tr id="tr_diretoria">
					<td>
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td class="subtituloDireita" style="text-align:center" ><b>Nome</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Área Temática</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Nome do Curso</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Período</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Etapa <br>de Ensino que<br> se destina</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Nivel<br>do Curso</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Modalidade<br> de Ensino</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Carga Horária<br>Total do Curso<br>Min/Máx</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Carga Horária <br>Presencial Exigida<br>Min/Máx(%)</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Situação</b></td>
							</tr>
							<?
							$arrDiretoria = recuperaDiretoriaEquipe();
							$arrDiretoria = is_array($arrDiretoria) ? $arrDiretoria : array();
							/* teste commit */
							foreach($arrDiretoria as $k => $diretoria){ ?>
								<tr>
									<td style="text-align:center" >
										<?=$diretoria['nome'];?>
										<br>
										<b><?=$diretoria['tpedesc'];?></b><br>
										<a style="cursor:pointer" onclick="abreDetalhe( <?=$diretoria['pfdid'] ?> )"> 
										Detalhe do profissional
										</a>
										<input type="hidden" name="pfdid_dir[]" value="<?=$diretoria['pfdid'] ?>"/>
									</td>
									<td style="text-align:center" ><?=$diretoria['atedesc'] ?></td>
										<td style="text-align:center" >
											<?php if( possuiCurso($diretoria['pfdcpf'])&&!$diretoria['curid'] ){ ?>
												<label style="color:red"> <b>Docente possui curso cadastrado.</b> </label><br>
											<?php }else{ ?>
												<a style="cursor:pointer;" onclick="imprimirCurso( <?=$diretoria['curid'] ?> )"><?=$diretoria['curdesc'] ?></a>
											<?php }?>
										</td>
										<td style="text-align:center" ><?=$diretoria['pcfdesc'] ?></td>
									<td style="text-align:center" >
										<?php 
											if( $diretoria['curid'] != '' ){
										?>
										<img border="0" style="cursor:pointer;" align="top" src="../imagens/info.gif" 
											 onmouseover="SuperTitleAjax('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=toolEtapaCurso&curid=<?=$diretoria['curid'] ?>')" 
									     	 onmouseout="SuperTitleOff( this )">
										<?php 
											}
										?>
									</td>
									<td style="text-align:center" ><?=$diretoria['ncudesc'] ?></td>
									<td style="text-align:center" ><?=$diretoria['moddesc'] ?></td>
									<td style="text-align:center" ><?=$diretoria['curch'] ?></td>
									<td style="text-align:center" ><?=$diretoria['curpercpre'] ?></td>
									<td style="text-align:center" ><?=$diretoria['situacao'] ?></td>
								</tr>
							<?} ?>
						</table>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita" style="text-align:center" ><b>Docentes</b></td>
				</tr>
				<tr id="tr_docentes">
					<td>
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td class="subtituloDireita" style="text-align:center" ><b>Nome</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Área Temática</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Nome do Curso</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Período</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Etapa <br>de Ensino que<br> se destina</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Nivel<br>do Curso</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Modalidade<br> de Ensino</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Carga Horária<br>Total do Curso<br>Min/Máx</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Carga Horária <br>Presencial Exigida<br>Min/Máx(%)</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Situação</b></td>
							</tr>
							<?php 
							
								$docentes = recuperaDocentes();
								$docentes = is_array($docentes) ? $docentes : Array();
								
								foreach($docentes as $k => $docente){
							?>
								<tr>
									<td style="text-align:center" >
										<?=$docente['no_docente'] ?><br>
										<a style="cursor:pointer" onclick="abreDetalhe( <?=$docente['pfdid'] ?> )"> 
										Detalhe do profissional
										</a><input type="hidden" name="pfdid[]" value="<?=$docente['pfdid'] ?>"/>
									</td>
									<td style="text-align:center" ><?=$docente['atedesc'] ?></td>
										<td style="text-align:center" >
											<?php if( possuiCurso($docente['pfdcpf'])&&!$docente['curid'] ){ ?>
												<label style="color:red"> <b>Docente possui curso cadastrado.</b> </label><br>
											<?php }else{ ?>
												<a style="cursor:pointer;" onclick="imprimirCurso( <?=$docente['curid'] ?> )"><?=$docente['curdesc'] ?></a>
											<?php }?>
										</td>
										<td style="text-align:center" ><?=$docente['pcfdesc'] ?></td>
									<td style="text-align:center" >
										<?php 
											if( $docente['curid'] != '' ){
										?>
										<img border="0" style="cursor:pointer;" align="top" src="../imagens/info.gif" 
											 onmouseover="SuperTitleAjax('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=toolEtapaCurso&curid=<?=$docente['curid'] ?>')" 
									     	 onmouseout="SuperTitleOff( this )">
										<?php 
											}
										?>
									</td>
									<td style="text-align:center" ><?=$docente['ncudesc'] ?></td>
									<td style="text-align:center" ><?=$docente['moddesc'] ?></td>
									<td style="text-align:center" ><?=$docente['curch'] ?></td>
									<td style="text-align:center" ><?=$docente['curpercpre'] ?></td>
									<td style="text-align:center" ><?=$docente['situacao'] ?></td>
								</tr>
							<?php 
								}
							?>
						</table>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita" style="text-align:center" ><b>Auxiliar de Educação Infantil, Monitor de Atividade Complementar e Interprete de Libras </b></td>
				</tr>
				<tr id="tr_docentes">
					<td>
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td class="subtituloDireita" style="text-align:center" ><b>Nome</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Área Temática</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Nome do Curso</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Período</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Etapa <br>de Ensino que<br> se destina</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Nivel<br>do Curso</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Modalidade<br> de Ensino</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Carga Horária<br>Total do Curso<br>Min/Máx</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Carga Horária <br>Presencial Exigida<br>Min/Máx(%)</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Situação</b></td>
							</tr>
							<?php 
							
								$docentes = recuperaAuxiliares();
								$docentes = is_array($docentes) ? $docentes : Array();
								
								foreach($docentes as $k => $docente){
							?>
								<tr>
									<td style="text-align:center" >
											<?=$docente['no_docente'] ?><br>
										<a style="cursor:pointer" onclick="abreDetalhe( <?=$docente['pfdid'] ?> )">  
										Detalhe do profissional
										</a><br><?=$docente['tipo_docente'] ?><input type="hidden" name="pfdid[]" value="<?=$docente['pfdid'] ?>"/>
									</td>
									<td style="text-align:center" ><?=$docente['atedesc'] ?></td>
										<td style="text-align:center" >
											<?php if( possuiCurso($docente['pfdcpf'])&&!$docente['curid'] ){ ?>
												<label style="color:red"> <b>Docente possui curso cadastrado.</b> </label><br>
											<?php }else{ ?>
												<a style="cursor:pointer;" onclick="imprimirCurso( <?=$docente['curid'] ?> )"><?=$docente['curdesc'] ?></a>
											<?php }?>
										</td>
										<td style="text-align:center" ><?=$docente['pcfdesc'] ?></td>
									<td style="text-align:center" >
										<?php 
											if( $docente['curid'] != '' ){
										?>
										<img border="0" style="cursor:pointer;" align="top" src="../imagens/info.gif" 
											 onmouseover="SuperTitleAjax('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=toolEtapaCurso&curid=<?=$docente['curid'] ?>')" 
									     	 onmouseout="SuperTitleOff( this )">
										<?php 
											}
										?>
									</td>
									<td style="text-align:center" ><?=$docente['ncudesc'] ?></td>
									<td style="text-align:center" ><?=$docente['moddesc'] ?></td>
									<td style="text-align:center" ><?=$docente['curch'] ?></td>
									<td style="text-align:center" ><?=$docente['curpercpre'] ?></td>
									<td style="text-align:center" ><?=$docente['situacao'] ?></td>
								</tr>
							<?php 
								}
							?>
						</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloCentro">Ordem de Prioridade</td>
	</tr>
	<tr>
		<td colspan="2" >
			<?
			$sql = "SELECT DISTINCT
						cur.curid,
						cur.curdesc,
					    moc.moddesc,
					    moc.modid
					FROM
						catalogocurso.curso cur
					    inner join pdeinterativo.planoformacaodocente pf on pf.curid = cur.curid
					    inner join catalogocurso.modalidadecurso moc on moc.modid = pf.modid
					WHERE
						pf.pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}";
			$arrCurso = $db->carregar( $sql );
			$arrCurso = $arrCurso ? $arrCurso : array();
			
			$sql = "SELECT DISTINCT
						pe.pcfid,
					    pe.pcfdesc
					FROM
					    pdeinterativo.planoformacaodocente pf
					    INNER JOIN pdeinterativo.periodocursoformacao  pe ON pe.pcfid  = pf.pcfid
					WHERE
						pf.pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}
						and pe.pcfstatus = 'A'
			        ORDER BY pe.pcfid";
			$arrPeriodo = $db->carregar( $sql );
			$arrPeriodo = $arrPeriodo ? $arrPeriodo : array();
			?>
			<?if( $arrCurso ){ ?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr bgcolor="#dedfde">
					<td style="text-align:center" colspan="10"><b>Relação de Profissionais</b></td>
				</tr>
				<tr>
					<td class="subtituloDireita" style="text-align:center" ><b>Curso</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Modalidade</b></td>
					<?foreach ($arrPeriodo as $periodo) {
						echo '<td class="subtituloDireita" style="text-align:center" ><b>'.$periodo['pcfdesc'].'</b></td>';
					}
					 ?>
				</tr>
				<?
				foreach ($arrCurso as $key => $curso) {
					$key % 2 ? $cor = "#dedfde" : $cor = ""; 
					?>
					<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td style="text-align:center" ><?=$curso['curdesc']; ?></td>
						<td style="text-align:center" ><?=$curso['moddesc']; ?></td>
						<?
						foreach ($arrPeriodo as $periodo) {
							$sql = "SELECT pesnome, pfdprioridade, pfdid FROM (
										SELECT DISTINCT
											pes.pesnome,
											pf.pfdprioridade,
											pf.pfdid
										FROM
											pdeinterativo.pessoa pes
										    INNER JOIN pdeinterativo.planoformacaodocente pf ON pf.pesid  = pes.pesid
										WHERE
											pf.pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}
										    and pf.pcfid = {$periodo['pcfid']}
										    and pf.curid = {$curso['curid']}
										    and pf.modid = {$curso['modid']}
										union all
										SELECT DISTINCT
										    d.no_docente,
										    pf.pfdprioridade,
										    pf.pfdid
										FROM 
										    educacenso_".ANO_CENSO.".tab_docente d 
										INNER JOIN pdeinterativo.planoformacaodocente pf ON pf.pk_cod_docente = d.pk_cod_docente
										INNER JOIN educacenso_".ANO_CENSO.".tab_dado_docencia  dc ON dc.fk_cod_docente = d.pk_cod_docente
										WHERE 
											id_tipo_docente = 1
										    and pf.pcfid ={$periodo['pcfid']}
										    and pf.curid = {$curso['curid']}
										    and pf.modid = {$curso['modid']}
										  	AND pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']}
									) as f
    								ORDER BY 
    									pfdprioridade";
							
							$arNome = $db->carregar( $sql );
							$arNome = $arNome ? $arNome : array();
							
							$valor = '';
							$html = '<td style="text-align:left" >';
							$habilita = 'N';
							if( sizeof($arNome) > 1 ) $habilita = 'S';
							 
							foreach ($arNome as $chave => $nome) {
								$pfdprioridade = $nome['pfdprioridade'] ? $nome['pfdprioridade'] : $chave + 1;
								if( count($arNome) > 1 ){
									$valor .= $pfdprioridade.'° '.' '.$nome['pesnome'].'<br>';
								}else{
									$valor .= '&nbsp;<b>1°</b> - '.' '.$nome['pesnome'].'<br>'; 
								}
							}
							
							$html .= $valor.'</td>';
							echo $html;
						}
						?>
					</tr>		
			<?  } ?>
			</table>
			<?php 
			} else {
				print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
				print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
				print '</table>';
			} ?>
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloCentro">Demanda Social</td>
	</tr>
	<tr>
		<td colspan="2" >
			<?php 
				$cursos = recuperaCursosDemandaSocial();
				$cursos = $cursos ? $cursos : Array();
				
				if( is_array($cursos) && count($cursos) > 0 ){
				foreach( $cursos as $curso ){
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="5" align="center">
				<tr>
					<td class="subtituloDireita" style="text-align:center" colspan="2"><b>Nome do Curso</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Área Temática</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Publico-alvo da demanda social</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Nível do Curso</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Modalidade do curso</b></td>
					<td class="subtituloDireita" style="text-align:center" ><b>Carga horária total</b></td>
				</tr>
				<tr>
					<td class="subtituloDireita" style="text-align:center" colspan="2"><?=$curso['curdesc'] ?></td>
					<td class="subtituloDireita" style="text-align:center" ><?=$curso['atedesc'] ?></td>
					<td class="subtituloDireita" style="text-align:center" >
						<img border="0" style="cursor:pointer;" align="top" src="../imagens/info.gif" 
							 onmouseover="SuperTitleAjax('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=toolPublicoAlvoDemandaSocialCurso&curid=<?=$curso['curid'] ?>')" 
					     	 onmouseout="SuperTitleOff( this )">
						<?php $publicos = recuperaPublicoAlvoDemandaSocialCurso( $curso['curid'] ); ?>
					</td>
					<td class="subtituloDireita" style="text-align:center" ><?=$curso['ncudesc'] ?></td>
					<td class="subtituloDireita" style="text-align:center" >
						<?php 
						
						$modalidades = recuperaModalidadeCurso( $curso['curid'] );
						
						if( is_array($modalidades) ){
							foreach( $modalidades as $modalidade){
								echo $modalidade."<br>";
							}
						}
						?>
					</td>
					<td class="subtituloDireita" style="text-align:center" >De <?=$curso['curchmim'] ?> até <?=$curso['curchmax'] ?></td>
				</tr>
				<?php 
				
				$semestres = pegaSemestresCurso( $curso['curid'] );
				
				if( is_array($semestres) ){
				foreach( $semestres as $semestre ){ 
				?>
				<tr>
					<td  colspan="7">
					<b>Indicados para o <?=$semestre['pcfdesc'] ?></b>
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="5" align="center" id="tabela_<?=$curso['curid']."_".$semestre['pcfid']  ?>" style="width:100%" >
							<tr bgcolor="#e9e9e9">
								<td><b>CPF</b></td>
								<td><b>Nome</b></td>
								<td><b>Publico-alvo da demanda social</b></td>
								<td><b>Modalidade</b></td>
								<td><b>Email</b></td>
								<td><b>Telefone Fixo</b></td>
								<td><b>Telefone Celular</b></td>
							</tr>
						<?php 
						
						$docentes = recuperaProfissionaisCurso( $curso['curid'], $semestre['pcfid'] );
						
						if( is_array($docentes) ){
							foreach( $docentes as $k => $docente ){
						?>
							<tr <?=($k%2==0) ? 'style="background-color:white"' : '' ?> >
								<td style="color: red;"><?=str_replace(',','.',$docente['num_cpf']) ?></td>
								<td style="color: red;"><?=$docente['no_docente'] ?></td>
								<td style="color: red;"><?=$docente['fexdesc'] ?></td>
								<td style="color: red;"><?=$docente['moddesc'] ?></td>
								<td style="color: red;"><?=$docente['pfdemail'] ?></td>
								<td style="color: red;"><?=$docente['pfdtelefone'] ?></td>
								<td style="color: red;"><?=$docente['pfdecelular'] ?></td>
							</tr>
						<?php 
							}
						}
						?>
						<?php 
						
						$membros = recuperaMembrosCurso( $curso['curid'], $semestre['pcfid'] );
						
						if(is_array($membros)){
							foreach( $membros as $k => $membro ){
						?>
							<tr <?=($k%2==0) ? 'style="background-color:white"' : '' ?> >
								<td>
									<?=trim($membro['mdscpf']) ?></td>
								<td>
									<?=$membro['mdsnome'] ?></td>
								<td>
									<?php 
									
									$sql = "SELECT
												paddesc as descricao
											FROM
												catalogocurso.publicoalvodemandasocial
											WHERE
												padid = ".$membro['padid'];
									$paddesc = $db->pegaUm($sql);
									echo $paddesc; 
									?>
								</td>
								<td>
									<?php 
									$sql = "SELECT
												moddesc as descricao
											FROM 
												catalogocurso.modalidadecurso 
											WHERE
												modid = ".$membro['modid'];
									$modalidade = $db->pegaUm($sql);
									echo $modalidade;
									?>
								</td>
								<td><?=$membro['mdsemail'] ?></td>
								<td><?=$membro['mdstelefonefixo'] ?></td>
								<td>
									<?=$membro['mdscelular'] ?>
								</td>
							</tr>
						<?php 
							}
						}
						?>
						</table>
					</td>
				</tr>
				<?php 
				}
				}
				?>
			</table>
			<?php 
				}
				}else{
			?>	
				<center>
					<label style="color:red">Não Possui</label>
				</center>
			<?php 
				}
			?>
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloCentro">Sugestões de Curso</td>
	</tr>
	<tr>
		<td colspan="2" >
			<?
			$arrFormacao = carregaCursoSugerido();
//			$arrFormacao = Array();
			extract($arrFormacao);
			$focformcontinuada = empty($focformcontinuada) ? 'N' : $focformcontinuada;
			
			if( !empty($focid) ){
				$fexid = $db->carregarColuna( "SELECT fex.fexdesc as descricao 
										FROM pdeinterativo.cursosugeridofuncao 	csf 
										    inner join catalogocurso.funcaoexercida fex on fex.fexid = csf.fexid
										WHERE
											fex.fexstatus = 'A'
										    and csf.focid = $focid" );
				
				$pk_cod_etapa_ensino = $db->carregarColuna( "SELECT tee.no_etapa_ensino as descricao 
														FROM pdeinterativo.cursosugeridoetapa 	cse 
														    inner join educacenso_".ANO_CENSO.".tab_etapa_ensino tee on tee.pk_cod_etapa_ensino = cse.pk_cod_etapa_ensino
														WHERE
														    cse.focid = $focid" );
				
				$pk_cod_disciplina = $db->carregarColuna( "SELECT tad.no_disciplina as descricao 
													FROM pdeinterativo.cursosugeridodisciplina 	csd 
													    inner join educacenso_".ANO_CENSO.".tab_disciplina tad on tad.pk_cod_disciplina = csd.pk_cod_disciplina
													WHERE
													    csd.focid = $focid" );
				
				$pk_cod_mod_ensino = $db->carregarColuna( "SELECT tme.no_mod_ensino as descricao 
													FROM pdeinterativo.cursosugeridomodalidade csm 
													    inner join educacenso_".ANO_CENSO.".tab_mod_ensino tme on tme.pk_cod_mod_ensino 	= csm.pk_cod_mod_ensino
													WHERE
													    csm.focid = $focid" );
			}
			
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="10" align="center">
				<tr>
					<td class="subtituloDireita" width="15%" align="right">
						A escola deseja sugerir curso de formação continuada ? 
					</td>
					<td>
						<?=($focformcontinuada == 'S' ? 'Sim' : 'Não'); ?> 
					</td>
				</tr>
				<?php if( $focformcontinuada == 'S' ){?>
				<tr>
					<td class="subtituloDireita">
						Nome do Curso
					</td>
					<td colspan="3">
						<?=$foccurso ?>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita" align="right">
						Descrição
					</td>
					<td colspan="3">
						<?=$focdescricao ?>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita" align="right">
						Justificativa e diferença em relação aos cursos já apresentados
					</td>
					<td colspan="3">
						<?=$focjustificativa ?>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita" align="right">
						Público Alvo
					</td>
					<td valign="top">
						Função Exercida:<br>
						<?php	
						if( is_array($fexid) ){
							foreach($fexid as $fex ){
								echo " - ".$fex."<br>";
							}	
						}else{
							echo "<label style=\"color:red\">Não informado</label>";
						}	
						?>
					</td>
					<td valign="top">
						Etapa de ensino em que leciona<br>
						<?php	
						if( is_array($pk_cod_etapa_ensino) ){
							foreach($pk_cod_etapa_ensino as $etapa_ensino ){
								echo " - ".$etapa_ensino."<br>";
							}	
						}else{
							echo "<label style=\"color:red\">Não informado</label>";
						}	
						?>
					</td>
					<td valign="top">
						Disciplina em que leciona<br>
						<?php	
						if( is_array($pk_cod_disciplina) ){
							foreach($pk_cod_disciplina as $disciplina ){
								echo " - ".$disciplina."<br>";
							}	
						}else{
							echo "<label style=\"color:red\">Não informado</label>";
						}	
						?>
					</td>
					<td valign="top">
						Modalidade em que leciona<br>
						<?php	
						if( is_array($pk_cod_mod_ensino) ){
							foreach($pk_cod_mod_ensino as $mod_ensino ){
								echo " - ".$mod_ensino."<br>";
							}	
						}else{
							echo "<label style=\"color:red\">Não informado</label>";
						}	
						?>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita" align="right">
						Quantidade de professores interessados
					</td>
					<td>
						<?=$focqtdprof?>
					</td>
				</tr>
				<?php }?>
			</table>
		</td>
	</tr>
</table>
<?php 

	$docid = pegaDocid( $_SESSION['pdeinterativo_vars']['pdeid'] );
	$pendencias = pegaPendencias($docid);

?>
<?php if( $pendencias != '' ){?>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	<tr>
		<td class="SubTituloDireita" width="20%">Pendências</td>
		<td>
			<label style="color:red"><?=$pendencias ?></label>
		</td>
	</tr>
</table>
<?php }?>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	<tr>
		<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
		<td>
			<?php 
			$docid = pegaDocid( $_SESSION['pdeinterativo_vars']['pdeid'] );
			wf_desenhaBotoesNavegacao( $docid , array( 'pdeid' => $_SESSION['pdeinterativo_vars']['pdeid']));
			?>
			<br>
			<input type="button" name="anterior" value="Anterior" onclick="divCarregando();window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_1_cursosugerido';">
		</td>
	</tr>
</table>