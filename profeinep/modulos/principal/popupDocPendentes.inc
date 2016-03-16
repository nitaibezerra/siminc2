<html>
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
</head>
<body>
<?php
$titulo_modulo = "Lista de Documentos Pendentes";
monta_titulo( $titulo_modulo, '' );

$sql = " select
				td.tpddsc, 			
				nd.nudnumero
				from profeinep.processoprofeinep p
			inner join profeinep.numeracaodocumento nd on p.prcid = nd.prcid
			inner join profeinep.tipodocumento td on nd.tpdid = td.tpdid
			left join profeinep.anexos a on a.nudid = nd.nudid  and a.anxstatus = 'A'
			where nudstatus = 'A' and p.prcid='".$_SESSION['profeinep_var']['prcid']."'";
$cabecalho = array("Tipo Documento", "Número do Documento");
$db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '', '' );
?>
</body>
</html>