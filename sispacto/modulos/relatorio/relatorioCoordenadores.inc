<?php

if ($_POST){
	ini_set("memory_limit","256M");
	include("resultCoordenadores.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Quantitativo Coordenadores e Orientadores', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">
function gerarRelatorio(tipo_relatorio){
	var formulario = document.formulario;

	if(tipo_relatorio == 'html'){
		document.getElementById('tipo_relatorio').value = 'html';
	}else{
		document.getElementById('tipo_relatorio').value = 'xls';
	}
	
	selectAllOptions( formulario.estado );
	selectAllOptions( formulario.municipio );
			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
	formulario.submit();
	
	janela.focus();
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function mostra(tipo){
	var formulario = document.formulario;
	if(tipo=='m'){
		document.getElementById('tr_estado').style.display = 'none';
		document.getElementById('tr_municipio').style.display = '';
	}else if(tipo=='e'){
		document.getElementById('tr_estado').style.display = 'none';
		document.getElementById('tr_municipio').style.display = 'none';
	}
}

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="">
<input type="hidden" name="esfera" id="esfera" value="">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>

		<td class="subtitulodireita" width="20%">Esfera: </td>
		<td>
					Municipal: <input <?php echo $_REQUEST['esfera'] == "M" ? "checked='checked'" : "" ?> type="radio" name="esfera" onclick="mostra('m')" value="M" >
					Estadual: <input  <?php echo $_REQUEST['esfera'] == "E" ? "checked='checked'" : "" ?> type="radio" name="esfera" onclick="mostra('e')" value="E" >
		</td>
	</tr>
	<tr id="tr_estado" style="display: none;">
	<?php
				// Filtros do relatório

				$stSql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)', null, null, true ); 
		?>
	</tr>
	<tr id="tr_municipio" style="display: none;">
	<?php			
				
				$stSql = "SELECT
							muncod AS codigo,
							estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
						  FROM
						  	territorios.municipio	
						  WHERE
						  	1=1
						  ORDER BY
						  	descricao";
				$stSqlCarregados = "";
				$where = array(
								array(
										"codigo" 	=> "estuf",
										"descricao" => "Estado <b style='size: 8px;'>(sigla)</b>",
										"numeric"	=> false
							   		  )
							   );
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where, null, true ); 
		?>
		</tr>
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="GerarRelatorio" value="Gerar Relatório HTML" onclick="javascript:gerarRelatorio('html');"/>
			<input type="button" name="GerarRelatorio" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio('xls');"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>


