<html>
<head>
	<script language="javascript"> 
	tmt_Move_WindowX = (screen.width - 400 ) / 2; 
	tmt_Move_WindowY = (screen.height - 300 ) / 2; 
	self.moveTo(tmt_Move_WindowX,tmt_Move_WindowY); 
	</script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../../includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
</head>
	<body>
<?php 
	
	/* Funçõe que recuperam o Hitórico do Documento da Obra Individual.*/
	$documento = wf_pegarDocumento( $_REQUEST['docid'] );
	$atual     = wf_pegarEstadoAtual( $_REQUEST['docid'] );
	$historico = wf_pegarHistorico( $_REQUEST['docid'] );
?>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript">
		
		$(document).ready(function()
		{
			self.focus();
		});
		</script>
		
		<form action="" method="post" name="formulario">
			<table class="tabela"  align="center">
				<thead>
					<tr>
						<td style="text-align: center; background-color: #e0e0e0;" colspan="5">
							<b style="font-size: 10pt;">Histórico de Tramitações da Obra<br/></b>
							<div><?php echo $documento['docdsc']; ?></div>
						</td>
					</tr>
					<?php if ( count( $historico ) ) : ?>
						<tr>
							<td style="width: 20px;"  align="center"><b>Seq.</b></td>
							<td style="width: 200px;" align="center"><b>Onde Estava</b></td>
							<td style="width: 200px;" align="center"><b>O que aconteceu</b></td>
							<td style="width: 90px;"  align="center"><b>Quem fez</b></td>
							<td style="width: 120px;" align="center"><b>Quando fez</b></td>
						</tr>
					<?php endif; ?>
				</thead>
				<?php $i = 1; ?>
				<?php foreach ( $historico as $item ) : ?>
					<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
					<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
						<td align="center" ><?=$i?>.</td>
						<td style="color:#008000;">
							<?php echo $item['esddsc']; ?>
						</td>
						<td valign="middle" style="color:#133368">
							<?php echo $item['aeddscrealizada']; ?>
						</td>
						<td style="font-size: 6pt;">
							<?php echo $item['usunome']; ?>
						</td>
						<td style="color:#133368">
							<?php echo $item['htddata']; ?>
						</td>
					</tr>
					<?php $i++; ?>
				<?php endforeach; ?>
				<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
				<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
					<td style="text-align: right;" colspan="5">
						Estado atual: <span style="color:#008000;"><?php echo $atual['esddsc']; ?></span>
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>