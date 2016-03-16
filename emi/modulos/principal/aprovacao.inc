<?php 

if ( $_REQUEST["submetido"] )
{
	
	if($_REQUEST["itens_aprovados"])
	{
		for($i=0; $i<count($_REQUEST["itens_aprovados"]); $i++)
		{
			$sql = "UPDATE emi.empap SET papflagaprovado = 'f' WHERE papexercicio = '{$_SESSION['exercicio']}' AND papid = ".$_REQUEST["itens_aprovados"][$i];
			$db->executar($sql);
		}
	}
	
	for($i=0; $i<count($_REQUEST["papflagaprovado"]); $i++)
	{
		$sql = "UPDATE emi.empap SET papflagaprovado = 't' WHERE papexercicio = '{$_SESSION['exercicio']}' AND papid = ".$_REQUEST["papflagaprovado"][$i];
		$db->executar($sql);
	}
	
	$db->commit();
	echo '<script>
			alert("Dados gravados com sucesso.");
		  </script>';
}

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

monta_titulo("Ensino Médio Inovador", "Aprovação");

if(!$_REQUEST["estuf"]) {
	
	$emi->listaEstados( "aprovacao" ); 

} else {
	if( $_SESSION["emi"]["emeidPai"] ) {
		$emi->montaCabecalho( $_SESSION["emi"]["emeidPai"], "secretaria" );
	}

	echo '<form method="post" action="" id="formAprovacao">
			<input type="hidden" name="submetido" value="1" />
			<input type="hidden" name="estuf" value="'.$_REQUEST["estuf"].'" />';
	
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
										trim(papmeta) as meta,
										papflagaprovado
									FROM
										emi.empap
									WHERE
										comid = {$dadosComponentes[$k]["id"]} AND
										emeid = {$_SESSION["emi"]["emeidPai"]} AND
										papexercicio = '{$_SESSION['exercicio']}' AND 
										papstatus = 'A'";
							$dadosPap = $db->carregar( $sql );
							
							if ( $dadosPap ) {
								
								echo '<center>
								<br />
											<table class="listagem" width="90%" cellspacing="0" cellpadding="4">
												<tr>
													<th width="40%">Ação/Atividade</th>
													<th width="40%">Meta</th>
													<th width="20%">Aprovação</th>
												</tr>';
								
								for( $l = 0; $l < count( $dadosPap ); $l++ ) {
									if($dadosPap[$l]["papflagaprovado"]=='t')
									{
										$papAprovado = '<input type="hidden" name="itens_aprovados[]" value="'.$dadosPap[$l]["id"].'" />';
									}
									
									echo '<tr>
										  	<td style="border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc; color:#cc0000;">'.$dadosPap[$l]["atividade"].'</td>
										  	<td style="border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc; color:#cc0000;">'.$dadosPap[$l]["meta"].'</td>
										  	<td style="text-align: center; border-collapse: collapse; border-right: 1px solid #ccc; border-bottom: 1px solid #ccc; color:#cc0000;">
										  		<input type="checkbox" name="papflagaprovado[]" value="'.$dadosPap[$l]["id"].'" '.(($dadosPap[$l]["papflagaprovado"]=='t') ? 'checked="checked"' : '').' />
										  		'.$papAprovado.'
										  	</td>
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
		
		echo "<tr>
				<td style=\"background-color: #c0c0c0; text-align: center;\">
					<input type=\"button\" value=\"Salvar Aprovação\" id=\"btSubmete\" onclick=\"submete();\" />
				</td>
			</tr>
			</table>";
	}
	
	echo '</form>
	
	<script>
		function submete()
		{
			var form 		= document.getElementById("formAprovacao");
			var bt 			= document.getElementById("btSubmete");
			var aprovado 	= document.getElementsByName("papflagaprovado[]");
			var flag		= false;
			
			bt.disabled = true;
			
			for(var i=0; i<aprovado.length; i++)
			{
				if(aprovado[i].checked) flag = true;
			}
			
			if(!flag)
			{
				alert("Pelo menos uma opção deve ser selecionada.");
				bt.disabled = false;
				return;
			}
			form.submit();
		}
	</script>';
}

?>