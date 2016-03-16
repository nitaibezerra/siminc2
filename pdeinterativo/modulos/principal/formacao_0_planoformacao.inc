<?
$pdeid = $_SESSION['pdeinterativo_vars']['pdeid'];
monta_titulo( "Plano de Formação", "&nbsp;");
$permissoes = pegaPermissao();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<script>
function escolherCurso( pfdid ){

	window.open( 'pdeinterativo.php?modulo=principal/popUpCursoFormacao&acao=A&pfdid='+pfdid, 
				 'popUpEscolherCurso', 'width=1000,height=400,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	
}

function desvincularCurso( pfdid ){

	if( confirm('Deseja desvincular este curso?') ){

		$('#requisicao').val('desvincularCurso');
		$('#pfdidExcluir').val( pfdid );
		$('#frmListaCursos').submit();
	}
}

function imprimirCurso( curid ){
	if( parseInt(curid) > 0 ){
		return window.open('pdeinterativo.php?modulo=principal/popUpImpressaoCursoFormacao&acao=A&curid='+curid, 
						   'modelo', 
						   "height=600,width=950,scrollbars=yes,top=50,left=200" );
	}
}

function abreDiag52( ){
	return window.open('pdeinterativo.php?modulo=principal/formacaoDiag52&acao=A', 
					   'modelo', 
					   "height=400,width=950,scrollbars=yes,top=50,left=200" );
}

function abreDetalhe( pfdid ){
	return window.open('pdeinterativo.php?modulo=principal/popUpDetalheDocente&acao=A&pfdid='+pfdid, 
					   'modelo', 
					   "height=400,width=950,scrollbars=yes,top=50,left=200" );
}


</script>
<form method="post" name="frmListaCursos" id="frmListaCursos">
	<input type="hidden" name="requisicao" id="requisicao" value=""/>
	<input type="hidden" name="pfdidExcluir" id="pfdidExcluir" value=""/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="10" align="center">
		<tr>
			<td class="blue subtituloDireita" width="10%">Orientações</td>
			<td class="blue" >
				<br><br>Nesta tela, a escola deverá indicar os cursos desejados para cada profissional e o sistema exibirá automaticamente as informações sobre o curso. Atenção: este planejamento definirá apenas a demanda da escola, não se configurando como pré-inscrição e nem o direito à vaga. Esta demanda será submetida à validação do Secretário Municipal ou Estadual e posteriormente ao Planejamento Estratégico do Estado, no âmbito do Fórum Estadual Permanente de Apoio à Formação Docente.
				<br><br>Observe que os nomes dos docentes e da equipe gestora foram obtidos a 
				partir das respostas dadas no Diagnóstico. Caso deseje excluir algum docente, retorne à tela "5.2 Docentes" no Diagnóstico, assinale a opção 
				"Não" na pergunta relativa à formação docente e clique em "Salvar". 
				<br><br>A escolha do curso deverá observar os seguintes requisitos:
				<br><br>1. Necessidades da escola em consonância com o  seu Projeto Político Pedagógico;
				<br>2. Áreas do conhecimento ministradas pelo docente;
				<br>3. Áreas críticas identificadas no Diagnóstico. <a style="cursor:pointer;" onclick="abreDiag52( )">Clique aqui</a> para visualizar as disciplinas críticas.
				<br>4. Formação acadêmica e experiência;
				<br>5. Expectativas pessoais, disposição e disponibilidade do profissional.
				<br><br>O Plano de Formação deve reunir propostas para os próximos quatro anos, lembrando que o ano indicado é aquele em que o profissional se dispõe a estar cursando as aulas. Mas lembre-se: ao solicitar atendimento em determinado ano, não significa que este ocorrerá necessariamente naquele exercício, em função da capacidade de atendimento das universidades. Anualmente, a escola poderá ajustar o seu planejamento, à medida em que os cursistas forem matriculados, novos profissionais sejam incorporados à equipe e novos cursos sejam ofertados. 
				<br><br>Cada professor poderá ser indicado somente para um curso, mesmo que lecione em mais de
				uma escola. Assim, caso ocorra de um professor já ter sido selecionado para um curso por uma
				outra escola e a escola avalie que o mesmo devería fazer outro curso, será necessário solicitar
				à primeira escola que cancele esta solicitação. Para isso, a primeira escola deverá marcar a
				opção 'não há curso de interesse', permitindo que o professor seja novamente disponibilizado
				para seleção de curso por outro estabelecimento de ensino
			</td>
			<td>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Relação de Profissionais</td>
			<td colspan="2">
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
									<td class="subtituloDireita" style="text-align:center" ><b>Curso</b></td>
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
									
								foreach($arrDiretoria as $k => $diretoria){ ?>
									<tr>
										<td style="text-align:center" >
											<?=$diretoria['nome'];?>
											<b><?=$diretoria['tpedesc'];?></b><br>
											<a style="cursor:pointer" onclick="abreDetalhe( <?=$diretoria['pfdid'] ?> )"> 
											Detalhe do profissional
											</a>
											<input type="hidden" name="pfdid_dir[]" value="<?=$diretoria['pfdid'] ?>"/>
										</td>
										<td style="text-align:center" >
											<?php 
												if( $diretoria['pfdnhainteresse'] != 't' && $diretoria['pfdnhacurso'] != 't' ){
													if( $permissoes['gravar'] ){
											?>
											<img border="0" align="top" src="../imagens/consultar.gif" style="cursor:pointer;" onclick="escolherCurso(<?=$diretoria['pfdid'] ?>)">
											<?php   }else{?>
											<img border="0" align="top" src="../imagens/consultar_01.gif" >
											<?php   }?>
											<?php 
												}elseif( $diretoria['pfdnhacurso'] == 't' ){
													echo "Não há curso disponível. <br>";
												}else{
													echo "Não há curso de interesse. <br>";
												}
												if( ($diretoria['curid'] != '' || $diretoria['pfdnhainteresse'] == 't' || $diretoria['pfdnhacurso'] == 't') && $permissoes['gravar'] ){
											?>
											<img border="0" align="top" src="../imagens/excluir.gif" style="cursor:pointer;" onclick="desvincularCurso(<?=$diretoria['pfdid'] ?>)">
											<?php 
												}
											?>
										</td>
										<td style="text-align:center" ><?=$diretoria['atedesc'] ?></td>
										<td style="text-align:center" >
											<?php if( possuiCurso($diretoria['pfdcpf'])&&!$diretoria['curid'] ){ ?>
												<label style="color:red"> <b>Docente possui curso cadastrado em outra escola.</b> </label><br>
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
									<td class="subtituloDireita" style="text-align:center" width="5%"><b>Curso</b></td>
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
											</a><input type="hidden" name="pfdid[]" value="<?=$docente['pfdid'] ?>"/></td>
										<td style="text-align:center" >
											<?php 
												if( $docente['pfdnhainteresse'] != 't' && $docente['pfdnhacurso'] != 't' ){
													if( $permissoes['gravar'] ){
											?>
											<img border="0" style="cursor:pointer;" align="top" src="../imagens/consultar.gif" onclick="escolherCurso(<?=$docente['pfdid'] ?>)">
											<?php   }else{?>
											<img border="0" align="top" src="../imagens/consultar_01.gif">
											<?php 
													}
												}elseif( $docente['pfdnhacurso'] == 't' ){
													echo "Não há curso disponível. <br>";
												}else{
													echo "Não há curso de interesse. <br>";
												}
												if( ($docente['curid'] != '' || $docente['pfdnhainteresse'] == 't' || $docente['pfdnhacurso'] == 't') && $permissoes['gravar'] ){
											?>
											<img border="0" style="cursor:pointer;" align="top" src="../imagens/excluir.gif" onclick="desvincularCurso(<?=$docente['pfdid'] ?>)">
											<?php 
												}
											?>
										</td>
										<td style="text-align:center" ><?=$docente['atedesc'] ?></td>
										<td style="text-align:center" >
											<?php if( $docente['pfdcpf'] == '' ){?>
											<?php echo $docente['pfdcpf'];?>
											<?php }?>
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
									<td class="subtituloDireita" style="text-align:center" width="5%"><b>Curso</b></td>
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
											</a><br><?=$docente['tipo_docente'] ?><input type="hidden" name="pfdid[]" value="<?=$docente['pfdid'] ?>"/></td>
										<td style="text-align:center" >
											<?php 
												if( $docente['pfdnhainteresse'] != 't' && $docente['pfdnhacurso'] != 't' ){
													if( $permissoes['gravar'] ){
											?>
											<img border="0" style="cursor:pointer;" align="top" src="../imagens/consultar.gif" onclick="escolherCurso(<?=$docente['pfdid'] ?>)">
											<?php   }else{?>
											<img border="0" align="top" src="../imagens/consultar_01.gif">
											<?php 
													}
												}elseif( $docente['pfdnhacurso'] == 't' ){
													echo "Não há curso disponível. <br>";
												}else{
													echo "Não há curso de interesse. <br>";
												}
												if( ($docente['curid'] != '' || $docente['pfdnhainteresse'] == 't' || $docente['pfdnhacurso'] == 't') && $permissoes['gravar'] ){
											?>
											<img border="0" style="cursor:pointer;" align="top" src="../imagens/excluir.gif" onclick="desvincularCurso(<?=$docente['pfdid'] ?>)">
											<?php 
												}
											?>
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
	</table>
</form>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	<tr>
		<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
		<td>
			<input type="button" name="anterior"   value="Anterior" onclick="divCarregando();window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_0_orientacoes';">
<!--			<input type="button" name="salvar"     value="Salvar" >-->
<!--			<input type="button" name="salvarCont" value="Salvar e Continuar">-->
			<input type="button" name="proximo"    value="Próximo"  onclick="divCarregando();window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=formacao_0_planoformacao&aba1=formacao_2_relacaoprioridade';">
		</td>
	</tr>
</table>