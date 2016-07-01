<html>
<head>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<table cellpadding="0" border="0" style="width:95.3%" align="center" >
	<tr>
		<td style="background-color: #C9C9C9;" align="center"><font size=2><b>Detalhamento Financeiro da Subação</font></b></td>
	</tr>
	<tr>
		<td align='center' style="background-color: #dcdcdc;"><b>PTRES:</b> <?= $ptres ?></td>
		<td></td>
	</tr>
</table>
<?php
$sql = "select 
			s.sbacod, s.sbatitulo, coalesce(SUM(sd.sadvalor),0) as valor 
		from monitora.pi_subacao s 
			inner join monitora.pi_subacaodotacao sd on sd.sbaid = s.sbaid
			where sd.ptrid = {$_GET['ptrid']} and sd.sbaid <> {$_GET['sbaid']}
		group by s.sbacod, s.sbatitulo
";

$cabecalho = array ("Código", "Título", "Valor");
$db->monta_lista($sql,$cabecalho,60,20,'S','95%','');

?>
<center>
	<input type="button" name="bot" value="Fechar" onclick="self.close();">
</center>
</body>
</html>