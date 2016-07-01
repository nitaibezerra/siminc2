<?php

if( $_REQUEST['req'] ){
	include_once APPRAIZ.'par/modulos/relatorio/proinfancia/resultRelatorioMobEquipProinfancia.inc';
	$_REQUEST['req']();
	die();
}

include APPRAIZ. '/includes/Agrupador.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatórios de Mobiliário e Equipamentos - Proinfância", '<b>Filtros de Pesquisa</b>');

?>
<html>
<body>
<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<form method="post" name="formulario" id="formulario">
	<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<!--<tr>
			<td class="SubTituloDireita" style="width: 33%" valign="top"><b>Ordenação</b></td>
			<td>
				<?php
				$campoAgrupador = new Agrupador( 'formulario' );
				$campoAgrupador->setOrigem( 'agrupadorOrigem', null, Array() );
				$campoAgrupador->setDestino( 'agrupador', null, Array());
				$campoAgrupador->exibir();
				?>
			</td>
		</tr>-->
		<tr>
			<td class="SubTituloDireita" style="width: 33%" valign="top"><b>Colunas</b></td>
			<td>
				<?php
				
				$campoColuna = new Agrupador( 'formulario' );
				$campoColuna->setOrigem( 'colunaOrigem', null, Array() );
				$campoColuna->setDestino( 'coluna', null, Array(
																Array('codigo' => 'obrid',						'descricao' => 'ID Obra'),
																Array('codigo' => 'preid', 						'descricao' => 'ID Pré-Obra'),
																Array('codigo' => 'estuf',						'descricao' => 'UF'),
																Array('codigo' => 'mundescricao', 				'descricao' => 'Município'),
																Array('codigo' => 'processo', 					'descricao' => 'Número do Processo'),
																Array('codigo' => 'sub_valor', 					'descricao' => 'Valor da Subação'),
																Array('codigo' => 'boo_empenho', 				'descricao' => 'Empenho'),
																Array('codigo' => 'boo_pagamento', 				'descricao' => 'Pagamento'),
																Array('codigo' => 'sbdplanointerno',	 		'descricao' => 'Plano Interno'),
																Array('codigo' => 'sbdptres',	 				'descricao' => 'PTRES'),
																Array('codigo' => 'programa',					'descricao' => 'Programa'),
																Array('codigo' => 'codigo', 					'descricao' => 'Subação'),
																Array('codigo' => 'sbadsc',						'descricao' => 'Descrição da Subação'),
																Array('codigo' => 'sbdano',						'descricao' => 'Ano da Subação'),
																//Status PAR
																Array('codigo' => 'ssudescricao', 				'descricao' => 'Status da Subação'),
																Array('codigo' => 'frmdsc',		 				'descricao' => 'Forma de Execução'),
																Array('codigo' => 'dimcod',	 					'descricao' => 'Dimensão'),
																Array('codigo' => 'tiposubacao',				'descricao' => 'Tipo da Subação'),
																Array('codigo' => 'qtditemcomposicao_aprovado',	'descricao' => 'Quantidade Aprovada de Itens de Composição'),
// 																Array('codigo' => 'qtditemcomposicao',			'descricao' => 'Item de Composição'),
																Array('codigo' => 'boo_documento',				'descricao' => 'Documento'),
																Array('codigo' => 'dopnumerodocumento',			'descricao' => 'Número do Termo'),
																Array('codigo' => 'esddsc_par',					'descricao' => 'Situação da Pre-Obra'),
																Array('codigo' => 'obrnumprocessoconv',			'descricao' => 'N° Processo da Obra'),
																Array('codigo' => 'numconvenio',				'descricao' => 'Nº Convênio da obra '),
																Array('codigo' => 'obranoconvenio',				'descricao' => 'Ano Convênio da obra'),
																Array('codigo' => 'predescricao',				'descricao' => 'Nome da Obra'),
																Array('codigo' => 'esddsc_obras',				'descricao' => 'Situação no Obras 2.0'),
																Array('codigo' => 'perc_exec_inst',				'descricao' => '% Executado Instituição'),
																Array('codigo' => 'perc_exec_emp',				'descricao' => '% Executado Empresa'),
																Array('codigo' => 'tpodsc',						'descricao' => 'Tipologia'),
																Array('codigo' => 'prfdesc',					'descricao' => 'Programa Obras 2.0'),
																Array('codigo' => 'toodescricao',				'descricao' => 'Fonte'),
																Array('codigo' => 'esfera',						'descricao' => 'Esfera'),
						
																));
				$campoColuna->exibir();
				?>
			</td>
		</tr>
		
		
		<tr>
	    	<td class="SubTituloDireita" colspan="2"><center>Filtros</center></td>
		</tr>
		<tr>
	    	<td class="SubTituloDireita">Número de Processo:</td>
	        <td>
	        	<?php
	            $numeroprocesso = simec_htmlentities($_POST['numeroprocesso']);
	            echo campo_texto('numeroprocesso', 'N', 'S', '', 60, 200, '#####.######/####-##', '', '', '', '', '', '', $numeroprocesso);
	            ?>
			</td>
		</tr>
		<tr>
	    	<td class="SubTituloDireita" width="30%">ID:</td>
	        <td><?php echo campo_texto('obrid', 'N', 'S', 'Preid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['obrid']); ?></td>
		</tr>
		<tr>
	    	<td class="SubTituloDireita" width="30%">Pré ID:</td>
	        <td><?php echo campo_texto('preid', 'N', 'S', 'Preid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['preid']); ?></td>
		</tr>
		<?php
			//Filtro dos Estados do obras 2
			$sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid = 105 AND esdstatus = 'A' ORDER BY esdordem";
			$stSqlCarregados = "";
			mostrarComboPopup( '<b>Situação da obra:(situações Obras 2.0)</b>', 'esdid',  $sql, $stSqlCarregados, 'Selecione o Documento' ); 
		?>
		<tr>
			<td class="subtitulodireita"><b>Esfera:</b></td>
			<td>
				<input type="radio" value="E" 	id="esfera" name="esfera" <? if($_REQUEST["esfera"] == "S") { echo "checked"; } ?> /> Estadual
				<input type="radio" value="M" 	id="esfera" name="esfera" <? if($_REQUEST["esfera"] == "N") { echo "checked"; } ?> /> Municipal
				<input type="radio" value="" 	id="esfera" name="esfera" <? if($_REQUEST["esfera"] == "")  { echo "checked"; } ?> /> Todas
			</td>
		</tr>
		<?php
			//Filtro UF
			$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY 1";
			$stSqlCarregados = "";
			mostrarComboPopup( '<b>UF:</b>', 'estuf',  $sql, $stSqlCarregados, 'Selecione o Documento' ); 
		?>
		<tr>
	    	<td class="SubTituloDireita" width="30%">Município:</td>
	        <td><?php echo campo_texto('mundescricao', 'N', 'S', 'Municipio', 60, 200, '', '', '', '', '', '', '', $_REQUEST['mundescricao']); ?></td>
		</tr>
		<?php
			//Ano subação
			$sql = "SELECT DISTINCT ano as codigo, ano as descricao FROM public.anos WHERE ano::integer > 2010 AND ano::integer <= to_char(now(),'YYYY')::integer";
			$stSqlCarregados = "";
			mostrarComboPopup( '<b>Ano da subação:</b>', 'sbdano',  $sql, $stSqlCarregados, 'Selecione o Ano' ); 
		?>
		<tr>
			<td class="subtitulodireita"><b>Empenhado:</b></td>
			<td>
				<input type="radio" value="S" id="empenhado" name="empenhado" <? if($_REQUEST["empenhado"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="empenhado" name="empenhado" <? if($_REQUEST["empenhado"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value=""  id="empenhado" name="empenhado" <? if($_REQUEST["empenhado"] == "")  { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Termo Gerado:</b></td>
			<td>
				<input type="radio" value="S" id="termogerado" name="termogerado" <? if($_REQUEST["termogerado"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="termogerado" name="termogerado" <? if($_REQUEST["termogerado"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value=""  id="termogerado" name="termogerado" <? if($_REQUEST["termogerado"] == "")  { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Termo Validado:</b></td>
			<td>
				<input type="radio" value="S" id="termovalidado" name="termovalidado" <? if($_REQUEST["termovalidado"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="termovalidado" name="termovalidado" <? if($_REQUEST["termovalidado"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value=""  id="termovalidado" name="termovalidado" <? if($_REQUEST["termovalidado"] == "")  { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<!--  
		<tr>
			<td class="subtitulodireita"><b>Publicação Anexada:</b></td>
			<td>
				<input type="radio" value="S" id="publicacao_anexada" name="publicacao_anexada" <? if($_REQUEST["publicacao_anexada"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="publicacao_anexada" name="publicacao_anexada" <? if($_REQUEST["publicacao_anexada"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value=""  id="publicacao_anexada" name="publicacao_anexada" <? if($_REQUEST["publicacao_anexada"] == "")  { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		-->
		<tr>
			<td class="subtitulodireita"><b>Pagamento Solicitado:</b></td>
			<td>
				<input type="radio" value="S" id="pagamento_solicitado" name="pagamento_solicitado" <? if($_REQUEST["pagamento_solicitado"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="pagamento_solicitado" name="pagamento_solicitado" <? if($_REQUEST["pagamento_solicitado"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value=""  id="pagamento_solicitado" name="pagamento_solicitado" <? if($_REQUEST["pagamento_solicitado"] == "")  { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" style="text-align: center;">
				<input type="button" id="pesquisar" value="Pesquisar" style="cursor: pointer;"/>
				<input type="button" id="exportar" value="Exportar XLS" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
</body>
<script type="text/javascript">
	function onOffCampo( campo ){
		var div_on 	= document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input 	= document.getElementById( campo + '_campo_flag' );
		
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}else{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}

	$(document).ready(function(){
		
		$('#pesquisar').click(function(){

			var form = $('#formulario');
			
			if( $('#coluna').children().size() < 1 ){
				alert('Selecione pelo menos uma coluna');
				$('#coluna').focus();
				return false;
			}

			$('#coluna').children().prop('selected',true);
			$('#esdid').children().prop('selected',true);
			$('#estuf').children().prop('selected',true);
			$('#sbdano').children().prop('selected',true);
			var janela = window.open('par.php?modulo=relatorio/proinfancia/formRelatorioMobEquipProinfancia&acao=A&req=gerar_relatorio&'+$('#formulario').serialize(),'page','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
			janela.focus();
		});
		
		$('#exportar').click(function(){

			var form = $('#formulario');
			
			if( $('#coluna').children().size() < 1 ){
				alert('Selecione pelo menos uma coluna');
				$('#coluna').focus();
				return false;
			}

			$('#coluna').children().prop('selected',true);
			$('#esdid').children().prop('selected',true);
			$('#estuf').children().prop('selected',true);
			$('#sbdano').children().prop('selected',true);
			var janela = window.open('par.php?modulo=relatorio/proinfancia/formRelatorioMobEquipProinfancia&acao=A&req=gerar_relatorio_xls&'+$('#formulario').serialize(),'page','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
			janela.focus();
		});
	});
</script>
</html>