<?php

if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("resultIndicadorTCU.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Indicadores do TCU', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">
function gerarRelatorio(){
	var formulario = document.formulario;

	if (formulario.elements['agrupador'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}	
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.IFES );
	selectAllOptions( formulario.indicador );
	selectAllOptions( formulario.componente );
			
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
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
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
				
				$stSql =	"SELECT ent.entid AS codigo, ent.entsig as descricao 
							FROM academico.indicadorestcu tcu
							LEFT JOIN
							academico.lancamentoindicador lci ON lci.tcuid = tcu.tcuid
							INNER JOIN
							academico.movimentoindicador mov ON mov.mviid = lci.mviid
							INNER JOIN
							entidade.entidade ent ON ent.entid = mov.entid";
				$stSqlCarregados = "";
				mostrarComboPopup( 'IFES', 'IFES',  $stSql, $stSqlCarregados, 'Selecione o(s) IFES' );
				
				$stSql = "SELECT
							tcuid AS codigo,
							tcucodigo || ' - ' || tcudsc AS descricao
						  FROM
						  	academico.indicadorestcu
						  WHERE
							tcutipo = 'I'";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Indicador', 'indicador',  $stSql, $stSqlCarregados, 'Selecione o(s) indicador(es)' ); 
				
				$stSql = "SELECT
							tcuid AS codigo,
							tcucodigo || ' - ' || tcudsc AS descricao
						  FROM
						  	academico.indicadorestcu
						  WHERE
							tcutipo = 'C'";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Componente', 'componente',  $stSql, $stSqlCarregados, 'Selecione o(s) componente(es)' ); 
		?>
					
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio();"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>
<?
function agrupador(){
	return array(
				array('codigo' 	  => 'IFES',
					  'descricao' => 'IFES'),				
				array('codigo' 	  => 'indicador',
					  'descricao' => 'Indicador'),
				array('codigo' 	  => 'componente',
					  'descricao' => 'Componente')
				);
}
?>