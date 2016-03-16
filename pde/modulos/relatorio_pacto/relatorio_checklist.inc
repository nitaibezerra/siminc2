<?php
// exibe consulta
if ( isset( $_REQUEST['form'] ) == true ) {
	
	include "checklist_resultado.inc";
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
		
		if( tipo == 'exibir' ){
			 
			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos um agrupador!' );
				return false;
			}
			
			selectAllOptions( agrupador );
			selectAllOptions( document.getElementById( 'atividade' ) );
			selectAllOptions( document.getElementById( 'etapa' ) );
			selectAllOptions( document.getElementById( 'processo' ) );
			selectAllOptions( document.getElementById( 'subprocesso' ) );
			selectAllOptions( document.getElementById( 'executores' ) );
			selectAllOptions( document.getElementById( 'validadores' ) );
			selectAllOptions( document.getElementById( 'certificadores' ) );
			selectAllOptions( document.getElementById( 'destinatarios' ) );
			
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
			
			selectAllOptions( agrupador );
			selectAllOptions( document.getElementById( 'atividade' ) );
			selectAllOptions( document.getElementById( 'etapa' ) );
			selectAllOptions( document.getElementById( 'processo' ) );
			selectAllOptions( document.getElementById( 'subprocesso' ) );
			selectAllOptions( document.getElementById( 'executores' ) );
			selectAllOptions( document.getElementById( 'validadores' ) );
			selectAllOptions( document.getElementById( 'certificadores' ) );
			selectAllOptions( document.getElementById( 'destinatarios' ) );

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
													'codigo'    => 'itemprazo',
													'descricao' => 'Prazo do item'
						),
						'itemcritico' => array(
													'codigo'    => 'itemcritico',
													'descricao' => 'Crítico'
						),
						'execucao_agrupador' => array(
													'codigo'    => 'execucao_agrupador',
													'descricao' => 'Execução'
						),
						'executores' => array(
													'codigo'    => 'executores',
													'descricao' => 'Executor(es)'
						),
						
						'validacao_agrupador' => array(
													'codigo'    => 'validacao_agrupador',
													'descricao' => 'Validação'
						),
						'validadores' => array(
													'codigo'    => 'validadores',
													'descricao' => 'Validador(es)'
						),
						'certificacao_agrupador' => array(
													'codigo'    => 'certificacao_agrupador',
													'descricao' => 'Certificação'
						),
						'certificadores' => array(
													'codigo'    => 'certificadores',
													'descricao' => 'Certificador(es)'
						),
						'etapas' => array(
													'codigo'    => 'etapas',
													'descricao' => 'Etapas'
						),
						'processos' => array(
													'codigo'    => 'processos',
													'descricao' => 'Processos'
						),
						'subprocessos' => array(
													'codigo'    => 'subprocessos',
													'descricao' => 'Sub-processos'
						),
						'atividades' => array(
													'codigo'    => 'atividades',
													'descricao' => 'Atividades'
						),
						'pessoas' => array(
													'codigo'    => 'pessoas',
													'descricao' => 'Pessoa'
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
				// Etapas
				$stSql = "  SELECT 
								CASE 
									WHEN ati3.atitipoenem = 'E' THEN ati3.atiid 
									WHEN ati4.atitipoenem = 'E' THEN ati4.atiid 
								END as codigo,
								CASE 
									WHEN ati3.atitipoenem = 'E' THEN ati3._atinumero ||' - '|| ati3.atidescricao 
									WHEN ati4.atitipoenem = 'E' THEN ati4._atinumero ||' - '|| ati4.atidescricao
								END as descricao 
							FROM 
								pde.itemchecklist icl
							LEFT JOIN 
								pde.atividade ati ON ati.atiid = icl.atiid AND ati.atistatus = 'A' 
							LEFT JOIN 
								pde.atividade ati2 ON ati2.atiid = ati.atiidpai AND ati2.atistatus = 'A'
							LEFT JOIN 
								pde.atividade ati3 ON ati3.atiid = ati2.atiidpai AND ati3.atistatus = 'A'
							LEFT JOIN 
								pde.atividade ati4 ON ati4.atiid = ati4.atiidpai AND ati4.atistatus = 'A'
							ORDER BY 2";
				$stSql = "SELECT 
									ati.atiid as codigo,
									ati._atinumero ||' - '|| ati.atidescricao as descricao 
							FROM pde.atividade ati --ON ati.atiid = icl.atiid AND ati.atistatus = 'A' 
								where ati.atistatus = 'A' and atitipoenem = 'E'
							ORDER BY ati._atinumero";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Etapas', 'etapa',  $stSql, $stSqlCarregados, 'Selecione a(s) Etapa(s)' );
			
				// Processos
				$stSql = "  SELECT 
								CASE 
									WHEN ati2.atitipoenem = 'P' THEN ati2.atiid 
									WHEN ati3.atitipoenem = 'P' THEN ati3.atiid 
								END as codigo,
								CASE 
									WHEN ati2.atitipoenem = 'P' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
									WHEN ati3.atitipoenem = 'P' THEN ati3._atinumero ||' - '|| ati3.atidescricao
								END as descricao 
							FROM 
								pde.itemchecklist icl
							LEFT JOIN 
								pde.atividade ati ON ati.atiid = icl.atiid AND ati.atistatus = 'A' 
							LEFT JOIN 
								pde.atividade ati2 ON ati2.atiid = ati.atiidpai AND ati2.atistatus = 'A'
							LEFT JOIN 
								pde.atividade ati3 ON ati3.atiid = ati2.atiidpai AND ati3.atistatus = 'A'
							ORDER BY 2";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Processos', 'processo',  $stSql, $stSqlCarregados, 'Selecione o(s) Processo(s)' );
			
				// Sub-Processos
				$stSql = "  SELECT 
								CASE 
									WHEN ati2.atitipoenem = 'S' THEN ati2.atiid 
								END as codigo,
								CASE 
									WHEN ati2.atitipoenem = 'S' THEN ati2._atinumero ||' - '|| ati2.atidescricao 
								END as descricao
							FROM 
								pde.itemchecklist icl
							INNER JOIN 
								pde.atividade ati ON ati.atiid = icl.atiid AND ati.atistatus = 'A' 
							INNER JOIN 
								pde.atividade ati2 ON ati2.atiid = ati.atiidpai AND ati2.atistatus = 'A'
							ORDER BY 2";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Sub-processos', 'subprocesso',  $stSql, $stSqlCarregados, 'Selecione o(s) Sub-processo(s)' );
			
				// Atividades
				$stSql = " SELECT
								ati.atiid AS codigo,
								ati._atinumero||' - '||ati.atidescricao AS descricao
							FROM 
								pde.atividade ati 
							INNER JOIN 
								pde.itemchecklist icl ON icl.atiid = ati.atiid 
							GROUP BY
								ati.atiid, ati.atidescricao, ati._atinumero, ati._atiordem  
							ORDER BY
								ati._atiordem";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Atividades', 'atividade',  $stSql, $stSqlCarregados, 'Selecione a(s) Atividade(s)' );

				// Executor(es)
				$stSql = " SELECT
								ent.entid AS codigo,
								ent.entnome AS descricao
							FROM 
								entidade.entidade ent 
							INNER JOIN 
								pde.checklistentidade cle ON cle.entid = ent.entid
							WHERE
								cle.tpvid=1
							ORDER BY
								2";
				mostrarComboPopup( 'Executor(es)', 'executores',  $stSql, $stSqlCarregados, 'Selecione o(s) Executor(es)', Array(), '', '', '', Array('descricao'), Array('dsc') );

				// Validador(es)
				$stSql = " SELECT
								ent.entid AS codigo,
								ent.entnome AS descricao
							FROM 
								entidade.entidade ent 
							INNER JOIN 
								pde.checklistentidade cle ON cle.entid = ent.entid
							WHERE
								cle.tpvid=2
							ORDER BY
								ent.entnome";
				mostrarComboPopup( 'Validador(es)', 'validadores',  $stSql, $stSqlCarregados, 'Selecione o(s) Validador(es)', Array(), '', '', '', Array('descricao'), Array('dsc') );
				
				// Certificador(es)
				$stSql = " SELECT
								ent.entid AS codigo,
								ent.entnome AS descricao
							FROM 
								entidade.entidade ent 
							INNER JOIN 
								pde.checklistentidade cle ON cle.entid = ent.entid
							WHERE
								cle.tpvid=3
							ORDER BY
								ent.entnome";
				mostrarComboPopup( 'Certificador(es)', 'certificadores',  $stSql, $stSqlCarregados, 'Selecione o(s) Certificador(es)', Array(), '', '', '', Array('descricao'), Array('dsc') );
				
				// Pessoa(s)
				$stSql = " SELECT
								ent.entid AS codigo,
								ent.entnome AS descricao
							FROM 
								entidade.entidade ent 
							INNER JOIN 
								pde.checklistentidade cle ON cle.entid = ent.entid
							WHERE
								cle.tpvid in (1,2,3)
							ORDER BY
								ent.entnome";
				mostrarComboPopup( 'Pessoa(s)', 'pessoas',  $stSql, $stSqlCarregados, 'Selecione a(s) Pessoa(s)', Array(), '', '', '', Array('descricao'), Array('dsc') );
		?>
		<tr>
			<td class="SubTituloEsquerda" colspan="2">Pendências</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Executado:</td>
			<td><input type="radio" name="possuiexecucao" value="" checked> Não aplica <input type="radio" name="possuiexecucao" value="nao" > Não <input type="radio" name="possuiexecucao" value="sim"> Sim</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Validado:</td>
			<td><input type="radio" name="possuivalidacao" value="" checked> Não aplica <input type="radio" name="possuivalidacao" value="nao" > Não <input type="radio" name="possuivalidacao" value="sim"> Sim</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Certificado:</td>
			<td><input type="radio" name="possuicertificacao" value="" checked> Não aplica <input type="radio" name="possuicertificacao" value="nao" > Não <input type="radio" name="possuicertificacao" value="sim"> Sim</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Prazo vencido:</td>
			<td><input type="radio" name="prazovencido" value="nao" checked> Não <input type="radio" name="prazovencido" value="sim"> Sim</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Prazo de Execução:</td>
			<td>  
           	 	<?=campo_data2('data_inicio_exec', 'N', 'S', 'Data Início', '##/##/####')?>&nbsp;até&nbsp;
		    	<?=campo_data2('data_fim_exec', 'N', 'S', 'Data Fim', '##/##/####')?>
            </td> 
		</tr>
		<tr>
			<td class="SubTituloDireita">Prazo de Validação:</td>
			<td>  
           	 	<?=campo_data2('data_inicio_val', 'N', 'S', 'Data Início', '##/##/####')?>&nbsp;até&nbsp;
		    	<?=campo_data2('data_fim_val', 'N', 'S', 'Data Fim', '##/##/####')?>
            </td> 
		</tr>
		<tr>
			<td class="SubTituloDireita">Prazo de Certificação:</td>
			<td>  
           	 	<?=campo_data2('data_inicio_cert', 'N', 'S', 'Data Início', '##/##/####')?>&nbsp;até&nbsp;
		    	<?=campo_data2('data_fim_cert', 'N', 'S', 'Data Fim', '##/##/####')?>
            </td> 
		</tr>
		<tr>
			<td class="SubTituloDireita">Prazo final:</td>
			<td>  
           	 	<?=campo_data2('data_inicio', 'N', 'S', 'Data Início', '##/##/####')?>&nbsp;até&nbsp;
		    	<?=campo_data2('data_fim', 'N', 'S', 'Data Fim', '##/##/####')?>
            </td> 
		</tr>
		<tr>
			<td class="SubTituloDireita">Relatório Aberto/Fechado:</td>
			<td>
				<input type="radio" name="arvore_" value="true" checked> Aberto 
				<input type="radio" name="arvore_" value=""> Fechado
			</td>
		</tr>
		<tr>
			<td class="SubTituloEsquerda" colspan="2">Envio de Email</td>
		</tr>
		<?php 
			// Destinatário(s)
			$stSql = " SELECT
							ent.entid AS codigo,
							ent.entnome AS descricao
						FROM 
							entidade.entidade ent 
						INNER JOIN 
							pde.checklistentidade cle ON cle.entid = ent.entid
						WHERE
							cle.tpvid in (1,2,3)
						ORDER BY
							ent.entnome";
			mostrarComboPopup( 'Destinatário(s)', 'destinatarios',  $stSql, $stSqlCarregados, 'Selecione o(s) Destinatário(s)', Array(), '', '', '', Array('descricao'), Array('dsc') );
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