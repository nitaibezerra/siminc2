<?php
set_time_limit(0);
//Constantes
define('LIMIT', 15);

// recebendo o AJAX do scroll
if( isset($_REQUEST["qtdLinhas"]) && isset($_REQUEST["qtdTotal"]) && isset($_REQUEST["classe"]) ){
	header("Content-Type: text/html; charset=ISO-8859-1");
	//When User Scrolls This Query Is Run Start
	$getPostsText = "";
	
	$qtdInicial = $_REQUEST['qtdLinhas'];
	$qtdDiminuida = $_REQUEST['qtdTotal'] - $_REQUEST['qtdLinhas'];
	$qtdExibicao = $qtdDiminuida > LIMIT ? LIMIT : $qtdDiminuida;

	//$qtdExibicao = 5;
	
	$sql = "SELECT
				prgcod, 
				acacod, 
				unicod, 
				loccod 
			FROM 
				monitora.acao 
			WHERE 
				prgano ='{$_SESSION['exercicio']}' 
			AND 
				acasnrap = false 
				{$_SESSION['cods']}		
			ORDER BY prgcod, acacod, unicod, loccod
			LIMIT $qtdExibicao OFFSET $qtdInicial";
	
	$acoes = $db->carregar( $sql );
	
	if( is_array($acoes) ){
		// Pegando o mês atual
		$mes_atual = (int)date('n');
		
		$arDados = array();
		$count = 0;
		foreach($acoes as $key){
			$escreve_conteudo = true;
			for($mes = 1; $mes <= $mes_atual; $mes++){
				$sql = "SELECT
							rofmes::integer, 
							MAX(rofdatainclusao) - interval '1 day' as dataatu, 
							SUM(rofdot_ini) as rofdot_ini, 
							SUM(rofautorizado) as rofautorizado,  
							SUM(rofempenhado) as empenhado,
							SUM(rofliquidado_favorecido) as rofliquidado_favorecido, 
							SUM(rofpago) as rofpago,
							me.exprealizado,
							md.fisqtdeprevistoano as previsto
						 FROM 
							financeiro.execucao fe
						 INNER JOIN monitora.acao ma ON ma.prgcod = fe.prgcod and ma.acacod = fe.acacod and ma.unicod = fe.unicod and ma.loccod = fe.loccod and ma.prgano = fe.rofano
						 INNER JOIN monitora.execucaopto me ON me.acaid = ma.acaid
						 INNER JOIN monitora.referencia r ON r.refcod = me.refcod and r.refmes_ref = '".to_string($mes)."'
						 INNER JOIN monitora.dadofisico md ON md.acaid = me.acaid
						 WHERE 
							fe.prgcod = '{$key['prgcod']}' 
						 AND 
							fe.acacod = '{$key['acacod']}' 
						 AND 
							fe.unicod = '{$key['unicod']}' 
						 AND 
							fe.loccod = '{$key['loccod']}' 
						 AND 
							fe.rofano = '{$_SESSION['exercicio']}'
						 AND
							fe.rofmes = '".to_string($mes)."'
						 GROUP BY ma.acaid,
							fe.rofmes,
							me.exprealizado,
							previsto
						 ORDER BY
							fe.rofmes";

				$dadosfinanceiroscompletos = $db->carregar( $sql );
							
				if(is_array($dadosfinanceiroscompletos)){
					$dadofinanceiro = array();
					foreach($dadosfinanceiroscompletos as $linhafin) {
						$dadofinanceiro['rofautorizado'] 			+= $linhafin['rofautorizado'];
						$dadofinanceiro['rofpago'] 					+= $linhafin['rofpago'];
						$dadofinanceiropormes[$linhafin['rofmes']] 	= array("empenhado" => $linhafin['empenhado'],
																	   "pago" 	   => $linhafin['rofpago']);
					}
					if($escreve_conteudo){
						// isto deverá rodar apenas uma vez a cada linha <tr></tr>
						$previsto 	  					  = ($dadosfinanceiroscompletos[0]['previsto']) ? formata_numero($dadosfinanceiroscompletos[0]['previsto']) : '&nbsp;';
						$arDados[$count]['colunaFixa'] 	  = "{$key['prgcod']}.{$key['acacod']}.{$key['unicod']}.{$key['loccod']}";
						$arDados[$count]['rofautorizado'] = number_format($dadofinanceiro['rofautorizado'], 2, ',', '.');
						$arDados[$count]['previsto'] 	  = $previsto;
						$escreve_conteudo = false;
					}
					
					$exprealizado = ( $dadosfinanceiroscompletos[0]['exprealizado']) == 0 ? '&nbsp' : formata_numero($dadosfinanceiroscompletos[0]['exprealizado']);
						
					// Se for um array significa que há registros naquele mês
					$arDados[$count]['mes'][$mes]['empenhado']    = number_format( $dadofinanceiropormes[1]['empenhado'], 0, ',', '.');
					$arDados[$count]['mes'][$mes]['liquidado']    = number_format( $dadofinanceiropormes[1]['liquidado'], 0, ',', '.');
					$arDados[$count]['mes'][$mes]['pago']     	  = number_format( $dadofinanceiropormes[1]['pago'], 0, ',', '.');
					$arDados[$count]['mes'][$mes]['exprealizado'] = $exprealizado;
					
				} else {
					// não há registros
					if($escreve_conteudo){
						// isto deverá rodar apenas uma vez a cada linha <tr></tr>
						$arDados[$count]['colunaFixa'] 	  = "{$key['prgcod']}.{$key['acacod']}.{$key['unicod']}.{$key['loccod']}";
						$arDados[$count]['rofautorizado'] = "&nbsp;";
						$arDados[$count]['previsto'] 	  = "&nbsp;";
						$escreve_conteudo = false;
					}
					
					$arDados[$count]['mes'][$mes]['empenhado']     	= "0";
					$arDados[$count]['mes'][$mes]['liquidado']     	= "0";
					$arDados[$count]['mes'][$mes]['pago']     	  	= "0";
					$arDados[$count]['mes'][$mes]['exprealizado']	= "&nbsp;";
				} // fim do if
			} // fim do for de mes
		$count++;
		} // fim do foreach de ações
	} // fim do if de ações
	
	echo simec_json_encode($arDados); die;
}

