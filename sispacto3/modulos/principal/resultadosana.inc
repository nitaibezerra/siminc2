<?

include_once APPRAIZ . "includes/library/simec/Grafico.php";

$grafico = new Grafico(Grafico::K_TIPO_COLUNA);

if(is_numeric($_REQUEST['tpacodigoescola'])) {

	$sql = "SELECT 
			id_escola, no_entidade, upper(rede) as rede, no_municipio, upper(uf) as uf, upper(inse) as inse, pc_formacao_docente,
			qt_aluno_previsto, qt_aluno_presente, nivel_1_leit, nivel_2_leit, nivel_3_leit, nivel_4_leit,
			nivel_sempontuacao_esc, nivel_1_esc, nivel_2_esc, nivel_3_esc, nivel_4_esc,
			nivel_1_mat, nivel_2_mat, nivel_3_mat, nivel_4_mat, fk_cod_municipio 
			FROM sispacto3.ana WHERE id_escola='".$_REQUEST['tpacodigoescola']."'";
	
	$ana = $db->pegaLinha($sql);

}

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro"><span style="font-size:large;">RESULTADOS DA AVALIAÇÃO NACIONAL DA ALFABETIZAÇÃO - 2013</span></td>
</tr>
<? if($ana) : ?>

<tr>
	<td>
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td class="SubTituloDireita" width="30%"><span style="font-size:medium;">INEP:</span></td>
			<td><?=$ana['id_escola']?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%"><span style="font-size:medium;">ESCOLA / REDE:</span></td>
			<td><?=$ana['no_entidade']?> / <?=$ana['rede']?></td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" width="30%"><span style="font-size:medium;">MUNÍCIPIO / UF:</span></td>
			<td><?=$ana['no_municipio']?> / <?=$ana['uf']?></td>
		</tr>
		
		<tr>
			<td colspan="2">
			<p>A Avaliação Nacional de Alfabetização (ANA) pretende diagnosticar os níveis de alfabetização e letramento em Língua Portuguesa e alfabetização Matemática, apontando fatores contextuais sobre as condições do trabalho em cada escola. Os resultados de desempenho apresentados nesta primeira edição, realizada em 2013, devem ser interpretados considerando as informações do contexto escolar.</p>
			<p>Os resultados de desempenho nas áreas avaliadas são expressos em escalas de proficiência. As escalas de Língua Portuguesa (Leitura) e de Matemática da ANA 2013 são compostas por quatro níveis progressivos e cumulativos. Isso significa uma organização da menor para a maior proficiência. Quando um percentual de alunos foi posicionado em determinado nível da escala, pode-se pressupor que, além de terem desenvolvido as habilidades referentes a este nível, provavelmente também desenvolveram as habilidades referentes aos níveis anteriores.</p>
			<p>A escala de proficiência de Língua Portuguesa (Escrita) também é composta por quatro níveis e, no geral, pressupõe a progressão da aprendizagem de um nível para outro. Contudo, é importante ressaltar que o processo de aquisição da escrita não ocorre em etapas lineares.</p>
			</td>
		</tr>
	</table>
	
	</td>
</tr>


<tr>
	<td>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	
		<tr>
			<td class="SubTituloCentro" colspan="2"><span style="font-size:large;">INDICADORES CONTEXTUAIS</span></td>
		</tr>
		
		<tr>
			<td colspan="2">
			<p>O Indicador de Nível Socioeconômico e o Indicador de Adequação da Formação Docente produzem informações sobre o contexto em que cada escola desenvolve o trabalho educativo.</p>
			<p>O Indicador de Nível Socioeconômico possibilita, de modo geral, situar o público atendido pela escola em um estrato ou nível social, apontando o padrão de vida referente a cada um de seus estratos. Esse indicador é calculado a partir da escolaridade dos pais e da posse de bens e contratação de serviços pela família dos alunos. Para melhor caracterizar as escolas foram criados sete grupos, de modo que, no Grupo 1, estão as escolas com nível socioeconômico mais baixo e, no Grupo 7, com nível socioeconômico mais alto.</p>
			<p>O Indicador de Formação Docente analisa, em cada escola, a formação dos docentes dos anos iniciais do Ensino Fundamental que lecionam Língua Portuguesa e Matemática. Apresenta, assim, o percentual de disciplinas de Língua Portuguesa e Matemática que são regidas por professores com Licenciatura em Pedagogia/Normal Superior, Licenciatura em Letras-Língua Portuguesa ou Matemática, respectivamente.</p>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" width="30%"><span style="font-size:medium;">NÍVEL SOCIOECONÔMICO:</span></td>
			<td><?=$ana['inse'] ?></td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" width="30%"><span style="font-size:medium;">FORMAÇÃO DOCENTE:</span></td>
			<td><?=$ana['pc_formacao_docente'] ?>%</td>
		</tr>
		
	
	</table>
	</td>
