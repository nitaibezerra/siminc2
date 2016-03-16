<?php
include APPRAIZ. '/includes/Agrupador.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatórios Monitoramento PAC 2", '<b>Filtros de Pesquisa</b>');


function colunas(){
	return array(
				array('codigo' => 'predescricao',			'descricao' => 'Nome da obra'),
				array('codigo' => 'estuf', 					'descricao' => 'Estado'),
				array('codigo' => 'muncod',				 	'descricao' => 'Município'),
				array('codigo' => 'empnumero',				'descricao' => 'Número do Empenho'),
				array('codigo' => 'empvalorempenho',		'descricao' => 'Valor do Empenho'),
				array('codigo' => 'pagdatapagamento',		'descricao' => 'Data do Pagamento'),
				array('codigo' => 'pagvalorparcela',		'descricao' => 'Valor do Pagamento'),
				array('codigo' => 'esfera',					'descricao' => 'Esfera'),
				array('codigo' => 'obrid', 					'descricao' => 'ID da Obra'),
				array('codigo' => 'obrpercentultvistoria',	'descricao' => '% executado')
				);
}
?>
<html>
<head>
	<script type="text/javascript" src="/includes/prototype.js"></script>
</head>
<body>
<form action="par.php?modulo=relatorio/popUpRelatorioPAC2&acao=A" method="post" name="formulario" id="formulario">
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width: 33%" valign="top"><b>Colunas</b></td>
		<td>
			<?php
			
			$coluna = colunas();
			$campoColuna = new Agrupador( 'formulario' );
			$campoColuna->setOrigem( 'colunaOrigem', null, $coluna );
			$campoColuna->setDestino( 'coluna', null, array(
															array('codigo' => 'predescricao',			'descricao' => 'Nome da obra'),
															array('codigo' => 'estuf', 					'descricao' => 'Estado'),
															array('codigo' => 'muncod',				 	'descricao' => 'Município'),
															array('codigo' => 'empnumero',				'descricao' => 'Número do Empenho'),
															array('codigo' => 'empvalorempenho',		'descricao' => 'Valor do Empenho'),
															array('codigo' => 'pagdatapagamento',		'descricao' => 'Data do Pagamento'),
															array('codigo' => 'pagvalorparcela',		'descricao' => 'Valor do Pagamento'),
															array('codigo' => 'esfera',					'descricao' => 'Esfera'),
															array('codigo' => 'dadospagamento',			'descricao' => 'Detalhe Pagamento'),
															array('codigo' => 'obrid', 					'descricao' => 'ID da Obra'),
															array('codigo' => 'obrpercentultvistoria',	'descricao' => '% Executado')
															));
			$campoColuna->exibir();
			?>
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Pesquisar" onclick="geraPopRelatorio();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>
</body>
<script type="text/javascript">
	function geraPopRelatorio(){
		var form = $('formulario');
		
		if (form.elements['coluna'][0] == null){
			alert('Selecione pelo menos uma coluna!');
			return false;
		}

		selectAllOptions( form.coluna );
		form.target = 'page';
		var janela = window.open('par.php?modulo=relatorio/popUpRelatorioPAC2&acao=A','page','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
		janela.focus();
		form.submit();
	}
</script>
</html>