/**
 * Função que monta a condição da consulta
 * @param array $dados
 * @author Rodrigo Pereira de Souza Silva
 * @return string
 */
function condicao($dados){

	$sql = '';
	if(is_array($dados)){

		foreach($dados as $chave => $valores){
				
			if( $valores[0] != "" && is_array($valores) ){
				$sql .= "AND {$chave} IN ( ";
				foreach( $valores as $valor ){ //dbg($valores,1);
					$sql .= "'{$valor}',";
				}
				$sql = substr($sql,0,-1); // removendo a última vírgula inválida
				$sql .= " ) "; // fechando o parênteses

			}// fim do if
		}// fim do foreach
		return $sql;
	}else{
		return false;
	}// fim do if

}

/**
 * Função que monta a tabela do Relatório Físico / Financeiro
 * @author Rodrigo Pereira de Souza Silva
 */
function tabela_relatorio_acao(){
	global $db;

	$condicao = condicao($_POST);

	$sql = "SELECT 
				prgcod, 
				acacod, 
				unicod, 
				loccod 
			FROM 
				monitora.acao 
			WHERE 
				prgano ='{$_SESSION['exercicio']}' 
			AND 
				acasnrap = false 
				{$condicao}
			ORDER BY prgcod, acacod, unicod, loccod
			LIMIT ".LIMIT;
				
	$sql_count = "SELECT
						count(prgcod) as qtd
				  FROM 
						monitora.acao 
				  WHERE 
						prgano ='{$_SESSION['exercicio']}' 
				  AND 
						acasnrap = false 
						{$condicao}";
						
	$qtd = $db->pegaUm($sql_count);
	echo '<input type="hidden" name="qtdTotal" id="qtdTotal" value="'.$qtd.'" />';
				
	//salvando o sql na sessão para que ele possa ser usado durante o scroll function
	$_SESSION['cods'] = $condicao;

				$acoes = $db->carregar( $sql );

				if( is_array($acoes) ){
					
					//quantidade de resultados
					$qtd_total = count($acoes);

					// Pegando o mês atual
					$mes_atual = (int)date('n');
					// contador de cores
					$i = 0;

					// Construindo a tabela
					$cabecalho = '
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">&nbsp;
					<tr>
						<td>
							<div class="fullScreenTable">
								<table>
									<thead>
										<tr>
											<th><b>Programa/Ação/Localizador</b></th>
											<th><b>Dotação Lei + Créditos)</b></th>
											<th><b>Meta Física Estimada</b></th>';

					$topo = '';
					for($mes = 1; $mes <= $mes_atual; $mes++){
						$cabecalho .= '
							  <!--<th><b>'.pega_mes($mes).'/'.$_SESSION['exercicio'].'</b></th>-->
							  <th><b>'.pega_mes($mes).'/'.$_SESSION['exercicio'].' Empenhado</b></th>
							  <th><b>'.pega_mes($mes).'/'.$_SESSION['exercicio'].' Liquidado</b></th>
							  <th><b>'.pega_mes($mes).'/'.$_SESSION['exercicio'].' Pago</b></th>
							  <th><b>'.pega_mes($mes).'/'.$_SESSION['exercicio'].' Meta Física Realizada</b></th>
							  <!--<th><b>Descrição</b></th>-->
							 ';
							
					}
					$cabecalho .= '</tr>
						</thead>';
					$conteudo = '<tbody>';

					// Array que armazenará o total mensal
					$arTotalMes = array();
					foreach($acoes as $key){
						$escreve_conteudo = true;
							
						for($mes = 1; $mes <= $mes_atual; $mes++){
								
							$sql = "SELECT
									--ma.acaid,
									rofmes::integer, 
									MAX(rofdatainclusao) - interval '1 day' as dataatu, 
									SUM(rofdot_ini) as rofdot_ini, 
									SUM(rofautorizado) as rofautorizado,  
									SUM(rofempenhado) as empenhado,
									SUM(rofliquidado_favorecido) as rofliquidado_favorecido, 
									SUM(rofpago) as rofpago,
									me.exprealizado,
									
									md.fisqtdeprevistoano as previsto
									
								 FROM 
									financeiro.execucao fe
									
								 INNER JOIN monitora.acao ma ON ma.prgcod = fe.prgcod and ma.acacod = fe.acacod and ma.unicod = fe.unicod and ma.loccod = fe.loccod and ma.prgano = fe.rofano
								 INNER JOIN monitora.execucaopto me ON me.acaid = ma.acaid
								 INNER JOIN monitora.referencia r ON r.refcod = me.refcod and r.refmes_ref = '".to_string($mes)."'
								 
								 INNER JOIN monitora.dadofisico md ON md.acaid = me.acaid
								 
								 WHERE 
									fe.prgcod = '{$key['prgcod']}' 
								 AND 
									fe.acacod = '{$key['acacod']}' 
								 AND 
									fe.unicod = '{$key['unicod']}' 
								 AND 
									fe.loccod = '{$key['loccod']}' 
								 AND 
									fe.rofano = '{$_SESSION['exercicio']}'
								 AND
									fe.rofmes = '".to_string($mes)."'
								 GROUP BY ma.acaid,
									fe.rofmes,
									me.exprealizado,
									
									previsto
									
								 ORDER BY
									fe.rofmes";

							$dadosfinanceiroscompletos = $db->carregar( $sql );

							if(is_array($dadosfinanceiroscompletos)){ //dbg($sql,1);
								//dbg($dadosfinanceiroscompletos);
									
								$dadofinanceiro = array();
								foreach($dadosfinanceiroscompletos as $linhafin) {
									//$dataatualizada 							= $linhafin['dataatu'];
									$dadofinanceiro['rofdot_ini'] 				+= $linhafin['rofdot_ini'];
									$dadofinanceiro['rofautorizado'] 			+= $linhafin['rofautorizado'];
									$dadofinanceiro['empenhado'] 				+= $linhafin['empenhado'];
									$dadofinanceiro['rofliquidado_favorecido'] 	+= $linhafin['rofliquidado_favorecido'];
									$dadofinanceiro['rofpago'] 					+= $linhafin['rofpago'];
									$dadofinanceiropormes[$linhafin['rofmes']] 	= array("empenhado" => $linhafin['empenhado'],
																				   "liquidado" => $linhafin['rofliquidado_favorecido'],
																				   "pago" 	   => $linhafin['rofpago']);
								}
								$dadofinanceiropormes['totalautorizado'] = $dadofinanceiro['rofautorizado'];
								//dbg($dadofinanceiro,1);
								if($escreve_conteudo && $i < LIMIT){
									// isto deverá rodar apenas uma vez a cada linha <tr></tr>
									$conteudo .= "<tr>
												  <!-- Informações -->
												  <td><b>{$key['prgcod']}.{$key['acacod']}.{$key['unicod']}.{$key['loccod']}</b></td><!-- Programa/Ação/Localizador -->
												  <td>".number_format($dadofinanceiro['rofautorizado'], 2, ',', '.')."</td><!-- Dotação Lei + Créditos) -->
												  <td>".( ($dadosfinanceiroscompletos[0]['previsto']) ? formata_numero($dadosfinanceiroscompletos[0]['previsto']) : '&nbsp;' )."</td><!-- Meta Física Estimada -->
											 ";

									$arTotalMes[0]['rofautorizado'] 	+= $dadofinanceiro['rofautorizado'];
									$arTotalMes[0]['previsto'] 			+= $dadosfinanceiroscompletos[0]['previsto'];
									$escreve_conteudo = false;
									$i++;
								}
									
								$arTotalMes[$mes]['empenhado'] 		+= $dadofinanceiropormes[1]['empenhado'];
								$arTotalMes[$mes]['liquidado'] 		+= $dadofinanceiropormes[1]['liquidado'];
								$arTotalMes[$mes]['pago'] 	   		+= $dadofinanceiropormes[1]['pago'];
								$arTotalMes[$mes]['realizada'] 		+= $dadosfinanceiroscompletos[0]['exprealizado'];
									
								// Se for um array significa que há registros naquele mês
								$conteudo .= '
										  <!-- mês '.pega_mes($mes).' -->
										  <td>'.number_format( $dadofinanceiropormes[1]['empenhado'], 0, ',', '.').'</td>
										  <td>'.number_format( $dadofinanceiropormes[1]['liquidado'], 0, ',', '.').'</td>
										  <td>'.number_format( $dadofinanceiropormes[1]['pago'], 0, ',', '.').'</td>
										  <td>'.( $dadosfinanceiroscompletos[0]['exprealizado'] == 0 ? '&nbsp' : formata_numero($dadosfinanceiroscompletos[0]['exprealizado'])).'</td><!-- Meta Física Realizada -->
										  <!--<td>BLA</td> Descrição -->
										 ';
							}else{
								// não há registros
								if($escreve_conteudo  && $i < LIMIT){
									// isto deverá rodar apenas uma vez a cada linha <tr></tr>
									$conteudo .= "<tr>
												  <!-- Informações -->
												  <td><b>{$key['prgcod']}.{$key['acacod']}.{$key['unicod']}.{$key['loccod']}</b></td><!-- Programa/Ação/Localizador -->
												  <td>&nbsp;</td><!-- Dotação Lei + Créditos) -->
												  <td>&nbsp;</td><!-- Meta Física Estimada -->
											 ";
									$escreve_conteudo = false;
									$i++;
								}
									
								$conteudo .= '
										  <!-- mês '.pega_mes($mes).' -->
										  <td>0</td>
										  <td>0</td>
										  <td>0</td>
										  <td>&nbsp;</td><!-- Meta Física Realizada -->
										  <!--<td>BLA</td> Descrição -->
										 ';
							}// fim do if

						}// fim do for
							
						$conteudo .= '</tr>';

					}// fim do foreach

					// montando a tabela
					$tabela =  $cabecalho;
					$tabela .= $conteudo;
					$tabela .= '</tbody>';
					$tabela .= '</table>'; // fechando a tabela
					$tabela .= '</div>'; // fim do div do conteudo .FullScreenTable
					$tabela .= '</td>';
					$tabela .= '</tr>';
					$tabela .= '</table>';

					//escrevendo a div do total
					$tabela .= '<div id="total">
						<table id="footer">
							<thead>
								<tr>
									<th><div><p><b>Total:</b></p></div></th>
									<th><div><p><b>'.( $arTotalMes[0]['rofautorizado'] 	== 0 ? 0 : number_format($arTotalMes[0]['rofautorizado'], 2, ',', '.')).'</b></p></div></th>
									<th><div><p><b>'.( $arTotalMes[0]['previsto'] 		== 0 ? 0 : number_format($arTotalMes[0]['previsto'], 0, ',', '.')).'</b></p></div></th>';
					for($mes = 1; $mes <= $mes_atual; $mes++){
						//			$tabela .= '<th><div><p><b>'.( $arTotalMes[$mes]['rofautorizado'] 	== 0 ? 0 : number_format($arTotalMes[$mes]['rofautorizado'], 0, ',', '.')).'</b></p></div></th>';
						//			$tabela .= '<th><div><p><b>'.( $arTotalMes[$mes]['previsto'] 		== 0 ? 0 : number_format($arTotalMes[$mes]['previsto'], 0, ',', '.')).'</b></p></div></th>';
						$tabela .= '<th><div><p><b>'.( $arTotalMes[$mes]['empenhado'] 		== 0 ? 0 : number_format($arTotalMes[$mes]['empenhado'], 0, ',', '.')).'</b></p></div></th>';
						$tabela .= '<th><div><p><b>'.( $arTotalMes[$mes]['liquidado'] 		== 0 ? 0 : number_format($arTotalMes[$mes]['liquidado'], 0, ',', '.')).'</b></p></div></th>';
						$tabela .= '<th><div><p><b>'.( $arTotalMes[$mes]['pago'] 			== 0 ? 0 : number_format($arTotalMes[$mes]['pago'], 0, ',', '.')).'</b></p></div></th>';
						$tabela .= '<th><div><p><b>'.( $arTotalMes[$mes]['realizada'] 		== 0 ? 0 : number_format($arTotalMes[$mes]['realizada'], 0, ',', '.')).'</b></p></div></th>';
					}
					$tabela .= '	</tr>
					</thead>
				</table>
			</div>
			<div id="loader">
			</div>';



				}else{
					//se $acoes não for uma array significa que nada foi encontrado
					$tabela = '<table cellspacing="1" cellpadding="3" bgcolor="#ffffff" align="center" class="tabela">
					<tr>
						<td style="text-align: center; color: rgb(238, 0, 0);">Não foram encontrados registros.</td>
					</tr>
				   </table>';
				}

				return $tabela;

}

