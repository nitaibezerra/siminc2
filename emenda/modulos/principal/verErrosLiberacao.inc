<?php

$emecod   = explode(",", $_REQUEST["emecod"]);
$motivo   = explode(",", $_REQUEST["motivo"]);
$entidade = explode(",", $_REQUEST["entidade"]);

monta_titulo( 'As seguintes emendas não foram liberadas:', '');
?>

<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/prototype.js"></script>
	    <script type="text/javascript" src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script src="js/emenda.js"></script>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
			<tr>
				<td class="subtitulocentro">Código da Emenda</td>
				<td class="subtitulocentro">Nome da Entidade</td>
				<td class="subtitulocentro">Motivo</td>
			</tr>
			<?php

				for( $i = 0; $i < count($emecod); $i++ ){
					
					$cor = ($i % 2) ? "#f4f4f4": "#e0e0e0";
					
					print "<tr bgcolor='{$cor}'>"
						. "		<td width='80px' align='center'>{$emecod[$i]}</td>"
						. "		<td>". iconv("UTF-8", "ISO-8859-1", $entidade[$i]). "</td>"
						. "		<td align='justify'>". iconv("UTF-8", "ISO-8859-1", $motivo[$i]). "</td>"
						. "</tr>";
					
				}
				
			?>
			<tr bgcolor="#D0D0D0">
				<td colspan="3">
					<input type="button" style="cursor: pointer;" onclick="self.close();" value="Fechar"/>
				</td>
			</tr>
		</table>
	</body>
</html>