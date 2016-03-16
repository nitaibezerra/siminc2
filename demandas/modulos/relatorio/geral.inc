<?php

include APPRAIZ . '/includes/Agrupador.php';
include APPRAIZ . '/includes/Relatorio.php';
include 'RelatorioGeral.class.inc';

$agrupador = array();
// Executa ações de acordo com o que for passado no subAcao
switch ( $_REQUEST['subAcao'] ) {
	case 'salvar' : Relatorio::gravarParametrosBanco( $_REQUEST['titulo'] ); unset( $_REQUEST ); break;
	case 'excluir' : 
		Relatorio::excluirParametroBanco( $_REQUEST['prtid'] );
		echo '<script type="text/javascript"> location.href = "?modulo=relatorio/geral&acao=R"; </script>';
		break;
	case 'carregar' :
		$_REQUEST = Relatorio::carregarParametrosBanco( $_REQUEST['prtid'] );
		if ( $_REQUEST['agrupador'] ){
			foreach ( $_REQUEST['agrupador'] as $valorAgrupador ) {
				array_push( $agrupador, array( 'codigo' => $valorAgrupador, 'descricao' => RelatorioGeralTraducao::pegarTitulo( $valorAgrupador ) ) );
			}
		}
		break;
		
	case 'carregarExibir' : $_REQUEST = Relatorio::carregarParametrosBanco( $_REQUEST['prtid'] );
	case 'relatorio' : include 'resultado_geral.inc'; exit(); break;
	case 'planilha'  : include 'resultado_geral.inc'; exit(); break;
	case 'publico'   : 
		Relatorio::parametroBancoTornarPublico( $_REQUEST['prtid'] );
		break;
}

/**
 * Monta o HTTML do combo_popup
 *
 * @param string $stDescricao Descrição do campo
 * @param string $stNomeCampo Nome do campo que será criado HTML/PHP
 * @param string $sql_combo SQL para carregar as opções
 * @param string $sql_carregados SQL para mostrar campos selecionados
 * @param string $stTextoSelecao Texto para aparecer no popup
 * @author Cristiano Teles
 * @since 25/11/2008
 */
function mostrarComboboxPopup( $stDescricao, $stNomeCampo, $sql_combo, $sql_carregados, $stTextoSelecao ){
	global $db, $$stNomeCampo;
	
	if ( $_REQUEST[$stNomeCampo] && $_REQUEST[$stNomeCampo][0] != '' && !empty( $sql_carregados ) ) {
		$sql_carregados = sprintf( $sql_carregados, "'" . implode( "','", $_REQUEST[$stNomeCampo] ) . "'" );
		$$stNomeCampo = $db->carregar( sprintf( $sql_combo, $sql_carregados ) );
	}
	
	echo '<tr>
				<td width="195" class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );">
					' . $stDescricao . '
					<input type="hidden" id="' . $stNomeCampo . '_campo_flag" name="' . $stNomeCampo . '_campo_flag" value="0"/>
				</td>
				<td>
	';
	echo '<div id="' . $stNomeCampo . '_campo_off" style="color:#a0a0a0;';
	echo !empty( $$stNomeCampo ) ? 'display:none;' : '';
	echo '" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>';
	echo '<div id="' . $stNomeCampo . '_campo_on" '; 
	echo empty( $$stNomeCampo ) ? 'style="display:none;"' : '';
	echo '>';
	combo_popup( $stNomeCampo, sprintf( $sql_combo, '' ), $stTextoSelecao, '400x400', 0, array(), '', 'S', true, true );
	echo '</div>
			</td>
		</tr>';
}


// variáveis que alimentam o formulário
// variáveis dos filtros gerais
$titulo = '';
$nao_mostra_filtro_impressao = false;
$prtid = '';
// FIM variáveis que alimentam o formulário

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório de Indicadores', 'Módulo Demandas' );

?>
<script language="JavaScript">
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

/**
 * Realiza submissão de formulário. Caso o exercício (ano) tenha sido
 * alterado a submissão é realizada para a própria página, caso contrário
 * para uma nova janela.
 * 
 * @return void
 */
