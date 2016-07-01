<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/dtree/dtree.js"></script>
		<script type="text/javascript" src="../includes/prototype.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
	</head>

<?php
	
	cte_verificaSessao();
	
	// Pega os dados do pareer, caso existam
	$sql = "SELECT esppar FROM cte.escolaproep WHERE entid = {$_REQUEST['entid']}";
	$parecer = $db->pegaUm($sql);
	
	if ($_REQUEST['gravar']){
		
		$dadosParecer = pg_escape_string(trim($_REQUEST['esppar']));
		
		if ($parecer){
			$sql = "";
			$sql = $db->executar("
							UPDATE cte.escolaproep SET esppar = '{$dadosParecer}'
							WHERE entid = '{$_REQUEST['entid']}'");
			
			$db->commit();
			
			echo "
				<script>
					alert('Operação realizada com sucesso!');
					window.parent.opener.location.reload();
					self.close();
				</script>";
			
		}else{
			$sql = "";
			$sql = $db->executar("
							INSERT INTO 
								cte.escolaproep (esppar, entid, espano) 
							VALUES 
								('{$dadosParecer}', '{$_REQUEST['entid']}', '{$_SESSION['exercicio']}')");
			
			$db->commit();
			
			echo "
				<script>
					alert('Operação realizada com sucesso!');
					window.parent.opener.location.reload();
					self.close();
				</script>";
			
		}
		
	}
	
?>
	<body>
		<form action="brasilpro.php?modulo=principal/manterParecerProep&acao=A" name="form" method="post">
			<input type="hidden" name="gravar" value="gravar">
			<input type="hidden" name="entid" value="<? echo $_REQUEST['entid']; ?>">
			<table bgcolor="#f5f5f5"  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tbody>
					<tr>
						<td colspan="2" style="font-weight: bold; text-align: center;">
							<?php
								$nome = $db->pegaUm("SELECT entnome FROM entidade.entidade where entid = '{$_REQUEST['entid']}'");
								echo $nome; 
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita">Parecer</td>
						<td>
							<?php if ($parecer){$esppar = $parecer;} ?>
							<?= campo_textarea( 'esppar', 'N', 'S', '', '80', '10', '2000'); ?>
						</td>
					</tr>
					<tr bgcolor="#C0C0C0">
						<td></td>
						<td>
							<div style="float: left;">
								<input type="hidden" name="submit" value="1" />
								<input type="submit" value="Salvar" style="cursor: pointer" <?php if($somenteLeitura=="N") echo "disabled"; ?>> 
								<input type="button" value="Fechar" style="cursor: pointer" onclick="self.close();">
							</div>
						</td>
					</tr>
				</tbody>
			</table>
		</form>
	</body>
</html>