</tr>

<tr>
	<td>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	
		<tr>
			<td class="SubTituloCentro" colspan="2"><span style="font-size:large;">PARTICIPAÇÃO NA AVALIAÇÃO</span></td>
		</tr>
		
		<tr>
			<td colspan="2">
			<p>O quadro a seguir mostra a previsão de estudantes para participar da ANA, com base nos dados do Censo Escolar, e o número de estudantes que efetivamente realizou as provas.</p>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" width="30%"><span style="font-size:medium;">ESTUDANTES PREVISTOS:</span></td>
			<td><?=$ana['qt_aluno_previsto'] ?></td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" width="30%"><span style="font-size:medium;">ESTUDANTES QUE REALIZARAM AS PROVAS:</span></td>
			<td><?=$ana['qt_aluno_presente'] ?></td>
		</tr>
		
	
	</table>
	</td>
</tr>


<tr>
	<td>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	
		<tr>
			<td class="SubTituloCentro" colspan="2"><span style="font-size:large;">DISTRIBUIÇÃO DOS ALUNOS DA ESCOLA POR NÍVEL DE PROFICIÊNCIA EM LEITURA</span></td>
		</tr>
		
		<tr>
			<td colspan="2">
			<?
			$dadosleit[] = array('valor' => $ana['nivel_1_leit'], 'descricao' => 'Nível 1', 'categoria' => '');
			$dadosleit[] = array('valor' => $ana['nivel_2_leit'], 'descricao' => 'Nível 2', 'categoria' => '');
			$dadosleit[] = array('valor' => $ana['nivel_3_leit'], 'descricao' => 'Nível 3', 'categoria' => '');
			$dadosleit[] = array('valor' => $ana['nivel_4_leit'], 'descricao' => 'Nível 4', 'categoria' => '');
			
			$grafico->setFormatoTooltip("function() { return '<span><span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 2, ',', '.') + ' %</b>'; }")->montarGraficoLinha($dadosleit);
			?>
			</td>
		</tr>
		
		<tr>
			<td colspan="2">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
				<td class="SubTituloDireita" width="25%">Nível 1: Desempenho até 425 pontos</td>
				<td>Ler palavras dissílabas, trissílabas e polissílabas com estruturas silábicas canônicas, com base em imagem. Ler palavras dissílabas, trissílabas e polissílabas com estruturas silábicas não canônicas, com base em imagem.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Nível 2: Desempenho maior que 425 até 525 pontos</td>
				<td>Identificar a finalidade de textos como convite, cartaz, texto instrucional (receita) e bilhete. Localizar informação explícita em textos curtos (com até cinco linhas) em gêneros como piada, parlenda, poema, tirinha (história em quadrinhos em até três quadros), texto informativo e texto narrativo. Identificar o assunto de textos, cujo assunto pode ser identificado no título ou na primeira linha em gêneros como poema e texto informativo. Inferir o assunto de um cartaz apresentado em sua forma estável, com letras grandes e mensagem curta e articulação da linguagem verbal e não verbal.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Nível 3: Desempenho maior que 525 até 625 pontos</td>
				<td>Inferir o assunto de texto de divulgação científica para crianças. Localizar informação explícita, situada no meio ou final do texto, em gêneros como lenda e cantiga folclórica. Identificar o referente de um pronome pessoal do caso reto em gêneros como tirinha e poema narrativo. Inferir relação de causa e consequência em gêneros como tirinha, anedota, fábula e texto de literatura infantil. Inferir sentido com base em elementos verbais e não verbais em tirinha. Reconhecer significado de expressão de linguagem figurada em gêneros como poema narrativo, texto de literatura infantil e tirinha.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Nível 4: Desempenho maior que 625 pontos</td>
				<td>Inferir sentido de palavra em texto verbal. Reconhecer os participantes de um diálogo em uma entrevista ficcional. Inferir sentido em texto verbal. Reconhecer relação de tempo em texto verbal. Identificar o referente de pronome possessivo em poema.</td>
				</tr>
				
			</table>
			</td>
		</tr>
		
		<tr>
			<td colspan="2">
			<?
			$sql = "(
					SELECT '<span style=float:right;font-size:medium;>Total Estado</span>' as descricao, 
						   '<span style=float:right;font-size:medium;>'||nivel_1_leit||' %</span>' as nivel_1_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_2_leit||' %</span>' as nivel_2_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_3_leit||' %</span>' as nivel_3_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_4_leit||' %</span>' as nivel_4_leit
					FROM sispacto3.anaregiao WHERE removeacento(uf) ilike removeacento('".$ana['uf']."') AND rede ilike '".$ana['rede']."'
					) UNION ALL (
					SELECT '<span style=float:right;font-size:medium;>Total Município</span>' as descricao, 
						   '<span style=float:right;font-size:medium;>'||nivel_1_leit||' %</span>' as nivel_1_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_2_leit||' %</span>' as nivel_2_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_3_leit||' %</span>' as nivel_3_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_4_leit||' %</span>' as nivel_4_leit
					FROM sispacto3.anamunicipio WHERE fk_cod_municipio='".$ana['fk_cod_municipio']."' AND rede ilike '".$ana['rede']."'
					) UNION ALL (
					SELECT '<span style=float:right;font-size:medium;><b>Sua escola</b></span>' as descricao,
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_1_leit']." %</b></span>' as nivel_1_leit, 
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_2_leit']." %</b></span>' as nivel_2_leit, 
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_3_leit']." %</b></span>' as nivel_3_leit, 
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_4_leit']." %</b></span>' as nivel_4_leit
					)";
			$cabecalho = array("&nbsp;","Nível 1","Nível 2","Nível 3","Nível 4");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, false);
			?>
			</td>
		</tr>
	
	</table>
	</td>
