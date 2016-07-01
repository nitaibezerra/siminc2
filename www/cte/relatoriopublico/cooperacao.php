<?php
$inuid =  $_REQUEST['inuid'];
$itrid =  $_REQUEST['itrid'];
//$inuid =  $_SESSION['inuid'];
//$itrid =  $_SESSION['itrid'];
?>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
<link rel="STYLESHEET" type="text/css" href="estiloImprimir.css" media="print"> 
<script src="script.js"></script>
<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>TERMO DE COOPERAÇÃO</b></td>
	</tr>
	<tr>
		<td>
		<iframe id="carregaIframe" marginheight="0" marginwidth="0" name="Localtermo" 
		src="termo.php?inuid=<?=$inuid;?>&itrid=<?=$itrid;?>" 
		width="100%" frameborder="0" allowtransparency="true" height="900pt" scrolling="auto"></iframe>
		</td>
	</tr>
</table>

