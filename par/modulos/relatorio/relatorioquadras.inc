<?php

include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

	jQuery.noConflict();
	
	function quadras_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador = $( 'colunas' );

		// Tipo de relatorio
		formulario.pesquisa.value='1';

		prepara_formulario();
		selectAllOptions( formulario.colunas );
		
		if( tipo == 'exibir' ){

			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos uma agrupador!' );
				return false;
			}

			selectAllOptions( formulario.estuf );
			selectAllOptions( formulario.muncod );
					
			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';	
			formulario.submit();
			
			janela.focus();
			
		}
		
	}

	function onOffCampo( campo ) {
		var div_on  = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input   = document.getElementById( campo + '_campo_flag' );
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
<form action="/par/par.php?modulo=relatorio/relatorioquadras_resultado&acao=A" method="post" name="filtro" id="filtro"> 
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="10%">Agrupadores</td>
			<td>
				<?php
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'uf' => array(
													'codigo'    => 'uf',
													'descricao' => '01. Estado'
						),
						'municipio' => array(
													'codigo'    => 'municipio',
													'descricao' => '02. Municipio'
						),
						'escola' => array(
													'codigo'    => 'escola',
													'descricao' => '03. Escola'
						),
						'ano' => array(
													'codigo'    => 'ano',
													'descricao' => '04. Ano'
						),
						'dependencia' => array(
													'codigo'    => 'dependencia',
													'descricao' => '05. Dependência'
						),
						'situacao' => array(
													'codigo'    => 'situacao',
													'descricao' => '06. Situação'
						),
						'quadra' => array(
													'codigo'    => 'quadra',
													'descricao' => '07. Quadra'
						),
						'tpmdsc' => array(
													'codigo'    => 'tpmdsc',
													'descricao' => '08. Grupo de Município'
						)
						
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoColunas', null, $origem );
					$agrupador->setDestino( 'colunas', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloesquerda" colspan="2">
				<strong>Filtros</strong>
			</td>
			<?
				// UF
				$ufSql = " SELECT	
								est.estuf AS codigo,
								estdescricao AS descricao
							FROM 
								territorios.estado est
							ORDER BY
								estdescricao ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estuf',  $ufSql, '', 'Selecione o(s) Estado(s)' );
				
				// Municípios
				$munSql = " SELECT
								tm.muncod AS codigo,
								tm.estuf || ' - ' || tm.mundescricao AS descricao
							FROM 
								territorios.municipio tm
							ORDER BY
								mundescricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Município', 'muncod',  $munSql, '', 'Selecione o(s) Município(s)' );
				
				// Grupo de Municipios
				$tpmSql = " SELECT 
								tpmid as codigo, 
								tpmdsc as descricao
							FROM 
								territorios.tipomunicipio
							WHERE
								tpmstatus = 'A' AND gtmid = 7";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Grupo de Municípios', 'tpmid',  $tpmSql, '', 'Selecione o(s) Grupo(s) de Municípos' );

			?>
		
		<tr>
			<td bgcolor="#CCCCCC" width="10%"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" id="visualizar" value="Visualizar" onclick="quadras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>