function submeterFormulario( tipo, boVerificaCampos )
{
	var formulario = document.formulario;
	var nomerel = '';
	var qtd = 0;
	<?php $qtdrel = 0; ?>
	prepara_formulario();
	selectAllOptions( formulario.agrupador );
	
	if ( tipo == 'relatorio' )
	{
		if ( boVerificaCampos != false ) {
			if ( formulario.agrupador.options.length == 0 ) {
				alert( 'Escolha pelo menos um agrupador.' );
				return;
			}
			/*
			if ( document.formulario.titulo.value == '' ){
				alert( 'É necessário informar o título do relatório!' );
				document.formulario.titulo.focus();
				return;
			}
			*/
		}
		formulario.action = 'demandas.php?modulo=relatorio/geral&acao=R';
		window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';
	}
	else
	{
		if ( tipo == 'planilha' ) {
			formulario.action = 'demandas.php?modulo=relatorio/geral&acao=R&tipoRelatorio=xls';
		}
		else if ( tipo == 'salvar' ) {
			if ( formulario.agrupador.options.length == 0 ) {
				alert( 'Escolha pelo menos um agrupador.' );
				return;
			}
			if ( document.formulario.titulo.value == '' ){
				alert( 'É necessário informar o título do relatório!' );
				document.formulario.titulo.focus();
				return;
			}
			var nomesExistentes = new Array();
			<?php
				$sqlNomesConsulta = "select prtdsc from public.parametros_tela";
				$nomesExistentes = $db->carregar( $sqlNomesConsulta );
				if ( $nomesExistentes )
				{
					foreach ( $nomesExistentes as $linhaNome )
					{
						print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
					}
				}
			?>
			var confirma = true;
			var i, j = nomesExistentes.length;
			for ( i = 0; i < j; i++ )
			{
				if ( nomesExistentes[i] == document.formulario.titulo.value )
				{
					confirma = confirm( 'Deseja alterar a consulta já existente?' );
					break;
				}
			}
			if ( !confirma )
			{
				return;
			}
			formulario.action = 'demandas.php?modulo=relatorio/geral&acao=R';
		}
		else
		{
			formulario.action = '';
		}
		formulario.target = '_top';
	}
	document.formulario.subAcao.value = tipo;
	formulario.submit();
}

function alterarAno()
{
	var formulario = document.formulario;
	formulario.alterar_ano.value = '1';
	submeterFormulario('relatorio');
}
</script>
<form action="" method="post" name="formulario">
	
	<input type="hidden" name="form" value="1"/> <!-- indica envio de formulário -->
	<input type="hidden" name="alterar_ano" value="0"/> <!-- indica se há mudança de ano no formulário -->
	<input type="hidden" name="prtid" value="<?= $prtid ?>"/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="subAcao" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->

	<!-- ----------------------------------------------------------------------- -->
	
	<!-- INSTITUCIONAL -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td class="SubTituloDireita" valign="top">Título</td>
				<td>
					<?php 
					$titulo = isset( $_REQUEST['titulo'] ) ? $_REQUEST['titulo'] : '';
					echo campo_texto( 'titulo', 'N', 'S', '', 78, 100, '', '' ); ?>
				</td>
			</tr>