</tr>

<tr>
	<td>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	
		<tr>
			<td class="SubTituloCentro" colspan="2"><span style="font-size:large;">DISTRIBUIÇÃO DOS ALUNOS DA ESCOLA POR NÍVEL DE PROFICIÊNCIA EM ESCRITA</span></td>
		</tr>
		
		<tr>
			<td colspan="2">
			<?
			$dadosesc[] = array('valor' => $ana['nivel_sempontuacao_esc'], 'descricao' => 'Sem pontuação', 'categoria' => '');
			$dadosesc[] = array('valor' => $ana['nivel_1_esc'], 'descricao' => 'Nível 1', 'categoria' => '');
			$dadosesc[] = array('valor' => $ana['nivel_2_esc'], 'descricao' => 'Nível 2', 'categoria' => '');
			$dadosesc[] = array('valor' => $ana['nivel_3_esc'], 'descricao' => 'Nível 3', 'categoria' => '');
			$dadosesc[] = array('valor' => $ana['nivel_4_esc'], 'descricao' => 'Nível 4', 'categoria' => '');
			
			$grafico->setFormatoTooltip("function() { return '<span><span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 2, ',', '.') + ' %</b>'; }")->montarGraficoLinha($dadosesc);
			?>
			</td>
		</tr>
		
		<tr>
			<td colspan="2">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
				<td class="SubTituloDireita" width="25%">Nível 1: Desempenho até 400 pontos</td>
				<td>Neste nível, foram agrupados desde os alunos que, em geral, são capazes de:<br>- Escrever palavras com sílabas canônicas (consoante e vogal) e não canônicas, com alguma dificuldade, pela omissão e/ou troca de letras;<br> até os que são capazes de:<br>- Escrever ortograficamente palavras marcadas pela presença de sílabas canônicas.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Nível 2: Desempenho maior que 400 até 500 pontos</td>
				<td>Escrever ortograficamente palavras com sílabas não canônicas;<br>Escrever textos incipientes apresentados na forma de apenas uma frase;<br>Produzir textos narrativos, a partir de uma dada situação, que apresentam ausência ou inadequação dos elementos formais (segmentação, pontuação, ortografia, concordância verbal e concordância nominal) e da textualidade (coesão e coerência), evidenciando ainda um distanciamento da norma padrão da língua.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Nível 3: Desempenho maior que 500 até 580 pontos</td>
				<td>Escrever textos narrativos com mais de uma frase, a partir de uma situação dada;<br>Produzir textos narrativos com poucas inadequações relativas à segmentação, concordância verbal e concordância nominal, embora com algum comprometimento dos elementos formais e da textualidade, evidenciando uma aproximação à norma padrão da língua.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Nível 4: Desempenho maior que 580 pontos</td>
				<td>Produzir textos narrativos, a partir de uma situação dada, atendendo adequadamente ao uso de elementos formais e da textualidade, evidenciando o atendimento à norma padrão da língua.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Sem Pontuação</td>
				<td>Cadernos de prova que não foram pontuados por conter a escrita de palavras sem relação semântica com a imagem apresentada ou escrita incompreensível.</td>
				</tr>
				
			</table>
			</td>
		</tr>
		
		<tr>
			<td colspan="2">
			<?
			$sql = "(
					SELECT '<span style=float:right;font-size:medium;>Total Estado</span>' as descricao, 
						   '<span style=float:right;font-size:medium;>'||nivel_1_esc||' %</span>' as nivel_1_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_2_esc||' %</span>' as nivel_2_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_3_esc||' %</span>' as nivel_3_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_4_esc||' %</span>' as nivel_4_leit
					FROM sispacto3.anaregiao WHERE removeacento(uf) ilike removeacento('".$ana['uf']."') AND rede ilike '".$ana['rede']."'
					) UNION ALL (
					SELECT '<span style=float:right;font-size:medium;>Total Município</span>' as descricao, 
						   '<span style=float:right;font-size:medium;>'||nivel_1_esc||' %</span>' as nivel_1_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_2_esc||' %</span>' as nivel_2_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_3_esc||' %</span>' as nivel_3_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_4_esc||' %</span>' as nivel_4_leit
					FROM sispacto3.anamunicipio WHERE fk_cod_municipio='".$ana['fk_cod_municipio']."' AND rede ilike '".$ana['rede']."'
					) UNION ALL (
					SELECT '<span style=float:right;font-size:medium;><b>Sua escola</b></span>' as descricao,
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_1_esc']." %</b></span>' as nivel_1_leit, 
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_2_esc']." %</b></span>' as nivel_2_leit, 
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_3_esc']." %</b></span>' as nivel_3_leit, 
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_4_esc']." %</b></span>' as nivel_4_leit
					)";
			$cabecalho = array("&nbsp;","Nível 1","Nível 2","Nível 3","Nível 4");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, false);
			?>
			<br>
			<span style="font-size:small;">* Os percentuais exibidos não incluem os cadernos de prova "Sem Pontuação".</span>
			</td>
		</tr>
	
	</table>
	</td>
