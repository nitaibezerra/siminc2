<?php 
if($_REQUEST['sifid']){
	$justificativa = recuperarJustificativa($_REQUEST['sifid']); 
}	
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td align="center"><label class="TituloTela" style="color:#000000;">Justificativa</label></td>
	</tr>			
</table>	
	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td><?php echo $justificativa; ?></td>
	</tr>		
</table>	