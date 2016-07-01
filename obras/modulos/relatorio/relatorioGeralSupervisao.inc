<?php
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo '<br/>';
monta_titulo( 'Relatório Geral de Supervisão', '&nbsp;' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

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
		formulario.action	= 'obras.php?modulo=relatorio/popUpRelatorioGeralSupervisao&acao=A';
		
		var janela = window.open( '', 'relatorio', 'width=800,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();

		var agrupador  = document.getElementById( 'agrupador' );

		selectAllOptions( agrupador );
		selectAllOptions( document.getElementById( 'uf' ) );
		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'esdid' ) );
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
				//Agrupadores: brasil, uf, situação da obra, situação da supervisão da obra, nome da obra
				$origem = array(
					'situacaoobra' => array(
						'codigo'    => 'situacaoobra',
						'descricao' => 'Situação da Obra'
					),
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
						'descricao' => 'Situação da Supervisão (Obra)'
					),
				);
				
				// exibe agrupador
				$agrupador->setOrigem( 'naoAgrupador', null, $origem );
				$agrupador->setDestino( 'agrupador', null, $destino );
				$agrupador->exibir();
			?>
			</td>
		</tr>
		<!-- Situação da Supervisão -->
		<?php
		$sql = "SELECT DISTINCT
					esdid AS codigo,
					esddsc AS descricao
				FROM 
					workflow.estadodocumento
				WHERE
					tpdid = ".OBR_TIPO_DOCUMENTO_OBRA."
				ORDER BY
					esddsc ASC";
		$sqlCarregados 	= "";
		$arrVisivel 	= array("descricao");
		$arrOrdem		= array("descricao");
		mostrarComboPopup('Situação da Supervisão:', 'esdid', $sql, $sqlCarregados, 'Selecione a(s) Situação(ões) da Supervisão', null, null, null, null, $arrVisivel, $arrOrdem);
		?>
		
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
			<td class="SubTituloDireita" style="width: 190px;">Obra em Repositório:</td>
			<td>
				<input type="radio" name="repositorio" value="S" /> Sim
				<input type="radio" name="repositorio" value="N" /> Não
				<input type="radio" name="repositorio" value="" /> Todas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Possui Supervisão Finalizada:</td>
			<td>
				<input type="radio" name="supervisao" onclick="exibeSubFiltros('S')" value="S" /> Sim
				<input type="radio" name="supervisao" onclick="exibeSubFiltros('N')" value="N" /> Não
				<input type="radio" name="supervisao" onclick="exibeSubFiltros('N')" value="" /> Todas
				<script>
					function verificaSubFiltro(tipo)
					{
						if(tipo == "entre"){
							document.getElementById('fim_subfiltro').style.display = "";
						}else{
							document.getElementById('fim_subfiltro').style.display = "none";
						}
					}
					
					function exibeSubFiltros(tipo)
					{
						if(tipo == "S"){
							document.getElementById('inicio_subfiltro').style.display = "";
						}else{
							document.getElementById('inicio_subfiltro').style.display = "none";
						}
					}
				</script>
				<span style="padding-left:10px;display:none" id="inicio_subfiltro"  >
					<?php $arrTipoSupervisao = array( 
														0=> array("codigo" => ">", "descricao" => "Maior que"),
														1=> array("codigo" => ">=", "descricao" => "Maior Igual a"),
														2=> array("codigo" => "=", "descricao" => "Igual a"),
														3=> array("codigo" => "<", "descricao" => "Menor que"),
														4=> array("codigo" => "<=", "descricao" => "Menor Igual a"),
														5=> array("codigo" => "entre", "descricao" => "Entre")
													)?>
					<?php $db->monta_combo("tiposupervisao",$arrTipoSupervisao,"S","","verificaSubFiltro","") ?>
					<?php echo campo_texto("subfiltro_inicio","N","S","",10,20,"[#]","","","","","id='subfiltro_inicio'") ?>
					<span id="fim_subfiltro" style="display:none"  >
						de 
						<?php echo campo_texto("subfiltro_fim","N","S","",10,20,"[#]","","","","","id='subfiltro_fim'") ?>
					</span>
				</span>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorio();" style="cursor:pointer;" />
			</td>
		</tr>
	</table>
</form>