/**
 * Função que exibe o formulário de filtro do relatório
 * @return string
 * @author Rodrigo Pereira de Souza Silva
 */
function formulario_relatorio_acao(){
	$sql_prgcod = "SELECT
						prgcod as codigo,
						prgcod || ' - ' || prgdsc as descricao 
					FROM 
						monitora.programa
					WHERE
						prgano = '{$_SESSION['exercicio']}'
					AND
						prgstatus = 'A'";

	$sql_acacod = "SELECT
						acacod as codigo,
						acacod || ' - ' || acadsc as descricao 
					FROM 
						monitora.acao
					WHERE
						prgano = '{$_SESSION['exercicio']}'
					AND
						acastatus = 'A'";

	$sql_unicod = "SELECT
						unicod as codigo,
						unicod || ' - ' || unidsc as descricao 
					FROM 
						public.unidade
					WHERE
						unistatus = 'A'
					AND 
						unicod ilike '26%'";

	$sql_loccod = "SELECT
						loccod as codigo,
						loccod || ' - ' || locdsc as descricao 
					FROM 
						public.localizador";
	?>
<script type="text/javascript">
		function validaFormulario() {
			
			selectAllOptions( frm.prgcod );
			selectAllOptions( frm.acacod );
			selectAllOptions( frm.unicod );
			selectAllOptions( frm.loccod );

			if( (frm.prgcod.value != "") || (frm.acacod.value != "") || (frm.unicod.value != "") || (frm.loccod.value != "") ){
				document.frm.submit();
				return true;
			}else{
				alert('Preencha uma das opções.');
				return false;
			}
			
		}
	</script>
<form name="frm" method="post"
	action="monitora.php?modulo=principal/acao/relatorioacao&acao=A">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td align="right" class="SubTituloDireita">Programa:</td>
		<td><?php combo_popup( "prgcod", $sql_prgcod, "Programa", "400x400", 0, array(), "", "S", false, false, 5, 400 ); ?>
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita">Ação:</td>
		<td><?php combo_popup( "acacod", $sql_acacod, "Ação", "400x400", 0, array(), "", "S", false, false, 5, 400 ); ?>
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita">Unidade:</td>
		<td><?php combo_popup( "unicod", $sql_unicod, "Unidade", "400x400", 0, array(), "", "S", false, false, 5, 400 ); ?>
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita">Localizador:</td>
		<td><?php combo_popup( "loccod", $sql_loccod, "Localizador", "400x400", 0, array(), "", "S", false, false, 5, 400 ); ?>
		</td>
	</tr>
	<tr>
		<td><input type="button" value="Pesquisar"
			onclick="javascript:validaFormulario();" /></td>
		<td>&nbsp;</td>
	</tr>
	<tr>
		<td><input type="hidden" name="requisicao" value="pesquisar"></td>
	</tr>
</table>
</form>
	<?php

}

