<?php

if ( $_REQUEST["inusituacaoadesao"] ){
	
	$escolaAtiva->aderiProgramaEscolaAtiva( $_REQUEST["inusituacaoadesao"] ); 
	
}

// inclui o cabeçalho padrão do SIMEC
include  APPRAIZ."includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
print "<br/>";

// Monta as abas
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Programa Escola Ativa", "" );

// verifica a Instrumento informada e faz as seguranças necessárias
if ( $_REQUEST["inuid"] || $_SESSION["escolaAtiva"]["inuid"] ){
	
	$_SESSION["escolaAtiva"]["inuid"] = $_REQUEST["inuid"] ? $_REQUEST["inuid"] : $_SESSION["escolaAtiva"]["inuid"];
	$_SESSION["escolaAtiva"]["tipo"]  = $_REQUEST["tipo"]  ? $_REQUEST["tipo"]  : $_SESSION["escolaAtiva"]["tipo"]; 

	if ( !escolaAtivaVerInstrumento( $_SESSION["escolaAtiva"]["inuid"], $_SESSION["escolaAtiva"]["tipo"] ) ){
		
		print "<script>"
			. "    alert('O Instrumento informado não existe ou está em branco!');"
			. "	   history.back(-1);"
			. "</script>";
		
		die;
			
	}
	
	// pega o itrid do instrumento
	$eaItrid 	 = escolaAtivaPegarItrid( $_SESSION["escolaAtiva"]["inuid"] );
	
	// verifica se o instrumento aderiu ao programa
	$eaAderiu    = escolaAtivaVerAderiu( $_SESSION["escolaAtiva"]["inuid"] );
	
	// busca os dados do instrumento
	$dadosEscola = $escolaAtiva->buscaTecnicosProfessores( $_SESSION["escolaAtiva"]["inuid"] );
	
}

$eaTitpoInstrumento = $_SESSION["escolaAtiva"]["tipo"] == "estado" ? "Estado" : "Município";

?>
<style>

#divTextoTermo{
	margin-bottom: 10px; 
	padding: 20px; 
	background: #fff; 
	overflow: auto; 
	height: 300px; 
	border: 1px black solid;
}

#cabecalho{
	text-align: center;
	margin-bottom: 20px;
}
	
#divTermoAdesao{
	text-align: center; 
	margin: 20px; 
	width: 700px;
}
	
