<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<link rel="stylesheet" href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/entidadesn.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom.min.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
		<style>
		.ui-widget
		{
			font-size: 10px;
		}
		</style>
		<script type="text/javascript">
		jQuery(document).ready(function()
		{
			jQuery('#btOk').click(function()
			{
				jQuery( "#dialog-message" ).dialog( "close" );
			});
			
		});
		
		jQuery(function() {
			jQuery( "#dialog:ui-dialog" ).dialog( "destroy" );
			
			jQuery( "#dialog-message" ).dialog({
				modal: true,
				height: 600,
				width: 550
			});
		});
		</script>
	</head>
	<body>
		<?php
		//define("EXCECAO", 43 );
		
		$anoref = $_GET['ano'] ? $_GET['ano'] : '2011';
		$_SESSION['par']['preid'] 	= ($_REQUEST['preid']) ? $_REQUEST['preid'] : $_SESSION['par']['preid'];
		
		$_SESSION['par']['prog'] = $_REQUEST['prog'] != $_SESSION['par']['prog'] ? $_REQUEST['prog'] : 'nulo';
		
		if($_REQUEST['preid'] && !$_GET['muncod']){
			$_SESSION['par']['preid'] 	= $_REQUEST['preid'];
			$_SESSION['par']['estuf'] 	= $_SESSION['par']['estuf'] ? $_SESSION['par']['estuf'] : recuperarUF($_SESSION['par']['muncod']);
		}
		
		if($_GET['muncod']){
			$_SESSION['par']['preid'] 	= $_GET['preid'];
			$_SESSION['par']['muncod'] 	= $_GET['muncod'];
			$_SESSION['par']['estuf'] 	= recuperarUF($_SESSION['par']['muncod']);
		}
		
		if(!$_REQUEST['preid']){
			unset($_SESSION['par']['preid']);
		}
		
		//includes
		
		require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
		//require_once APPRAIZ . 'includes/workflow.php';
		require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		//require_once APPRAIZ . "academico/classes/controle/ItensComposicaoControle.class.inc";
		
		//Fim includes
		$_SESSION['sisdiretorio'] = 'academico';
		
		$tipoAba = ($_REQUEST['tipoAba']) ? $_REQUEST['tipoAba'] : 'Dados';
		$chamada = 'pronatec'.ucwords($tipoAba);

		$arParametros = array('visao' => $chamada);
//		$obItensComposicaoControle = new ItensComposicaoControle();
//		
//		$obItensComposicaoControle->visualizacaoItensComposicaoPronatec($arParametros);
		
		$arquivo = APPRAIZ . $_SESSION['sisdiretorio'] . '/modulos/principal/pronatec/visao/' . $chamada . '.php';

	    require_once $arquivo;
		?>
	</body>
</html>