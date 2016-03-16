<?php
include "planotrabalho/_funcoes.php";

$pi = $_REQUEST['pi'];

if ( !empty($pi) && $_REQUEST['tipo'] != 2 ){
	$pi = str_replace('\\', '', $_REQUEST['pi']);
	$pi_array = explode(',', $pi);
}

if ( $_REQUEST['tipo'] == 2 ){
//	$pi = "'" . $pi . "'";
}

if ( $_REQUEST['submete_observacao'] == 1 ){
	if (count($pi_array) > 0){
		foreach ( $pi_array AS $valor ){
			
			$sql = "";
			$sql = "INSERT INTO
						monitora.planointernohistorico( plicod, pihobs, pihsituacao, pihdata, usucpf )
					VALUES
						( '".$valor."', '{$_REQUEST['pioobs']}', 'R', now(), '{$_SESSION['usucpf']}' )";
			
			$db->executar($sql);
			
			//enviaEmailStatusPi($valor);	
			
		}
	}else{
		$sql = "INSERT INTO
				monitora.planointernohistorico( plicod, pihobs, pihsituacao, pihdata, usucpf )
			VALUES
				( '".$pi."', '{$_REQUEST['pioobs']}', 'R', now(), '{$_SESSION['usucpf']}' )";
	
		$db->executar($sql);
		
		//enviaEmailStatusPi($pi);
		
	}
	
	$sql = "";
	$sql = "UPDATE
				monitora.planointerno
			SET
				plisituacao = 'R'
			WHERE
				plicod in ('".implode("','",$pi_array)."')";
	
	$db->executar($sql);
	$db->commit();
	

	
	
	echo "
		<script>
			alert('Operação realizada com sucesso!');
			window.opener.document.pesquisa.submit();
			//window.opener.location.reload();
			//window.parent.opener.location.href = window.opener.location;
			self.close();
		</script>";	
	exit;
}

?>
<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<script language="JavaScript">
		
			function submete_formulario(  ){
				
				var obs = document.getElementById('pioobs');
				var form = document.getElementById('formulario');
				
				if (obs.value == ''){
					alert('Favor preecher o campo observação!');
					return false;
				}
				
				form.submit();
					
			}
			
		</script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
	</head>
	<body>
		<form id="formulario" action="" method="post">
			<input type="hidden" name="submete" value="1"/>
			<input type="hidden" name="situacaonova" value="R">
			<input type="hidden" name="tipo" value="<?php echo $_REQUEST['tipo'] ?>"/>
			<table cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" style="border-top: medium none; border-bottom: medium none;" class="tabela">
				<tbody>
					<tr>
						<td align="center" width="100%">
							<label style="color: rgb(0, 0, 0);" class="TituloTela">
								Observação do Plano Interno
							</label>
						</td>
					</tr>
				</tbody>
			</table>
			<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" class="tabela">
				<tr>
					<td class="SubTituloDireita">Plano(s) Interno(s) Selecionado(s)</td>
					<td>
						<?php
						
							if ( !empty($pi_array) ){
								
								$pi_array = str_replace('\'', '', $pi_array);
								
								echo '<table border="0" cellspacing="0" cellpadding="2">';
								$cont = 0;
								for ( $i = 0; $i < count($pi_array); $i+=4  ){
									
									$cont++;
									$cor = ($cont % 2) ? '#dcdcdc' : '#ffffff';
									
									echo '<tr align="left" bgColor="'.$cor.'">
											<td>
												<a style="cursor:pointer;" onclick="mostradadospi(\''.$pi_array[$i].'\', \'consulta\');">
													' . $pi_array[$i] . '
												</a></td>
											<td>
												<a style="cursor:pointer;" onclick="mostradadospi(\''.$pi_array[$i+1].'\', \'consulta\');">
													' . $pi_array[$i+1] . '
												</a>
											</td>
											<td>
												<a style="cursor:pointer;" onclick="mostradadospi(\''.$pi_array[$i+2].'\', \'consulta\');">
													' . $pi_array[$i+2] . '
												</a>
											</td>
											<td>
												<a style="cursor:pointer;" onclick="mostradadospi(\''.$pi_array[$i+3].'\', \'consulta\');">
													' . $pi_array[$i+3] . '
												</a>
											</td>
										  </tr>';
									
									
								}	
							
								echo '</table>';
								
							}else if( !empty($pi) ){
								$pi = str_replace('\'', '', $_REQUEST['pi']);
								echo $pi; 
							}
						
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Observação</td>
					<td>
						<?php echo campo_textarea( 'pioobs', 'S', 'S', '', 55, 7, 500);?>
					</td>
				</tr>
				<tr bgcolor="#C0C0C0">
					<td></td>
					<td>
						<input type="submit" value="Salvar" onclick="return submete_formulario();" style="cursor:pointer;"/>
						<input type="button" value="Fechar" onclick="closeMessage();" style="cursor:pointer;"/>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>		