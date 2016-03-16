<?php
 
	/*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Fernando Bagno (fernandobagno@gmail.com)
   atualização: Victor Benzi (mcbenzi@gmail.com) - 11/12/2009
    */


if ( $_REQUEST["estuf"] ){
	
	if ( !emiVerificaUf( $_REQUEST["estuf"] ) ){
		
		print "<script>
				  alert('O estado selecionado não existe ou está em branco!');
				  history.back(-1);
			  </script>";
		die;
		
	}
	
	$dadosSecretaria = emiBuscaDadosSecretaria( $_REQUEST["estuf"] );
	
	unset( $_SESSION["emi"] );
	
	$_SESSION["emi"]["estuf"]	 = $_REQUEST["estuf"];
	$_SESSION["emi"]["entidpai"] = $dadosSecretaria["id"];
	$_SESSION["emi"]["nomepai"]  = $dadosSecretaria["nome"];
	
	$emi->InsereEntidade( $dadosSecretaria );
	
}


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

monta_titulo("Previsão Orçamentária - Exercício {$_SESSION['exercicio']}", "");

if ( empty($_REQUEST["estuf"]) ){
	
	$emi->listaEstados( "previsao" ); 

}else{

	$escolas = emiBuscaEscolasCadastradas( $_SESSION["emi"]["emeidPai"] );

	if ( $escolas ){
?>
		
		<table class="tabela" bgcolor="#f5f5f5" height="100%" cellspacing="2" cellpadding="4" align="center" border=0>
			<tr>
				<td width="20%" colspan="" class="subtitulodireita">Proponentes:</td>
				<td width="60%" colspan="2">
					<?php echo $_SESSION["emi"]["nomepai"]; ?>
				</td>
				<td class="subtitulodireita">Período de Execução:</td>
				<td>
					<?php echo $_SESSION["exercicio"] ?>
				</td>
			</tr>
		</table>
		<table class="tabela" bgcolor="#f5f5f5" height="100%" cellspacing="2" cellpadding="4" align="center" border=0>
			<?php
			
			echo "<tr>";
			
			// lista os itens
				$sql = "SELECT 
							itfid as codigo,
							itfdsc as descricao 
						FROM 
							emi.emitemfinanciavel 
						ORDER BY 
							itfdsc";
				
				$itensFinanciaveis = $db->carregar( $sql );
				
				echo "<td width='15%' class='subtitulocentro'> Itens Financiáveis </td>";
				for( $i = 0; $i < count($itensFinanciaveis); $i++ ){
					
					$cor = ( $i % 2 ) ? "#e0e0e0" : "#f4f4f4";
					
					echo "<td class='subtitulocentro'> {$itensFinanciaveis[$i]["descricao"]} </td>";
				}
				echo "<td class='subtitulocentro'>Total</td>";
				echo "<td class='subtitulocentro'>Custeio</td>";
				echo "<td class='subtitulocentro'>Capital</td>";
				echo "</tr>";
				echo "<tr bgColor='#e0e0e0'><td>SEE</td>";
				
				
				//se for SEE
				for( $i = 0; $i < count($itensFinanciaveis); $i++ ){
				$totalSec = '';
				$sql = "SELECT
								sum(mdototal) as total
							FROM
								 emi.emmatrizdistribuicaoorcamentar em
							INNER JOIN
								emi.empap ep ON ep.papid = em.papid 
							WHERE
								ep.emeid = {$_SESSION["emi"]["emeidPai"]} AND
								itfid = {$itensFinanciaveis[$i]["codigo"]} AND
								mdostatus = 'A' AND
								papexercicio = '{$_SESSION['exercicio']}' AND
								papstatus = 'A'";
					
					$totalSec = $db->pegaUm( $sql );
					
					$totalGeralItem = '';
					
					if ( !empty($totalSec) ){
						
						$totalGeralSec  = $totalGeralSec + $totalSec;
						$totalGeralItem = $totalSec;
						
					}
					//capital
					if($itensFinanciaveis[$i]["codigo"] == 9 || $itensFinanciaveis[$i]["codigo"] == 10){
						$totalCapital = $totalCapital + $totalSec;
					} else {//custeio
						$totalCusteio = $totalCusteio + $totalSec;
					}
					
					$totalSecFinal[$i] = $totalSec;
					$totalSec = !empty($totalSec) ? "R$ " . number_format($totalSec,2,',','.') : '-';
					
					echo "    <td align='right'>{$totalSec}</td>";
				}
				$totalCusteioSec = $totalCusteio;
				$totalCapitalSec = $totalCapital;
				$totalGeralSec = !empty($totalGeralSec) ? "R$ " . number_format($totalGeralSec,2,',','.') : '-';
				$totalCusteio = !empty($totalCusteio) ? "R$ " . number_format($totalCusteio,2,',','.') : '-';
				$totalCapital = !empty($totalCapital) ? "R$ " . number_format($totalCapital,2,',','.') : '-';
				echo "<td align='right' style='white-space: nowrap' bgcolor='#C0C0C0'><b>{$totalGeralSec}</b></td>";		
				echo "<td align='right' style='white-space: nowrap' bgcolor='#C0C0C0'><b>{$totalCusteio}</b></td>";		
				echo "<td align='right' style='white-space: nowrap' bgcolor='#C0C0C0'><b>{$totalCapital}</b></td>";				
				echo "</tr>";
				
				//se forem Escolas
				
				
				for( $j = 0; $j < count($escolas); $j++ ){
					$totalGeralItem = '';
					$totalEsc = '';
					$cor = ( $j % 2 ) ? "#e0e0e0" : "#f4f4f4";
					echo "<tr bgColor='{$cor}'><td>{$escolas[$j]["entnome"]}</td>";
					for( $i = 0; $i < count($itensFinanciaveis); $i++ ){
						// busca os totais das Escolas
						
						$sql = "SELECT
									sum(mdototal) as total
								FROM
									 emi.emmatrizdistribuicaoorcamentar em
								INNER JOIN
									emi.empap ep ON ep.papid = em.papid 
								WHERE
									ep.emeid = {$escolas[$j]["emeid"]} AND
									itfid = {$itensFinanciaveis[$i]["codigo"]} AND
									mdostatus = 'A' AND
									papexercicio = '{$_SESSION['exercicio']}' AND
									papstatus = 'A'";
						
						$totalEsc = $db->pegaUm( $sql );
						
						if ( !empty($totalEsc) ){
						
							$totalGeralItem  = $totalGeralItem + $totalEsc;
							$totalGeralItem2 = $totalGeralItem2 + $totalGeralItem;
						}
						
						//capital
						if($itensFinanciaveis[$i]["codigo"] == 9 || $itensFinanciaveis[$i]["codigo"] == 10 || $itensFinanciaveis[$i]["codigo"] == 11){
							$totalCapitalEsc = $totalCapitalEsc + $totalEsc;
						} else {//custeio
							$totalCusteioEsc = $totalCusteioEsc + $totalEsc;
						}
						
						$totalEscFinal[$i] = $totalEscFinal[$i] + $totalEsc;
						
						$sql = "SELECT
									sum(mdototal) as total
								FROM
									 emi.emmatrizdistribuicaoorcamentar em
								INNER JOIN
									emi.empap ep ON ep.papid = em.papid 
								WHERE
									ep.emeid = {$escolas[$j]["emeid"]} AND
									mdostatus = 'A' AND
									papexercicio = '{$_SESSION['exercicio']}' AND
									papstatus = 'A'";
								
						$totalGeralEsc[$j] = $db->pegaUm( $sql );
						
						
						$totalEsc = !empty($totalEsc) ? "R$ " . number_format($totalEsc,2,',','.') : '-';
						
						echo "    <td align='right'> {$totalEsc} </td>";
					
					}
					$totalCusteioEscFinal = $totalCusteioEscFinal + $totalCusteioEsc;
					$totalCapitalEscFinal = $totalCapitalEscFinal + $totalCapitalEsc;
					$totalGeralItem = !empty($totalGeralItem) ? "R$ " . number_format($totalGeralItem,2,',','.') : '-';
					$totalCusteioEsc = !empty($totalCusteioEsc) ? "R$ " . number_format($totalCusteioEsc,2,',','.') : '-';
					$totalCapitalEsc = !empty($totalCapitalEsc) ? "R$ " . number_format($totalCapitalEsc,2,',','.') : '-';
					echo "<td align='right' style='white-space:nowrap' bgcolor='#C0C0C0'><b>{$totalGeralItem}</b></td>";
					echo "<td align='right' style='white-space:nowrap' bgcolor='#C0C0C0'><b>{$totalCusteioEsc}</b></td>";
					echo "<td align='right' style='white-space:nowrap' bgcolor='#C0C0C0'><b>{$totalCapitalEsc}</b></td>";				
				}
				echo "</tr>";
				echo "    <tr bgcolor='#C0C0C0'><td><b>Total</b></td>";
				for( $j = 0; $j < count($itensFinanciaveis); $j++ ){
					$total[$j] = $totalSecFinal[$j] + $totalEscFinal[$j];
					$var2 = $total[$j];
					$totalGeral = $totalGeral + $var2;
					$total[$j] = !empty($total[$j]) ? "R$ " . number_format($total[$j],2,',','.') : '-';
					echo "<td align='right'><b>{$total[$j]}</b></td>";
				}
				$totalGeral = !empty($totalGeral) ? "R$ " . number_format($totalGeral,2,',','.') : '-';
				$totalCapitalGeral = $totalCapital + $totalCapitalEsc;
				$totalCapitalGeral = !empty($totalCapitalGeral) ? "R$ " . number_format($totalCapitalGeral,2,',','.') : '-';
				$totalCusteioFinal = $totalCusteioSec + $totalCusteioEscFinal;
				$totalCusteioFinal = !empty($totalCusteioFinal) ? "R$ " . number_format($totalCusteioFinal,2,',','.') : '-';
				$totalCapitalFinal = $totalCapitalSec + $totalCapitalEscFinal;
				$totalCapitalFinal = !empty($totalCapitalFinal) ? "R$ " . number_format($totalCapitalFinal,2,',','.') : '-';
				echo "<td align='right'><b>{$totalGeral}</b></td>";
				echo "<td align='right'><b>{$totalCusteioFinal}</b></td>";
				echo "<td align='right'><b>{$totalCapitalFinal}</b></td>";
			?>
		</table>
		
<?php }else{ ?>
		
		<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
			<tr>
				<td style='color:red;' align='center'>
					Esta secretaria não possui dados orçamentários cadastrados!
				</td>
			</tr>
		</table>
		
	<?php 
	} 		
}
 
?>