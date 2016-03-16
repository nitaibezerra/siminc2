<?php
include_once APPRAIZ . 'includes/classes/conteudoFlutuante.class.inc';
include_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';



$conteudo1 = '<table class="tabela" width="95%" bgcolor="FFFFFF" cellSpacing="1" border=0 cellPadding="0" align="center" style="margin-top: 5px; margin-bottom: 5px;">
										<tr>
											<th colspan="2">Órgão Estadual/Municipal de Educação</th>
										</tr>
										
										<tr>
											<td class="SubTituloDireita">CNPJ:</td>
											<td> 00.509.968/0001-48</td>
										</tr>
										<tr>
											<td class="SubTituloDireita">Nome:</td>
											<td> Tribunal superior do trabalho</td>
										</tr>
										<tr>
											<td class="SubTituloDireita">Endereço:</td>
											<td> Eixo Monumental</td>
										</tr>
										<tr>
											<td class="SubTituloDireita">Unidade Federativa:</td>
											<td> AL</td>
										</tr>
										<tr>
											<td class="SubTituloDireita">Município:</td>
											<td> Anadia</td>
										</tr>
										
										<tr>
											<th colspan="2">Sec. Estadual/Dirigente Municipal de Educação</th>
										</tr>
										
										<tr>
											<td class="SubTituloDireita">CNPJ:</td>
											<td> 00.509.968/0001-48</td>
										</tr>
										<tr>
											<td class="SubTituloDireita">Nome:</td>
											<td> Tribunal superior do trabalho</td>
										</tr>
										<tr>
											<td class="SubTituloDireita">Endereço:</td>
											<td> Eixo Monumental</td>
										</tr>
										<tr>
											<td class="SubTituloDireita">Unidade Federativa:</td>
											<td> AL</td>
										</tr>
										<tr>
											<td class="SubTituloDireita">Município:</td>
											<td> Anadia</td>
										</tr>
										
									</table>';
$perfis = arrayPerfil();
if( ( count($perfis == 1)) && ( in_array(CTE_PERFIL_CONSULTA_GERAL, $perfis ) 	|| 
								in_array(CTE_PERFIL_CONSULTA_ESTADUAL, $perfis ) || 
								in_array(CTE_PERFIL_CONSULTA_MUNICIPAL, $perfis ) )){
	//não altera
	$conteudo2 = '';
}else{
	//pode alterar
	$conteudo2 = '<table>
				<tr>
					<td><img src="../imagens/pro_letramento.jpg" border=0 /></td>
					<td> Clique na imagem para se aderir ao Programa Pró-Letramento. </td>
				</tr>
			</table>';
}	

$conteudo3 = '<div id="divTermoAdesao">
						
						<div id="divTextoTermo" align="center" >
							
							<div id="cabecalho" align="center" style="font-weight: bold;">
								
								<img width="80" height="80" src="/imagens/brasao.gif"/><br>
									MINISTÉRIO DA EDUCAÇÃO<br>
									SECRETARIA DE EDUCAÇÃO CONTINUADA, ALFABETIZAÇÃO E DIVERSIDADE<br>
									DEPARTAMENTO DE EDUCAÇÃO PARA DIVERSIDADE<br>
									COORDENAÇÃO-GERAL DE EDUCAÇÃO DO CAMPO<br>
									PROGRAMA ESCOLA ATIVA<br>
							</div>
							
							<div style="font-size: 10px; font-family:arial; color:#5E5E5E; margin: 5px 5px 5px 5px;">
								Senhor Dirigente Municipal de Educação,<br>
								
								<br>
									O Ministério da Educação, no âmbito das ações do Plano de Desenvolvimento da Educação, desenvolveu o Programa Escola Ativa para atendimento a todas as escolas do campo que oferecem os anos iniciais do Ensino Fundamental em turmas organizadas sob a forma de multisseriação. 
							
							<br>
							<br>
									O Programa, que completa dez anos de sua implantação, se estrutura a partir dos componentes Pedagógicos e de Gestão, organizados com vistas a oferecer uma metodologia adequada às classes multisseriadas.						
								
								<br>
								<br>
									No ato da adesão, as prefeituras comprometem-se a assegurar as condições necessárias para o desenvolvimento do Programa, sobretudo no que concerne à formação das equipes escolares, ao apoio para assessoramento técnico e ao monitoramento às escolas por meio da disponibilização de um profissional de nível técnico para esta finalidade, bem como na garantia do padrão mínimo de funcionamento destas unidades escolares.
								
								<br>
								<br>
									Informações adicionais sobre os procedimentos administrativos necessários para formalização do atendimento e esclarecimentos sobre a dinâmica do Programa, bem como a legislação que o respalda podem ser obtidas pelo endereço eletrônico http://www.mec.gov.br/secad/escolaativa. 
								
								<br>
								<br>
									Para aderir ao Programa ou atualizar as metas para 2010 e 2011, confirme a sua adesão. 
								
							</div>
	
						</div>
						<div style=" text-align: center; margin-bottom: 5px;">
						<input type="button" onclick="tratarAdesao( 1 );" name="concordoTermo" id="concordoTermo" value="Confirmo a Adesão" />
						<input type="button" onclick="tratarAdesao( 2 );" name="naoConcordoTermo" id="naoConcordoTermo" value="Não Confirmo a Adesão" />
						<input type="button" onclick="tratarAdesao( 3 );" name="naoPossuiMultisseriadas" id="naoPossuiMultisseriadas" value="Não há Classes Multisseriadas" />
						</div>
					</div>									