/**
 * Função que converte um número inteiro para string
 * Se o número for menor que 10 ele retorna o número com 0 a esquerda
 * @param $n integer
 * @return string
 * @author Rodrigo Pereira de Souza Silva
 */
function to_string($n = 0){

	if($n < 10){
		return '0'.$n;
	}else{
		return (string)$n;
	}

}

/**
 * Função que escreve o nome do mês de acordo com o número passado por parâmetro
 * @param integer $mes
 * @return string
 * @author Rodrigo Pereira de Souza Silva
 */
function pega_mes($mes = 0){
	switch ($mes) {
		case 1:
			return 'Jan';
			break;

		case 2:
			return 'Fev';
			break;

		case 3:
			return 'Mar';
			break;

		case 4:
			return 'Abr';
			break;

		case 5:
			return 'Mai';
			break;

		case 6:
			return 'Jun';
			break;

		case 7:
			return 'Jul';
			break;

		case 8:
			return 'Ago';
			break;

		case 9:
			return 'Set';
			break;

		case 10:
			return 'Out';
			break;

		case 11:
			return 'Nov';
			break;

		case 12:
			return 'Dez';
			break;

		default:
			return 'false';
			break;
	}
}

// Começando a construir a página
include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Relatório Físico / Financeiro';
monta_titulo($titulo_modulo,'');

