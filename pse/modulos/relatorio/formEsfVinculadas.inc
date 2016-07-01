<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("resultEsfVinculadas.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório ESF Vinculadas', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript"><!--
function gerarRelatorio(tipo_relatorio){
	var formulario = document.formulario;

	if(tipo_relatorio == 'html'){
		document.getElementById('tipo_relatorio').value = 'html';
	}else{
		document.getElementById('tipo_relatorio').value = 'xls';
	}
	
	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.estado );
	selectAllOptions( formulario.municipio );
	selectAllOptions( formulario.equipe );
			
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

--></script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value="">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
		<?php
			
				// Filtros do relatório

				$stSql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)' ); 
			
				
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
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where ); 
				
				$stSql = "SELECT
							cnecodigocnes AS codigo,
							cnecodigocnes ||' - '|| cnenomefantasia AS descricao
						  FROM
						  	pse.scnes s
						  INNER JOIN pse.escolaesf e on e.cneid = s.cneid
						  INNER JOIN pse.avaliacao a on a.cneid = e.cneid
						  group by cnecodigocnes, cnenomefantasia
						  ORDER BY
						  	descricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Equipe Saúde da Família', 'equipe',  $stSql, $stSqlCarregados, 'Selecione a(s) Equipe(s) Saúde da Família' );

				$stSql = "SELECT
							ent.entid AS codigo,
							ent.entcodent ||' - '|| ent.entnome AS descricao
						  FROM
						  	pse.escolaesf e
						  INNER JOIN pse.planoacaoc1 p on p.pamid = e.pamid
						  INNER JOIN entidade.entidade ent on ent.entid = p.entid
						  INNER JOIN pse.avaliacao a on a.cneid = e.cneid
						  group by ent.entid, ent.entcodent, ent.entnome
						  ORDER BY
						  	descricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Escola', 'escola',  $stSql, $stSqlCarregados, 'Selecione a(s) Escolas(s)' );
				
			?>
					
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
<?
function agrupador(){
	return array(
				array('codigo' 	  => 'estado',
					  'descricao' => 'Estado'),				
				array('codigo' 	  => 'municipio',
					  'descricao' => 'Município'),
				array('codigo' 	  => 'equipe',
					  'descricao' => 'Equipe Saúde da Família'),
				array('codigo' 	  => 'escola',
					  'descricao' => 'Escola')
				);
}
?>