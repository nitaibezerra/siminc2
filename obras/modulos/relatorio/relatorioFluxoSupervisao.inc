<?php
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo '<br/>';
monta_titulo( 'Relatório de Fluxo de Supervisão', '&nbsp;' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript"><!--

	function exibeRelatorio()
	{
		var cont = 0;
		$('[name=orgid[]]').each(function()
		{
			if( $(this).attr('checked') ) cont++;
		});

		if( cont == 0 )
		{
			alert('Pelo menos 1(um) "Tipo de Ensino" deve ser selecionado.');
			return false;
		}

		if( $('#agrupador option').length == 0 )
		{
			alert('Pelo menos 1(um) "Agrupador" deve ser escolhido.');
			return false;
		}
		
		var formulario 		= document.getElementById('formRelatorio');
		formulario.action	= 'obras.php?modulo=relatorio/popUpRelatorioFluxoSupervisao&acao=A';
		
		var janela = window.open( '', 'relatorio', 'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();

		var agrupador  = document.getElementById( 'agrupador' );

		selectAllOptions( agrupador );
		selectAllOptions( document.getElementById( 'uf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'regcod' ) );
		formulario.target = 'relatorio';
		formulario.submit();
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

	$(document).ready(function()
	{
		$('#naoAgrupador').css('width', '250px');
		$('#agrupador').css('width', '250px');
	});
//
--></script>

<form id="formRelatorio" method="post" action="">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" style="width:250px;">Tipo de Ensino:</td>
			<td>
			<?php 
			$orgaos = $db->carregar("SELECT orgid,orgdesc FROM obras.orgao");
			for($i=0; $i<count($orgaos); $i++)
			{
				echo '<input type="checkbox" id="orgid" name="orgid[]" value="'.$orgaos[$i]['orgid'].'" />'.$orgaos[$i]["orgdesc"].'&nbsp;';
			}
			?>	
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Agrupadores:</td>
			<td>
			<?php
				// Início dos agrupadores
				$agrupador = new Agrupador('formRelatorio','');
				
				// Dados padrão de destino (nulo)
				if(!$agrupador2){
					/*$destino = array('nuprocesso' => array(
										'codigo'    => 'nuprocesso',
										'descricao' => 'Nº do Processo'
									)
								);*/
				}
				else{
					$destino = $agrupador2;
				}
				
				// Dados padrão de origem
				//Agrupadores: brasil, uf, empresa, situação supervisão do grupo, situação da supervisão da obra, nível de estado da supervisão (grupo), nível de estado da supervisão (obra), nome da obra
				$origem = array(
					/*'situacaosupervisaogrupo' => array(
						'codigo'    => 'situacaosupervisaogrupo',
						'descricao' => 'Situação da Supervisão do Grupo'
					),*/
					'situacaogrupo' => array(
						'codigo'    => 'situacaogrupo',
						'descricao' => 'Situação da Supervisão do Grupo'
					),
					'empresa' => array(
						'codigo'    => 'empresa',
						'descricao' => 'Empresa'
					),/*
					'situacaoobra' => array(
						'codigo'    => 'situacaoobra',
						'descricao' => 'Situação da Obra'
					),*/
					'uf' => array(
						'codigo'    => 'uf',
						'descricao' => 'UF'
					),
					'municipio' => array(
						'codigo'    => 'municipio',
						'descricao' => 'Município'
					),	
					'regiao' => array(
						'codigo'    => 'regiao',
						'descricao' => 'Região'
					),
					'brasil' => array(
						'codigo'    => 'brasil',
						'descricao' => 'Brasil'
					),
					'nomeobra' => array(
						'codigo'    => 'nomeobra',
						'descricao' => 'Nome da Obra'
					),
					'situacaosupervisaoobra' => array(
						'codigo'    => 'situacaosupervisaoobra',
						'descricao' => 'Situação da Supervisão da Obra'
					),
					'nivelsupervisao' => array(
						'codigo'    => 'nivelsupervisao',
						'descricao' => 'Nível do Estado da Supervisão (Obra)'
					),
					'nivelsupervisaogrupo' => array(
						'codigo'    => 'nivelsupervisaogrupo',
						'descricao' => 'Nível do Estado da Supervisão (Grupo)'
					),
					'grupo' => array(
						'codigo'    => 'grupo',
						'descricao' => 'Grupo'
					)
				);
				
				// exibe agrupador
				$agrupador->setOrigem( 'naoAgrupador', null, $origem );
				$agrupador->setDestino( 'agrupador', null, $destino );
				$agrupador->exibir();
			?>
			</td>
		</tr>
		<!-- Data de Início da Supervisão -->
		<tr>
			<td class="SubTituloDireita" style="width:300px;">Data de Início da Supervisão:</td>
			<td>
			<?=campo_data2('dt_inicio_supervisao','N','S','','','','', null,'', '', 'dt_inicio_supervisao' );?>
			&nbsp;até&nbsp;
			<?=campo_data2('dt_fim_supervisao','N','S','','','','', null,'', '', 'dt_fim_supervisao' );?>
			</td>
		</tr>
				
		<!-- Região -->
		<?php
		$sql = "SELECT DISTINCT
					regcod AS codigo,
					regdescricao AS descricao
				FROM 
					territorios.regiao
				ORDER BY
					regdescricao ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Região:', 'regcod', $sql, $sqlCarregados, 'Selecione a(s) Região(ões)', null, null, null, null, $arrVisivel, $arrOrdem);
		?>
		
		<!-- UF -->
		<?php
		$sql = "SELECT DISTINCT
					estuf AS codigo,
					estdescricao AS descricao
				FROM 
					territorios.estado
				ORDER BY
					estdescricao ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('UF:', 'uf', $sql, $sqlCarregados, 'Selecione a(s) UF(s)', null, null, null, null, $arrVisivel, $arrOrdem);
		?>
		
		<!-- Município -->
		<?php
		$sql = "SELECT DISTINCT
					muncod AS codigo,
					mundescricao AS descricao
				FROM 
					territorios.municipio
				ORDER BY
					mundescricao ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Município:', 'muncod', $sql, $sqlCarregados, 'Selecione o(s) Município(s)', null, null, null, null, $arrVisivel, $arrOrdem);
		?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorio();" style="cursor:pointer;" />
			</td>
		</tr>
	</table>
</form>