if( $_POST['requisicao'] != 'pesquisar' ){
	//// Senão eu exibo o formulário de filtro
	formulario_relatorio_acao();
}else{
	formulario_relatorio_acao();
	// Se algum dado for enviado via POST então eu exibo a tabela
	echo tabela_relatorio_acao();
	?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-fixedheadertable/fixedHeaderTable.css" />
<style type="text/css">
div.fullScreenTable {
	width: 1300px;
	height: 350px;
	overflow: hidden;
	float: left;
}
</style>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-fixedheadertable/jquery.fixedheadertable.1.0.js"></script>
<script type="text/javascript">
			$(document).ready(function() {				

				var colunaFixa = true;
				
				/**
				* fixCol1 				-> deixa sempre visível a primeira coluna da tabela
				* colBorder 			-> coluna com ou sem borda
				* footer				-> Tabela com ou sem rodapé
				* footerId  			-> id do elemento que irá se tornar o rodapé da tabela
				* footerScroll			-> rodapé com ou sem scroll
				* cloneHeaderToFooter	-> clona o cabeçalho da tabela no rodapé
				*/
				//simples
				//$('.fullScreenTable').fixedHeaderTable();
				//com a primeira coluna fixa
				//$('.fullScreenTable').fixedHeaderTable({fixCol1:true});
				//rodapé com o total e com scroll
				//$('.fullScreenTable').fixedHeaderTable({colBorder: true, footer:true, footerId:'total', footerScroll:true});
				//rodapé com o total e com scroll e primeira coluna fixa
				$('.fullScreenTable').fixedHeaderTable({fixCol1:colunaFixa, colBorder:true, footer:true, footerId:'total', footerScroll:true});
				//rodapé com paginação
				//$('.fullScreenTable').fixedHeaderTable({colBorder: true, footer:true, footerId:'paginacao'});
				//clonando o cabeçalho
				//$('.fullScreenTable').fixedHeaderTable({colBorder: true, footer:true, cloneHeaderToFooter:true});
				//clonando o cabeçalho e primeira coluna fixa
				//$('.fullScreenTable').fixedHeaderTable({fixCol1:true, colBorder: true, footer:true, cloneHeaderToFooter:true});

				$('.fht_table_body').scroll(function(){
	
					var scroll_tam = $('.fht_table_body').attr("scrollHeight");
					var max = $('.fht_table_body').attr("scrollHeight") - 350;

					if ( max <= $('.fht_table_body').scrollTop() && $('.fht_table_body tbody tr').length < $('#qtdTotal').val()  ){
						lastPostFunc( $('.fht_table_body tbody tr').length, $('#qtdTotal').val()  );
					}
				});

				var nrTotalLinhas = 0;
				function lastPostFunc( qtdLinhas, qtdTotal ){

					// desabilitando o scroll para que o usuário não envie requisições
					if( nrTotalLinhas == qtdLinhas ){
						return false;
					}
					
					// pegando a classe da ultima td
					if( $(".fht_orig_table tbody td:last").hasClass("even") ){
						var classe = 1;
					}else{
						var classe = 2;
					}

					// exibindo a imagem do carregando
					divCarregando( $('.fht_orig_table')[0] );
					
					nrTotalLinhas = qtdLinhas;

					$.post('<?php echo $_SERVER['HTTP_REFERER']; ?>', '&qtdLinhas='+qtdLinhas+'&qtdTotal='+qtdTotal+'&classe='+classe, function(data, textStatus) {
						//textStatus contains the status: success, error, etc
						if(textStatus == 'success'){
							$.each(data, function(i,item){
								if(classe % 2){
								 	var corLinha = "odd borderRight"; 
								} else {
									var corLinha = "even borderRight";
								}
								
								if(colunaFixa){ // Verifica se foi definido coluna fixa
									if(data[i].colunaFixa){
										var htmlColFixa = "<tr>";
											htmlColFixa += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'><strong> "+ data[i].colunaFixa +"  </strong></p></div></td><!-- Programa/Ação/Localizador -->";
											htmlColFixa += "</tr>";
									
										$(".fht_fixed_col_body tbody tr:last").after( htmlColFixa );
		
										var htmlColBody = "<tr>";
											htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].rofautorizado +" </p></div></td><!-- Programa/Ação/Localizador -->";
											htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].previsto +" </p></div></td><!-- Programa/Ação/Localizador -->";
											
											$.each(data[i].mes, function(j){
												htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].mes[j].empenhado +" </p></div></td><!-- Programa/Ação/Localizador -->";
												htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].mes[j].liquidado +" </p></div></td><!-- Programa/Ação/Localizador -->";
												htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].mes[j].pago +" </p></div></td><!-- Programa/Ação/Localizador -->";
												htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].mes[j].exprealizado +" </p></div></td><!-- Programa/Ação/Localizador -->";
											});
		
											htmlColBody += "</tr>";
	
											classe++;
										
										$(".fht_orig_table tbody tr:last").after(htmlColBody);
									}
								} else { // se nao for coluna fixa
									if(data[i].colunaFixa){
										var htmlColBody = "<tr>";
											htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'><strong> "+ data[i].colunaFixa +"  </strong></p></div></td><!-- Programa/Ação/Localizador -->";
											htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].rofautorizado +" </p></div></td><!-- Programa/Ação/Localizador -->";
											htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].previsto +" </p></div></td><!-- Programa/Ação/Localizador -->";
											
											$.each(data[i].mes, function(j){
												htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].mes[j].empenhado +" </p></div></td><!-- Programa/Ação/Localizador -->";
												htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].mes[j].liquidado +" </p></div></td><!-- Programa/Ação/Localizador -->";
												htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].mes[j].pago +" </p></div></td><!-- Programa/Ação/Localizador -->";
												htmlColBody += "<td class=\""+corLinha+"\"><div class='tableData'><p class='tableData'> "+ data[i].mes[j].exprealizado +" </p></div></td><!-- Programa/Ação/Localizador -->";
											});
		
											htmlColBody += "</tr>";
	
											classe++;
										
										$(".fht_orig_table tbody tr:last").after(htmlColBody);
									}
								}
					        }); // fim do each
					        //removendo a div carregando
							divCarregado();
						}
						
					}, 'json');
					
				};
			});

				
			$('.fullScreenTable').find('table tbody tr')
				.live('mouseover',function(){
					$('td', this).addClass('highlight');
			    })
			    .live('mouseout',function(){
			    	$('td', this).removeClass('highlight');
			    });
			
	</script>

	<?php

}
?>
<div id="divdebug"></div>