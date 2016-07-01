<?php
if ($_POST['tipo']){
	ini_set("memory_limit","256M");
	include("quantitativosubacaoresult.inc");
	exit;
}

header('content-type: text/html; charset=ISO-8859-1');

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Quantitativos de Subação', '&nbsp;' );

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

function carregaFiltros(){
	var formulario = document.formulario;
	
	if( document.getElementById('tr_estados').style.display == 'table-row' ){
		document.getElementById('tr_subacao').style.display = 'none';
		document.getElementById('tr_municipio').style.display = 'none';
		formulario.filtros.value = false; 
	} else {
		document.getElementById('tr_estados').style.display = 'table-row';
		document.getElementById('tr_subacao').style.display = 'table-row';
		document.getElementById('tr_municipio').style.display = 'table-row';
		
		formulario.filtros.value = true; 
	}
}

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

function exibeRelatorio(){
	var formulario = document.formulario;
	agrupador = document.getElementById( 'agrupador' );

	formulario.tipo.value = true;

	if (formulario.elements['agrupadorFiltro'][0] == null){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}

//	if( !document.formulario.elements['estado'][0].value ){
//		alert('Selecione pelo menos um Estado!');
//		return false;
//	}
	
	selectAllOptions( formulario.agrupadorFiltro );
	selectAllOptions( formulario.agrupadorFiltroColuna );
	selectAllOptions( formulario.estado	   );
	selectAllOptions( formulario.municipio	   );
	selectAllOptions( formulario.subacao	   );
	selectAllOptions( formulario.dimensao  );
	selectAllOptions( formulario.areas  );
	selectAllOptions( formulario.indicador  );
	//selectAllOptions( formulario.forma 	   );
	
	var janela = window.open( '', 'relatorio', 'width=1000,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
	formulario.submit();
	
	janela.focus();
}

function abrepopupMunicipio(){
		window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/combo_municipios_bandalarga.php','Municipios','width=400,height=400,scrollbars=1');
	}
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
<input type="hidden" name="tipo" id="tipo" value="">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<input type="hidden" id="filtros" name="filtros" value="<?=$_POST['filtros']?>">
	<tr id="tr_agrupador">
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupadorFiltro', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr id="tr_colunas">
		<td class="SubTituloDireita" valign="top">Colunas</td>
		<td>
			<?
			$matriz = agrupadorColunas();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigemCol', null, $matriz );
			$campoAgrupador->setDestino( 'agrupadorFiltroColuna', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr>
		<td></td>
		<td>
		<?
		//Filtro de Estado
		$stSql = "SELECT
					estuf AS codigo,
					estdescricao AS descricao
				  FROM
				  	territorios.estado";
		if($_POST['estado']){
		$stSqlCarregados1 = "SELECT
								estuf AS codigo,
								estdescricao AS descricao
							  FROM
							  	territorios.estado
							  WHERE
							  	estuf IN ('" .implode("', '", $_POST['estado'])."')";
		}
		mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados1, 'Selecione o(s) Estado(s)' ); 
		 ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" > Municípios: </td>
		<td>
		
		<?
		//Filtro de Municípios
                $sqlComboMunicipio = "
                    select mundescricao as nome,
                            estuf as siglaestado
                    from territorios.municipio
                    order by estuf
                ";
		 ?>
		 <select multiple="multiple" size="5" name="municipios[]" 
        id="municipios"  
        ondblclick="abrepopupMunicipio();"  
        class="CampoEstilo" style="width:400px;" >
        <option value="">Duplo clique para selecionar da lista</option>
        </select>
		</td>
	</tr>
	<?
	
			//Filtro de Subação
			$stSql = "select distinct ppsid as codigo, ppsdsc as descricao  from cte.proposicaosubacao order by ppsdsc";
			
			if( $_POST['subacao'][0] ){
				$stSqlCarregados1 = "select distinct ppsid as codigo, ppsdsc as descricao  from cte.proposicaosubacao
									WHERE
										 ppsid IN (" .implode(", ", $_POST['subacao']).")                   
									ORDER BY
									    ppsdsc";
			}
			mostrarComboPopup( 'Subação', 'subacao',  $stSql, $stSqlCarregados1, 'Selecione a(s) Subação(s)' ); 
	
		
			//Filtro de Dimensão
			$stSql = "SELECT
					      dimcod as codigo,
					      dimcod || ' - ' || dimdsc as descricao
					FROM
						cte.dimensao
					WHERE
					      dimstatus = 'A' AND
					      itrid = '".INSTRUMENTO_DIAGNOSTICO_ESTADUAL."'
					ORDER BY
					      dimcod";

			if( $_POST['dimensao'][0] ){
				$stSqlCarregados2 = "SELECT
									      dimcod as codigo,
									      dimcod || ' - ' || dimdsc as descricao
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
					      a.ardcod as codigo,
					      d.dimcod || '.' || a.ardcod || ' - ' || substr( a.arddsc, 0, 95 ) || '...' as descricao
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
									      a.ardcod as codigo,
									      d.dimcod || '.' || a.ardcod || ' - ' || substr( a.arddsc, 0, 95 ) || '...' as descricao
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
					      i.indcod as codigo,
					      d.dimcod || '.' || a.ardcod || '.' || i.indcod || ' - ' || substr( i.inddsc, 0, 95 ) || '...' as descricao
					FROM
						cte.indicador i
					INNER JOIN cte.areadimensao a ON a.ardid = i.ardid
					INNER JOIN cte.dimensao d ON d.dimid = a.dimid
					WHERE
					      i.indstatus = 'A' AND
					      d.itrid = '" . INSTRUMENTO_DIAGNOSTICO_ESTADUAL . "'
					ORDER BY
					      d.dimcod,
					      a.ardcod,
					      i.indcod";
			
			if( $_POST['indicador'][0] ){
				$stSqlCarregados4 = "SELECT
									      i.indcod as codigo,
									      d.dimcod || '.' || a.ardcod || '.' || i.indcod || ' - ' || substr( i.inddsc, 0, 95 ) || '...' as descricao
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
					      prgdsc as descricao
					FROM
						cte.programa                        
					ORDER BY
					    prgdsc";
			
			if( $_POST['programa'][0] ){
				$stSqlCarregados6 = "SELECT
									      prgid as codigo,
									      prgdsc as descricao
									FROM
										cte.programa    
									WHERE
										 prgid IN (" .implode(", ", $_POST['programa']).")                   
									ORDER BY
									    prgdsc";
			}
			mostrarComboPopup( 'Programa', 'programa',  $stSql, $stSqlCarregados6, 'Selecione o(s) Programa(s)' ); 
			
			//Filtro de Status Subação
			$stSql = "SELECT
					      ssuid as codigo,
					      ssudescricao as descricao
					FROM
						cte.statussubacao
					ORDER BY
					    ssudescricao";
			
			if( $_POST['status'][0] ){
				$stSqlCarregados7 = "SELECT
									      ssuid as codigo,
									      ssudescricao as descricao
									FROM
										cte.statussubacao
									WHERE
										ssuid IN (" .implode(", ", $_POST['status']).")
									ORDER BY
									    ssudescricao";
			}
			mostrarComboPopup( 'Status Subação', 'status',  $stSql, $stSqlCarregados7, 'Selecione o(s) Status de Subação' ); 
			
			//Filtro de Forma de Execução
			$stSql = "SELECT
					      frmid as codigo,
					      frmdsc as descricao
					FROM
						cte.formaexecucao
					WHERE
					    frmbrasilpro = false AND
					    frmtipo      = 'E'
					ORDER BY
					    frmdsc";
			
			if( $_POST['forma'][0] ){
				$stSqlCarregados8 = "SELECT
									      frmid as codigo,
									      frmdsc as descricao
									FROM
										cte.formaexecucao
									WHERE
									    frmbrasilpro = false AND
									    frmtipo      = 'E' AND
										frmid IN (" .implode(", ", $_POST['forma']).")
									ORDER BY
									    frmdsc";
			}
			mostrarComboPopup( 'Forma de Execução', 'forma',  $stSql, $stSqlCarregados8, 'Selecione a(s) Forma(s) de Execução' ); 
			
			?>
	
	
	<tr>
		<td colspan="2">
		 <input
                    type="button"
                    name="filtrar"
                    value="Visualizar"
                    onclick="exibeRelatorio();"
                />
                <input type="checkbox" value="xls" name="xls"> Gerar em Planilha Excel
		
		</td>
	</tr>
	
<?
function agrupador(){
	return array(
				array('codigo' 	  => 'estuf',
					  'descricao' => 'Estados'),
				array('codigo' 	  => 'muncod',
					  'descricao' => 'Municípios'),
				array('codigo' 	  => 'sbaid',
					  'descricao' => 'Subações')
				);
}
function agrupadorColunas(){
	return array(
				array('codigo' 	  => 'qtd2007',
					  'descricao' => 'Quantitativo 2007'),
				array('codigo' 	  => 'qtd2008',
					  'descricao' => 'Quantitativo 2008'),
				array('codigo' 	  => 'qtd2009',
					  'descricao' => 'Quantitativo 2009'),
				array('codigo' 	  => 'qtd2010',
					  'descricao' => 'Quantitativo 2010'),
				array('codigo' 	  => 'qtd2011',
					  'descricao' => 'Quantitativo 2011'),
				array('codigo' 	  => 'qtdtotaldosanos',
					  'descricao' => 'Somatório quantitativo dos anos')
				);
}
?>	

