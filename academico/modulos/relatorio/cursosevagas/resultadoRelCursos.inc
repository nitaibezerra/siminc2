<?php
	
// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = acaMontaSqlRelCursos();
$agrupador = acaMontaAgpRelCursos();
$coluna    = acaMontaColunaRelCursos();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador( $agrupador, $dados ); 
$rel->setColuna( $coluna );
$rel->setTotNivel( true );


?>


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
		
		<!--  Monta o Relatório -->
		<?php echo $rel->getRelatorio(); ?>
		
	</body>
</html>