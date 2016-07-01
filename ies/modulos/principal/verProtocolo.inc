<?php

$iesid = $_SESSION["ies"]["iesid"];
$pbiid = iesPegaProjeto( $iesid );

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Programa IES - MEC/BNDES", "" );

$ies->cabecalhoIES( $iesid );

$sql = "SELECT
			pbiprotocolo, to_char(max(htddata), 'DD/MM/YYYY') as data
		FROM
			ies.projetobndesies pb
		LEFT JOIN
			workflow.historicodocumento wh ON wh.docid = pb.docid
		WHERE
			pbiid = {$pbiid}
		GROUP BY
			pbiprotocolo";

$dados = $db->pegaLinha( $sql );

?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<title>SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação</title>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Programa IES - MEC/BNDES</title>
	</head>
	<body>
		<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
			<tr>
				<td class="subtitulodireita" width="190px">Data</td>
				<td>
					<?php echo $dados["data"]; ?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Nº de Protocolo</td>
				<td>
					<?php echo $dados["pbiprotocolo"]; ?>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td colspan="2">
					<input type="button" value="Fechar" onclick="self.close();" style="cursor: pointer;">
					<input type="button" value="Imprimir" onclick="self.print();" style="cursor: pointer;">
				</td>
			</tr>
		</table>
	</body>
</html>