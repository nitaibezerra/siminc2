<?php 

$sbaid = $_GET['id'] ? $_GET['id'] : 1;
$anoref = $_GET['ano'] ? $_GET['ano'] : 1;

$obBeneficiarioController = new BeneficiarioControle();
$arBeneficiarios = $obBeneficiarioController->carregaBeneficiarioPorSbaid($sbaid, $anoref);

?>
<html>
	<head>
		<title>PAR - Cadastro de subação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
	</head>
	<body>
		<?php monta_titulo( 'Cadastro de beneficiários', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  ); ?>
		<form action="" method="post">
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr bgcolor="#e9e9e9" align="center">
					<td colspan="6"><strong>Beneficiários</strong></td>
				</tr>
				<tr bgcolor="#e9e9e9" align="center">
					<!-- th width="80px" align="left" valign="top">Ações</th -->
					<th align="left" valign="top">Descrição</th>
					<th align="right" valign="top">Total urbana</th>
					<th align="right" valign="top">Total rural</th>
					<th align="right" valign="top">Total</th>
				</tr>
				<?php $x=2; ?>
				<?php foreach($arBeneficiarios as $beneficiario): ?>
					<?php 
					$cor = ($x % 2) ? '#f0f0f0' : 'white';
					$x++;
					?>
					<tr style="background:<?php echo $cor ?>;">
						<!-- td align="left" valign="top">
							<img src="../imagens/alterar.gif">
						</td -->
						<td align="left" valign="top"><?php echo ucwords($beneficiario['bendsc']) ?></td>
						<td align="right" valign="top">
							<?php echo campo_texto('quantidadeurbana', 'S', 'S', '',10,10,'','','','','','','',$beneficiario['vlrurbano']) ?>
						</td>
						<td align="right" valign="top">
							<?php echo campo_texto('quantidaderural', 'S', 'S', '',10,10,'','','','','','','',$beneficiario['vlrrural']) ?>
						</td>
						<td align="right" valign="top"><?php echo $beneficiario['total'] ?></td>
					</tr>
				<?php endforeach; ?>
				<tr>
					<td colspan="5">
						<input type="submit" value="Salvar">
					</td>
				</tr>		
			</table>
		</form>
	</body>
</html>