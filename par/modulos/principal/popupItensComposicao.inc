<?php
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$anoref = $_GET['ano'] ? $_GET['ano'] : date('Y');

if($_REQUEST['download'] == 'S'){
	unset($_REQUEST['download']);
	$arqid = $_REQUEST['arqid'];
	
	$file = new FilesSimec(null,null,'obras');
	$file->getDownloadArquivo($arqid);
	die;
} 

require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";

$sbaid = $_GET['sbaid'] ? $_GET['sbaid'] : 1;

?>
<html>
	<head>
		<title>PAR - Cadastro de Itens Composição</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
		<script type="text/javascript" src="../includes/prototype.js"></script>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/entidadesn.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
		<script type="text/javascript">
			jQuery.noConflict();
		</script>
	</head>
	<body>
	<?php 
		$obItensComposicaoControle = new ItensComposicaoControle();
	?>
	<?php if(!$_REQUEST['icoid']) :?>
		<?php
			$arParametros = array('visao' => 'cadItensComposicao', 'sbaid' => $sbaid); 
			$obItensComposicaoControle->visualizacaoItensComposicao($arParametros, $anoref); 
		?>
	<?php else: ?>
		<?php 
			$tipoAba = ($_REQUEST['tipoAba']) ? $_REQUEST['tipoAba'] : 'dados';
			$chamada = 'aba'.ucwords($tipoAba);
			$arParametros = array('visao' => $chamada, 'icoid' => $_REQUEST['icoid']);
			$obItensComposicaoControle->visualizacaoItensComposicao($arParametros, $anoref);
		?>
	<?php endif; ?>
	</body>
</html>