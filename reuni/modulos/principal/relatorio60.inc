
<?

$unicod = $_SESSION['unicod'];
$unitpocod = 'U';
//$unicod = 26234;

?>


<form name="formulario" method="post" style="margin: 0px;">
<table class="tabela" bgcolor="#f5f5f5" width="100%" cellpadding="0"
	cellspacing="0">

	<?

	$sql_unidade = " SELECT unidsc
				 FROM public.unidade 
				 WHERE public.unidade.unicod = '".$unicod."'				
				";

	$unidsc = $db->pegaUm($sql_unidade);

	//----------------- Investimento ------------------

	$sql_valor_total_investimento = "
										SELECT lidvalor 
										FROM reuni.limitedesppercent as lim
										INNER JOIN reuni.tipodesp as tipo on tipo.tipid = lim.tipid							
										WHERE tipo.tipcod = 4 AND
										lim.unicod = '".$unicod."' AND
										lim.unitpocod = 'U'								
										
		";

	$item_financeiro_valor_total_investimento = $db->pegaUm($sql_valor_total_investimento);


	$sql_valor_lancado_investimento = "
										SELECT SUM(lantotal) 
										FROM reuni.lancamentodesppercent as lan
										INNER JOIN reuni.tipodesp as tipo on tipo.tipid = lan.tipid							
										WHERE tipo.tipcod = 4 AND
										lan.unicod = '".$unicod."' AND
										lan.unitpocod = 'U'								
										
		";

	$item_financeiro_valor_lancado_investimento = $db->pegaUm($sql_valor_lancado_investimento);

	$item_financeiro_valor_disponivel_investimento = $item_financeiro_valor_total_investimento - $item_financeiro_valor_lancado_investimento;
	$item_financeiro_valor_total_investimento = number_format( $item_financeiro_valor_total_investimento, 2, ",", "." );
	$item_financeiro_valor_lancado_investimento = number_format( $item_financeiro_valor_lancado_investimento, 2, ",", "." );

	$item_financeiro_valor_disponivel_investimento = number_format( $item_financeiro_valor_disponivel_investimento, 2, ",", "." );


	//----------------- Despesas ------------------
	$sql_valor_total_despesa = "
										SELECT lidvalor 
										FROM reuni.limitedesp as lim
										INNER JOIN reuni.tipodesp as tipo on tipo.tipid = lim.tipid							
										WHERE tipo.tipcod = 5 AND
										lim.unicod = '".$unicod."' AND
										lim.unitpocod = 'U'								
										
		";
	$item_financeiro_valor_total_despesa = $db->pegaUm($sql_valor_total_despesa);

	$sql_valor_lancado_despesa= "
										SELECT SUM(lantotal) 
										FROM reuni.lancamentodesp as lan
										INNER JOIN reuni.tipodesp as tipo on tipo.tipid = lan.tipid							
										WHERE tipo.tipcod = 5 AND
										lan.unicod = '".$unicod."' AND
										lan.unitpocod = 'U'								
										
		";

	$item_financeiro_valor_lancado_despesa = $db->pegaUm($sql_valor_lancado_despesa);


	$item_financeiro_valor_disponivel_despesa = $item_financeiro_valor_total_despesa - $item_financeiro_valor_lancado_despesa;

	$item_financeiro_valor_total_despesa = number_format( $item_financeiro_valor_total_despesa, 2, ",", "." );
	$item_financeiro_valor_lancado_despesa = number_format( $item_financeiro_valor_lancado_despesa, 2, ",", "." );
	$item_financeiro_valor_disponivel_despesa = number_format( $item_financeiro_valor_disponivel_despesa, 2, ",", "." );
	?>

	<tr>
		<td>
		<table border="1" width="100%" class="tabela" bgcolor="#f5f5f5"
			cellSpacing="0" cellPadding="3" align="center">
			<tr bgcolor="#dfdfdf">
				<td width="70%" align="center" ><B>PLANO APLICAÇÃO</B>
				<BR> 
				ANO EXERCÍCIO 2007</td>
				<td width="30%">&nbsp;</td>
			</tr>
			<tr>
				<td width="70%" align="left" >INSTITUIÇÃO: <?=$unicod ?>
				- <?=$unidsc ?> </b></td>
				<td width="30%">U.G e GESTÃO/CNPJ<br />
				<br />
				</td>
			</tr>
			<tr>
				<td width="70%" align="left" >ÓRGÃO DO MEC: <b>
				SECRETARIA DE EDUCAÇÃO SUPERIOR - CONCEDENTE </b></td>
				<td width="30%">U.G e GESTÃO<br />
				<br />
				</td>
			</tr>
			<tr bgcolor="#dfdfdf">
				<td width="100%" colspan="2" align="center" ><u>CLASSIFICAÇÃO
				ORÇAMENTÁRIA<u></td>

			</tr>
			<tr>
				<td width="70%" align="left" valign="top"  rowspan="2">Programa(s) de Trabalho
				de Título(s) do(s) Projeto(s)/Atividade(s):	<br>
				Programa 1073 - Universidade do Século XXI <br>
				Ação 8551 - Complementação para o funcionamento das Instituições Federais de Ensino Superior.
				</td>
				<td width="30%" align="left"><span style="text-align: center;">FONTE
				DE RECURSOS<br />
				[X]Tesouro<br />
				[ ]Outras Fontes</td>
			</tr>
			<tr>
				
				<td width="30%" align="left"><span style="text-align: center;">VALOR<br />
				<br />
			
			</tr>
		</table>
		<table border="1" width="100%" class="tabela" bgcolor="#f5f5f5"
			cellSpacing="0" cellPadding="3" align="center">
			<tr bgcolor="#dfdfdf">
				<td colspan="4" align="center" ><u>APLICAÇÃO<u></td>

			</tr>
			<tr>
				<td colspan="4" align="left" >Órgão Aplicador:
				Ministério da Educação</td>
			</tr>
			<tr>
				<td colspan="4" align="left" >Unidade Aplicadora: <?=$unidsc ?></td>
			</tr>
			<tr>
				<td colspan="4" align="left" >Descrição do
				Projeto/Atividade de Aplicação: Plano de Reestruturação e Expansão da
				Universidade - REUNI
				</td>
			</tr>

			<!-- Despesas Fixas -->

			<!-- Pessoal-->

			<?

			$sql_despesa_pessoal = "SELECT
										substr( lfxcodnatureza, 1, 2 ) || '.' ||
										substr( lfxcodnatureza, 3, 2 ) || '.' ||
										substr( lfxcodnatureza, 5, 2 )
										as codigo,
										lfxdscnatureza, lfxvalor
										FROM reuni.lancamentofixo 						
										WHERE unicod = '".$unicod."' AND lfxtipo = '1) - Despesas Orçamentárias de Pessoal'
										ORDER BY lfxordem
			
				";

			$resultado  = $db->carregar($sql_despesa_pessoal);

			/*$sqp_soma_pessoal = "SELECT SUM(lfxvalor) FROM reuni.lancamentofixo
							 		WHERE unicod = ".$unicod." AND lfxtipo = '1) - Despesas Orçamentárias de Pessoal'	
				";
			$total_pessoal = $db->pegaUm($sqp_soma_pessoal);*/
			

			?>


			<table width="100%" align="center" border="1" class="tabela"
				cellpadding="3" cellspacing="0">
				<colgroup>
					<col style="width: 35%;" />
					<col style="width: 30%;" />
					<col style="width: 35%;" />
				</colgroup>
				<thead>
					<tr bgcolor="#dfdfdf">
						<td align="left"  colspan="3"><b>1) - Despesas
						Orçamentárias de Pessoal</b></td>
					</tr>
				</thead>
				<?php if ( count( $resultado ) ) : ?>
				<thead>
					<tr bgcolor="#dfdfdf">
						<td style="text-align: center;"><b>CÓDIGO DE DESPESA -
						ESPECIFICAÇÃO</b></td>
						<td style="text-align: center;"><b>FUNÇÃO</b></td>
						<td style="text-align: center;"><b>VALOR ATUAL (R$)</b></td>

					</tr>
				</thead>
				<?php $cor = ''; ?>
				<? $resultado = is_array( $resultado ) ? $resultado : array(); ?>
				<?php foreach ( $resultado as $indice => $atributo ) :

				?>

				<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
				<tr bgcolor="<?= $cor ?>"
					onmouseout="this.style.backgroundColor='<?= $cor ?>';"
					onmouseover="this.style.backgroundColor='#ffffcc';">
					<td style="text-align: center;"><?= $atributo['codigo'] ?></td>
					<td style="text-align: left;">&nbsp;</td>
					<!-- <td style="text-align: right;"><?= number_format($atributo['lfxvalor'], 2, ",", ".") ?></td> -->
					<td style="text-align: right;">0,00</td> 
				</tr>
				<?php endforeach; ?>
				<?php else : ?>
				<tr>
					<td
						style="text-align: center; padding: 15px; background-color: #fafafa; color: #404040; font-weight: bold; font-size: 10px;"
						colspan="6">Não há Despesas.</td>
				</tr>
				<?php endif; ?>
				<tr>
					<!-- <td colspan="3" align="center" >TOTAL: <?= number_format($total_pessoal, 2, ",", ".") ?></td> -->
					<td colspan="3" align="center" >TOTAL: 0,00</td>
				</tr>

			</table>

			<!-- Bolsas/Capes -->

			<?

			$sql_despesa_bolsas = "SELECT
									substr( lfxcodnatureza, 1, 2 ) || '.' ||
									substr( lfxcodnatureza, 3, 2 ) || '.' ||
									substr( lfxcodnatureza, 5, 2 )
									as codigo,
									lfxdscnatureza, lfxvalor
									FROM reuni.lancamentofixo 						
									WHERE unicod = '".$unicod."' AND lfxtipo = '2) - Despesas Orçamentárias (Bolsas/Capes)'
									ORDER BY lfxordem
			
		
			";
			$resultado  = $db->carregar($sql_despesa_bolsas);

			$sqp_soma_bolsas = "SELECT SUM(lfxvalor) FROM reuni.lancamentofixo
						 		WHERE unicod = '".$unicod."' AND lfxtipo = '2) - Despesas Orçamentárias (Bolsas/Capes)'	
			";
			$total_bolsas = $db->pegaUm($sqp_soma_bolsas);
			?>


			<table width="100%" align="center" border="1" class="tabela"
				cellpadding="3" cellspacing="0">
				<colgroup>
					<col style="width: 35%;" />
					<col style="width: 30%;" />
					<col style="width: 35%;" />
				</colgroup>

				<thead>
					<tr bgcolor="#dfdfdf">
						<td align="left"  colspan="3"><b>2) - Despesas
						Orçamentárias (Bolsas/Capes)</b></td>
					</tr>
				</thead>

				<?php if ( count( $resultado ) ) : ?>
				<thead>
					<tr bgcolor="#dfdfdf">
						<td style="text-align: center;"><b>CÓDIGO DE DESPESA -
						ESPECIFICAÇÃO</b></td>
						<td style="text-align: center;"><b>FUNÇÃO</b></td>
						<td style="text-align: center;"><b>VALOR ATUAL (R$)</b></td>

					</tr>
				</thead>
				<?php $cor = ''; ?>
				<? $resultado = is_array( $resultado ) ? $resultado : array(); ?>
				<?php foreach ( $resultado as $indice => $atributo ) :

				?>

				<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
				<tr bgcolor="<?= $cor ?>"
					onmouseout="this.style.backgroundColor='<?= $cor ?>';"
					onmouseover="this.style.backgroundColor='#ffffcc';">
					<td style="text-align: center;"><?= $atributo['codigo'] ?></td>
					<td style="text-align: left;">&nbsp;</td>
					<!-- <td style="text-align: right;"><?= number_format($atributo['lfxvalor'], 2, ",", ".") ?></td> -->
					<td style="text-align: right;">0,00</td>
					
				</tr>
				<?php endforeach; ?>
				<?php else : ?>
				<tr>
					<td
						style="text-align: center; padding: 15px; background-color: #fafafa; color: #404040; font-weight: bold; font-size: 10px;"
						colspan="6">Não há Despesas.</td>
				</tr>
				<?php endif; ?>
				<tr>
					 <!-- <td colspan="3" align="center" >TOTAL: <?= number_format($total_bolsas, 2, ",", ".") ?></td> -->
					 <td colspan="3" align="center" >TOTAL: 0,00</td>
				</tr>

			</table>

			<!-- Assitencia Estudantil -->

			<?

			$sql_despesa_bolsas_assitencia = "SELECT tipcod, tipdsc
										   FROM reuni.tipodesp						
										   WHERE tipcod = 3
		
			";
			$cabecalho  = $db->carregar($sql_despesa_bolsas_assitencia);

			$sql_despesa_assitencia = "SELECT
										substr( lfxcodnatureza, 1, 2 ) || '.' ||
										substr( lfxcodnatureza, 3, 2 ) || '.' ||
										substr( lfxcodnatureza, 5, 2 )
										as codigo,
										lfxdscnatureza, lfxvalor
										FROM reuni.lancamentofixo 						
										WHERE unicod = '".$unicod."' AND lfxtipo = '3) - Despesas Orçamentárias (Assistência Estudantil)'
										ORDER BY lfxordem
			
		
			";
			$resultado  = $db->carregar($sql_despesa_assitencia);

			$sqp_soma_assitencia = "SELECT SUM(lfxvalor) FROM reuni.lancamentofixo
						 		WHERE unicod = '".$unicod."' AND lfxtipo = '3) - Despesas Orçamentárias (Assistência Estudantil)'	
			";
			$total_assitencia = $db->pegaUm($sqp_soma_assitencia);

			?>

			<table width="100%" align="center" border="1" class="tabela"
				cellpadding="3" cellspacing="0">
				<colgroup>
					<col style="width: 35%;" />
					<col style="width: 30%;" />
					<col style="width: 35%;" />
				</colgroup>

				<thead>
					<tr bgcolor="#dfdfdf">
						<td align="left"  colspan="3"><b>3) - Despesas
						Orçamentárias (Assistência Estudantil)</b></td>
					</tr>
				</thead>

				<?php if ( count( $resultado ) ) : ?>

				<thead>
					<tr bgcolor="#dfdfdf">
						<td style="text-align: center;"><b>CÓDIGO DE DESPESA -
						ESPECIFICAÇÃO</b></td>
						<td style="text-align: center;"><b>FUNÇÃO</b></td>
						<td style="text-align: center;"><b>VALOR ATUAL (R$)</b></td>

					</tr>
				</thead>
				<?php $cor = ''; ?>
				<? $resultado = is_array( $resultado ) ? $resultado : array(); ?>
				<?php foreach ( $resultado as $indice => $atributo ) :

				?>

				<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
				<tr bgcolor="<?= $cor ?>"
					onmouseout="this.style.backgroundColor='<?= $cor ?>';"
					onmouseover="this.style.backgroundColor='#ffffcc';">
					<td style="text-align: center;"><?= $atributo['codigo'] ?></td>
					<td style="text-align: left;">&nbsp;</td>
					<!-- <td style="text-align: right;"><?= number_format($atributo['lfxvalor'], 2, ",", ".") ?></td> -->
					<td style="text-align: right;">0,00</td>					
				</tr>
				<?php endforeach; ?>
				<?php else : ?>
				<tr>
					<td
						style="text-align: center; padding: 15px; background-color: #fafafa; color: #404040; font-weight: bold; font-size: 10px;"
						colspan="6">Não há Despesas.</td>
				</tr>
				<?php endif; ?>
				<tr>
					<!-- <td colspan="3" align="center" >TOTAL: <?= number_format($total_assitencia, 2, ",", ".") ?></td> -->
					 <td colspan="3" align="center" >TOTAL: 0,00</td>					
				</tr>

			</table>


			<!-- Despesas variáveis -->

			<!-- Investimento -->

			<?

			$sql_investimento = "
				select
					substr( ndpcod, 1, 2 ) || '.' ||
					substr( ndpcod, 3, 2 ) || '.' ||
					substr( ndpcod, 5, 2 ) || '.' ||
					substr( ndpcod, 7, 2 )
						as codigo,
					ndpdsc as descricao, lan.lantotal as valor
				from naturezadespesa as nat
					inner join reuni.lancamentodesppercent as lan on lan.ndpid = nat.ndpid
					inner join reuni.tipodesp as tipo on tipo.tipid = lan.tipid
				where
					substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
					ndpstatus = 'A' and
					lan.unicod = '".$unicod."' and
					lan.unitpocod = 'U' and 
					tipo.tipcod = '4'
					
				order by ndpcod
			";

			$resultado_investimento = $db->carregar($sql_investimento);
			
			?>
			<table width="100%" align="center" border="1" class="tabela"
				cellpadding="3" cellspacing="0">
				<colgroup>
					<col style="width: 35%;" />
					<col style="width: 30%;" />
					<col style="width: 35%;" />
				</colgroup>

				<thead>
					<tr bgcolor="#dfdfdf">
						<td align="left"  colspan="3"><b>4) - Despesas
						Orçamentárias (Investimento)</b></td>
					</tr>
				</thead>

				<?php if ( !empty( $resultado_investimento ) ) : ?>
				<thead>
					<tr bgcolor="#dfdfdf">
						<td style="text-align: center;"><b>CÓDIGO DE DESPESA -
						ESPECIFICAÇÃO</b></td>
						<td style="text-align: center;"><b>FONTE</b></td>
						<td style="text-align: center;"><b>VALOR ATUAL (R$)</b></td>

					</tr>
				</thead>
				<?php $cor = ''; ?>
				<? $resultado_investimento = is_array( $resultado_investimento ) ? $resultado_investimento : array(); ?>
				<?php foreach ( $resultado_investimento as $indice => $atributo ) :

				?>

				<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
				<tr bgcolor="<?= $cor ?>"
					onmouseout="this.style.backgroundColor='<?= $cor ?>';"
					onmouseover="this.style.backgroundColor='#ffffcc';">
					<td style="text-align: center;"><?= $atributo['codigo'] ?></td>
					<td style="text-align: center;">&nbsp;</td>
					<td style="text-align: right;"><?= number_format($atributo['valor'], 2, ",", ".") ?></td>
				</tr>
				<?php endforeach; ?>	
				<?php else : ?>
				<tr>
					<td
						style="text-align: center; padding: 15px; background-color: #fafafa; color: #404040; font-weight: bold; font-size: 10px;"
						colspan="6">Não há Investimentos.</td>
				</tr>
				<?php endif; ?>
				<tr>
					<td align="center">TOTAL DISPONÍVEL: <br/><?= $item_financeiro_valor_disponivel_investimento ?></td>			
					<td align="center">TOTAL: <br/><?= $item_financeiro_valor_total_investimento ?></td>
					<td align="center">TOTAL LANÇADO: <br/><?= $item_financeiro_valor_lancado_investimento ?></td>
				</tr>
				

			</table>


			<!-- Outras Despesas -->

			<?

			$sql_despesa = "
				select
					substr( ndpcod, 1, 2 ) || '.' ||
					substr( ndpcod, 3, 2 ) || '.' ||
					substr( ndpcod, 5, 2 ) || '.' ||
					substr( ndpcod, 7, 2 )
						as codigo,
					ndpdsc as descricao, lan.lantotal as valor
				from naturezadespesa as nat
					inner join reuni.lancamentodesp as lan on lan.ndpid = nat.ndpid
					inner join reuni.tipodesp as tipo on tipo.tipid = lan.tipid
				where
					substr( ndpcod, 1, 4 ) in ( '3390', '4490' ) and
					ndpstatus = 'A' and
					lan.unicod = '".$unicod."' and
					lan.unitpocod = 'U' and 
					tipo.tipcod = '5'
					
				order by ndpcod
			";
			

			$resultado_despesa = $db->carregar($sql_despesa);
			
		



			$sqp_despesa   = "SELECT SUM(lantotal) FROM reuni.lancamentodesp as lan
						 				WHERE lan.unicod = '".$unicod."' AND lan.tipid = (select tipid from reuni.tipodesp where tipodesp.tipcod = 5) 	
			";
			$total_despesa  = $db->pegaUm($sqp_despesa );

			?>

			<table width="100%" align="center" border="1" class="tabela"
				cellpadding="3" cellspacing="0">
				<colgroup>
					<col style="width: 35%;" />
					<col style="width: 30%;" />
					<col style="width: 35%;" />
				</colgroup>

				<thead>
					<tr bgcolor="#dfdfdf">
						<td align="left" colspan="3"><b>5) - Despesas Orçamentárias
						(Outras Despesas)</b></td>
					</tr>
				</thead>

				<?php if ( !empty( $resultado_despesa ) ) : ?>
				<thead>
					<tr bgcolor="#dfdfdf">
						<td style="text-align: center;"><b>CÓDIGO DE DESPESA -
						ESPECIFICAÇÃO</b></td>
						<td style="text-align: center;"><b>FONTE</b></td>
						<td style="text-align: center;"><b>VALOR ATUAL (R$)</b></td>

					</tr>
				</thead>
				<?php $cor = ''; ?>
				<? $resultado_despesa = is_array( $resultado_despesa ) ? $resultado_despesa : array(); ?>
				<?php foreach ( $resultado_despesa as $indice => $atributo ) :

				?>

				<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
				<!-- <tr bgcolor="<?= $cor ?>"
					onmouseout="this.style.backgroundColor='<?= $cor ?>';"
					onmouseover="this.style.backgroundColor='#ffffcc';">
					<td style="text-align: center;"><?= $atributo['codigo'] ?></td>
					<td style="text-align: center;">&nbsp;</td>
					 <td style="text-align: right;"><?= number_format($atributo['valor'], 2, ",", ".") ?></td>
				</tr> -->
				<?php endforeach; ?>
				<?php else : ?>
				<tr>
					<td
						style="text-align: center; padding: 15px; background-color: #fafafa; color: #404040; font-weight: bold; font-size: 10px;"
						colspan="6">Não há Ourtas Despesas.</td>
				</tr>
				<?php endif; ?>
				<tr>
					<!-- <td align="center">TOTAL DISPONÍVEL: <br/><?= $item_financeiro_valor_disponivel_despesa ?></td>					
					<td align="center">TOTAL: <br/><?= $item_financeiro_valor_total_despesa ?></td>
					<td align="center">TOTAL LANÇADO: <br/><?= $item_financeiro_valor_lancado_despesa?></td> -->
					
					<td align="center">TOTAL DISPONÍVEL: <br/>0,00</td>					
					<td align="center">TOTAL: <br/>0,00</td>
					<td align="center">TOTAL LANÇADO: <br/>0,00</td>
					
				</tr>

			</table>


			<tr bgcolor="#dfdfdf">
				<td colspan="4" align="center" ><u>APROVAÇÃO<u></td>
			</tr>
			<tr>
			<?
				/*$dia = date("d");
				$mes = ucfirst( strtolower( mes_extenso( (integer) date("m") ) ) );    
				$ano = date("Y"); */
			?>
			<!--  <tr>
				<td>
					Brasília-DF, 27 de Dezembro de 2007
				</td>
			</tr>
		    -->
				<td align="center" >					
					<br/>
					<br />
					<br />
					<br />
					<b>Maria Ieda Costa Diniz</b><br />
					<i>Secretária de Educação Superior Substituta</i><br />
					<br />
					<br />					
				</td>
			</tr>
			<tr>
				<td>
					Processo de Concessão n.º
				</td>
			</tr>
			
		</table>

</table>
</form>

