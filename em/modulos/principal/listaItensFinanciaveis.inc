<html>
	<head>
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>	    
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script>
	    	function voltarMatriz()
	    	{
	    		document.location.href = 'em.php?modulo=principal/matrizGap&acao=A';
	    	}
	    </script>
	</head>
	<body>	
		<?php
		
		monta_titulo("Lista de Ítens Financiáveis Cadastrados", "PRC 2012");
		echo montaCabecalhoPRC($_SESSION['emiid'], $_SESSION['mcpid'], $_REQUEST['papid'], false);
		echo "<br/>";
		
		$sql = "select 
					mcpdsc,
					itfdsc,
					--'' as unidade, 
					mdoqtd,
					mdovalorunitario,
					mdototal
				from 
					em.macrocampo mc
				inner join 
					em.emigap gp on gp.mcpid = mc.mcpid
				inner join 
					em.emimatrizdistribuicaoorcamentargap md on md.papid = gp.papid
				inner join 
					em.emiitemfinanciavel if on if.itfid = md.itfid 
				where 
					gp.emiid = ".$_SESSION['emiid'];
		
		$cabecalho = array('Macrocampo', 'Ìtens Financiaveis'/*, 'Unidade'*/, 'Quantidade','Valor Unitário (R$)', 'Valor Total (R$)');
		$db->monta_lista($sql, $cabecalho, 100, 10, 'S', '', 'S');
		
		?>		
	</body>
</html>