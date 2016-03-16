<html>
	<head>
		<title> Simec - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		<?php 
		include APPRAIZ . 'includes/classes/relatorio.class.inc';
			
		$estuf = $_REQUEST['estuf'];
			
		include APPRAIZ . '/par/classes/modelo/RelatorioAtendimentoEstado.class.inc';
		$relAtendimentoEstado = new RelatorioAtendimentoEstado($estuf);
		$dados = $relAtendimentoEstado->montarData();
		$agrup = $relAtendimentoEstado->montarAgrupador();
		$col = $relAtendimentoEstado->montarColuna();
			
		$r = new montaRelatorio();
		$r->setAgrupador($agrup, $dados, $agrupcol);
		$r->setColuna($col);
		$r->setBrasao(false);
		$r->setTotNivel(true);
		$r->setEspandir(false);
		$r->setMonstrarTolizadorNivel(true);
		$r->setTotalizador(true);
		$r->setTolizadorLinha(true);
		echo '<br /><table cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" align="center" class="tabela" style="border-top: none; border-bottom: none;"><tbody><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">Convênios de 2007 a 2010: '.$estuf.'</td></tr></tbody></table>';
		echo $r->getRelatorio();
		?>		
	</body>
</html>
<?php die();?>