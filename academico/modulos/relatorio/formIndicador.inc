<?php
//ver($_POST,d);
//if ( substr($_POST['requisicao'],0,6) == 'filtra' || $_POST['requisicao'] == 'reseta' ){
//	include("formIndicadorAjax.inc");
//	die;
//}
if ( $_POST ){
	include("resultIndicador.inc");
	die;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Indicadores', '&nbsp;' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
//function resetaEstuf(){
//	var html = '<select style="width: 400px;" class="CampoEstilo" id="estuf" name="estuf[]" size="5" multiple="multiple" tipo="combo_popup" disabled="disabled" maximo="0">'+
//			   '<option value="">Escolha uma Região...</option>'+
//			   '</select>';
//	$('#divEstuf').html(html);
//}
//function resetaMuncod(){
//	var html = '<select style="width: 400px;" class="CampoEstilo" id="estuf" name="estuf[]" size="5" multiple="multiple" tipo="combo_popup" disabled="disabled" maximo="0">'+
//			   '<option value="">Escolha um Estado...</option>'+
//			   '</select>';
//	$('#divMuncod').html(html);
//}
//function resetaCamp(){
//	var html = '<select style="width: 400px;" class="CampoEstilo" id="estuf" name="estuf[]" size="5" multiple="multiple" tipo="combo_popup" disabled="disabled" maximo="0">'+
//			   '<option value="">Escolha uma Instituição...</option>'+
//			   '</select>';
//	$('#divCamp').html(html);
//}
//function resetaReit(){
//	var html = '<select style="width: 400px;" class="CampoEstilo" id="estuf" name="estuf[]" size="5" multiple="multiple" tipo="combo_popup" disabled="disabled" maximo="0">'+
//			   '<option value="">Escolha uma Instituição...</option>'+
//			   '</select>';
//	$('#divReit').html(html);
//}


//var flag = true;
//$(document).ready(function() {
//
//	$('#regcod').live('dblclick', function() {
//		resetaEstuf();
//		resetaMuncod();
//		resetaCamp();
//		resetaReit();
//	});
//
//	//Espera evento Dbclick. Live mantem a ação após criar novo objeto com mesmo nome.
//	$('#estuf').live('dblclick', function() {
//
//		// Flag só deixa executar se não for ajax
//		if( flag ){
//			var arFiltro = new Array();
//			var tipoFiltro = '';
//
//			//Carrega Filtros
//			if($('#regcod').children().length && $('#regcod option')[0].value){
//				$('#regcod option').each(function(){
//					arFiltro.push($(this).val());
//					tipoFiltro = 'regcod';
//				});
//			}
//			
//			//Carrega Variaveis de filtros
//			var param = new Array();
//			param.push({name : 'requisicao', value : 'filtraEstuf'}, 
//					   {name : 'arFiltro', value : arFiltro}
//  					  );
//			//Só faz Ajax se houver filtro			   
//			if( tipoFiltro != '' ){
//				$.ajax({
//					type	 : "POST",
//					url		 : window.location,
//					data	 : param,
//					async    : false,
//					success	 : function(data){
//									$('#divEstuf').html(data);
//									//Diz para Flag que é Ajax
//									flag = false;
//									$('#estuf').attr("disabled",false);
//									$('#estuf').dblclick();
//							   }
//					 });
//			}
//			return false;
//		}
//		//Diz para a Flag que não é Ajax
//		flag = true;
//	});
//
//	//Filtro Municipios
//	$('#muncod').live('dblclick', function() {
//
//		if( flag ){
//			var arFiltro = new Array();
//			var tipoFiltro = '';
//
//			if($('#estuf').children().length && $('#estuf option')[0].value){
//				$('#estuf option').each(function(){
//					arFiltro.push($(this).val());
//					tipoFiltro = 'est.estuf';
//				});
//			}
//			
//			var param = new Array();
//			param.push({name : 'requisicao', value : 'filtraMuncod'}, 
//					   {name : 'arFiltro', value : arFiltro}
//  					  );
//			if( tipoFiltro != '' ){
//				$.ajax({
//					type	 : "POST",
//					url		 : window.location,
//					data	 : param,
//					async    : false,
//					success	 : function(data){
//									$('#divMuncod').html(data);
//									flag = false;
//									$('#muncod').attr("disabled",false);
//									$('#muncod').dblclick();
//							   }
//					 });
//			}
//			return false;
//		}
//		flag = true;
//	});
//	
//});

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

function gerarRelatorioXLS(){
	document.getElementById('req').value = 'geraxls';
	gerarRelatorio();
}

function gerarRelatorio(){
	var formulario  = document.formulario;
	var agrupadores = document.getElementsByName('agrupador[]');
	var agrup = false;
	
	for(x=0;x<agrupadores.length;x++ ){
		if(agrupadores[x].checked){
			agrup = true;
		}
	}
	if( !agrup ){
		alert('Selecione pelo menos um agrupador!');
		return false;
	}
	
	if (formulario.elements['ano'][0] == null){
		alert('Selecione pelo menos um ano!');
		return false;
	}	
	
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
//	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.ano );
	selectAllOptions( formulario.regcod );
	selectAllOptions( formulario.estuf );
	selectAllOptions( formulario.muncod );
	selectAllOptions( formulario.entidunidade );
	selectAllOptions( formulario.entidcampus );
	selectAllOptions( formulario.entidreitoria );
	selectAllOptions( formulario.itpid );
	
	formulario.target = 'relatorio';	
	formulario.submit();
	
	janela.focus();
}

//-->
</script>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" id="req" name="req" value=""/>	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top" colspan="2" style="text-align: center"><strong>Agrupadores</strong></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"></td>
		<td>
			<?
			$sql = "SELECT 'brasil'      as codigo, 'Brasil'          as descricao UNION ALL
					SELECT 'regiao'      as codigo, 'Região'          as descricao UNION ALL
					SELECT 'estado'      as codigo, 'Estado'          as descricao UNION ALL
					SELECT 'municipio'   as codigo, 'Município'       as descricao UNION ALL
					SELECT 'instituicao' as codigo, 'Instituição' 	  as descricao UNION ALL
					SELECT 'campus'      as codigo, 'Campus/Reitoria' as descricao";
			$db->monta_checkbox('agrupador[]', $sql, '');
//			$matrizAG = array(
////							array('codigo' 	  => 'indicador',
////								  'descricao' => 'Indicador'),				
//							array('codigo' 	  => 'brasil',
//								  'descricao' => 'Brasil'),
//							array('codigo' 	  => 'regiao',
//								  'descricao' => 'Região'),
//							array('codigo' 	  => 'estado',
//								  'descricao' => 'Estado'),
//							array('codigo' 	  => 'municipio',
//								  'descricao' => 'Município'),
//							array('codigo' 	  => 'instituicao',
//								  'descricao' => 'Instituição'),
//							array('codigo' 	  => 'campus',
//								  'descricao' => 'Campus/Reitoria')
//							);
			
//			$campoAgrupador = new Agrupador( 'formulario' );
//			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matrizAG );
//			$campoAgrupador->setDestino( 'agrupador', null);
//			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top" colspan="2" style="text-align: center"><strong>Colunas</strong></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Ano</td>
		<td>
			<?
			$matrizCL = array(
							array('codigo' 	  => '2005',
								  'descricao' => '2005'),				
							array('codigo' 	  => '2006',
								  'descricao' => '2006'),
							array('codigo' 	  => '2007',
								  'descricao' => '2007'),
							array('codigo' 	  => '2008',
								  'descricao' => '2008'),
							array('codigo' 	  => '2009',
								  'descricao' => '2009'),
							array('codigo' 	  => '2010',
								  'descricao' => '2010'),
							array('codigo' 	  => '2011',
								  'descricao' => '2011'),
							array('codigo' 	  => '2012',
								  'descricao' => '2012')
							);
			
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'anoOrigem', null, $matrizCL );
			$campoAgrupador->setDestino( 'ano', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top" colspan="2" style="text-align: center"><strong>Filtros</strong></td>
	</tr>
<!--	<tr>-->
<!--		<td class="SubTituloDireita" valign="top">Região(ões)</td>-->
<!--		<td>-->
			<?php
		
			// Filtros do relatório
			
//			$sql = "SELECT 
//						regcod AS codigo, 
//						regdescricao AS descricao
//		  			  FROM 
//		  			  	territorios.regiao;";
//			
//			$stSqlCarregados = "";
//			combo_popup( 'regcod', $sql, 'Selecione a(s) Regiões', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, false, null, null, true, false);
		//	mostrarComboPopup( 'Região', 'regcod',  $sql, $stSqlCarregados, 'Selecione a(s) Regiões' );
			?>
<!--		</td>-->
<!--	</tr>-->
<!--	<tr>-->
<!--		<td class="SubTituloDireita" valign="top">Estado(s)</td>-->
<!--		<td>-->
<!--			<div id="divEstuf">-->
			<?php
		
//			$sql = "SELECT 
//						estuf AS codigo, 
//						estdescricao AS descricao
//		 			FROM 
//		 				territorios.estado;";
//			
//			$stSqlCarregados = ""; 
//			mostrarComboPopup( 'Unidades da Federação', 'estuf',  $sql, $stSqlCarregados, 'Selecione a(s) Federações' );
//			combo_popup( 'estuf', $sql, 'Selecione a(s) Estado(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, false, null, null, true, false);
			?>
			
<!--			</div>-->
<!--		</td>-->
<!--	</tr>-->
<!--	<tr>-->
<!--		<td class="SubTituloDireita" valign="top">Municipio(s)</td>-->
<!--		<td>-->
<!--			<div id="divMuncod">-->
			<?php
		
//			$sql = "SELECT 
//						muncod AS codigo, 
//						mundescricao AS descricao
//				  	FROM 
//				  		territorios.municipio
//				  	ORDER BY
//				  		mundescricao;";
//			
//			$stSqlCarregados = "";
//			$arWhere = array();
//			$arWhere[] = array("codigo" => "estuf",
//							   "descricao" => "Estado");
//			mostrarComboPopup( 'Município', 'muncod',  $sql, $stSqlCarregados, 'Selecione o(s) Município(s)', $arWhere );
//			combo_popup( 'muncod', $sql, 'Selecione a(s) Municipio(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, false, null, null, true, false);
			?>
<!--			</div>-->
<!--		</td>-->
<!--	</tr>-->
<!--	<tr>-->
<!--		<td class="SubTituloDireita" valign="top">Instituição(ões)</td>-->
<!--		<td>-->
			<?php
		
//			$sql = "SELECT 
//						e.entid AS codigo,
//						e.entnome AS descricao
//				  	FROM entidade.entidade e
//				  	JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
//				  	WHERE									   
//				  		funid = " . ACA_ID_ESCOLAS_TECNICAS;
//			
//			$stSqlCarregados = "";
//			$arWhere = array();
//			$arWhere[] = array("codigo" => "entnome",
//							   "descricao" => "Instituição");
//			mostrarComboPopup( 'Instituição', 'entidunidade',  $sql, $stSqlCarregados, 'Selecione a(s) Instituição(ões)', $arWhere);
//			combo_popup( 'entidunidade', $sql, 'Selecione a(s) Instituição(ões)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, false, null, null, true, false);
			?>
<!--		</td>-->
<!--	</tr>-->
<!--	<tr>-->
<!--		<td class="SubTituloDireita" valign="top">Campus</td>-->
<!--		<td>-->
<!--			<div id="divCamp">-->
			<?php
		
//			$sql = "SELECT 
//						e.entid AS codigo,
//						e.entnome AS descricao
//				  	FROM entidade.entidade e
//				  	JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
//				  	WHERE									   
//				  		funid = " . ACA_ID_UNED;
//			
//			$stSqlCarregados = "";
//			$arWhere = array();
//			$arWhere[] = array("codigo" => "entnome",
//							   "descricao" => "Campus");
//			mostrarComboPopup( 'Campus', 'entidcampus',  $sql, $stSqlCarregados, 'Selecione o(s) Campus(i)', $arWhere);
//			combo_popup( 'entidcampus', $sql, 'Selecione a(s) Campus(i)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, false, null, null, true, false);
			?>
<!--			</div>-->
<!--		</td>-->
<!--	</tr>-->
<!--	<tr>-->
<!--		<td class="SubTituloDireita" valign="top">Reitoria(s)</td>-->
<!--		<td>-->
<!--			<div id="divReit">-->
			<?php
		
//			$sql = "SELECT 
//						e.entid AS codigo,
//						e.entnome AS descricao
//				  	FROM entidade.entidade e
//				  	JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
//				  	WHERE									   
//				  		funid = " . ACA_ID_REITORIA;
//			
//			$stSqlCarregados = "";
//			$arWhere = array();
//			$arWhere[] = array("codigo" => "entnome",
//							   "descricao" => "Reitoria");
//			mostrarComboPopup( 'Reitoria', 'entidreitoria',  $sql, $stSqlCarregados, 'Selecione a(s) Reitoria(i)', $arWhere);
//			combo_popup( 'entidreitoria', $sql, 'Selecione a(s) Reitoria(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, false, null, null, true, false);
			?>
<!--			</div>-->
<!--		</td>-->
<!--	</tr>-->
<!--	<tr>-->
<!--		<td class="SubTituloDireita" valign="top">Indicador(es)</td>-->
<!--		<td>-->
			<?php
		
//			$sql = "SELECT 
//						itpid AS codigo, 
//						itpdsc AS descricao
//		  			FROM 
//		  				academico.indicadorestcuprofissional
//		  			WHERE
//		  				itpstatus = 'A';";
//			
//			$stSqlCarregados = "";
//			mostrarComboPopup( 'Indicadores', 'itpid',  $sql, $stSqlCarregados, 'Selecione o(s) Indicador(es)');
//			combo_popup( 'itpid', $sql, 'Selecione o(s) Indicador(es)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, false, null, null, true, false);
			?>
<!--		</td>-->
<!--	</tr>-->
	<?php

	// Filtros do relatório
	
	$sql = "SELECT 
				regcod AS codigo, 
				regdescricao AS descricao
  			  FROM 
  			  	territorios.regiao;";
	
	$stSqlCarregados = "";
	mostrarComboPopup( 'Região', 'regcod',  $sql, $stSqlCarregados, 'Selecione a(s) Regiões' );
	
	$sql = "SELECT 
				estuf AS codigo, 
				estdescricao AS descricao
 			FROM 
 				territorios.estado;";
	
	$stSqlCarregados = "";
	mostrarComboPopup( 'Unidades da Federação', 'estuf',  $sql, $stSqlCarregados, 'Selecione a(s) Federações' );

	$sql = "SELECT 
				muncod AS codigo, 
				mundescricao AS descricao
		  	FROM 
		  		territorios.municipio
		  	ORDER BY
		  		mundescricao;";
	
	$stSqlCarregados = "";
	$arWhere = array();
	$arWhere[] = array("codigo" => "estuf",
					   "descricao" => "Estado");
	mostrarComboPopup( 'Município', 'muncod',  $sql, $stSqlCarregados, 'Selecione o(s) Município(s)', $arWhere );
	
	$sql = "SELECT 
				e.entid AS codigo,
				e.entnome AS descricao
		  	FROM entidade.entidade e
		  	JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
		  	WHERE									   
		  		funid = " . ACA_ID_ESCOLAS_TECNICAS;
	
	$stSqlCarregados = "";
	$arWhere = array();
	$arWhere[] = array("codigo" => "entnome",
					   "descricao" => "Instituição");
	mostrarComboPopup( 'Instituição', 'entidunidade',  $sql, $stSqlCarregados, 'Selecione a(s) Instituição(ões)', $arWhere);
	
	$sql = "SELECT 
				e.entid AS codigo,
				e.entnome AS descricao
		  	FROM entidade.entidade e
		  	JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
		  	WHERE									   
		  		funid = " . ACA_ID_UNED;
	
	$stSqlCarregados = "";
	$arWhere = array();
	$arWhere[] = array("codigo" => "entnome",
					   "descricao" => "Campus");
	mostrarComboPopup( 'Campus', 'entidcampus',  $sql, $stSqlCarregados, 'Selecione o(s) Campus(i)', $arWhere);
	
	$sql = "SELECT 
				e.entid AS codigo,
				e.entnome AS descricao
		  	FROM entidade.entidade e
		  	JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
		  	WHERE									   
		  		funid = " . ACA_ID_REITORIA;
	
	$stSqlCarregados = "";
	$arWhere = array();
	$arWhere[] = array("codigo" => "entnome",
					   "descricao" => "Reitoria");
	mostrarComboPopup( 'Reitoria', 'entidreitoria',  $sql, $stSqlCarregados, 'Selecione a(s) Reitoria(i)', $arWhere);

	$sql = "SELECT 
				itpid AS codigo, 
				itpdsc AS descricao
  			FROM 
  				academico.indicadorestcuprofissional
  			WHERE
  				itpstatus = 'A';";
	
	$stSqlCarregados = "";
	mostrarComboPopup( 'Indicadores', 'itpid',  $sql, $stSqlCarregados, 'Selecione o(s) Indicador(es)');
	?>			
	<tr>
		<td>&nbsp;</td>
		<td>
			<input type="button" value="Visualizar"    onclick="gerarRelatorio();">
			<input type="button" value="VisualizarXLS" onclick="gerarRelatorioXLS();">
		</td>
	</tr>	
</table>	
</form>