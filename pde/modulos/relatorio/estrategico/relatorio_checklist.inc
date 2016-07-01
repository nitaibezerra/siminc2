<?php
// exibe consulta
if ( isset( $_REQUEST['form'] ) == true ) {
	
	include "popup_checklist_resultado.inc";
	exit;
	
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$titulo_modulo = "Relatório Checklist";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link> 
<script type="text/javascript">

	
	function checklist_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador  = document.getElementById( 'agrupadores' );

		// Tipo de relatorio
		formulario.pesquisa.value='1';

		
		prepara_formulario();
		selectAllOptions( formulario.agrupadores );
		
		selectAllOptions( agrupador );
		selectAllOptions( document.getElementById( 'projeto' ) );
		selectAllOptions( document.getElementById( 'esdid' ) );
		selectAllOptions( document.getElementById( 'perid' ) );
		
		if( tipo == 'exibir' ){
			 
			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos um agrupador!' );
				return false;
			}
			
			formulario.target = 'resultadoChecklistGeral';
			var janela = window.open( '', 'resultadoChecklistGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();

		}
		
		if( tipo == 'exibirxls' ){
		
			formulario.pesquisa.value='2';
			 
			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos um agrupador!' );
				return false;
			}
			
		}
		
		formulario.submit();
		
	}
	
	
	/* Função para substituir todos */
	function replaceAll(str, de, para){
	    var pos = str.indexOf(de);
	    while (pos > -1){
			str = str.replace(de, para);
			pos = str.indexOf(de);
		}
	    return (str);
	}
			
				
	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
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
<form action="" method="post" name="filtro"> 
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Agrupadores</td>
			<td>
				<?php
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'itemprazo' => array(
													'codigo'    => 'projeto',
													'descricao' => 'Projeto'
						),
						'itemcritico' => array(
													'codigo'    => 'atividade1',
													'descricao' => 'Atividade'
						),
						'execucao_agrupador' => array(
													'codigo'    => 'item',
													'descricao' => 'Item Checklist'
						),						
						'validacao_agrupador' => array(
													'codigo'    => 'estado',
													'descricao' => 'Estado'
						)
						
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupadores', null, $origem );
					$agrupador->setDestino( 'agrupadores', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		</table>
		
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr><td colspan="2" class="SubTituloEsquerda">Filtros</td></tr>
			<?php
				// Projetos
				$sql = "SELECT
							a.atiid as codigo,
							a.atidescricao as descricao
						FROM 
							pde.atividade a
						WHERE
							a.atiestrategico = true and
							a.atiidpai is null and
							a.atistatus = 'A' and
							a.atiid NOT IN (".PROJETO_PDE.", ".PROJETOENEM.", ".PROJETOSEB.")
						ORDER BY
							2";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Projetos', 'projeto',  $sql, $stSqlCarregados, 'Selecione o(s) Projeto(s)' );
				
				// Estado(s)
				$sql = "SELECT
							esdid AS codigo,
							esddsc AS descricao
						FROM 
							workflow.estadodocumento 
						WHERE
							tpdid = ".TIPO_FLUXO_MONITORAMENTO."
						ORDER BY
							2";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado(s)', 'esdid',  $sql, $stSqlCarregados, 'Selecione o(s) Estado(s)', Array(), '', '', '', Array('descricao'), Array('dsc') );
				
				// Periodicidade da Meta(s)
				$sql = "SELECT
							perid AS codigo,
							perdsc AS descricao
						FROM 
							painel.periodicidade
						WHERE
							perstatus = 'A'
						ORDER BY
							2";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Peridiodo(s) da Meta', 'perid',  $sql, $stSqlCarregados, 'Selecione o(s) Periodo(s)', Array(), '', '', '', Array('descricao'), Array('dsc') );
		?>
		<tr>
			<td class="SubTituloEsquerda" colspan="2">Pendências</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Prazo de Execução:</td>
			<td>  
           	 	<?=campo_data2('data_execucao_meta_inicio', 'N', 'S', 'Data Início', '##/##/####')?>&nbsp;até&nbsp;
		    	<?=campo_data2('data_execucao_meta_fim', 'N', 'S', 'Data Fim', '##/##/####')?>
            </td> 
		</tr>
		<tr>
			<td class="SubTituloDireita">Prazo de Validação:</td>
			<td>  
           	 	<?=campo_data2('data_validacao_meta_inicio', 'N', 'S', 'Data Início', '##/##/####')?>&nbsp;até&nbsp;
		    	<?=campo_data2('data_validacao_meta_fim', 'N', 'S', 'Data Fim', '##/##/####')?>
            </td> 
		</tr>
		<tr>
			<td class="SubTituloDireita">Prazo final:</td>
			<td>  
           	 	<?=campo_data2('data_meta_inicio', 'N', 'S', 'Data Início', '##/##/####')?>&nbsp;até&nbsp;
		    	<?=campo_data2('data_meta_fi', 'N', 'S', 'Data Fim', '##/##/####')?>
            </td> 
		</tr>
		<tr>
			<td class="SubTituloDireita">Relatório Aberto/Fechado:</td>
			<td>
				<input type="radio" name="arvore_" value="true" checked> Aberto 
				<input type="radio" name="arvore_" value=""> Fechado
			</td>
		</tr>
<!--		<tr>-->
<!--			<td class="SubTituloEsquerda" colspan="2">Envio de Email</td>-->
<!--		</tr>-->
		<?php 
			// Destinatário(s)
//			$stSql = " SELECT
//							ent.entid AS codigo,
//							ent.entnome AS descricao
//						FROM 
//							entidade.entidade ent 
//						INNER JOIN 
//							pde.checklistentidade cle ON cle.entid = ent.entid
//						WHERE
//							cle.tpvid in (1,2)
//						ORDER BY
//							ent.entnome";
//			mostrarComboPopup( 'Destinatário(s)', 'destinatarios',  $stSql, $stSqlCarregados, 'Selecione o(s) Destinatário(s)', Array(), '', '', '', Array('descricao'), Array('dsc') );
		?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="checklist_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				<input type="button" value="Visualizar XLS" onclick="checklist_exibeRelatorioGeral('exibirxls');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>