<?
$municod = $_REQUEST['municod'];
?>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
<script src="script.js"></script>
<table class="tabela" align="center" bgcolor="" cellspacing="1" cellpadding="3" style="font-family:Arial, Verdana; font-size:10px;">
	<tr>
	<td class="tituloPrincipalAbas"><b>INDICADORES DEMOGRÁFICOS  E EDUCACIONAIS</b></td>
	</tr>
	<tr>
		<td  style="margin: auto; height:100%;">
		<div>
			<iframe id="carregaIframe" src="conteudoIndicadores.php?municod=<?=$municod;?>" width="100%" height="100%"   marginwidth="0" marginheight="0" frameborder="0" vspace="0" hspace="0" style="overflow:visible; width:100%; display:none" scrolling="no" ></iframe>
		</div>
		</td>
	</tr>
</table>
