<?php
	if ($_POST['tipo_relatorio']){
		ini_set("memory_limit","256M");
		include("resultRelatorioItensFinanc.inc");
		exit;
	}
	
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';	

	monta_titulo( 'Relatório Itens Financiados', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript">

	function gerarRelatorio(tipo){
		var formulario = document.formulario;
		var tipo_relatorio = tipo;

		selectAllOptions( formulario.estuf );
		selectAllOptions( formulario.municipio );		
		
		if(tipo_relatorio == 'visual'){
			document.getElementById('tipo_relatorio').value = 'visual';
		} else {
			document.getElementById('tipo_relatorio').value = 'xls';
		}
		
		var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';
		formulario.submit();
		janela.focus();
	}
	
	function onOffCampo( campo ) {
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		} else {
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}	
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>
	
	<table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<td class="subtituloesquerda" colspan="2">
					<strong>Filtros</strong>
				</td>
			</tr>
				<?php 	
					#UF
					$ufSql = "
						Select 	estuf as codigo,
								estdescricao as descricao
						From territorios.estado est
						Order by estdescricao";
					$stSqlCarregados = "";
					mostrarComboPopup( 'Estado', 'estuf',  $ufSql, '', 'Selecione o(s) Estado(s)' );
					
				?>
			<tr>
				<td class="subtitulodireita">Ano de Seleção</td>
				<td>
					<?php
						$anoCorrente = date("Y");
						
						$arrayAno = array();
												
						for($i=$anoCorrente; $i>=2008; $i--){
							 array_push($arrayAno, array( "codigo" => "$i", "descricao" => "$i" ));
						}
						
						/*
						$arrayAno = array(
							array( 'codigo' => '2008', 'descricao' => '2008' ),
							array( 'codigo' => '2009', 'descricao' => '2009' ),
							array( 'codigo' => '2010', 'descricao' => '2010' ),
							array( 'codigo' => '2011', 'descricao' => '2011' ),
							array( 'codigo' => '2012', 'descricao' => '2012' )
						);
						*/
						$memanoreferencia = $_POST['memanoreferencia'];
						$db->monta_combo( "memanoreferencia", $arrayAno, 'S', '', '', '', 'Selecione o item caso queira filtrar por ano!', '244');
					?>
				</td>
			</tr>

			<tr>
				<td class="subtitulodireita" >&nbsp;</td>
				<td align="center">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('visual');"/>
					<input type="button" name="Gerar Relatório" value="Visualizar XLS" onclick="javascript:gerarRelatorio('xls');"/>
				</td>
			</tr>
	</table>
</form>
</body>
</html>