</tr>

<tr>
	<td>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	
		<tr>
			<td class="SubTituloCentro" colspan="2"><span style="font-size:large;">DISTRIBUIÇÃO DOS ALUNOS DA ESCOLA POR NÍVEL DE PROFICIÊNCIA EM MATEMÁTICA</span></td>
		</tr>
		
		<tr>
			<td colspan="2">
			<?
			$dadosmat[] = array('valor' => $ana['nivel_1_mat'], 'descricao' => 'Nível 1', 'categoria' => '');
			$dadosmat[] = array('valor' => $ana['nivel_2_mat'], 'descricao' => 'Nível 2', 'categoria' => '');
			$dadosmat[] = array('valor' => $ana['nivel_3_mat'], 'descricao' => 'Nível 3', 'categoria' => '');
			$dadosmat[] = array('valor' => $ana['nivel_4_mat'], 'descricao' => 'Nível 4', 'categoria' => '');
			
			$grafico->setFormatoTooltip("function() { return '<span><span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 2, ',', '.') + ' %</b>'; }")->montarGraficoLinha($dadosmat);
			?>
			</td>
		</tr>
		
		<tr>
			<td colspan="2">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
				<td class="SubTituloDireita" width="25%">Nível 1: Desempenho até 425 pontos</td>
				<td>Reconhecer representação de figura geométrica plana ou espacial em objetos de uso cotidiano; maior frequência em gráfico de colunas; planificação de figura geométrica espacial (paralelepípedo); horas e minutos em relógio digital. Associar objeto de uso cotidiano à representação de figura geométrica espacial; Contar objetos dispostos em forma organizada ou não; Comparar medidas de comprimento em objetos do cotidiano.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Nível 2: Desempenho maior que 425 até 525 pontos</td>
				<td>Reconhecer nomenclatura de figura geométrica plana; valor monetário de cédula; figura geométrica plana em uma composição com várias outras. Associar a escrita por extenso de números naturais com até três algarismos à sua representação simbólica; valor monetário de uma cédula a um agrupamento de moedas e cédulas; Completar sequência numérica crescente de números naturais não consecutivos. Comparar números naturais com até três algarismos não ordenados. Estimar uma medida entre dois números naturais com dois algarismos; Resolver problema de adição sem reagrupamento.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Nível 3: Desempenho maior que 525 até 575 pontos</td>
				<td>Reconhecer frequências iguais em gráfico de colunas; composição de números naturais com até três algarismos, apresentada por extenso Completar sequência numérica decrescente de números naturais não consecutivos. Calcular adição de duas parcelas com reagrupamento; Associar valor monetário de um conjunto de moedas ao valor de uma cédula; a representação simbólica de números naturais com até três algarismos à sua escrita por extenso; Resolver problema de subtração, com números naturais de até dois algarismos, com ideia de comparar e retirar e problema de divisão com ideia de repartir.</td>
				</tr>
				
				<tr>
				<td class="SubTituloDireita">Nível 4: Desempenho maior que 575 pontos</td>
				<td>Reconhecer composição e decomposição aditiva de números naturais com até três algarismos; medidas de tempo em relógios analógicos; informações em gráfico de barras. Calcular subtração de números naturais com até três algarismos com reagrupamento. Associar medidas de tempo entre relógio analógico e digital. Resolver problema de subtração como operação inversa da adição¸ com números naturais; problemas com a ideia de comparar números naturais de até três algarismos; problema de multiplicação com a ideia de proporcionalidade; problema de multiplicação com a ideia de combinação; problema de divisão com ideia de proporcionalidade e problema que envolve medidas de tempo (dias de semanas).</td>
				</tr>
				
			</table>
			</td>
		</tr>
		
		<tr>
			<td colspan="2">
			<?
			$sql = "(
					SELECT '<span style=float:right;font-size:medium;>Total Estado</span>' as descricao, 
						   '<span style=float:right;font-size:medium;>'||nivel_1_mat||' %</span>' as nivel_1_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_2_mat||' %</span>' as nivel_2_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_3_mat||' %</span>' as nivel_3_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_4_mat||' %</span>' as nivel_4_leit
					FROM sispacto3.anaregiao WHERE removeacento(uf) ilike removeacento('".$ana['uf']."') AND rede ilike '".$ana['rede']."'
					) UNION ALL (
					SELECT '<span style=float:right;font-size:medium;>Total Município</span>' as descricao, 
						   '<span style=float:right;font-size:medium;>'||nivel_1_mat||' %</span>' as nivel_1_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_2_mat||' %</span>' as nivel_2_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_3_mat||' %</span>' as nivel_3_leit, 
						   '<span style=float:right;font-size:medium;>'||nivel_4_mat||' %</span>' as nivel_4_leit
					FROM sispacto3.anamunicipio WHERE fk_cod_municipio='".$ana['fk_cod_municipio']."' AND rede ilike '".$ana['rede']."'
					) UNION ALL (
					SELECT '<span style=float:right;font-size:medium;><b>Sua escola</b></span>' as descricao,
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_1_mat']." %</b></span>' as nivel_1_leit, 
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_2_mat']." %</b></span>' as nivel_2_leit, 
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_3_mat']." %</b></span>' as nivel_3_leit, 
						   '<span style=float:right;font-size:medium;><b>".$ana['nivel_4_mat']." %</b></span>' as nivel_4_leit
					)";
			$cabecalho = array("&nbsp;","Nível 1","Nível 2","Nível 3","Nível 4");
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, false);
			?>
			</td>
		</tr>
	
	</table>
	</td>
</tr>
<? else :?>

<tr>
	<td align="center"><span style="font-size:large;color:red;">ESCOLA NÃO FOI ENCONTRADA NOS REGISTROS. INEP SELECIONADO : <?=$_REQUEST['tpacodigoescola'] ?></span></td>
</tr>

<? endif;?>

</table>