<?php
/*
			<tr>
				<td class="SubTituloDireita" valign="top">Exercício</td>
				<td>
					<?
						$sqlAno = 'select rofano as codigo, rofano as descricao from financeiro.execucao group by rofano order by rofano desc';
						$db->monta_combo( 'ano', $sqlAno, 'S', '', 'alterarAno', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Escala</td>
				<td>
					<select name="escala" class="CampoEstilo">
						<option value="1" <?= $escala == '1' ? 'selected="selected"' : '' ; ?>>R$ 1 (Reais)</option>
						<option value="1000" <?= $escala == '1000' ? 'selected="selected"' : '' ; ?>>R$ 1.000 (Milhares de Reais)</option>
					</select>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Filtro</td>
				<td>
					<input type="checkbox" name="nao_mostra_filtro_impressao" id="nao_mostra_filtro_impressao" value="1" <?= $nao_mostra_filtro_impressao ? 'checked="checked"' : '' ; ?>/>
					<label for="nao_mostra_filtro_impressao">Não mostrar filtros</label>
				</td>
			</tr>
*/  ?>
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">Agrupadores</td>
				<td>
					<?php
						$matriz = RelatorioGeralTraducao::pegarAgrupadores();
						$campoAgrupador = new Agrupador( 'formulario' );
						$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
						$campoAgrupador->setDestino( 'agrupador', 4 );
						$campoAgrupador->exibir();
					?>
				</td>
			</tr>
	</table>
	
	<!-- OUTROS FILTROS -->
	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'outros' );">
				<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
				Relatórios Gerenciais
				<input type="hidden" id="outros_flag" name="outros_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="outros_div_filtros_off">
		<!--
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
			</tr>
		</table>
		-->
	</div>

	<div id="outros_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
					<?php
						$sql = sprintf(
							"SELECT Case when prtpublico = true and usucpf = '%s' then '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '<a href=\"javascript: carregar_consulta(' || prtid || ');\">' || prtdsc || '</a>' as descricao FROM public.parametros_tela WHERE mnuid = %d AND prtpublico = TRUE",
							$_SESSION['usucpf'],
							$_SESSION['mnuid'],
							$_SESSION['usucpf']
						);
						$cabecalho = array( 'Ação', 'Nome' );
					?>
					<td><?php $db->monta_lista_simples( $sql, null, 50, 50, null, null, null ); ?>
					</td>
				</tr>
		</table>
	</div>
						<script language="javascript">	//alert( document.formulario.agrupador_combo.value );	</script>

	<!-- FIM OUTROS FILTROS -->
	
	<!-- MINHAS CONSULTAS -->
	
	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'minhasconsultas' );">
				<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
				Minhas Consultas
				<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="minhasconsultas_div_filtros_off">
		<!--
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td><span style="color:#a0a0a0;padding:0 30px;">nenhum filtro</span></td>
			</tr>
		</table>
		-->
	</div>
	<div id="minhasconsultas_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Consultas:</td>
					<?php
						$sql = sprintf(
							"SELECT Case when prtpublico = false then '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '<a href=\"javascript: carregar_consulta(' || prtid || ')\">' || prtdsc || '</a>' as descricao FROM public.parametros_tela WHERE mnuid = %d AND usucpf = '%s'",
							$_SESSION['mnuid'],
							$_SESSION['usucpf']
						);
						$cabecalho = array(
							'Ação',
							'Nome'
						);
					?>
					<td><?php $db->monta_lista_simples( $sql, null, 50, 50, null, null, null ); ?></td>
				</tr>
		</table>
	</div>
	
	<!-- FIM MINHAS CONSULTAS -->

	<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'inst' );">
				<img border="0" src="/imagens/mais.gif" id="inst_img"/>&nbsp;
				Filtros
				<input type="hidden" id="inst_flag" name="inst_flag" value="0" />
			</td>
		</tr>
	</table>
	<div id="inst_div_filtros_off">
	</div>
	<div id="inst_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<?php
			
			
			//////////// MÊS //////////////
			//$stSql = 'SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid = 31 %s ORDER BY esdid';
			//$stSqlCarregados = " AND esdid IN ( %s )";
			//mostrarComboboxPopup( 'Situação', 'esdid',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(s)' );
			$mes = array();
			if ( $_REQUEST['mes'] && $_REQUEST['mes'][0] != '' )
			{
				for($i=0; $i<count($_REQUEST['mes']); $i++){
					if($_REQUEST['mes'][$i] == '1') $sql_carregados3 .= "select '1' as codigo, 'Janeiro' as descricao";
					if($_REQUEST['mes'][$i] == '2') $sql_carregados3 .= "select '2' as codigo, 'Fevereiro' as descricao";
					if($_REQUEST['mes'][$i] == '3') $sql_carregados3 .= "select '3' as codigo, 'Março' as descricao";
					if($_REQUEST['mes'][$i] == '4') $sql_carregados3 .= "select '4' as codigo, 'Abril' as descricao";
					if($_REQUEST['mes'][$i] == '5') $sql_carregados3 .= "select '5' as codigo, 'Maio' as descricao";
					if($_REQUEST['mes'][$i] == '6') $sql_carregados3 .= "select '6' as codigo, 'Junho' as descricao";
					if($_REQUEST['mes'][$i] == '7') $sql_carregados3 .= "select '7' as codigo, 'Julho' as descricao";
					if($_REQUEST['mes'][$i] == '8') $sql_carregados3 .= "select '8' as codigo, 'Agosto' as descricao";
					if($_REQUEST['mes'][$i] == '9') $sql_carregados3 .= "select '9' as codigo, 'Setembro' as descricao";
					if($_REQUEST['mes'][$i] == '10') $sql_carregados3 .= "select '10' as codigo, 'Outubro' as descricao";
					if($_REQUEST['mes'][$i] == '11') $sql_carregados3 .= "select '11' as codigo, 'Novembro' as descricao";
					if($_REQUEST['mes'][$i] == '12') $sql_carregados3 .= "select '12' as codigo, 'Dezembro' as descricao";
					if($i != count($_REQUEST['mes'])-1) $sql_carregados3 .= " union all ";
					 
				}
				
				if($sql_carregados3) $mes = $db->carregar( $sql_carregados3 );					
			}
			$stSql = " select '1' as codigo, 'Janeiro' as descricao
  					   union all
					   select '2' as codigo, 'Fevereiro' as descricao
					   union all
					   select '3' as codigo, 'Março' as descricao
					   union all
					   select '4' as codigo, 'Abril' as descricao
					   union all
					   select '5' as codigo, 'Maio' as descricao
					   union all
					   select '6' as codigo, 'Junho' as descricao
					   union all					   
					   select '7' as codigo, 'Julho' as descricao
					   union all					   
					   select '8' as codigo, 'Agosto' as descricao
					   union all					   
					   select '9' as codigo, 'Setembro' as descricao
					   union all					   
					   select '10' as codigo, 'Outubro' as descricao
					   union all					   
					   select '11' as codigo, 'Novembro' as descricao
					   union all					   
					   select '12' as codigo, 'Dezembro' as descricao";
			
			mostrarComboPopup( 'Mês:', 'mes',  $stSql, '', 'Selecione o(s) Mês(es)' );
						
			//////////// SITUAÇÃO //////////////
			//$stSql = 'SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid = 31 %s ORDER BY esdid';
			//$stSqlCarregados = " AND esdid IN ( %s )";
			//mostrarComboboxPopup( 'Situação', 'esdid',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(s)' );
			$esdid = array();
			if ( $_REQUEST['esdid'] && $_REQUEST['esdid'][0] != '' )
			{
				for($i=0; $i<count($_REQUEST['esdid']); $i++){
					if($_REQUEST['esdid'][$i] == '91') $sql_carregados3 .= "select '91' as codigo, 'Em Análise' as descricao";
					if($_REQUEST['esdid'][$i] == '92') $sql_carregados3 .= "select '92' as codigo, 'Em Atendimento' as descricao";
					if($_REQUEST['esdid'][$i] == '100') $sql_carregados3 .= "select '100' as codigo, 'Cancelada' as descricao";
					if($_REQUEST['esdid'][$i] == '93') $sql_carregados3 .= "select '93' as codigo, 'Aguardando validação' as descricao";
					if($_REQUEST['esdid'][$i] == '135') $sql_carregados3 .= "select '135' as codigo, 'Invalidada' as descricao";
					if($_REQUEST['esdid'][$i] == '95') $sql_carregados3 .= "select '95' as codigo, 'Validada' as descricao";
					if($_REQUEST['esdid'][$i] == '170') $sql_carregados3 .= "select '170' as codigo, 'Validada Fora do Prazo' as descricao";
					if($i != count($_REQUEST['esdid'])-1) $sql_carregados3 .= " union all ";
					 
				}
				
				if($sql_carregados3) $esdid = $db->carregar( $sql_carregados3 );					
			}
			$stSql = " select '91' as codigo, 'Em Análise' as descricao
  					   union all
					   select '92' as codigo, 'Em Atendimento' as descricao
					   union all
					   select '100' as codigo, 'Cancelada' as descricao
					   union all
					   select '93' as codigo, 'Aguardando validação' as descricao
					   union all
					   select '135' as codigo, 'Invalidada' as descricao
					   union all
						select '95' as codigo, 'Validada' as descricao
					   union all					   
					   select '170' as codigo, 'Validada Fora do Prazo' as descricao ";
			
			mostrarComboPopup( 'Situação:', 'esdid',  $stSql, '', 'Selecione a(s) Situação(ões)' );				
			

			/////////// GERENTES ////////////
			$stSql = "SELECT DISTINCT u.usucpf as codigo, u.usunome ||' - '|| c.celnome as descricao		
					FROM demandas.usuarioresponsabilidade ur
					 INNER JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
					 INNER JOIN demandas.celula c ON c.celid = ur.celid
					 WHERE ur.rpustatus = 'A' AND ur.pflcod = " . DEMANDA_PERFIL_GERENTE_PROJETO . ' %s ';
			$stSqlCarregados = " AND u.usucpf IN ( %s ) ";
			mostrarComboboxPopup( 'Gerentes', 'usucpfgerente',  $stSql, $stSqlCarregados, 'Selecione o(s) Gerentes(s)' );
			
			//////////// PRIORIDADES ///////////////
		 	$stSql = 'select priid as codigo, pridsc as descricao FROM demandas.prioridade %s ORDER BY priid';
			$stSqlCarregados = " WHERE priid IN ( %s ) ";
			mostrarComboboxPopup( 'Prioridades', 'priid',  $stSql, $stSqlCarregados, 'Selecione a(s) Prioridades(s)' );
			
			///////////// TÉCNICO ////////////////
			$stSql = 'SELECT d.usucpfexecutor as codigo, u.usunome as descricao FROM seguranca.usuario u 
						INNER JOIN demandas.demanda d ON u.usucpf = d.usucpfexecutor
						%s ORDER BY u.usunome';
		 	$stSqlCarregados = ' WHERE d.usucpfexecutor IN ( %s ) ';
		 	mostrarComboboxPopup( 'Técnico', 'usucpfexecutor',  $stSql, $stSqlCarregados, 'Selecione o(s) Técnico(s)' );

		 	////////// ORIGEM DA DEMANDA /////////////
			$stSql = 'SELECT ordid as codigo, orddescricao as descricao FROM demandas.origemdemanda %s ORDER BY ordid';
			$stSqlCarregados = ' WHERE ordid IN ( %s ) ';
		 	mostrarComboboxPopup( 'Origem da Demanda', 'ordid',  $stSql, $stSqlCarregados, 'Selecione a(s) Origens das demandas(s)' );

		 	/////////// ORGÃO ///////////////
		 	$stSql = "SELECT DISTINCT ug.ungcod as codigo, ug.ungdsc as descricao
				FROM
				 demandas.demanda d
				 LEFT JOIN seguranca.usuario u ON d.usucpfdemandante = u.usucpf
				 LEFT JOIN public.unidadegestora ug ON u.ungcod = ug.ungcod OR u.ungcod = ug.ungcod
				 %s ORDER BY ug.ungdsc ";
		 	$stSqlCarregados = ' WHERE ug.ungcod IN ( %s ) ';
		 	//dbg($stSql);
		 	mostrarComboboxPopup( 'Orgão', 'ungcod',  $stSql, $stSqlCarregados, 'Selecione o(s) Orgão(s)' );
		 	
		 	///////// USUÁRIO DEMANDANTE ////////////
			$stSql = 'SELECT DISTINCT d.usucpfdemandante as codigo, u.usunome as descricao FROM seguranca.usuario u 
				INNER JOIN demandas.demanda d ON u.usucpf = d.usucpfdemandante
				%s
				ORDER BY u.usunome'; 	
		 	$stSqlCarregados = " WHERE u.usucpfdemandante IN ( %s ) ";
		 	mostrarComboboxPopup( 'Usuário demandante', 'usucpfdemandante',  sprintf( $stSql, '' ), $stSqlCarregados, 'Selecione o(s) Usuário(s) Demandante(s)' );
		 	
		 	///////// SISTEMA /////////////
		 	$stSql = 'SELECT sd.sidid as codigo, sd.siddescricao as descricao FROM demandas.sistemadetalhe sd
		 			  INNER JOIN demandas.demanda d ON d.sidid = sd.sidid %s ORDER BY sd.siddescricao';
		 	$stSqlCarregados = " WHERE sd.sidid IN ( %s ) ";
		 	mostrarComboboxPopup( 'Sistema', 'sidid',  $stSql, $stSqlCarregados, 'Selecione o(s) Sistema(s)' );
			?>
		</table>
	</div>
	
	<!-- FIM FILTROS -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td align="center">
					<input type="button" name="Gerar Relatório" value="Visualizar" onclick="javascript:submeterFormulario( 'relatorio' );"/>
					&nbsp;
					<input type="button" name="Exportar Planilha" value="Exportar Planilha" onclick="javascript:submeterFormulario( 'planilha' );"/>

					&nbsp;
					<input type="button" name="Salvar Consulta" value="Salvar Consulta" onclick="javascript:submeterFormulario( 'salvar' );"/>
				</td>
			</tr>
	</table>
	
	<!-- ----------------------------------------------------------------------- -->

</form>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script type="text/javascript">
	
	onOffBloco( 'inst' );
	//onOffBloco( 'func' );
	//onOffBloco( 'econ' );
	//onOffBloco( 'outros' );
	//onOffBloco( 'minhasconsultas' );

	function tornar_publico( prtid )
	{
		document.formulario.subAcao.value = 'publico';
		document.formulario.prtid.value = prtid;
		document.formulario.submit();
	}
	
	function excluir_relatorio( prtid )
	{
		document.formulario.subAcao.value = 'excluir';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}
	
	function carregar_consulta( prtid )
	{
		document.formulario.subAcao.value = 'carregar';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}
	
	function carregar_relatorio( prtid )
	{
		document.formulario.subAcao.value = 'carregarExibir';
		document.formulario.prtid.value = prtid;
		submeterFormulario( 'relatorio', false );
	}

</script>