';
$conteudo4 = '<table class="tabela" width="100%" bgcolor="FFFFFF" cellSpacing="1" border=0 cellPadding="0" align="center" style="margin-top: 5px; margin-bottom: 5px;">
										<tr>
											<th>Subação</th>
											<th>Valor</th>
										</tr>
										<tr>
											<td>Construir cardernos pedagógicos para Jovens e Adultos...</td>
											<td>R$100.00,00</td>
										</tr>
										<tr>
											<td style="background-color: #E1E1E1;"> Construir 01 unidade escolar com 06 salas de aulas...</td>
											<td style="background-color: #E1E1E1;">R$80.00,00</td>
										</tr>
										<tr>
											<td> Construir 09 unidades escolares de 02 salas de aula...</td>
											<td>R$30.00,00</td>
										</tr>
									</table>';

$qtdColunas = 3;
$areaFlutuante = new ConteudoFlutuante();
$arrDivLocalColuna 	= array(0=>array(0=>0, 1=>1),
							1=>array(0=>2),
							2=>array(0=>3)
							);
$arrTituloDiv 		= array(0=>'Dados da Unidade', 1=>'Programa Pró Letramento', 2=>'Programa Escola Ativa', 3=>'Relatórios de subações que constam no termo de Cooperação',4=>'teste');
$arrConteudoDiv 	= array(0=>$conteudo1, 1=>$conteudo2, 2=>$conteudo3, 3=>$conteudo4);
$arrScroll 			= array(0=>false, 1=>false, 2=>true, 3=>false);
$arrEstadoBox 		= array(0=>'min', 1=>'max', 2=>'max', 3=>'max');

?>

<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
<div id="erro" ></div>
<table class="tabela" width="95%" bgcolor="FFFFFF" cellSpacing="0" border=0 cellPadding="3" align="center" style="padding-left: 2px; padding-right: 5px;">
	<tr>
		<td> 				
			<div id="Ferramentas" class="Ferramentas">
					<div id="colid_max" class="groupItem" style="width:100%;">
						<div style="-moz-user-select: none;" class="itemHeader">
							<div class="TituloCombo" style="height:10px;">Ferramentas</div>
							<a id="a" class="closeEl" href="#">	<img border="0" src="../imagens/menos.gif"/> </a>
						</div>
						<div class="itemContent" id="itemContent" style="padding: 2px 2px 2px 2px; text-align: center;"  >
							<div style="float: left;   padding: 2px 2px 2px 2px; margin-right:3px;"><a id="a" href="/cte/cte.php?modulo=principal/estrutura_avaliacao&acao=A">	<img class="globo" width="30" height="30"  title="Indicadores Qualitativos"  border="0" src="../imagens/icones/terra.jpg"/> <br>Indicadores Qualitativos </a></div>
							<div style="float: left;  padding: 2px 2px 2px 2px; margin-right:3px;"><a id="a" href="/cte/cte.php?modulo=principal/comparativoPlano&acao=A">	<img class="globo" title="Questões Pontuais" width="30" height="30"  border="0" src="../imagens/icones/exclamacao.jpg"/> <br>Questões Pontuais</a></div>
							<div style="float: left; padding: 2px 2px 2px 2px; margin-right:3px;"><a id="a" href="#">	<img class="globo" title="Dados Demográficos e Educacionais Quantitativos"  width="30" height="30" border="0" src="../imagens/icones/dados.jpg"/> <br>Dados Demográficos e <br>Educacionais Quantitativos</a></div>
							<div style="float: left;  padding: 2px 2px 2px 2px; margin-right:3px;"><a id="a" href="#">	<img class="globo" title="Monitoramento PAR"  border="0" src="../imagens/icones/monitora.jpg"/> <br>Monitoramento PAR</a></div>
							<div style="float: left;   padding: 2px 2px 2px 2px; margin-right:3px;"><a id="a" href="#">	<img class="globo" title="Monitoramento PAR"  width="33" height="30" border="0" src="../imagens/icones/monitoraFina.jpg"/> <br>Monitoramento Financeiro PAR</a></div>
							<div style="float: left;   padding: 2px 2px 2px 2px; margin-right:3px;"><a id="a" href="#">	<img class="globo" title="Monitoramento PAR"  width="33" height="30" border="0" src="../imagens/icones/configuracao.jpg"/> <br>Configuração global da Tela</a></div>
							<input onclick="salva();" type="button" value="Salvar" />
							<input onclick="addColuna();" type="button" value="addColuna" />
							<input onclick="addDiv();" type="button" value="addDiv" />
						</div>
					</div>
			</div>
		</td>
	</tr>
	<tr>
		<td style="padding-left: 8px;">
			<?php $areaFlutuante->criaConteudoFlutuante($qtdColunas, $arrDivLocalColuna, $arrTituloDiv, $arrConteudoDiv, $arrScroll, $arrEstadoBox); 
			
			?>
		</td>
	</tr>
	<tr>
<td colspan="4" id="botoes_acao" style="text-align:right;padding-right: 35px;">  <a id="link_comparar"  href="#" rel="lyteframe" rev="width: 800px; height: 450px; scrolling: auto;"><input id="botao_comparar" style="display:none" type="button" onclick="limpaComparacao();" value="Comparar" /></a></td>
</tr>
</table>

