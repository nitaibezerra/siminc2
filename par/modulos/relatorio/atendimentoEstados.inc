<?php

if ($_POST['estado']){
	ini_set("memory_limit","256M");
	include("atendimentoEstadosResultado.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório Geral', '&nbsp;' );


header('content-type: text/html; charset=ISO-8859-1');
exibeMapaRegionalizador();

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">

	function exibeRegionalizador( regiao ){
		
		var formulario = document.formulario;
		
		selectAllOptions( formulario.agrupadorFiltro );
		selectAllOptions( formulario.dimensao  );
		selectAllOptions( formulario.areas     );
		selectAllOptions( formulario.indicador );
		selectAllOptions( formulario.plano     );
		selectAllOptions( formulario.programa  );
		
		document.getElementById('estado').value = regiao;
		
		var janela = window.open( '', 'relatorio', 'width=1000,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';	
	
		formulario.submit();
		
		janela.focus();
	}
	
	
	function gerarRelatorio(){
		
		var formulario = document.formulario;
		
		if( document.getElementById('estado').value == "" ){
			alert('Selecione um Estado!');
			return false;
		}
		
		selectAllOptions( formulario.agrupadorFiltro );
		selectAllOptions( formulario.dimensao  );
		selectAllOptions( formulario.areas     );
		selectAllOptions( formulario.indicador );
		selectAllOptions( formulario.plano     );
		selectAllOptions( formulario.programa  );
				
		var janela = window.open( '', 'relatorio', 'width=1000,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';	
	
		formulario.submit();
		
		janela.focus();
	}
	
	function carregaFiltros(){
		
		var formulario = document.formulario;
		
		if( document.getElementById('tr_dimensao').style.display == 'table-row' ){
			document.getElementById('tr_agrupador').style.display = 'none';
			document.getElementById('tr_dimensao').style.display = 'none';
			document.getElementById('tr_areas').style.display = 'none';
			document.getElementById('tr_indicador').style.display = 'none';
			document.getElementById('tr_programa').style.display = 'none';
	
			formulario.filtros.value = false; 
		} else {
			document.getElementById('tr_agrupador').style.display = 'table-row';
			document.getElementById('tr_dimensao').style.display = 'table-row';
			document.getElementById('tr_areas').style.display = 'table-row';
			document.getElementById('tr_indicador').style.display = 'table-row';
			document.getElementById('tr_programa').style.display = 'table-row';
	
			formulario.filtros.value = true; 
		}
		
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
<input type="hidden" id="filtros" name="filtros" value="<?=$_POST['filtros']?>">
<input type="hidden" id="estado" name="estado" value="<?=$_POST['estado']?>">
	<tr>
		<td class="SubTituloDireita" valign="top" colspan="2">
			<img src="../imagens/lupa_grafico.gif" title="Abrir Filtros" alt="Abrir Filtros" onclick="carregaFiltros();">
		</td>
	</tr>
	<tr id="tr_agrupador">
		<td class="SubTituloDireita" valign="top">Colunas Adicionais</td>
		<td>
			<?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', 4, $matriz );
			$campoAgrupador->setDestino( 'agrupadorFiltro', 4);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<?
	//Filtro de Dimensão
	$stSql = "SELECT
			      dimid as codigo,
			      dimcod || ' - ' || removeacento(dimdsc) as descricao
			FROM
				cte.dimensao
			WHERE
			      dimstatus = 'A' AND
			      itrid = '".INSTRUMENTO_DIAGNOSTICO_ESTADUAL."'
			ORDER BY
			      dimcod";
	
	if( $_POST['dimensao'][0] ){
		$stSqlCarregados2 = "SELECT
							      dimid as codigo,
							      dimcod || ' - ' || removeacento(dimdsc) as descricao
							FROM
								cte.dimensao
							WHERE
							      dimstatus = 'A' AND
							      itrid = '".INSTRUMENTO_DIAGNOSTICO_ESTADUAL."' AND
							      dimid IN (" .implode(", ", $_POST['dimensao']).")
							ORDER BY
							      dimcod";
	}
	mostrarComboPopup( 'Dimensão', 'dimensao',  $stSql, $stSqlCarregados2, 'Selecione a(s) Dimensão(ões)' ); 
			
	//Filtro de Áreas
	$stSql = "SELECT
			      a.ardid as codigo,
			      d.dimcod || '.' || a.ardcod || ' - ' || removeacento(substr( a.arddsc, 0, 95 )) || '...' as descricao
			FROM
				cte.areadimensao a
			INNER JOIN cte.dimensao d ON d.dimid = a.dimid
			WHERE
			      a.ardstatus = 'A' AND
			      d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "'
			ORDER BY
			      d.dimcod,
			      a.ardcod";
	
	if( $_POST['areas'][0] ){
		$stSqlCarregados3 = "SELECT
							      a.ardid as codigo,
							      d.dimcod || '.' || a.ardcod || ' - ' || removeacento(substr( a.arddsc, 0, 95 )) || '...' as descricao
							FROM
								cte.areadimensao a
							INNER JOIN cte.dimensao d ON d.dimid = a.dimid
							WHERE
							      a.ardstatus = 'A' AND
							      d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "' AND
							      a.ardid IN (" .implode(", ", $_POST['areas']).")
							ORDER BY
							      d.dimcod,
							      a.ardcod";
	}
	mostrarComboPopup( 'Áreas', 'areas',  $stSql, $stSqlCarregados3, 'Selecione a(s) Área(s)' ); 
			
	//Filtro de Indicador
	$stSql = "SELECT
			      i.indid as codigo,
			      d.dimcod || '.' || a.ardcod || '.' || i.indcod || ' - ' || removeacento(substr( i.inddsc, 0, 95 )) || '...' as descricao
			FROM
				cte.indicador i
			INNER JOIN cte.areadimensao a ON a.ardid = i.ardid
			INNER JOIN cte.dimensao 	d ON d.dimid = a.dimid
			WHERE
			      i.indstatus = 'A' AND
			      d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "'
			ORDER BY
			      d.dimcod,
			      a.ardcod,
			      i.indcod";
	
	if( $_POST['indicador'][0] ){
		$stSqlCarregados4 = "SELECT
							      i.indid as codigo,
							      d.dimcod || '.' || a.ardcod || '.' || i.indcod || ' - ' || removeacento(substr( i.inddsc, 0, 95 )) || '...' as descricao
							FROM
								cte.indicador i
							INNER JOIN cte.areadimensao a ON a.ardid = i.ardid
							INNER JOIN cte.dimensao d ON d.dimid = a.dimid
							WHERE
							      i.indstatus = 'A' AND
							      d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "' AND
							      i.indid IN (" .implode(", ", $_POST['indicador']).")
							ORDER BY
							      d.dimcod,
							      a.ardcod,
							      i.indcod";
	}
	mostrarComboPopup( 'Indicador', 'indicador',  $stSql, $stSqlCarregados4, 'Selecione o(s) Indicador(es)' ); 
			
	//Filtro de Programa
	$stSql = "SELECT
			      prgid as codigo,
			      removeacento(prgdsc) as descricao
			FROM
				cte.programa                        
			ORDER BY
			    prgdsc";
	
	if( $_POST['programa'][0] ){
		$stSqlCarregados6 = "SELECT
							      prgid as codigo,
							      removeacento(prgdsc) as descricao
							FROM
								cte.programa    
							WHERE
								 prgid IN (" .implode(", ", $_POST['programa']).")                   
							ORDER BY
							    prgdsc";
	}
	mostrarComboPopup( 'Programa', 'programa',  $stSql, $stSqlCarregados6, 'Selecione o(s) Programa(s)' ); 
			
?>
</table>
</form>
</body>
</html>
<?
function agrupador(){
	return array(
				array('codigo' 	  => '1',
					  'descricao' => 'Ass. Técnica - 01 - Qtd. Painel'),	
				array('codigo' 	  => '2',
					  'descricao' => 'Ass. Técnica - 02 - Indicador Painel'),				
				array('codigo' 	  => '3',
					  'descricao' => 'Ass. Técnica - 03 - Qtd. Outra Fonte'),				
				array('codigo' 	  => '4',
					  'descricao' => 'Ass. Técnica - 04 - Fonte')
				);
}
?>
<script>
<?php if( $_POST['filtros'] != 'true' ){ ?>
	document.getElementById('tr_agrupador').style.display 	= 'none';
	document.getElementById('tr_dimensao').style.display 	= 'none';
	document.getElementById('tr_areas').style.display 		= 'none';
	document.getElementById('tr_indicador').style.display 	= 'none';
	document.getElementById('tr_programa').style.display 	= 'none';
<? } ?>
</script>