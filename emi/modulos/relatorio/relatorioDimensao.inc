<?php 

if ( $_REQUEST["estuf"] ) {
	
	if ( !emiVerificaUf( $_REQUEST["estuf"] ) ){
		
		print "<script>
				  alert('O estado selecionado não existe ou está em branco.');
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

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

monta_titulo("Ensino Médio Inovador", "Relatório de Dimensão");

if(!$_REQUEST["estuf"]) {
	
	$emi->listaEstados( "dimensao" ); 

} else {
	if( $_SESSION["emi"]["emeidPai"] ) {
		$emi->montaCabecalho( $_SESSION["emi"]["emeidPai"], "secretaria" );
	}

	//$tppid = EMI_TIPO_ENTIDADE_ESCOLA;
	$tppid = EMI_TIPO_ENTIDADE_SEC;
	
	// Recuperando as 'Dimensões'
	$sql = "SELECT
				emdid as id,
				dimcod as codigo,
				dimdsc as descricao
			FROM 
				emi.emdimensao ed
			INNER JOIN 
				cte.dimensao cd ON ed.dimid = cd.dimid";
	$dadosDimensao = $db->carregar( $sql );
	
	if ( $dadosDimensao ) {
		echo '<table class="listagem" width="95%" bgcolor="#f5f5f5" height="100%" cellspacing="8" cellpadding="8" align="center" border=0>';
		
		for( $i = 0; $i < count( $dadosDimensao ); $i++ ) {
			
			$cor = "#c0c0c0";
			
			echo '<tr bgcolor="'.$cor.'">
					<td><b>'.$dadosDimensao[$i]["codigo"].' '.$dadosDimensao[$i]["descricao"].'</b></td>
				  </tr>';
			
			// Recupera as 'Linhas de Ação'
			$sql = "SELECT 
						lacid as id, 
						laccod as codigo, 
						lacdsc as descricao
					FROM 
						emi.emlinhaacao
					WHERE
						emdid = {$dadosDimensao[$i]["id"]} AND
						tppid = {$tppid}";
			$dadosLinhaAcao = $db->carregar( $sql );
			
			if ( $dadosLinhaAcao ) {
				for( $j = 0; $j < count( $dadosLinhaAcao ); $j++ ) {
					
					$cor = "#e0e0e0";
					
					echo '<tr bgcolor="'.$cor.'">
							<td><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosLinhaAcao[$j]["codigo"].' '.$dadosLinhaAcao[$j]["descricao"].'</b></td>
						  </tr>';
					
					// Recupera os 'Componentes'
					$sql = "SELECT
								comid as id,
								comcod as codigo,
								comdsc as descricao
							FROM
								emi.emcomponentes
							WHERE
								lacid = {$dadosLinhaAcao[$j]["id"]}
							ORDER BY
								codigo";
					$dadosComponentes = $db->carregar( $sql );
		
					if ( $dadosComponentes ) {
						for( $k = 0; $k < count( $dadosComponentes ); $k++ ) {
							
							$cor = ( $k % 2 ) ? "#f0f0f0" : "#f8f8f8";
							
							echo '<tr bgcolor="'.$cor.'">
									<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosComponentes[$k]["codigo"].' '.$dadosComponentes[$k]["descricao"];
							
							// PAPS
							$sql = "SELECT
										papid as id,
										trim(papcaoatividade) as atividade,
										trim(papmeta) as meta
									FROM
										emi.empap
									WHERE
										comid = {$dadosComponentes[$k]["id"]} AND
										emeid = {$_SESSION["emi"]["emeidPai"]} AND
										papstatus = 'A'";
							$dadosPap = $db->carregar( $sql );
							
							if ( $dadosPap ) {
								
								echo '<center>
								<br />
											<table class="listagem" width="90%" cellspacing="0" cellpadding="4">
												<tr>
													<th width="50%">Ação/Atividade</th>
													<th width="50%">Meta</th>
												</tr>';
								
								for( $l = 0; $l < count( $dadosPap ); $l++ ) {
									echo '<tr>
										  	<td style="border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc; color:#cc0000;">'.$dadosPap[$l]["atividade"].'</td>
										  	<td style="border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc; color:#cc0000;">'.$dadosPap[$l]["meta"].'</td>
										  </tr>';
								}
								
								echo '</table>
								<br />
								</center>
									</td>
								</tr>';
							}
						}
					}
				}
			}
		}
		
		echo "</table>";
	}
}

?>