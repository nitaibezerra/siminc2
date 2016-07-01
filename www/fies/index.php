<?php
include("fies.php");


?>
<html>
<head>
	<title>FIES - Financiamento Estudantil</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
</head>
<body>
<?monta_titulo('Simulador Fies', ''); ?>
<form action="result.php" method="POST">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td colspan="2" style="border-bottom: 1px solid #cccccc;" >
		<center>
			<div class="textoAzul2" align="left" style="font-weight: normal;">
			Este simulador possui um caráter meramente ilustrativo e visa possibilitar ao estudante interessado no Financiamento Estudantil informações aproximadas sobre a sua dívida futura, bem como o montante de recursos que deverá despender mensalmente para quitá-la. Lembramos que variáveis como, data da prestação, valor da semestralidade, data da assinatura do contrato, data da realização dos aditamentos, trazem variações nos valores simulados.<br><br>
			Para a informação dos valores da taxa de juros, deverá ser indicado no campo correspondente o valor praticado no seu contrato levando em conta as diferentes taxas. Para os contratos firmados até 2005 inclusive, a taxa de juros é de 9% ao ano.Para os contratos firmados a partir do segundo semestre de 2006 a taxa de juros é de 6,5% ao ano ou 3,5% ao ano exclusivamente para os cursos de licenciatura, pedagogia, normal superior e cursos constantes do Catálogo de Cursos Superiores de Tecnologia.<br><br> 
			Os valores das prestações foram calculados considerando os dados informados pelo estudante.
			</div>
		</td>
	</tr>
	<tr>
		<th colspan="2" class="SubTituloCentro" style="background: #DCDCDC;">Formulário para Simulação</th>
	</tr>
	<tr>
		<td class="SubTituloDireita">Processo Seletivo:</td>
		<td width="60%">
			<?php
				$comboArr = array(	
									array(
										"codigo" 	=> "2º semestre de 1999",
										"descricao" => "2º semestre de 1999"
									)
								  );
				for ($i=2000; $i <= 2010; $i++){
					array_push($comboArr, array(
													"codigo"    => "1º semestre de {$i}",
													"descricao" => "1º semestre de {$i}"
												));
												
					array_push($comboArr, array(
													"codigo"    => "2º semestre de {$i}",
													"descricao" => "2º semestre de {$i}"
												));
					
				}
								  
				$db->monta_combo("processoseletivo", $comboArr, 'S', "", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Quantidade de Semestres do Curso:</td>
		<td>
			<?= campo_texto( 'qtdsemcurso', 'N', 'S', '', 8, 10, '############', '', 'left', '', 0, ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Quantidade de Semestres já Concluídos:</td>
		<td>
			<?= campo_texto( 'qtdsemconc', 'N', 'S', '', 8, 10, '############', '', 'left', '', 0, ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Percentual de Financiamento:</td>
		<td>
			<?php
				$comboArr = array();
				for ($i=10; $i <= 100; $i++){
					array_push($comboArr, array(
													"codigo"    => "{$i}%",
													"descricao" => "{$i}%"
												));
				}
								  
				$db->monta_combo("percentfinanc", $comboArr, 'S', "", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr>
	<tr>
		<td class="SubTituloDireita">Taxa de juros a.a.:<br/><font size="1">(escolher a taxa de juros conforme seu contrato nos valores abaixo)</font></td>
		<td>
			<?php
				$comboArr = array(	
									array(
										"codigo" 	=> "3,5%",
										"descricao" => "3,5%"
									),
									array(
										"codigo" 	=> "6,5%",
										"descricao" => "6,5%"
									),
									array(
										"codigo" 	=> "9%",
										"descricao" => "9%"
									),
								  );
								  
				$db->monta_combo("taxajuros", $comboArr, 'S', "--", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de estudante:</td>
		<td>
			<?php
				$comboArr = array(	
									array(
										"codigo" 	=> "Bolsista Prouni 50%",
										"descricao" => "Bolsista Prouni 50%"
									),
									array(
										"codigo" 	=> "Bolsista Complementar 25%",
										"descricao" => "Bolsista Complementar 25%"
									),
									array(
										"codigo" 	=> "Não Bolsista",
										"descricao" => "Não Bolsista"
									),
								  );
								  
				$db->monta_combo("tipoestudante", $comboArr, 'S', "--", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr>
	<tr>
		<td class="SubTituloDireita">Valor da Mensalidade:<br/><font size="1">(Informar o valor da sua mensalidade deduzidos todos os descontos oferecidos pela instituição de ensino, inclusive os concedidos em virtude de pagamento pontual)</font></td>
		<td>
			<?= campo_texto( 'valmens', 'N', 'S', '', 8, 10, '#.###.###.###,##', '', 'left', '', 0, ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prazo de Carência:<br><font size="1">(em meses)</font> </td>
		<td>
			<? $carencia = '06'; ?>	
			<?= campo_texto( 'carencia', 'N', 'N', '', 3, 10, '', '', 'left', '', 0, ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data da assinatura do contrato:</td>
		<td>
		<?= campo_data2( 'dtasscontrato', 'S', 'S', 'Data da assinatura do contrato', 'S' ); ?>		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Escolha o melhor dia para Vencimento:</td>
		<td>
			<?php
				$comboArr = array(	
									array(
										"codigo" 	=> "05",
										"descricao" => "05"
									),
									array(
										"codigo" 	=> "10",
										"descricao" => "10"
									),
									array(
										"codigo" 	=> "15",
										"descricao" => "15"
									),
									array(
										"codigo" 	=> "20",
										"descricao" => "20"
									),
									array(
										"codigo" 	=> "25",
										"descricao" => "25"
									)
								  );
								  
				$db->monta_combo("diavenc", $comboArr, 'S', "", '', '', '', '', 'N', '');
		    ?>		
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	    	<input type="submit" name="btalterar" value="Continuar" onclick="" class="botao">
	    	&nbsp;&nbsp;&nbsp;&nbsp;
	    	<input type="reset" name="btcancelar" value="Limpar" class="botao">
	   </td>
	</tr>      
	<tr>
		<td colspan="2" style="border-top: 1px solid #cccccc;">
		<center>
			<div class="textoAzul2" align="left" style="font-weight: normal;">
Pela lei 11.552/2007, publicada no DOU em 19/11/2007, os contratos de FIES, assinados a partir de 2008, passam a contar com prazo de carência de 6 meses a partir do término do prazo de utilização bem como o prazo de amortização II passa de até 1,5 e meia o prazo de utilização para até 2 vezes o prazo de utilização.<br>
Dessa forma um contrato FIES passa a ter 4 fases distintas:<br><br>

1 - PRAZO DE UTILIZAÇÃO : Prazo contado a partir do primeiro mês de ingresso no FIES até o último mês do prazo de utilização (considera-se os semestres suspensos ou encerrados sem início de amortização). Nessa fase o estudante paga prestações de juros a cada 3(três) meses de até 50,00, nos meses MARÇO, JUNHO, SETEMBRO e DEZEMBRO.<br>
2 - PRAZO DE CARÊNCIA : Fixo em 6 meses imediatamente subseqüentes ao PRAZO DE UTILIZAÇÃO. O prazo de carência é opcional porém o estudante deve se manifestar caso não o queira. Nessa fase as prestações têm a mesma regra do PRAZO DE UTILIZAÇÃO.<br>
3 - PRAZO DE AMORTIZAÇÃO I : Fixo em 12 meses imediatamente subseqüentes ao PRAZO DE CARÊNCIA. Nessa fase o estudante paga prestações mensais cujo valor é exatamente o mesmo repassado mensalmente à IES– INSTITUIÇÃO DE ENSINO SUPERIOR em função do último aditamento do aluno.<br>
4 - PRAZO DE AMORTIZAÇÃO II : Até 2 vezes o PRAZO DE UTILIZAÇÃO(o simulador adota exatamente 2(duas) vezes). Nessa fase o sistema calcula a prestação PRICE em função do saldo devedor do contrato no dia da mudança para essa fase, em função ainda da taxa de juros e do prazo dessa fase.<br><br>

Durante as fases PRAZO DE UTILIZAÇÃO, PRAZO DE CARÊNCIA e PRAZO DE AMORTIZAÇÃO I, os juros excedentes ao valor da prestação calculado são incorporados ao saldo devedor do contrato no mês da sua apuração(cálculo). 			</div>
		</center>
		</td>
	</tr>
</table>
</form>
</body>
</html>