#divListaEscolas table {
	width: 100%;                
}
</style>
<form action="" method="post" name="formulario" id="formulario">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td width="100%" valign="top" style="background: none repeat scroll 0% 0%;">
				
				<?php if ( $eaAderiu ){ ?>

					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
						<tr>
							<td class="subtitulodireita" width="190px"><?php print $eaTitpoInstrumento ?>:</td>
							<td>
								<strong><?php print escolaAtivaVerInstrumento( $_SESSION["escolaAtiva"]["inuid"], $_SESSION["escolaAtiva"]["tipo"] ); ?></strong>
							</td>
						</tr>
						<tr>
							<td class="subtitulodireita">Secretário(a) de Educação do Estado:</td>
							<td>
								<?php print campo_texto( "entnome", 'N', 'N', "", "50", "", "", "", "", "", "", "id='entnome'", "", "" ); ?>
								<a onclick="escolaAtivaAbreSecretario();" title="Inserir/editar secretário(a)" style="cursor:pointer;">
									<img src='../imagens/alterar.gif' align="absmiddle"> Inserir 
								</a>
							</td>
						</tr>
						<tr>
							<td colspan="2">
								<br/>
								<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
									<tr align="center">
										<td width="30%" rowspan="2" class="subtitulocentro"> Nome das escolas <img src="../imagens/IconeAjuda.gif"  align="absmiddle" onmouseover="return escape('Selecione apenas escolas multisseriadas que funcionam no campo e que desenvolverão o Programa Escola Ativa em 2010.');"> </td>
										<td width="8%"  rowspan="2" class="subtitulocentro"> Prédios/ Anexo <img src="../imagens/IconeAjuda.gif"  align="absmiddle" onmouseover="return escape('O número de escolas e/ou prédios escolares anexos (escola mãe, filhas ...) implica na definição do quantitativo de Kit pedagógico que deverá ser encaminhado para o município, sendo que será destinado apenas um kit pedagógogico por prédio.');"> </td>
										<td width="8%"  rowspan="2" class="subtitulocentro"> Qtd. Turmas </td>
										<td width="30%" colspan="6" class="subtitulocentro"> Alunos <img src="../imagens/IconeAjuda.gif"  align="absmiddle" onmouseover="return escape('O número de alunos representa o número de cadernos de ensino aprendizagem (livros) necessários para atendimento da demanda do município por série/ano para 2010.');"> </td>
										<td width="4%"  rowspan="2" class="subtitulocentro"> Ação </td>
										<td width="1%"  rowspan="2" class="subtitulocentro"></td>
									</tr>
									<tr align="center">
										<td width="5%" class="subtitulocentro"> 1º ano </td>
										<td width="5%" class="subtitulocentro"> 2º ano </td>
										<td width="5%" class="subtitulocentro"> 3º ano </td>
										<td width="5%" class="subtitulocentro"> 4º ano </td>
										<td width="5%" class="subtitulocentro"> 5º ano </td>
										<td width="5%" class="subtitulocentro"> Total de <br> alunos </td>
									</tr>
									<tr align="center">
										<td width="30%">
											<?php 
											
												$sql = "SELECT 
															ent.entid as codigo,
															entcodent || ' - '|| ent.entnome as descricao
											            FROM 
											            	entidade.entidade ent
											            INNER JOIN 
											            	entidade.endereco d ON ent.entid = d.entid
											            INNER JOIN 
											            	entidade.funcaoentidade fe ON fe.entid = ent.entid
											            INNER JOIN 
											            	territorios.estado te ON te.estuf = d.estuf
											            INNER JOIN
											            	cte.instrumentounidade ci ON ci.estuf = te.estuf
											            WHERE 
											            	ent.entescolanova = false AND 
											            	fe.funid = 3 AND 
											            	ent.tpcid = 1 AND 
											            	ci.inuid = {$_SESSION["escolaAtiva"]["inuid"]}";
												
												$db->monta_combo("entid", $sql, "S", "Selecione...", '', '', '', '300', 'N','entid');
											
											?>
										</td>
										<td width="8%">
											<?php print campo_texto( "eaeqtdpredios", "N", $eaPodeEditar, "", "4", "4", "####", "", "", "", "", "id='eaeqtdpredios'" ); ?>
										</td>
										<td width="8%">
											<?php print campo_texto( "eaeqtdturmas", "N", $eaPodeEditar, "", "4", "4", "####", "", "", "", "", "id='eaeqtdturmas'" ); ?>
										</td>
										<td width="5%">
											<?php print campo_texto( "eaeqtdalunos1", "N", $eaPodeEditar, "", "4", "4", "####", "", "", "", "", "id='eaeqtdalunos1'", "", "", "escolaAtivaAtualizaTotalAlunos(null);" ); ?>
										</td>
										<td width="5%">
											<?php print campo_texto( "eaeqtdalunos2", "N", $eaPodeEditar, "", "4", "4", "####", "", "", "", "", "id='eaeqtdalunos2'", "", "", "escolaAtivaAtualizaTotalAlunos(null);" ); ?>
										</td>
										<td width="5%">
											<?php print campo_texto( "eaeqtdalunos3", "N", $eaPodeEditar, "", "4", "4", "####", "", "", "", "", "id='eaeqtdalunos3'", "", "", "escolaAtivaAtualizaTotalAlunos(null);" ); ?>
										</td>
										<td width="5%">
											<?php print campo_texto( "eaeqtdalunos4", "N", $eaPodeEditar, "", "4", "4", "####", "", "", "", "", "id='eaeqtdalunos4'", "", "", "escolaAtivaAtualizaTotalAlunos(null);" ); ?>
										</td>
										<td width="5%">
											<?php print campo_texto( "eaeqtdalunos5", "N", $eaPodeEditar, "", "4", "4", "####", "", "", "", "", "id='eaeqtdalunos5'", "", "", "escolaAtivaAtualizaTotalAlunos(null);" ); ?>
										</td>
										<td width="5%">
											<?php print campo_texto( "totalAlunos", "N", "N", "", "4", "4", "", "", "", "", "", "id='totalAlunos'" ); ?>
										</td>
										<td width="4%">
											<img src="../imagens/gif_inclui.gif" align="absmiddle" title="Incluir Escola" style="cursor: pointer;" onclick="escolaAtivaIncluiEscola();"/>
											<img src="../imagens/excluir.gif" align="absmiddle" title="Limpar" style="cursor: pointer;" onclick="escolaAtivaLimpaEscola();"/>
										</td>
									</tr>
								</table>
								<?php $escolaAtiva->listaEscolas( $_SESSION["escolaAtiva"]["inuid"] );?>
								<br/>		
							</td>
						</tr>
						<tr>
							<td colspan="2" class="subtitulocentro"> Outras Informações do Estado</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Nº de Técnicos:</td>
							<td>
								<?php
									$esaqtdtecnicos = $dadosEscola["esaqtdtecnicos"]; 
									print campo_texto( "esaqtdtecnicos", 'S', 'N', '', '4', '', '', '', '', '', '', 'id="esaqtdtecnicos"' ); 
								?>
								<img src="../imagens/IconeAjuda.gif"  align="absmiddle" onmouseover="return escape('O número de técnico será definido de acordo com o número de escolas ou prédios escolares, devendo ser informado apenas 01 técnico para cada 25 escolas/prédios escolares, adesas ao Programa Escola Ativa.');">
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Nº de Professores:</td>
							<td>
								<?php 
									$esaqtdprofessores = $dadosEscola["esaqtdprofessores"];
									print campo_texto( "esaqtdprofessores", 'S', $eaPodeEditar, '', '4', '', '', '', '', '', '', 'id="esaqtdprofessores"', '' ); 
								?>
								<img src="../imagens/IconeAjuda.gif"  align="absmiddle" onmouseover="return escape('O número de professores deverá representar o quantitativo de professores necessários para atendimento de todas as turmas multisseriadas adesas ao Programa Escola Ativa.');">
							</td>
						</tr>
					</table>
				
				<?php }else{ ?>
					
					<br/>
							
					<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
						<tr>
							<td class="subtitulodireita" width="190px">Estado:</td>
							<td>
								<strong><?php print escolaAtivaVerInstrumento( $_SESSION["escolaAtiva"]["inuid"], $_SESSION["escolaAtiva"]["tipo"] ); ?></strong>
							</td>
						</tr>
						<tr>
							<td colspan="2" align="center">
								<div id="divTermoAdesao">
									
									<div id="divTextoTermo" >
										<div id="cabecalho">
											<img width="80" height="80" src="/imagens/brasao.gif"/>
											<p>MINISTÉRIO DA EDUCAÇÃO</p>
											<p>SECRETARIA DE EDUCAÇÃO CONTINUADA, ALFABETIZAÇÃO E DIVERSIDADE</p>
											<p>DEPARTAMENTO DE EDUCAÇÃO PARA DIVERSIDADE</p>
											<p>COORDENAÇÃO-GERAL DE EDUCAÇÃO DO CAMPO</p>
											<p style="margin: 20px 0; font-size: 16px;">PROGRAMA ESCOLA ATIVA</p>
										</div>
										<div>
											<p>Senhor <?php echo $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ? "Dirigente Municipal": "Secretário Estadual"  ?> de Educação,</p>
											<p>O Ministério da Educação, no âmbito das ações do Plano de Desenvolvimento da Educação, desenvolveu o Programa Escola Ativa para atendimento a todas as escolas do campo que oferecem os anos iniciais do Ensino Fundamental em turmas organizadas sob a forma de multisseriação.</p>
											<p>O Programa, que completa dez anos de sua implantação, se estrutura a partir dos componentes Pedagógicos e de Gestão, organizados com vistas a oferecer uma metodologia adequada às classes multisseriadas.</p>
											<p>No ato da adesão, <?php echo $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ? "as prefeituras": "os Estados"  ?> comprometem-se a assegurar as condições necessárias para o desenvolvimento do Programa, sobretudo no que concerne à formação das equipes escolares, ao apoio para assessoramento técnico e ao monitoramento às escolas por meio da disponibilização de um profissional de nível técnico para esta finalidade, bem como na garantia do padrão mínimo de funcionamento destas unidades escolares.</p>
											<p>Informações adicionais sobre os procedimentos administrativos necessários para formalização do atendimento e esclarecimentos sobre a dinâmica do Programa, bem como a legislação que o respalda podem ser obtidas pelo endereço eletrônico http://www.mec.gov.br/secad/escolaativa.</p>
											<p>Para aderir ao Programa ou atualizar as metas para 2010 e 2011, confirme a sua adesão.</p>
										</div>
									</div>
									
									<input type="button" onclick="escolaAtivaAderiPrograma( 1 );" name="concordoTermo" id="concordoTermo" value="Iniciar Processo de Adesão" />
									<input type="button" onclick="escolaAtivaAderiPrograma( 2 );" name="naoConcordoTermo" id="naoConcordoTermo" value="Não Confirmo a Adesão" />
									<input type="button" onclick="escolaAtivaAderiPrograma( 3 );" name="naoPossuiMultisseriadas" id="naoPossuiMultisseriadas" value="Não há Classes Multisseriadas" />
									
								</div>
							</td>
						</tr>
					</table>
				
				<?php } ?>
					
			</td>
			<td width="100%" valign="top" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: center;" class="SubTituloDireita">
				<?php wf_desenhaBarraNavegacao( $docid , array( 'emeidPai' => $_SESSION["emi"]["emeidPai"] ) ); ?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td colspan="2">
				<input type="button" value="Salvar" style="cursor:pointer;" onclick="escolaAtivaSalvaPrograma();"/>
				<input type="button" value="Voltar" style="cursor:pointer;" onclick="history.back(-1);"/>
			</td>
		</tr>
	</table>
</form>